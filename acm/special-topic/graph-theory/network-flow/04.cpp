#include <bits/stdc++.h>

const int MAXN = 10000+10;
const int INF = 0x3f3f3f3f;

namespace hungary {
    int L, R;
    bool mark[MAXN];
    int left[MAXN], right[MAXN];
    std:: vector<int> G[MAXN];

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

    int maxmatch() {
        int ans = 0;
        for(int u=1; u <= L; ++u) if( !right[u] ) {
            memset(mark, 0, sizeof mark);
            if( match(u) ) ++ans;
        }
        return ans;
    }

    void solve(int N) {
        static std:: vector<int> A;
        for(int i=1; i <= 100; ++i) A.push_back(i*i);
        memset(left, 0, sizeof left);
        memset(right, 0, sizeof right);

        init(N, N);
        for(int i=1; i <= N; ++i)
            for(auto& a: A) if( a-i >= 1 && a-i < i ) G[a-i].push_back(i);
        int ans = maxmatch();

        while( true ) {
            ++L, ++R;
            for(auto& a: A) if( a-L >= 1 && a-L < L ) G[a-L].push_back(L);
            ans += maxmatch();
            if( L-ans > N ) break;
        }

        for(auto& a: A) if( a-L >= 1 && a-L < L ) G[a-L].pop_back();
        right[left[L]] = 0;
        --L; --R;
        maxmatch();

        printf("%d\n", L);
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
    }
};

int main()
{
    int N; std:: cin >> N;
    hungary:: solve(N);
    return 0;
}
