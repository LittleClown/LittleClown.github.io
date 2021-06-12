#include <bits/stdc++.h>

namespace solve {
    const int INF = 0x3f3f3f3f;
    const int MAXN = (1<<20) + 10;

    struct node {
        int B1, B2;
        int F1, F2;
        int cost;
        node(int B1=0, int B2=0, int F1=0, int F2=0, int cost=0):
            B1(B1), B2(B2), F1(F1), F2(F2), cost(cost) {}
    };

    int s, t;
    int dist[MAXN];
    std:: vector<node> nodes;
    
    void update(char* s, int& x, int& y) {
        x = y = 0;
        for(int i=0; s[i]; ++i) {
            if( s[i] == '+' ) x |= 1<<i;
            else if( s[i] == '-' ) y |= 1<<i;
        }     
    }

    int SPFA() {
        static bool inq[MAXN];
        static std:: queue<int> Q; 

        memset(dist, 0x3f, sizeof dist);

        Q.push(s);
        dist[s] = 0;
        inq[s] = true;

        while( !Q.empty() ) {
            int o = Q.front(); Q.pop();
            for(auto& e: nodes) {
                if( (o&e.B1) == e.B1 && (o&e.B2) == 0 ) {
                    int u = (o&e.F1) | e.F2;
                    if( dist[u] > dist[o]+e.cost ) {
                        dist[u] = dist[o]+e.cost;
                        if( !inq[u] ) {
                            inq[u] = true;
                            Q.push(u);
                        }
                    }
                }
            }
            inq[o] = false;
        }
        
        return dist[t] != INF? dist[t]: 0;
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

int main()
{
    int N = read();
    int M = read();
    int B1, B2, F1, F2, cost, re;
    char in[100];

    re = (1<<N)-1;
    for(int i=0; i < M; ++i) {
        cost = read();
        scanf("%s", in);
        solve:: update(in, B1, B2);
        scanf("%s", in);
        solve:: update(in, F2, F1);
        F1 = re^F1;
        solve:: nodes.push_back(solve:: node(B1, B2, F1, F2, cost));
    }

    solve:: s = re;
    solve:: t = 0;
    printf("%d\n", solve:: SPFA());
    
    return 0;
}
