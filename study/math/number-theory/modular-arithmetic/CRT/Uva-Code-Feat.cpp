#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
const int MAXLIMIT = 10000;

void EGCD(LL A, LL& X, LL B, LL& Y, LL& G) {
    if( !B ) {
        G = A; X = 1; Y = 0;
    } else {
        EGCD(B, Y, A%B, X, G);
        Y -= (A/B)*X;
    }
}

LL CRT(int N, int* a, int* m) {
    LL M = 1, ans=0;
    for(int i=0; i < N; ++i) M *= m[i];
    for(int i=0; i < N; ++i) {
        LL w = M/m[i], x, y, d;
        EGCD(w, x, (LL) m[i], y, d);
        ans = (ans+w*x*a[i]) % M;
    }
    return (ans+M)%M;
}

unordered_set<int> sval[10];
vector<LL> anss;
int A[10], X[10], K[10], Y[10][111], C, S, bc;

void solve_enum() {
    for(int c=0; c < C; ++c) if( c != bc ){
        sval[c].clear();
        for(int k=0; k < K[c]; ++k)
            sval[c].insert(Y[c][k]);
    }
    for(int t=0; S; ++t) {
        for(int k=0; k < K[bc]; ++k) {
            LL ans = (LL) t*X[bc] + Y[bc][k];
            if( ans == 0 ) continue;
            bool flag = true;
            for(int c=0; c < C; ++c) {
                if( c != bc and !sval[c].count(ans%X[c]) ) {
                    flag = false;
                    break;
                }
            }
            if( flag ) {
                printf("%lld\n", ans);
                if( --S == 0 ) return;
            }
        }
    }
}

void DFS(int c) {
    if( c == C ) anss.push_back(CRT(C, A, X));
    else {
        for(int k=0; k < K[c]; ++k) {
            A[c] = Y[c][k];
            DFS(c+1);
        }
    }
}

void solve_CRT() {
    anss.clear();
    DFS(0);
    sort(anss.begin(), anss.end());
    LL M = 1;
    for(int c=0; c < C; ++c) M *= X[c];
    for(int t=0; S; ++t) {
        for(auto ans: anss) {
            ans += M*t;
            if( ans > 0 ) {
                printf("%lld\n", ans);
                if( --S == 0 ) return;
            }
        }
    }
}

int main()
{
    while( scanf("%d%d", &C, &S) == 2 && C && S ) {
        LL totK = 1;
        bc = 0;
        for(int c=0; c < C; ++c) {
            scanf("%d%d", X+c, K+c);
            totK *= K[c];
            for(int k=0; k < K[c]; ++k) 
                scanf("%d", Y[c]+k);
            sort(Y[c], Y[c]+K[c]);
            if( K[c]*X[bc] < K[bc]*X[c] ) bc = c;
        }
        if( totK >= MAXLIMIT ) solve_enum();
        else solve_CRT();
        printf("\n");
    }
    return 0;
}
