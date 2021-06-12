#include <bits/stdc++.h>

struct node {
    char key;
    int siz;
    bool flip;
    node* lson;
    node* rson;

    int cmp(int x) {
        int cnt = lson->siz + 1;
        if( x == cnt ) return -1;
        return x < cnt? 0: 1;
    }
    void reverse() {
        flip ^= 1;
        std:: swap(lson, rson);
    }
    void pushdown() {
        if( !flip ) return;
        lson->reverse();
        rson->reverse();
        flip ^= 1;
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
    }
};

typedef node* root;
const int MAX_NODES = (1<<22) + 10;
node* null;
node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(char key) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->flip = false;
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

inline void build(root& o, int lft, int rht, char* s) {
    int mid = (lft+rht) >> 1;
    o = newnode(s[mid]);
    if( lft < mid ) build(o->lson, lft, mid-1, s);
    if( mid < rht ) build(o->rson, mid+1, rht, s);
    o->maintain();
}

const int MAXN = (1<<22) + 10;
char s[MAXN];

inline void Move(root& rt, int k) { splay(rt, k); }
inline void Insert(root& rt, int n) {
    for(s[1]=getchar(); s[1] < 32 || s[1] > 126;) s[1] = getchar();
    for(int i=2; i <= n; ++i) s[i] = getchar();
    root left = rt;
    root middle; build(middle, 1, n, s);
    root right = rt->rson;
    rt->rson = null;
    rt->maintain();
    rt = merge(left, merge(middle, right));
}
inline void Delete(root& rt, int n) {
    splay(rt->rson, n);
    rt->rson = rt->rson->rson;
    rt->maintain();
}
inline void Rotate(root& rt, int n) {
    splay(rt->rson, n+1);
    rt->rson->lson->reverse();
}
inline void Get(root& rt) {
    root o = rt->rson;
    for(; o->lson != null; o=o->lson) o->pushdown();
    printf("%c\n", o->key);
}
inline void Prev(root& rt) { splay(rt, rt->lson->siz); } 
inline void Next(root& rt) { splay(rt, rt->lson->siz+2); }

root rt;
inline void Init() {
    null = new node();
    null->key = '\0';
    null->siz = 0;
    null->flip = false;
    null->lson = NULL;
    null->rson = NULL;

    nodetop = nodepool;
    rt = newnode(31);
    rt->rson = newnode(127);
}

inline int read() {
    bool positive = true;
    char c = getchar();
    int s = 0;
    for(; c < '0' || c > '9'; c=getchar())
        if( c == '-' ) positive = false;
    for(; c >= '0' && c <= '9'; c=getchar())
        s = s*10 + c-'0';
    return s;
}

char cmd[20];

int main()
{
    Init();

    int N = read();
    for(int i=1; i <= N; ++i) {
        scanf("%s", cmd);
        switch( cmd[0] ) {
            case 'M': Move(rt, read()+1); break;
            case 'I': Insert(rt, read()); break;
            case 'D': Delete(rt, read()); break;
            case 'R': Rotate(rt, read()); break;
            case 'G': Get(rt); break;
            case 'P': Prev(rt); break;
            case 'N': Next(rt); break;
        }
    }
    return 0;
}
