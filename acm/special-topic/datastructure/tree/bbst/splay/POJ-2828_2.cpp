#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define  lc (o<<1)
#define  rc (o<<1|1)
#define  lson lc, lft, mid
#define  rson rc, mid+1, rht
#define  MID(lft, rht) (lft+rht>>1)

const int MAXN = 200000+10;

int sumv[MAXN<<2], ans[MAXN], pos[MAXN], val[MAXN], N;
void build(int o, int lft, int rht) {
    sumv[o] = rht-lft+1;
    if( lft == rht ) return;
    int mid = MID(lft, rht);
    build(lson);
    build(rson);
}

void query(int o, int lft, int rht, int pos, int val) {
    --sumv[o];
    if( lft == rht ) ans[lft] = val;
    else {
        int mid = MID(lft, rht);
        if( pos <= sumv[lc] ) query(lson, pos, val);
        else query(rson, pos-sumv[lc], val);
    }
}

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

inline void print(int s) {
    if( s > 9 ) print(s/10);
    putchar(s%10+'0');
}

int main()
{
    while( scanf("%d", &N) == 1 ) {
        build(1, 1, N);
        for(int i=1; i <= N; ++i) 
            pos[i] = read()+1, val[i] = read();
        for(int i=N; i; --i) 
            query(1, 1, N, pos[i], val[i]);
        for(int i=1; i <= N; ++i) 
            print(ans[i]), putchar(' ');
        putchar('\n');
    }    
    return 0;
}
