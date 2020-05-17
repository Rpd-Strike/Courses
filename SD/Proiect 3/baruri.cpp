#include <bits/stdc++.h>

using namespace std;

ifstream in("baruri.in");
ofstream out("baruri.out");

const int NMAX = 1e5;

int aib[NMAX + 5];
int N, M;

inline int LSB(int x)
{
    return x & (-x);
}

void update_aib(int pos, int val)
{
    while (pos <= NMAX) {
        aib[pos] += val;
        pos += LSB(pos);
    }
}

int query_aib(int pos)
{
    int ret = 0;
    while (pos > 0) {
        ret += aib[pos];
        pos -= LSB(pos);
    }
    return ret;
}

void query()
{
    int poz, d;
    in >> poz >> d;
    int lef = max(1, poz - d);
    int rig = min(N, poz + d);
    out << query_aib(rig) - query_aib(lef - 1) - (query_aib(poz) - query_aib(poz - 1)) << '\n';
}

void intra()
{
    int cati, unde;
    in >> cati >> unde;
    update_aib(unde, cati);
}

void pleaca()
{
    int cati, unde;
    in >> cati >> unde;
    update_aib(unde, -cati);
}

void muta()
{
    int cati, from, to;
    in >> cati >> from >> to;
    update_aib(from, -cati);
    update_aib(to, cati);
}

void solve()
{
    memset(aib, 0, sizeof(aib));

    in >> N;
    for (int i = 1;  i <= N;  ++i) {
        int x;
        in >> x;
        update_aib(i, x);
    }
    in >> M;
    for (int i = 0; i < M; ++i) {
        int type;
        in >> type;
        if (type == 0)
            query();
        else if (type == 1)
            intra();
        else if (type == 2)
            pleaca();
        else
            muta();
    }
}

int main()
{
    int T;
    in >> T;
    while (T--) {
        solve();
    }
}