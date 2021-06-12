#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

typedef unsigned long long LL;
const int MAXN = 100000+10;

LL N, K, S, id;
LL A[MAXN], B[MAXN];

inline LL SG(LL X) {
    if( X % K == 1 ) return X == 1? 0: SG((X-1)/K);
    return X/K*(K-1) + (X%K == 0? 0: X%K-1);
}

int main()
{
    S = 0;
    scanf("%llu%llu", &N, &K);
    for(int i=1; i <= N; ++i) {
        scanf("%llu", B+i);
        A[i] = SG(B[i]);
        S ^= A[i];
    }

    if( S == 0 ) printf("Bob\n");
    else {
        LL val = 1LL<<62;
        while( !(val&S) ) val >>= 1;
        for(id=1; id <= N && !(val&A[id]); ++id);
        LL e = A[id] ^ S;
        LL f = e == 0? 1: (e/(K-1)*K + (e%(K-1) == 0? 0: e%(K-1)+1));
        LL h = (B[id]-1)/K + 1;
        while( f < h ) f = f*K + 1;

        printf("Alice %llu %llu\n", id, f);
    }
    return 0;
}
