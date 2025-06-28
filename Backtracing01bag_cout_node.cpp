#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static ll node_cnt = 0;
int N; ll W_cap;
struct Item { int index; ll weight, value; double ratio; };
vector<Item> items;
vector<int> currX, bestX;
ll currW = 0, currV = 0, bestV = 0;

bool cmpItem(const Item &a, const Item &b) { return a.ratio > b.ratio; }

double bound(int i, ll remW) {
    double b = currV;
    for(int j = i; j < N; ++j) {
        if(items[j].weight <= remW) { remW -= items[j].weight; b += items[j].value; }
        else { b += items[j].ratio * remW; break; }
    }
    return b;
}

void backtrack(int i) {
    node_cnt++;
    if(i == N) {
        if(currV > bestV) { bestV = currV; bestX = currX; }
        return;
    }
    // 选
    if(currW + items[i].weight <= W_cap) {
        currX[i] = 1;
        currW += items[i].weight;
        currV += items[i].value;
        backtrack(i+1);
        currW -= items[i].weight;
        currV -= items[i].value;
    }
    // 不选且可能剪枝
    if(bound(i+1, W_cap - currW) > bestV) {
        currX[i] = 0;
        backtrack(i+1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> W_cap;
    items.resize(N);
    for(int i = 0; i < N; ++i) { items[i].index = i; cin >> items[i].value; }
    for(int i = 0; i < N; ++i) { cin >> items[i].weight; }
    for(auto &it : items) it.ratio = (double)it.value / it.weight;
    sort(items.begin(), items.end(), cmpItem);

    currX.assign(N, 0);
    bestX.assign(N, 0);
    backtrack(0);

    vector<int> ans(N);
    for(int i = 0; i < N; ++i) if(bestX[i]) ans[items[i].index] = 1;
    for(int x : ans) cout << x << '\n';

    cerr << "NODE_CNT=" << node_cnt << '\n';
    return 0;
}