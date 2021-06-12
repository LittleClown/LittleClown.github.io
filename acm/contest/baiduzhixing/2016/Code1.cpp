#include <set>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

static const int MAXN = 10000+10;

struct node {
    node *ch[2];
    int val;
    void Maintain() {
        val = 0;
        if( ch[0] ) val = ch[0]->val;
        if( ch[1] ) val = std:: max(val, ch[1]->val);
    }
};

node nodepool[MAXN<<5];
node* nodetop;
node* root;

inline node* newnode() {
    nodetop->ch[0] = nodetop->ch[1] = NULL;
    nodetop->val = 0;
    return nodetop++;
}

void Update(node* &o, int x, int v, int d=30) {
    if( o == NULL ) o = newnode();
    if( d == -1 ) o->val = v;
    else {
        int c = (x>>d) & 1;
        Update(o->ch[c], x, v, d-1);
        o->Maintain();
    }
}

int Query(node* &o, int x, int ans=0, int d=30) {
    if( o == NULL ) return 0;
    if( d == -1 ) return min(o->val, ans);
    int c = (x>>d) & 1;
    if( o->ch[c^1] ) {
        if( o->ch[c^1]->val < (ans^(1<<d)) )
            return max(o->ch[c^1]->val, Query(o->ch[c], x, ans, d-1));
        return Query(o->ch[c^1], x, ans^(1<<d), d-1);
    }
    return Query(o->ch[c], x, ans, d-1);
}

inline int read() {
    int s = 0;
    char c = getchar();
    bool positive = true;
    for(; !isdigit(c); c=getchar())
        if( c == '-' ) positive = false;
    for(; isdigit(c); c=getchar())
        s = s*10 + c-'0';
    return positive? s: -s;
}

multiset<int> ms[MAXN];
int A[MAXN];
int B[MAXN], bsiz;
int dp[2][MAXN];

inline void add(int x, int v) {
    int id = lower_bound(B, B+bsiz, x) - B;
    if( ms[id].upper_bound(v) == ms[id].end() )
        Update(root, B[id], v);
    ms[id].insert(v);
}

inline void sub(int x, int v) {
    int id = lower_bound(B, B+bsiz, x) - B;
    multiset<int>:: iterator it;
    it = ms[id].find(v);
    ms[id].erase(it);
    int vv = 0;
    if( !ms[id].empty() ) {
        it = ms[id].end();
        vv = *(--it);
    }
    if( vv < v )
        Update(root, B[id], vv);
}

void work() {
    int N = read();
    int K = read();
    int L = read();
    for(int i=1; i <= N; ++i) 
        A[i] = A[i-1]^read();
    
    memcpy(B, A+1, sizeof(int)* N);
    sort(B, B+N);
    bsiz = std:: unique(B, B+N) - B;

    memset(dp[0], 0, sizeof dp[0]);
    for(int i=1; i <= L; ++i)
        dp[0][i] = A[i];

    int now = 0, last = 1;
    for(int k=2; k <= K; ++k) {
        nodetop = nodepool;
        root = newnode();
        for(int i=0; i < bsiz; ++i) ms[i].clear();

        swap(now, last);
        for(int n=1; n <= N; ++n) {
            if( n > L+1 ) sub(A[n-L-1], dp[last][n-L-1]);
            dp[now][n] = Query(root, A[n]);
            add(A[n], dp[last][n]);
        }
    }
    printf("%d\n", dp[now][N]);
}

int main()
{
    int T_T = read();
    for(int kase=1; kase <= T_T; ++kase) {
        printf("Case #%d:\n", kase);
        work();
    }
    return 0;
}
