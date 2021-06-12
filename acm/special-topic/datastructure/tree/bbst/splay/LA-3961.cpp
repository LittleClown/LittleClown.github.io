#include <bits/stdc++.h>

struct node {
    int key;
    int siz;
    bool flip;
    node* prev;
    node* lson;
    node* rson;

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
    }
};

typedef node* root;

inline void zag(root& o) {
    root k = o->rson;
    o->rson = k->lson;
    k->lson->prev = o;
    k->lson = o;
    o->prev = k;
    o = k;
}
inline void zig(root& o) {
    root k = o->lson;
    o->lson = k->rson;
    k->rson->prev = o;
    k->rson = o;
    o->prev = k;
    o = k;
}
// 带父指针的 旋转 和 伸展操作 是此问题最大的难点（蒽，被刘汝佳的代码惯坏了。。
inline void rotate(root o, int d) {
    int d2 = -1;
    root p = o->prev;
    if( p != NULL ) d2 = p->lson == o? 0: 1;
    d? zig(o): zag(o);
    if( d2 != -1 ) d2? p->rson = o: p->lson = o;
    d? o->rson->maintain(): o->lson->maintain(); 
    o->prev = p;
    o->maintain();
}

// 注意，d 必须在 pushdown 之后计算，因为有交换子树的操作。
inline void splay(root o, root f) {
// o 必须先 pushdown() 一次，因为可能 o 已经是 f 的子节点，
// 但为了保持 “splay 操作后的根节点标记全部下传” 的传统，需要这么做。
    for(o->pushdown(); o->prev != f;) {
        root p = o->prev;
        if( p->prev == f ) {
            p->pushdown();
            o->pushdown();
            int d = p->lson == o? 0: 1;
            rotate(p, d^1);
            break;
        }

        root g = p->prev;
        g->pushdown();
        p->pushdown();
        o->pushdown();
        int d = p->lson == o? 0: 1;
        int d2 = g->lson == p? 0: 1;
        if( d == d2 ) rotate(g, d2^1), rotate(p, d^1);
        else rotate(p, d^1), rotate(g, d2^1);
    }
}

const int MAX_NODES = 100000+10;
node* null;
node nodepool[MAX_NODES];

inline void build(root& o, int lft, int rht) {
    int mid = (lft+rht) >> 1;
    o = nodepool + mid;
    o->key = mid;
    o->siz = 1;
    o->flip = false;
    if( lft < mid ) build(o->lson, lft, mid-1); else o->lson = null;
    if( mid < rht ) build(o->rson, mid+1, rht); else o->rson = null;
    o->lson->prev = o;
    o->rson->prev = o;
    o->maintain();
}

inline int solve(root& rt, int key) {
    root o = nodepool+key;
    splay(o, rt->prev);
    int ans = o->lson->siz;
    if( ans ) {
        root k = o->lson;
        k->reverse();
        for(; k->rson != null; k=k->rson) k->pushdown();
        splay(k, o);
        k->rson = o->rson;
        o->rson->prev = k;
        o = k;
    } else o = o->rson;
    rt = o;
    rt->prev = NULL;
    rt->maintain();
    return ans;
}

typedef std:: pair<int, int> pii;
const int MAXN = 100000+10;

root rt;
pii A[MAXN];
int N;

inline int read() {
    bool positive = true;
    char c = getchar();
    int s = 0;
    for(; c < '0' || c > '9'; c=getchar())
        if( c == '-' ) positive = true;
    for(; c >= '0' && c <= '9'; c=getchar())
        s = s*10 + c-'0';
    return positive? s: -s;
}

inline void Init() {
    for(int i=1; i <= N; ++i)
        A[i] = pii(read(), i);
    sort(A+1, A+N+1);
    build(rt, 1, N);
    rt->prev = NULL;
}

int main()
{
    null = new node();
    null->key = 0;
    null->siz = 0;
    null->flip = false;
    null->lson = NULL;
    null->rson = NULL;

    while( scanf("%d", &N) == 1 && N ) {
        Init();
        for(int i=1; i < N; ++i) 
            printf("%d ", solve(rt, A[i].second)+i);
        printf("%d\n", N);
    }
    return 0;
}
