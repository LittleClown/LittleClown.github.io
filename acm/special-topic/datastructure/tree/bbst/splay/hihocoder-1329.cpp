#include <bits/stdc++.h>

typedef long long LL;

struct node {
    int key;
    int siz;
    node* lson;
    node* rson;

    int cmp(int x) {
        int cnt = lson->siz + 1;
        if( x == cnt ) return -1;
        return x < cnt? 0: 1;
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
    }
};

typedef node* root;
typedef std:: pair<node*, node*> droot;
const int MAX_NODES = 200000+10;

node* null;
node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->lson = null;
    nodetop->rson = null;
    return nodetop++;
}

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
    int d = o->cmp(k);
    if( d == 1 ) k -= o->lson->siz+1;
    if( d != -1 ) {
        root& p = d? o->rson: o->lson;
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

inline int rank(root o, int key) {
    if( o == null ) return 0;
    if( key == o->key ) return o->lson->siz;
    if( key < o->key ) return rank(o->lson, key);
    return o->lson->siz+1 + rank(o->rson, key);
}

inline void insert(root& o, int id) {
    int k = rank(o, id);
    root left, right;
    root middle = newnode(id);
    split(o, k, left, right);
    o = merge(merge(left, middle), right);
}

inline void remove(root& o, int id1, int id2) {
    int k1 = rank(o, id1);
    int k2 = rank(o, id2+1);
    if( k1 >= k2 ) return;

    root left, middle, right;
    split(o, k1, left, right);
    split(right, k2-k1, middle, right);
    o = merge(left, right);
}

inline int query(root& o, int key) {
    if( o == null ) return 0;
    if( key < o->key ) return query(o->lson, key);
    return std:: max(o->key, query(o->rson, key));
}

root rt;
void Init() {
    null = new node();
    null->key = 0;
    null->siz = 0;
    null->lson = NULL;
    null->rson = NULL;

    nodetop = nodepool;
    rt = newnode(0);
    rt->rson = newnode(1000000001);
}

int N, arg1, arg2, arg3;
char cmd[20];
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
    Init();
    N = read();
    for(int i=1; i <= N; ++i) {
        scanf("%s", cmd);
        arg1 = std:: min(std:: max(read(), 1), 1000000000);
        if( cmd[0] == 'D' ) arg2 = std:: min(std:: max(read(), 1), 1000000000);

        switch( cmd[0] ) {
            case 'I': insert(rt, arg1); break;
            case 'Q': printf("%lld\n", query(rt, arg1)); break;
            case 'D': remove(rt, arg1, arg2); break;
        }
    }
    return 0;
}
