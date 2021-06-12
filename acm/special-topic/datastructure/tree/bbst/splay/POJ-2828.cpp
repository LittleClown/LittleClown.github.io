#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

struct node {
    int key;
    int siz;
    node* lson;
    node* rson;

    static node* null;

    int cmp(int x) {
        int cnt = lson->siz+1;
        if( x == cnt ) return -1;
        return x < cnt? 0: 1;
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
    o->rson = node:: null;
    o->maintain();
}

const int MAX_NODES = 200000+10;

node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 0;
    nodetop->lson = node:: null;
    nodetop->rson = node:: null;
    return nodetop++;
}

inline void insert(root& rt, int pos, int key) {
    root left, right;
    split(rt, pos+1, left, right);
    left->rson = newnode(key);
    left->rson->rson = right;
    left->rson->maintain();
    left->maintain();
    rt = left;
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

node* node:: null = new node();
root rt;
int N;

inline void init() {
    node:: null->key = 0;
    node:: null->siz = 0;
    node:: null->lson = NULL;
    node:: null->rson = NULL;
}

inline void printtree(root& o) {
    if( o == node:: null ) return;
    printtree(o->lson);
    print(o->key); putchar(' ');
    printtree(o->rson);
}

int main()
{
    init();
    while( scanf("%d", &N) == 1 ) {
        nodetop = nodepool;
        rt = newnode(0);
        for(int i=1; i <= N; ++i) {
            int arg1 = read();
            int arg2 = read();
            insert(rt, arg1, arg2);
        }
        splay(rt, 1);
        printtree(rt->rson);
        printf("\n");
    }
    return 0;
}
