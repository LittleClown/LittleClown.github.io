#include <bits/stdc++.h>

struct node {
    int key;
    int siz;
    int minv;
    bool flip;
    node* lson;
    node* rson;

    int cmp(int key) {
        int cnt = lson->siz + 1;
        if( key == cnt ) return -1;
        return key < cnt? 0: 1;
    }
    void reverse() {
        flip ^= 1;
        std:: swap(lson, rson);
    }
    void pushdown() {
        if( !flip ) return;
        lson->reverse();
        rson->reverse();
        flip = false;
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
        minv = std:: min(key, std:: min(lson->minv, rson->minv));
    }
};

typedef node* root;
node* null;

inline void zag(root& o) {
    root k = o->rson;
    o->rson = k->lson;
    k->lson = o;
    o = k;
}
inline void zig(root& o) {
    root k = o->lson;
    o->lson = k->rson;
    k->rson = o;
    o = k;
}
inline void rotate(root& o, int d) {
    d? zig(o): zag(o);
    d? o->rson->maintain(): o->lson->maintain();
    o->maintain();
}

inline void splay(root& o, int k) {
    o->pushdown();
    int d = o->cmp(k);
    if( d == 1 ) k -= o->lson->siz+1;
    if( d != -1 ) {
        root& p = d? o->rson: o->lson;
        p->pushdown();
        int d2 = p->cmp(k);
        if( d2 == 1 ) k -= p->lson->siz+1;
        if( d2 != -1 ) {
            splay((d2? p->rson: p->lson), k);
            if( d == d2 ) rotate(o, d^1);
            else rotate(p, d2^1);
        }
        rotate(o, d^1);
    }
}

inline void split(root o, int k, root& left, root& right) {
    splay(o, k);
    left = o;
    right = o->rson;
    o->rson = null;
    o->maintain();
}

inline root merge(root left, root right) {
    splay(left, left->siz);
    left->rson = right;
    left->maintain();
    return left;
}

inline int kth(root rt) {
    rt->pushdown();
    if( rt->key == rt->minv ) return rt->lson->siz + 1;
    if( rt->lson->minv < rt->rson->minv ) return kth(rt->lson);
    return kth(rt->rson) + rt->lson->siz + 1;
}

const int MAX_NODES = 100000+10;
node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->minv = key;
    nodetop->flip = false;
    nodetop->lson = null;
    nodetop->rson = null;
    return nodetop++;
}

inline void build(root& o, int lft, int rht, int* rank) {
    int mid = (lft+rht) >> 1;
    o = newnode(rank[mid]);
    if( lft < mid ) build(o->lson, lft, mid-1, rank);
    if( mid < rht ) build(o->rson, mid+1, rht, rank);
    o->maintain();
}

const int MAXN = 100000+10;
const int INF = 0x3f3f3f3f;
int N, A[MAXN], id[MAXN], rank[MAXN];
root rt;

inline bool cmp(int u, int v) { return A[u] < A[v]; }

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

inline void init() {
    nodetop = nodepool;
}

inline int solve() {
    int k = kth(rt);
    splay(rt, k);
    int ans = rt->lson->siz;
    if( ans ) {
        rt->lson->reverse();
        splay(rt->lson, rt->lson->siz);
        rt = merge(rt->lson, rt->rson);
    } else rt = rt->rson, rt->pushdown();
    return ans;
}

int main()
{    
    null = new node();
    null->key = 0;
    null->siz = 0;
    null->minv = INF;
    null->flip = false;
    null->lson = NULL;
    null->rson = NULL;

    while( scanf("%d", &N) == 1 && N ) {
        init();
        for(int i=1; i <= N; ++i) A[i] = read();
        for(int i=1; i <= N; ++i) id[i] = i;
        std:: stable_sort(id+1, id+N+1, cmp);
        for(int i=1; i <= N; ++i) rank[id[i]] = i;
        build(rt, 1, N, rank);
        for(int i=1; i < N; ++i) printf("%d ", solve()+i); 
        printf("%d\n", N);
    }
    return 0;
}
