namespace Dinic {
    struct edge {
        int from, to, cap, flow;
        edge(int from=0, int to=0, int cap=0, int flow=0):
            from(from), to(to), cap(cap), flow(flow) {}
    };

    int s, t;
    int cur[MAXN];
    int dist[MAXN];
    std:: queue<int> Q;
    std:: vector<edge> edges;
    std:: vector<int> G[MAXN];

    void addedge(int from, int to, int cap) {
        int siz = edges.size();
        edges.push_back(edge(from, to, cap, 0));
        edges.push_back(edge(to, from, 0, 0));
        G[from].push_back(siz);
        G[to].push_back(siz+1);
    }

    bool BFS() {
        memset(dist, -1, sizeof dist);

        Q.push(s);
        dist[s] = 0;

        while( !Q.empty() ) {
            int o = Q.front(); Q.pop();
            for(int i=0; i < G[o].size(); ++i) {
                edge& e = edges[G[o][i]];
                if( dist[e.to] != -1 || e.cap <= e.flow ) continue;
                dist[e.to] = dist[o]+1;
                Q.push(e.to);
            }
        }

        return dist[t] != -1;
    }

    int DFS(int o, int minflow) {
        if( o == t || minflow == 0 ) return minflow;
        int flow = 0;
        for(int& i=cur[o]; i < G[o].size(); ++i) {
            edge& e = edges[G[o][i]];
            if( dist[e.to] == dist[o]+1 ) {
                int f = DFS(e.to, std:: min(minflow, e.cap-e.flow));
                if( f <= 0 ) continue;
                e.flow += f;
                edges[G[o][i]^1].flow -= f;
                flow += f;
                minflow -= f;
                if( minflow == 0 ) break;
            }
        }
        return flow;
    }

    int maxflow() {
        int ans = 0;
        while( BFS() ) {
            memset(cur, 0, sizeof cur);
            ans += DFS(s, INF);
        }
        return ans;
    }

    void solve();
    void solve(int);
    void solve(int, int);
    void solve(int, int, int);
};
