#include <bits/stdc++.h>

namespace MCMF {
    const int MAXN = 1000+10;
    const int INF = 0x3f3f3f3f;

    struct edge {
        int from, to, cap, flow, cost;
        edge(int from=0, int to=0, int cap=0, int flow=0, int cost=0):
            from(from), to(to), cap(cap), flow(flow), cost(cost) {}
    }; 

    int s, t;
    int path[MAXN];
    int dist[MAXN];
    std:: vector<edge> edges;
    std:: vector<int> G[MAXN];

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

    void DFS(std:: vector<int>& ans, int o) {
        ans.push_back(o>>1);
        for(auto& i: G[o]) {
            edge& e = edges[i];
            if( e.flow == 1 ) DFS(ans, e.to|1);
        }
    }

    bool solve(std:: string* in) {
        static std:: vector<int> ans[2];
        for(int i=0; i < 2; ++i) {
            if( !SPFA() ) return false;
            for(int o = t; o != s;) {
                edges[path[o]].flow += 1;
                edges[path[o]^1].flow -= 1;
                o = edges[path[o]].from;
            }
        }

        for(auto& i: G[3]) {
            edge& e = edges[i];
            if( e.flow == 2 ) {
                printf("%d\n", 2);
                std:: cout << in[1] << "\n" << in[t>>1] << "\n" << in[1] << "\n";
                return true;
            }
        }

        ans[0].push_back(s>>1);
        ans[1].push_back(s>>1);
        int idx = 0;
        for(auto& i: G[3]) {
            edge& e = edges[i];
            if( e.flow == 1 ) {
                DFS(ans[idx++], e.to|1);
            }
        }

        ans[1].pop_back();
        std:: reverse(ans[1].begin(), ans[1].end());
        printf("%d\n", ans[0].size() + ans[1].size()-1);
        for(auto& a: ans[0]) std:: cout << in[a] << "\n";
        for(auto& a: ans[1]) std:: cout << in[a] << "\n";
        return true;
    }
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

const int MAXN = 100+10;
const int INF = 0x3f3f3f3f;
std:: unordered_map<std:: string, int> ump;
std:: string in[MAXN];

int main()
{
    int N = read();
    int M = read();
    for(int i=1; i <= N; ++i) {
        std:: cin >> in[i];
        ump[in[i]] = i;
        if( i == 1 || i == N ) MCMF:: addedge(i<<1, i<<1|1, 2, -1);
        else MCMF:: addedge(i<<1, i<<1|1, 1, -1);
    }

    for(int i=1; i <= M; ++i) {
        std:: string s, t;
        std:: cin >> s >> t;
        int ids = ump[s];
        int idt = ump[t];
        if( ids > idt ) std:: swap(ids, idt);
        if( ids == 1 && idt == N ) MCMF:: addedge(ids<<1|1, idt<<1, 2, 0);
        else MCMF:: addedge(ids<<1|1, idt<<1, 1, 0);
    }

    MCMF:: s = 1<<1;
    MCMF:: t = N<<1|1;
    if( !MCMF:: solve(in) ) puts("No Solution!");
    return 0;
}
