#include <bits/stdc++.h>
using namespace std;

struct node {
    int key;
    int siz;
    bool flip;
    node* lson;
    node* rson;
    node(int key=0): key(key), siz(0), flip(0), lson(NULL), rson(NULL) {}
    int cmp(int key) {
        int cnt = lson->siz + 1;
        if( key == cnt ) return -1;
        return key < cnt? 0: 1;
    }
    void pushdown() {
        if( !flip ) return;
        lson->flip ^= 1;
        rson->flip ^= 1;
        swap(lson, rson);
        flip = false;
    }
    void maintain() { siz = lson->siz + 1 + rson->siz; }
};

typedef node* root;
typedef pair<node*, node*> droot;
const int MAX_NODES = 100000+10;
node* null = new node();

node nodepool[MAX_NODES];
node* nodetop;

inline node* newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->flip = false;
    nodetop->lson = null;
    nodetop->rson = null;
    return nodetop++;
}

inline void zag(root& o) {
    node* k = o->rson;
    o->rson = k->lson;
    k->lson = o;
    o = k;
}
inline void zig(root& o) {
    node* k = o->lson;
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
    if( d == 1 ) k -= o->lson->siz + 1;
    if( d != -1 ) {
        root& p = d? o->rson: o->lson;
        p->pushdown();
        int d2 = p->cmp(k);
        if( d2 == 1 ) k -= p->lson->siz + 1;
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
    left->pushdown();
    left->rson = right;
    left->maintain();
    return left;
}

inline void build(root& o, int lft, int rht) {
    int mid = (lft+rht) >> 1;
    o = newnode(mid);
    if( lft < mid ) build(o->lson, lft, mid-1);
    if( mid < rht ) build(o->rson, mid+1, rht);
    o->maintain();
}

inline void print(root& o) {
    if( o == null ) return;
    o->pushdown();
    if( o->lson != null ) print(o->lson);
    if( o->key ) printf("%d\n", o->key);
    if( o->rson != null ) print(o->rson);
}

root rt;
inline void init(int N) {
    nodetop = nodepool;
    build(rt, 0, N);
}

int main()
{
    int N, Q;
    while( scanf("%d%d", &N, &Q) == 2 ) {
        init(N);
        while( Q-- ) {
            int lft, rht;
            root o, left, middle, right;
            scanf("%d%d", &lft, &rht);
            split(rt, lft, left, o);
            split(o, rht-lft+1, middle, right);
            middle->flip ^= 1;
            rt=merge(merge(left, right), middle);
        }
        print(rt);
    }
    return 0;
}
