#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;

typedef __int64 LL;
const int MAXN = 1000000+10;
char num[MAXN];

LL ModPower(LL A, LL X, LL M) {
    LL ans = 1;
    for(; X; X >>= 1, A = A*A%M)
        if( X&1 ) ans = ans*A%M;
    return ans;
}

LL GetPhi(LL X) {
    LL ans = X;
    LL SX = (LL) ceil(sqrt(1.0*X));
    for(int i=2; i < SX; ++i) if( X % i == 0 ) {
        ans = ans/i*(i-1);
        while( X % i == 0 ) X /= i;
    }
    if( X > 1 ) ans = ans/X*(X-1);
    return ans;
}

int main()
{
    LL A, B, C, phi;
    while( scanf("%I64d%s%I64d", &A, num, &C) == 3 ) {
        B = 0; 
        phi = GetPhi(C);
        bool flag = false;
        int len = strlen(num);
        for(int i=0; i < len; ++i) {
            B = B*10 + num[i]-'0';
            if( B > phi ) {
                flag = true;
                B %= phi;
            }
        }
        if( flag ) B += phi;
        printf("%I64d\n", ModPower(A, B, C));
    }
    return 0;
}
