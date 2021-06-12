#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

vector<LL> facs;
void GetFac(LL N) {
    facs.clear();
    LL sn = ceil(sqrt(1.0*N));
    for(int i=2; i <= sn; ++i) if( N%i == 0 ) {
        facs.push_back(i);
        for(N/=i; N%i == 0; N/=i);
    }
    if( N > 1 ) facs.push_back(N);
}

LL ModPower(LL A, LL X, LL M) {
    LL ans=1;
    for(; X; X >>= 1, A=A*A%M)
        if( X&1 ) ans=ans*A%M;
    return ans;
}

LL GetRoot(LL N) {
    LL phin = N-1;
    GetFac(phin);
    for(auto& e: facs) e=phin/e;
    for(LL g=2; ; ++g) {
        bool flag = true;
        for(auto& e: facs) if( ModPower(g, e, N) == 1 ) {
            flag = false;
            break;
        }
        if( flag ) return g;
    }
}

LL BSGS(LL A, LL B, LL C, LL G) {
    static unordered_map<LL, LL> ump;
    ump.clear();
    LL sc = ceil(sqrt(1.0*C));
    LL AA = 1;
    for(LL i=0; i < sc; ++i) {
        if( !ump.count(AA) ) ump[AA] = i;
        AA = (LL) AA*A%C;
    } 
    LL inv = ModPower(ModPower(A, sc, C), C-2, C);
    for(LL i=0; i < sc; ++i) {
        if( ump.count(B) ) {
            LL ret = sc*i + ump[B];
            if( ret%G == 0 ) return ret;
        }
        B = (LL) B*inv%C;
    }
    return -1;
}

LL EGCD(LL A, LL& X, LL B, LL& Y, LL& G) {
    if( !B ) {
        G = A; X = 1; Y = 0;
    } else {
        EGCD(B, Y, A%B, X, G);
        Y -= A/B*X;
    }
}

vector<LL> ans;
void work() {
    LL T_T, A, B, C;
    scanf("%lld", &T_T);
    while( T_T-- ) {
        scanf("%lld%lld%lld", &C, &A, &B);
        LL X, Y, G;
        EGCD(A, X, C-1, Y, G);
        LL g = GetRoot(C);
        LL R = BSGS(g, B, C, G);
        if( R == -1 ) { puts("No Solution"); continue; }
        R /= G;
        X = (LL) X*R;
        R = (C-1)/G;
        LL k = ceil(-X/R); 
        X += k*R;
        while( X < 0 ) X += R;
        while( X >= R ) X -= R;
        ans.clear();
        for(LL i=0; i < G; ++i) {
            ans.push_back(ModPower(g, X, C));
            X += R;
        }
        sort(ans.begin(), ans.end());
        for(auto& e: ans) printf("%lld ", e);
        printf("\n");
    }
}

int main() 
{
    work();
    return 0;
}
