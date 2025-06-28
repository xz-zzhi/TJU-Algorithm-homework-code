#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static ll node_cnt = 0;

struct Item { int index; ll weight, value; double ratio; };
bool cmpItem(const Item &a, const Item &b) { return a.ratio > b.ratio; }

struct Node { int idx; ll W, V; double ub; vector<int> a; };
struct CompareNode { bool operator()(const Node &a, const Node &b) const { return a.ub < b.ub; } };

void calcUb(Node &node, ll C, const vector<Item> &items) {
    ll w = node.W; double bound = node.V;
    int n = items.size(), i = node.idx;
    while(i < n && w + items[i].weight <= C) {
        w += items[i].weight; bound += items[i].value; ++i;
    }
    if(i < n) bound += (C - w) * items[i].ratio;
    node.ub = bound;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N; ll C;
    cin >> N >> C;
    vector<Item> items(N);
    for(int i = 0; i < N; ++i) { items[i].index = i; cin >> items[i].value; }
    for(int i = 0; i < N; ++i) { cin >> items[i].weight; items[i].ratio = (double)items[i].value / items[i].weight; }
    sort(items.begin(), items.end(), cmpItem);

    priority_queue<Node, vector<Node>, CompareNode> pq;
    Node root; root.idx = 0; root.W = root.V = 0; root.a.assign(N, 0);
    calcUb(root, C, items); pq.push(root);

    ll bestV = 0; vector<int> bestA;

    while(!pq.empty()) {
        node_cnt++;
        Node cur = pq.top(); pq.pop();
        if(cur.ub <= bestV) break;
        if(cur.V > bestV) { bestV = cur.V; bestA = cur.a; }
        if(cur.idx < N) {
            Node left = cur; left.idx = cur.idx + 1; calcUb(left, C, items);
            if(left.ub > bestV) pq.push(left);
            if(cur.W + items[cur.idx].weight <= C) {
                Node right = cur;
                right.W = cur.W + items[cur.idx].weight;
                right.V = cur.V + items[cur.idx].value;
                right.a[cur.idx] = 1;
                right.idx = cur.idx + 1;
                calcUb(right, C, items);
                if(right.ub > bestV) pq.push(right);
            }
        }
    }
    // 输出结果
    vector<int> ans(N);
    for(int i = 0; i < N; ++i) ans[items[i].index] = bestA[i];
    for(int x : ans) cout << x << '\n';

    // 输出节点计数
    cerr << "NODE_CNT=" << node_cnt << '\n';
    return 0;
}