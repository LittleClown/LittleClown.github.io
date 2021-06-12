#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

LL ModPower(LL A, LL X, LL M) {
    LL ans = 1;
    for(; X > 0; X>>=1, A=A*A %M)
        if( X&1 ) ans = ans*A %M;
    return ans;
}

LL GCD(LL A, LL B) {
    return B? GCD(B, A%B): A;
}
void EGCD(LL A, LL& X, LL B, LL& Y, LL& G) {
    if( !B ) {
        G = A; X = 1; Y = 0;
    } else {
        EGCD(B, Y, A%B, X, G);
        Y -= A/B*X;
    }
}

void GetFac(LL N, vector< pair<LL, LL> >& pfacs) {
    pfacs.clear();
    LL sn = ceil(sqrt(1.0*N));
    for(LL i=2; i <= sn; ++i) if( N%i == 0 ) {
        LL cnt = 1;
        for(N/=i; N%i == 0; N/=i) ++cnt;
        pfacs.push_back(make_pair(i, cnt));
    }
    if( N > 1 ) pfacs.push_back(make_pair(N, 1LL));
}

LL GetRoot(LL N, LL phin) {
    static vector< pair<LL, LL> > pfacs;
    GetFac(phin, pfacs);
    for(auto& e: pfacs)
        e.first = phin/e.first;
    for(LL g=2; ; ++g) {
        bool flag = true;
        for(auto& e: pfacs)
            if( ModPower(g, e.first, N) == 1 ) {
                flag = false;
                break;
            }
        if( flag ) return g;
    }
}

void solve2(LL A, LL B, LL C, vector<LL>& ans) {
    ans.clear();
    B %= C; if( B < 0 ) B += C;
    for(LL i=0; i < C; ++i) 
        if( ModPower(i, A, C) == B )
            ans.push_back(i);
}

LL EBSGS(LL A, LL B, LL C, LL phic, LL G) {
    A %= C; if( A < 0 ) A += C;
    B %= C; if( B < 0 ) B += C;
    LL alpha = 1, beta = 0;
    for(LL d; (d=GCD(A, C)) != 1; ) {
        if( B%d ) return -1; B /= d; C /= d;
        alpha = alpha*A/d %C;
        ++beta;
        if( B == alpha and beta%G == 0 ) return beta;
    }
    static unordered_map<LL, LL> ump;
    ump.clear();
    LL sc = ceil(sqrt(1.0*C));
    LL theta = 1;
    for(LL i=0; i < sc; ++i) {
        if( !ump.count(theta) ) ump[theta] = i+beta;
        theta = theta*A %C;
    }
    LL inv = ModPower(theta, phic-1, C);
    B = B*ModPower(alpha, phic-1, C) %C;
    for(LL i=0; i < sc; ++i) {
        if( ump.count(B) ) {
            LL ret = i*sc+ump[B];
            if( ret%G == 0 ) return ret;
        }
        B = B*inv% C;
    }
    return -1;
}

vector< vector<LL> > ansxs;
vector<LL> anss;
LL A[1000], M[1000];

LL CRT(int N, LL* A, LL* M) {
    LL T = 1, ans = 0;
    for(int i=0; i < N; ++i) T *= M[i];
    for(int i=0; i < N; ++i) {
        LL W = T/M[i], X, Y, G;
        EGCD(W, X, M[i], Y, G);
        ans = (ans+X*W*A[i])%T;
    }
    return (ans+T)%T;
}

void DFS(int dep, int N) {
    if( dep == N )
        anss.push_back(CRT(N, A, M));
    else {
        for(auto& e: ansxs[dep]) {
            A[dep] = e;
            DFS(dep+1, N);
        }
    }
}

void work() {
    static int T_T;
    static LL A, B, C, X, Y, G, g, R;
    static vector< pair<LL, LL> > Cfacs;
    scanf("%d", &T_T);
    while( T_T-- ) {
        scanf("%lld%lld%lld", &A, &C, &B);
        GetFac(C, Cfacs);

        for(auto& e: Cfacs) {
            LL p = e.first;
            e.first = ModPower(p, e.second, C*2);
            e.second = e.first - e.first/p;
        }

        ansxs.clear();
        bool flag = true;
        for(auto& e: Cfacs) {
            vector<LL> ansx;
            if( e.first == e.second*2 ) {
                solve2(A, B, e.first, ansx);
                if( !ansx.size() ) { flag = false; break; }
            }
            else if( B%e.first ) {
                EGCD(A, X, e.second, Y, G);
                g = GetRoot(e.first, e.second);
                if( g == -1 ) { flag = false; break; }
                R = EBSGS(g, B, e.first, e.second, G);
                if( R == -1 ) { flag = false; break; }
                R /= G; X *= R;
                R = e.second / G;
                X %= R;
                if( X < 0 ) X += R;
                for(int i=0; i < G; ++i) {
                    ansx.push_back(ModPower(g, X, e.first));
                    X += R;
                }
            } else {
                X = 0;
                LL T = e.first/(e.first-e.second);
                R = 1;
                while( ModPower(R, A, e.first) ) R *= T;
                G = e.first/R;
                for(int i=0; i < G; ++i) {
                    ansx.push_back(X);
                    X += R;
                }
            }
            
            
            sort(ansx.begin(), ansx.end());
            ansx.erase(unique(ansx.begin(), ansx.end()), ansx.end());
            ansxs.push_back(ansx);
        }
        if( flag ) {
            for(int i=0; i < Cfacs.size(); ++i) M[i] = Cfacs[i].first;
            anss.clear();
            DFS(0, Cfacs.size());
            sort(anss.begin(), anss.end());
            anss.erase(unique(anss.begin(), anss.end()), anss.end());
            for(auto& e: anss) printf("%lld ", e);
            putchar('\n');
        }
        else puts("No Solution");
    }
}

int main()
{
    work();
    return 0;
}
