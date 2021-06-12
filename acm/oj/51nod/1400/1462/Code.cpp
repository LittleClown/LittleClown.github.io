#pragma comment(linker, "/STACK:102400000,102400000")
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define lc (o<<1)
#define rc (o<<1|1)
using namespace std;

typedef long long LL;
const int MAXN = 100000 + 10;

int N, Q, u, v, w;
int from[MAXN], to[MAXN], nxt[MAXN], cnt;
int father[MAXN], son[MAXN], siz[MAXN], dep[MAXN];
int pos[MAXN], top[MAXN], dfs_cnt;
LL sumv[MAXN << 2], addv[MAXN << 2], mulv[MAXN << 2], ans[MAXN];

void Pushdown(int o, int lft, int rht) {
    sumv[o] += addv[o] * mulv[o];
    if (lft < rht) {
        mulv[lc] += mulv[o];
        mulv[rc] += mulv[o];
    }
    mulv[o] = 0;
}
int ul, ur, uv, uc;
void Update(int o, int lft, int rht, LL add) {
    if (ul <= lft && rht <= ur) {
        if (uc == 1) {
            Pushdown(o, lft, rht);
            addv[o] += uv;
        }
        else {
            mulv[o] += uv;
            sumv[o] += add;
        }
    }
    else {
        add += uv * addv[o];
        Pushdown(o, lft, rht);
        int mid = lft + rht >> 1;
        if (ul <= mid) Update(lc, lft, mid, add);
        if (ur > mid) Update(rc, mid + 1, rht, add);
    }
}
void Query(int o, int lft, int rht) {
    if (lft == rht) ans[lft] = sumv[o] + addv[o] * mulv[o];
    else {
        Pushdown(o, lft, rht);
        sumv[lc] += sumv[o];
        sumv[rc] += sumv[o];
        int mid = lft + rht >> 1;
        Query(lc, lft, mid);
        Query(rc, mid + 1, rht);
    }
}
void dfs(int o) {
    for (int u = from[o]; u; u = nxt[u]) {
        int v = to[u];
        dep[v] = dep[o] + 1;
        dfs(v);
        siz[o] += siz[v];
        if (siz[son[o]] < siz[v]) son[o] = v;
    }
}
void dfs(int o, int cur) {
    pos[o] = ++dfs_cnt;
    top[o] = cur;
    if (son[o]) dfs(son[o], cur);
    for (int u = from[o]; u; u = nxt[u]) {
        int v = to[u];
        if (v != son[o]) dfs(v, v);
    }
}
void Update(int L, int R) {
    while (top[L] != top[R]) {
        if (dep[top[L]] < dep[top[R]]) swap(L, R);
        ul = pos[top[L]]; ur = pos[L];
        Update(1, 1, N, 0LL);
        L = father[top[L]];
    }
    if (dep[L] > dep[R]) swap(L, R);
    ul = pos[L]; ur = pos[R];
    Update(1, 1, N, 0LL);
}
int main()
{
    scanf("%d", &N);
    for (int i = 2; i <= N; ++i) {
        scanf("%d", &u);
        father[i] = u;
        to[++cnt] = i; nxt[cnt] = from[u]; from[u] = cnt;
    }
    for (int i = 1; i <= N; ++i) siz[i] = 1;
    dfs(1); dfs(1, 1);
    scanf("%d", &Q);
    ul = 1;
    while(Q--) {
        scanf("%d%d%d", &uc, &ur, &uv);
        Update(ul, ur);
    }
    Query(1, 1, N);
    for (int i = 1; i <= N; ++i) printf("%I64d\n", ans[pos[i]]);
    return 0;
}
