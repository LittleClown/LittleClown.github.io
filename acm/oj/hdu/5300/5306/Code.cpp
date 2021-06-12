#include <bits/stdc++.h>
#define  lc (o<<1)
#define  rc (o<<1|1)
#define  lson lc, lft, mid
#define  rson rc, mid+1, rht
#define  MID(lft, rht) (lft+rht>>1)
using namespace std;

typedef long long LL;
const int MAXN = 4000000 + 10;

char buf[1024000*40+10], *ch;
LL sumv[MAXN];
bool leav[MAXN];
int maxv[MAXN], tagv[MAXN], cntv[MAXN];

int Read(int& s) {
    for(++ch; *ch<'0' or *ch>'9'; ++ch);
    for(s=0; *ch>='0' and *ch<='9'; ++ch) s = s*10 + *ch-'0';
    return s;
}

void Maintain(int o, int tag) {
    if( tagv[o] ) return;
    tagv[o] = tag;
    if( cntv[o] ) {
        sumv[o] += (LL) cntv[o] * tag;
        maxv[o] = tag;
        cntv[o] = 0;
    }
}

void Pushup(int o) {
    maxv[o] = std:: max(maxv[lc], maxv[rc]);
    sumv[o] = sumv[lc] + sumv[rc];
    cntv[o] = cntv[lc] + cntv[rc];
}

void Pushdown(int o) {
    if( tagv[o] ) {
        Maintain(lc, tagv[o]);
        Maintain(rc, tagv[o]);
    }
}

void Build(int o, int lft, int rht) {
    if( lft == rht ) {
        leav[o] = true;
        cntv[o] = 0;
        sumv[o] = maxv[o] = Read(tagv[o]);
    } else {
        int mid = MID(lft, rht);
        leav[o] = false;
        tagv[o] = 0;
        Build(lson);
        Build(rson);
        Pushup(o);
    }
}

void Clear(int o, int tag) {
    if( maxv[o] <= tag ) return;
    tagv[o] = 0;
    if( leav[o] ) {
        sumv[o] = maxv[o] = 0;
        cntv[o] = 1;
    } else {
        Clear(lc, tag);
        Clear(rc, tag);
        Pushup(o);
    }
}

int __ul, __ur, __uv;
void Update(int o, int lft, int rht) {
    if( maxv[o] <= __uv ) return;
    if( __ul<=lft and rht<=__ur ) {
        Clear(o, __uv);
        Maintain(o, __uv);
    } else {
        int mid = MID(lft, rht);
        Pushdown(o);
        if( __ul <= mid ) Update(lson);
        if( __ur > mid ) Update(rson);
        Pushup(o);
    }
}

LL __sum;
int __max, __ql, __qr;
void Query(int o, int lft, int rht) {
    if( __ql<=lft and rht<=__qr ) {
        __sum += sumv[o];
        __max = std:: max(__max, maxv[o]);
    } else {
        int mid = MID(lft, rht);
        Pushdown(o);
        if( __ql <= mid ) Query(lson);
        if( __qr > mid ) Query(rson);
        Pushup(o);
    }
}

int T_T, N, Q, op;
int main()
{
    fread(buf, 1, 35*1024000, stdin);
    ch = buf - 1;
    Read(T_T);
    while( T_T-- ) {
        Read(N); Read(Q);
        Build(1, 1, N);
        while( Q-- ) {
            if( Read(op) ) {
                Read(__ql); Read(__qr);
                __sum = __max = 0;
                Query(1, 1, N);
                if( op == 1 ) printf("%d\n", __max);
                else printf("%lld\n", __sum);
            } else {
                Read(__ul); Read(__ur); Read(__uv);
                Update(1, 1, N);
            }
        }
    }
    return 0;
}
