#include <bits/stdc++.h>

namespace MCMF {
    const int MAXN = 10000+10;
    const int INF = 0x3f3f3f3f;

    struct edge {
        int from, to, cap, flow, cost;
        edge(int from=0, int to=0, int cap=0, int flow=0, int cost=0):
            from(from), to(to), cap(cap), flow(flow), cost(cost) {}
    }; 

    int s, t;
    int cost, flow;
    int path[MAXN];
    int dist[MAXN];
    std:: vector<edge> edges;
    std:: vector<int> G[MAXN];

    void init(int source=0, int converge=1, int N=MAXN) {
        s = source; 
        t = converge;
        cost = 0;
        flow = 0;
        edges.clear();
        for(int i=0; i < N; ++i) G[i].clear();
    }

    void addedge(int from, int to, int cap, int cost) {
        int siz = edges.size();
        edges.push_back(edge(from, to, cap, 0, cost));
        edges.push_back(edge(to, from, 0, 0, -cost));
        G[from].push_back(siz);
        G[to].push_back(siz+1);
    }

    bool SPFA() {
        static std:: queue<int> Q;
        static bool inq[MAXN];

        memset(dist, 0x3f, sizeof dist);
        Q.push(s);
        inq[s] = true;
        dist[s] = 0;
 
        while( !Q.empty() ) {
            int o = Q.front(); Q.pop();
            for(int i=0; i < G[o].size(); ++i) {
                edge& e = edges[G[o][i]];
                if( e.cap > e.flow && dist[e.to] > dist[o]+e.cost ) {
                    dist[e.to] = dist[o] + e.cost;
                    path[e.to] = G[o][i];
                    if( inq[e.to] ) continue;
                    inq[e.to] = true;
                    Q.push(e.to);
                }
            }
            inq[o] = false;
        }
 
        return dist[t] != INF;
    }

    std:: pair<int, int> mincostmaxflow() {
        while( SPFA() ) {
            int mif = INF;
            for(int o=t; o != s;) {
                edge& e = edges[path[o]];
                mif = std:: min(mif, e.cap-e.flow);
                o = e.from;
            }
            for(int o = t; o != s;) {
                edges[path[o]].flow += mif;
                edges[path[o]^1].flow -= mif;
                o = edges[path[o]].from;
            }
            flow += mif;
            cost += mif*dist[t];
        }
        return std:: make_pair(flow, cost);
    }

    void solve();
    void solve(int);
    void solve(int, int);
    void solve(int, int, int);
};

inline int read() {
    bool positive = true;
    char c = getchar();
    int s = 0;
    for(; c < '0' || c > '9'; c=getchar())
        if( c == '-' ) positive = false;
    for(; c >= '0' && c <= '9'; c=getchar())
        s = s*10 + c-'0';
    return positive? s: -s;
}

const int INF = 0x3f3f3f3f;

int main()
{
    int N = read();
    int p = read();
    int m = read();
    int f = read();
    int n = read();
    int s = read();

    MCMF:: init(0, 1);
    for(int i=1; i <= N; ++i) {
        int val = read();
        MCMF:: addedge(0, i<<1, val, 0);
        MCMF:: addedge(0, i<<1|1, val, p);
        MCMF:: addedge(i<<1|1, 1, val, 0);
        if( i+1 <= N ) MCMF:: addedge(i<<1, (i+1)<<1, INF, 0);
        if( i+m <= N ) MCMF:: addedge(i<<1, (i+m)<<1|1, INF, f);
        if( i+n <= N ) MCMF:: addedge(i<<1, (i+n)<<1|1, INF, s);
    }

    std:: pair<int, int> ans = MCMF:: mincostmaxflow();

    printf("%d\n", ans.second);
    return 0;
}
