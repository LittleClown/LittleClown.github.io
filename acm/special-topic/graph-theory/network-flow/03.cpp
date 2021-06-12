#include <bits/stdc++.h>

const int MAXN = 1000+10;
const int INF = 0x3f3f3f3f;

namespace hungary {
    int L, R;
    std:: vector<int> G[MAXN];
    int left[MAXN], right[MAXN];
    bool mark[MAXN];

    void init(int L=0, int R=0) {
        hungary:: L = L;
        hungary:: R = R;
        for(int u=1; u <= L; ++u ) G[u].clear();
    }

    bool match(int u) {
        for(int i=0; i < G[u].size(); ++i) {
            int v = G[u][i];
            if( !mark[v] ) {
                mark[v] = true;
                if( !left[v] || match(left[v]) ) {
                    left[v] = u;
                    right[u] = v;
                    return true;
                }
            }
        }
        return false;
    }

    void solve() {
        memset(left, 0, sizeof left);
        memset(right, 0, sizeof right);

        int ans = 0;
        for(int u=1; u <= L; ++u) if( !right[u] ) {
            memset(mark, 0, sizeof mark);
            if( match(u) ) ++ans;
        }

        memset(mark, 0, sizeof mark);
        mark[0] = true;
        for(int o=1; o <= L; ++o) if( !mark[o] ) {
            mark[o] = true;
            printf("%d", o);
            for(int u=right[o]; !mark[u]; u=right[u]) {
                mark[u] = true; 
                printf(" %d", u);
            } 
            putchar('\n');
        }
        printf("%d\n", L-ans);
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

    hungary:: init(N, N);
    for(int i=1; i <= M; ++i) {
        int u = read();
        int v = read();
        hungary:: G[u].push_back(v);
    }

    hungary:: solve();
    return 0;
}
