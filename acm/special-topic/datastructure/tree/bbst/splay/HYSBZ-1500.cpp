#include <bits/stdc++.h>

const int INF = 0x3f3f3f3f;

struct node {
    int key;
    int siz;
    int setv;   // 懒惰标记，表示是否标记为同一个值
    int sumv;   // 该节点为根的子树的 $\sum key$
    int mxlv;   // 该节点为根的子树表示的序列左侧（可以为空）最大连续和
    int mxmv;   // 该节点为根的子树最大连续和
    int mxrv;   // 该节点为根的子树表示的序列右侧（可以为空）最大连续和
    bool flip;  // 懒惰标记，表示是否翻转
    node* lson;
    node* rson;

    static node* null;

    int cmp(int x) {
        int cnt = lson->siz + 1;
        if( x == cnt ) return -1;
        return x < cnt? 0: 1;
    }
    void reverse() {
        flip ^= 1;
        std:: swap(lson, rson);
        std:: swap(mxlv, mxrv);
    }
    void update(int tag) {
        setv = tag;
        key = tag;
        sumv = tag * siz;
        mxlv = mxrv = tag > 0? sumv: 0;
        mxmv = tag > 0? sumv: tag;
    }
    void pushdown() {
        if( flip ) {
            lson->reverse();
            rson->reverse();
            flip = false;
        }
        if( setv != -INF ) {
            if( lson != null ) lson->update(setv);
            if( rson != null ) rson->update(setv);
            setv = -INF;
        }
    }
    void maintain() {
        siz = lson->siz + 1 + rson->siz;
        sumv = lson->sumv + key + rson->sumv;
        mxlv = std:: max(lson->mxlv, lson->sumv + key + rson->mxlv);
        mxrv = std:: max(rson->mxrv, lson->mxrv + key + rson->sumv);
        mxmv = std:: max(lson->mxmv, rson->mxmv);
        mxmv = std:: max(mxmv, lson->mxrv + key + rson->mxlv);
    }
};

typedef node* root;
node* node:: null = new node();

inline void printtree(root o, int cur=0) {
    if( o == node:: null ) return;
    o->pushdown();
    o->maintain();
    printtree(o->lson, cur+1);
    printf("cur %d: key=%d, siz=%d, sum=%d\n", cur, o->key, o->siz, o->sumv);
    printtree(o->rson, cur+1);
    if( !cur ) printf("-------------------------------------\n");
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
    o->rson = node:: null;
    o->maintain();
}

inline root merge(root left, root right) {
    splay(left, left->siz);
    left->rson = right;
    left->maintain();
    return left;
}

/********************** 以上为 splay 基本操作 *******************/

const int MAX_NODES = 1000000+10;
std:: queue<root> Qnodepool;
node nodepool[MAX_NODES];

inline root newnode(int key=0) {
    root nodetop = Qnodepool.front(); Qnodepool.pop();
    nodetop->key = key;
    nodetop->siz = 1;
    nodetop->setv = -INF;
    nodetop->sumv = key;
    nodetop->mxlv = key > 0? key: 0;
    nodetop->mxmv = key;
    nodetop->mxrv = key > 0? key: 0;
    nodetop->flip = false;
    nodetop->lson = node:: null;
    nodetop->rson = node:: null;
    return nodetop;
}

inline void deletenode(root o) {
    if( o == node:: null ) return;
    deletenode(o->lson);
    deletenode(o->rson);
    Qnodepool.push(o);
}

inline void build(root& o, int lft, int rht, int* A) {
    int mid = (lft+rht) >> 1;
    o = newnode(A[mid]);
    if( lft < mid ) build(o->lson, lft, mid-1, A);
    if( mid < rht ) build(o->rson, mid+1, rht, A);
    if( A[mid] == -INF ) o->sumv = 0;
    o->maintain();
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

const int MAXN = 500000+10;
int A[MAXN];
root rt;

inline void INSERT(int pos, int tot) {
    for(int i=1; i <= tot; ++i) A[i] = read();
    root left, middle, right;
    build(middle, 1, tot, A);
    split(rt, pos+1, left, right);
    rt = merge(merge(left, middle), right);
}

inline void DELETE(int pos, int tot) {
    splay(rt, pos);
    splay(rt->rson, tot+1);
    
    deletenode(rt->rson->lson);

    rt->rson->lson = node:: null;
    rt->rson->maintain();
    rt->maintain();
}

inline void MODIFY(int pos, int tot, int tag) {
    splay(rt, pos);
    splay(rt->rson, tot+1);
    rt->rson->lson->update(tag);
    rt->rson->maintain();
    rt->maintain();
}

inline void REVERSE(int pos, int tot) {
    splay(rt, pos);
    splay(rt->rson, tot+1);
    rt->rson->lson->reverse();
    rt->rson->maintain();
    rt->maintain();
}

inline int GETSUM(int pos, int tot) {
    splay(rt, pos);
    splay(rt->rson, tot+1);
    return rt->rson->lson->sumv;
}

inline int MAXSUM() {
    return rt->mxmv;
}

inline void init() {
    while( !Qnodepool.empty() ) Qnodepool.pop();
    for(int i=0; i < MAX_NODES; ++i) Qnodepool.push(nodepool+i);
    node:: null->key = -INF;
    node:: null->siz = 0;
    node:: null->setv = -INF;
    node:: null->sumv = 0;
    node:: null->mxlv = 0;
    node:: null->mxmv = -INF;
    node:: null->mxrv = 0;
    node:: null->flip = false;
    node:: null->lson = NULL;
    node:: null->rson = NULL;
}

int main()
{
    init();

    int N = read();
    int Q = read();

    for(int i=1; i <= N; ++i) A[i] = read();
    A[0] = A[N+1] = -INF;
    build(rt, 0, N+1, A);
    while( Q-- ) {
        char cmd[20];
        scanf("%s", cmd);
        if( cmd[0] == 'M' && cmd[2] == 'X' ) {
            printf("%d\n", MAXSUM());
            continue;
        }

        int arg1 = read();
        int arg2 = read();

        switch( cmd[0] ) {
            case 'I': INSERT(arg1, arg2); break;
            case 'D': DELETE(arg1, arg2); break;
            case 'M': MODIFY(arg1, arg2, read()); break;
            case 'R': REVERSE(arg1, arg2); break;
            case 'G': printf("%d\n", GETSUM(arg1, arg2)); break;
        }
    }

    return 0;
}
