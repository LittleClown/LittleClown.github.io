#include <bits/stdc++.h>

struct node {
    int key;
    int siz;
    int addv;
    node* lson;
    node* rson;

    static node* null;

    int cmp(int s) {
        int cnt = lson->siz+1;
        if( s == cnt ) return -1;
        return s < cnt? 0: 1;
    }
    void update(int v) {
        key += v;
        addv += v;
    }
    void pushdown() {
        if( !addv ) return;
        if( lson != null ) lson->update(addv);
        if( rson != null ) rson->update(addv);
        addv = 0;
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
    }
};

typedef node* root;

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

inline void printtree(root o, int cur=0) {
    if( o == node:: null ) return;
    printtree(o->lson, cur+1);
    printf("cur %d: key=%d, siz=%d\n", cur, o->key, o->siz);
    printtree(o->rson, cur+1);
    if( !cur ) printf("---------------------------------\n");
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
            else rotate(p, d);
        }
        rotate(o, d^1);
    }
}

inline int kth(root o, int k) {
    o->pushdown();
    int d = o->cmp(k);
    if( d == -1 ) return o->key;
    return d? kth(o->rson, k - o->lson->siz - 1): kth(o->lson, k);
}

inline int rank(root o, int k) {
    if( o == node:: null ) return 0;
    o->pushdown();
    if( k <= o->key ) return rank(o->lson, k);
    return rank(o->rson, k) + o->lson->siz + 1;
}

const int MAX_NODES = 100000+10;
node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->addv = 0;
    nodetop->lson = node:: null;
    nodetop->rson = node:: null;
    return nodetop++;
}

inline void insert(root& rt, int key) {
    int k = rank(rt, key);
    splay(rt, k);
    root left = rt;
    root middle = newnode(key);
    root right = rt->rson;
    left->rson = middle;
    middle->rson = right;
    middle->maintain();
    left->maintain();
    rt = left;
}

inline void remove(root& rt, int M) {
    int k = rank(rt, M);
    if( k == 1 ) return;
    splay(rt, 1);
    splay(rt->rson, k);
    rt->rson->lson = node:: null;
    rt->rson->maintain();
    rt->maintain();
}

inline void update(root& rt, int addv) {
    if( rt->siz == 2 ) return;
    splay(rt, 1);
    splay(rt->rson, rt->rson->siz);
    rt->rson->lson->update(addv);
    rt->rson->maintain();
    rt->maintain();
}

const int INF = 0x3f3f3f3f;
root rt;

node* node:: null = new node();
inline void init() {
    node:: null->key = 0;
    node:: null->siz = 0;
    node:: null->addv = 0;
    node:: null->lson = NULL;
    node:: null->rson = NULL;

    nodetop = nodepool;
    rt = newnode(-INF);
    rt->rson = newnode(INF);
    rt->maintain();
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

int main()
{
    init();
    
    int N = read();
    int M = read();
    int tot = 2;
    for(int i=1; i <= N; ++i) {
        char cmd[20];
        scanf("%s", cmd);
        int arg = read();

        switch( cmd[0] ) {
            case 'I': if( arg >= M ) insert(rt, arg), ++tot; break;
            case 'A': update(rt, arg); break;
            case 'S': update(rt, -arg); remove(rt, M); break;
            case 'F': printf("%d\n", arg <= rt->siz-2? kth(rt, rt->siz - arg): -1); break;
        }
    }
    printf("%d\n", tot - rt->siz);

    return 0;
}
