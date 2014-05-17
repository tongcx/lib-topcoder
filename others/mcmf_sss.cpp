#include <vector>
#include <queue>
#define rep(i, n) for (int i = 0; i < n; i++)
#define sz(c) int((c).size())
#define pb push_back
using namespace std;
typedef vector<int> vi;
typedef int cost_type;
const int INF = 0x3F3F3F3F;
const cost_type CINF = 0x3F3F3F3F;

// MinCostMaxFlow - O(n^2 * m * U) with Successive Shortest Path
struct MCMF {
  int n, F;    // n is number of nodes
  cost_type C;
  vector<cost_type> cost, pot;
  vi dest, cap, flow;  // use sz(dest) as nar (including back arcs)
  vector<vi> adj;

  MCMF(int n):n(n) {adj.resize(n);}
  int inv(int a) { return a ^ 0x1;}
  int orig(int a) { return dest[inv(a)];}
  int capres(int a) { return cap[a] - flow[a];}
  cost_type rescost(int a) { return cost[a]+pot[orig(a)]-pot[dest[a]];}
  int arc(int i, int j, int u, cost_type c = 0) {
    dest.pb(j); adj[i].pb(sz(dest)-1); cap.pb(u); cost.pb(c);
    dest.pb(i); adj[j].pb(sz(dest)-1); cap.pb(0); cost.pb(-c);
    return sz(dest)-2;
  }

  bool dijkstra(int ini, int end) {
    vi ent(n, -1); vector<bool> mark(n);
    vector<cost_type> dist(n, CINF);
    dist[ini] = 0;
    while(true) {
      int u = -1; cost_type CC = CINF;
      rep(v, n) if (!mark[v] && dist[v] < CC) {u = v; CC = dist[v];}
      if (u == -1) break;
      mark[u] = true;
      for(auto a : adj[u]) {
        int v = dest[a];
        if (capres(a) > 0 && dist[v] > dist[u]+rescost(a)) {
          dist[v] = dist[u] + rescost(a);
          ent[v] = a;
        }
      }
    }

    if (!mark[end]) return false;
    rep(u, n) if (mark[u]) pot[u] += dist[u];
    int U = INF;
    for (int a = ent[end]; a != -1; a = ent[orig(a)]) U = min(U, capres(a));
    for (int a = ent[end]; a != -1; a = ent[orig(a)]) {
      C += U * cost[a];
      flow[a] += U;
      flow[inv(a)] -= U;
    }
    F += U;
    return true;
  }

  cost_type mcmf(int ini, int end) {
    flow = vi(sz(dest)); pot = vector<cost_type>(n);
    F = C = 0;
    while (dijkstra(ini, end));
    return C;
  }
};
