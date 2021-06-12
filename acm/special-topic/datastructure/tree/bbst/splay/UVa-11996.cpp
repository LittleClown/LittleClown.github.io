#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ULL;
const int MAXN = 400000+10;
const int hashkey = 137;

ULL xp[MAXN];

struct node {
    int key;
    int siz;
    ULL val;
    ULL reval;
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
        swap(lson, rson);
        swap(val, reval);
    }
    void pushdown() {
        if( !flip ) return;
        lson->reverse();
        rson->reverse();
        flip = false;
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
        val = lson->val + key*xp[lson->siz] + rson->val*xp[lson->siz+1];
        reval = rson->reval + key*xp[rson->siz] + lson->reval*xp[rson->siz+1];
    }
};

typedef node* root;
typedef pair<node*, node*> droot;
const int MAX_NODES = 400000+10;

node* null;
node* nodetop;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->val = 0;
    nodetop->reval = 0;
    nodetop->flip = false;
    nodetop->lson = null;
    nodetop->rson = null;
    return nodetop++;
}

inline void zag(root& rt) {
    root k = rt->rson;
    rt->rson = k->lson;
    k->lson = rt;
    rt = k;
}
inline void zig(root& rt) {
    root k = rt->lson;
    rt->lson = k->rson;
    k->rson = rt;
    rt = k;
}
inline void rotate(root& rt, int d) {
    d? zig(rt): zag(rt);
    d? rt->rson->maintain(): rt->lson->maintain();
    rt->maintain();
}

inline void splay(root& rt, int k) {
    rt->pushdown();
    int d = rt->cmp(k);
    if( d == 1 ) k -= rt->lson->siz+1;
    if( d != -1 ) {
        root& pt = d? rt->rson: rt->lson;
        pt->pushdown();
        int d2 = pt->cmp(k);
        if( d2 == 1 ) k -= pt->lson->siz+1;
        if( d2 != -1 ) {
            splay((d2? pt->rson: pt->lson), k);
            if( d == d2 ) rotate(rt, d^1);
            else rotate(pt, d);
        }
        rotate(rt, d^1);
    }
}

inline void split(root rt, int k, root& left, root& right) {
    splay(rt, k);
    left = rt;
    right = rt->rson;
    rt->rson = null;
    rt->maintain();
}

inline root merge(root left, root right) {
    splay(left, left->siz);
    left->rson = right;
    left->maintain();
    return left;
}

/* insert at (k+1)th position. */
inline void insert(root& rt, int k, int key) {
    root left, right;
    root middle = newnode(key);
    split(rt, k, left, right);
    rt = merge(merge(left, middle), right);
}

/* remove at (k+1)th position. */
inline void remove(root& rt, int k) {
    root left, middle, right;
    split(rt, k, left, right);
    split(right, 1, middle, right);
    rt = merge(left, right);
}

/* modify [lft+1, rht+1]. */
inline void update(root& rt, int lft, int rht) {
    splay(rt, lft);
    splay(rt->rson, rht-lft+2);
    rt->rson->lson->reverse(); /* update rt->rson->lson, rt->rson, rt */
    rt->rson->maintain();
    rt->maintain();
}

inline int query(root& rt, int p1, int p2) {
    int lft = 0, rht = rt->siz - p2;
    while( lft < rht ) {
        int mid = (lft+rht) >> 1;
        splay(rt, p1);
        splay(rt->rson, mid+1);
        ULL val1 = rt->rson->lson->val;
        splay(rt, p2);
        splay(rt->rson, mid+1);
        ULL val2 = rt->rson->lson->val;
        if( val1 == val2 ) lft = mid+1;
        else rht = mid;
    }
    return lft-1;
}

root rt;
char s[MAXN];
int N, Q, op, arg1, arg2;

inline void build(root& rt, int lft, int rht) {
    int mid = (lft+rht) >> 1;
    rt = newnode(s[mid]-'0');
    if( lft < mid ) build(rt->lson, lft, mid-1);
    if( mid < rht ) build(rt->rson, mid+1, rht);
    rt->maintain();
}

inline void Init() {
    nodetop = nodepool;
    scanf("%s", s+1);
    s[0] = s[N+1] = '0';
    build(rt, 0, N+1);
}

inline int read() {
    char c = getchar();
    int s = 0;
    for(; c < '0' || c > '9'; c=getchar());
    for(; c >= '0' && c <= '9'; c=getchar())
        s = s*10 + c-'0';
    return s;
}

int main()
{ 
    null = new node();
    memset(null, 0, sizeof(node));
    xp[0] = 1;
    for(int i=1; i < MAXN; ++i)
        xp[i] = xp[i-1] * hashkey;

    while( scanf("%d%d", &N, &Q) == 2 ) {
        Init();
        while( Q-- ) {
            op = read();
            arg1 = read();
            if( op != 2 ) arg2 = read();
            switch( op ) {
                case 1: insert(rt, arg1+1, arg2); break;
                case 2: remove(rt, arg1); break;
                case 3: update(rt, arg1, arg2); break;
                case 4: printf("%d\n", query(rt, arg1, arg2)); break;
            }
        }
    } 
    return 0;
}
