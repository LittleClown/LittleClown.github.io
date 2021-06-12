
#include <bits/stdc++.h>
    
namespace ISAP {
    static const int MAXN = 200+10;
    static const int INF = 0x3f3f3f3f;

    struct edge {
        int from, to, cap, flow;
        edge(int from=0, int to=0, int cap=0, int flow=0):
            from(from), to(to), cap(cap), flow(flow) {}
    };
 
    int s, t, n;
    int cnt[MAXN];
    int cur[MAXN];
    int path[MAXN];
    int dist[MAXN];
    std:: vector<edge> edges;
    std:: vector<int> G[MAXN];
    std:: queue<int> Q;
 
    void addedge(int from, int to, int cap) {
        int siz = edges.size();
        edges.push_back(edge(from, to, cap, 0));
        edges.push_back(edge(to, from, 0, 0));
        G[from].push_back(siz);
        G[to].push_back(siz+1);
    }
    
    void BFS() {
        memset(dist, 0x3f, sizeof dist);
        Q.push(t);
        dist[t] = 0;
 
        while( !Q.empty() ) {
            int o = Q.front(); Q.pop();
            for(int i=0; i < G[o].size(); ++i) {
                edge& e = edges[G[o][i]];
                if( dist[e.to] == INF && e.cap == 0 ) {
                    dist[e.to] = dist[o] + 1;
                    Q.push(e.to);
                }
            }
        }
    }

    int augment() {
        int mif = INF; 
        for(int o=t; o != s;) {
            edge& e = edges[path[o]];
            mif = std:: min(mif, e.cap-e.flow);
            o = e.from;
        }
        for(int o=t; o != s;) {
            edges[path[o]].flow += mif;
            edges[path[o]^1].flow -= mif;
            o = edges[path[o]].from;
        }
        return mif;
    }

    int maxflow() {
        BFS();
        memset(cur, 0, sizeof cur);
        memset(cnt, 0, sizeof cnt);
        for(int i=0; i < n; ++i) 
            if( dist[i] < n ) ++cnt[dist[i]];

        int ans = 0;
        for(int o=s; dist[o] < n; ) {
            if( o == t ) ans += augment(), o = s;
            bool ok = false;
            for(int i=cur[o]; i < G[o].size(); ++i) {
                edge& e = edges[G[o][i]];
                if( e.cap > e.flow && dist[o] == dist[e.to]+1 ) {
                    ok = true;
                    cur[o] = i;
                    path[e.to] = G[o][i];
                    o = e.to;
                    break;
                }
            }
            if( !ok ) {
                int d = n-1;
                for(int i=0; i < G[o].size(); ++i) {
                    edge& e = edges[G[o][i]];
                    if( e.cap > e.flow ) d = std:: min(d, dist[e.to]);
                }
                if( --cnt[dist[o]] == 0 ) break;
                ++cnt[dist[o] = d+1];
                cur[o] = 0;
                if( o != s ) o = edges[path[o]].from;
            }
        }
        return ans;
    }

    void solve();
    void solve(int);
    void solve(int, int);
    void solve(int, int, int);
};
 
void ISAP:: solve(int N, int M) {
    s = 0; t = M+1; n = M+2;
    int ans = maxflow();
    if( ans > 0 ) {
        printf("%d\n", ans);
        for(int o=0; o < N; ++o) {
            edge& e = edges[G[0][o]];
            if( e.cap == e.flow ) {
                for(auto& i: G[e.to]) {
                    edge& e2 = edges[i];
                    if( e2.cap == e2.flow ) {
                        printf("%d %d\n", e.to, e2.to);
                        break;
                    }
                }
            }
        }
    } else puts("No Solution!");
}

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

int main()
{
    int N = read();
    int M = read();

    for(int i=1; i <= N; ++i) 
        ISAP:: addedge(0, i, 1);

    for(int i=N+1; i <= M; ++i)
        ISAP:: addedge(i, M+1, 1);

    while( true ) {
        int u = read();
        int v = read();
        if( u == -1 && v == -1 ) break;
        if( u > v ) std:: swap(u, v);
        ISAP:: addedge(u, v, 1);
    }

    ISAP:: solve(N, M);
    return 0;
}
