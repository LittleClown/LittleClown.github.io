#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <algorithm>
#define REP(i, a, n) for(int i = (a); i < (n); ++i)
#define REPIN(i, a, n) for(int i = (a); i <= (n); ++i)
using namespace std;

typedef long long LL;
const int mod = 1e9 + 7;
const int maxn = 50 + 1;
LL dp[maxn][maxn][maxn][maxn], C[maxn][maxn], TEN[maxn], ans;
LL S1[maxn][maxn][maxn], S2[maxn][maxn][maxn], S3[maxn][maxn][maxn];

void INIT() {
    TEN[0] = 1;
    REP(i, 1, maxn) TEN[i] = TEN[i-1] * 10 % mod;
    C[0][0] = 1;
    REP(i, 1, maxn) {
        C[i][0] = C[i][i] = 1;
        REP(j, 1, i) C[i][j] = (C[i-1][j-1] + C[i-1][j]) % mod;
    }
}

inline void ADD(LL& ans, LL arg) {
    ans += arg;
    if(ans > mod) ans -= mod;
}

inline void SUB(LL& ans, LL arg) {
    ans -= arg;
    if(ans < 0) ans += mod;
}

int main()
{
    int T_T, K, N, P;
    char s[maxn];
    scanf("%d", &T_T);
    INIT();
    REPIN(kase, 1, T_T) {
        scanf("%d%s", &K, s + 1);
        N = strlen(s + 1);
        ans = 0;
        if(N <= 2) goto output;
        memset(dp[0], 0, sizeof dp[0]);
        memset(S1, 0, sizeof S1);
        memset(S2, 0, sizeof S2);
        memset(S3, 0, sizeof S3);
        REPIN(i, 1, N) {
            if(s[i] == '*') P = i;
            else s[i] -= '0';
        }
        REPIN(a, 1, N) REPIN(b, 1, N) {
            if(a != P && b != P && a != b)
                dp[0][P][a][b] = s[a] * s[b];
        }
        REPIN(i, 1, N) REPIN(a, 1, N) REPIN(b, 1, N) {
            if(a != i && b != i && a != b) {
                ADD(S1[0][i][b], dp[0][i][a][b]);
                ADD(S2[0][a][i], dp[0][i][a][b]);
                ADD(S3[0][a][b], dp[0][i][a][b]);
            }
        }
        REPIN(k, 1, K) REPIN(i, 1, N) {
            REPIN(a, 1, N) REPIN(b, 1, N) {
                if(a != i && b != i && a != b) {
                    LL& ans = dp[k][i][a][b];
                    ans = dp[k-1][i][a][b] * C[N-3][2] % mod;
                    ADD(ans, S1[k-1][i][b]);
                    ADD(ans, S2[k-1][a][i]);
                    ADD(ans, S3[k-1][a][b]);
                    SUB(ans, dp[k-1][i][a][b]);
                    SUB(ans, dp[k-1][i][a][b]);
                    SUB(ans, dp[k-1][i][a][b]);
                    ADD(ans, dp[k-1][i][b][a]);
                    ADD(ans, dp[k-1][a][i][b]);
                    ADD(ans, dp[k-1][b][a][i]);
                    ADD(S1[k][i][b], ans);
                    ADD(S2[k][a][i], ans);
                    ADD(S3[k][a][b], ans);
                }
            }
        }
        REP(i, 1, N) REP(a, 1, i) REPIN(b, i + 1, N)
            ans = (ans + (dp[K][i][a][b]) * TEN[i-a-1+N-b]) % mod;
        output:
            printf("Case #%d: %I64d\n", kase, ans);
    }
    return 0;
}
