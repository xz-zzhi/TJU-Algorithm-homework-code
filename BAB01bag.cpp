#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Item {
    int index;
    ll weight;
    ll value;
    double ratio;
};

// 比较函数，按价值密度降序
bool cmp(const Item& a,const Item& b) {
    return a.ratio>b.ratio;
}

// 分支界限节点
struct Node {
    int idx;
    ll W;
    ll V;
    double ub;
    vector<int> a;
};

// 策略：选剩余物品单价最高的，把能放的物品放进背包，如果碰到某个物品放不下，就按它的单价填满背包的剩余空间
void calcUb(Node& node,ll C,const vector<Item>& items) {
    ll w=node.W;
    double bound=node.V;
    int n=items.size();
    int i=node.idx;
    while(i<n&&w+items[i].weight<=C) {
        w+=items[i].weight;
        bound+=items[i].value;
        ++i;
    }
    if(i<n) {
        bound+=(C-w)*items[i].ratio;
    }
    node.ub=bound;
}

struct CompareNode {
    bool operator()(const Node& a,const Node& b) const {
        return a.ub<b.ub;
    }
};

int main() {
    int N;
    ll C;
    cin>>N>>C;

    vector<Item> items(N);
    for(int i=0; i<N; ++i) {
        items[i].index=i;
        cin>>items[i].value;
    }
    for(int i=0; i<N; ++i) {
        cin>>items[i].weight;
        items[i].ratio=(double)items[i].value/items[i].weight;
    }

    // 按价值密度排序
    sort(items.begin(),items.end(),cmp);

    priority_queue<Node,vector<Node>,CompareNode> pq;

    Node root;
    root.idx=0;
    root.W=0;
    root.V=0;
    root.a.assign(N,0);
    calcUb(root,C,items);
    pq.push(root);

    ll bestV=0;
    vector<int> bestA(N,0);

    while(!pq.empty()) {
        Node cur=pq.top(); pq.pop();

        if(cur.ub<=bestV) break; // 剩下节点最优已经不能再更新最后的答案

        // 更新最优，但并不是到达最后节点才进行更新
        if(cur.V>bestV) {
            bestV=cur.V;
            bestA=cur.a;
        }

        if(cur.idx<N) {
            // 不选当前物品，左节点
            Node left=cur;
            left.idx=cur.idx+1;
            calcUb(left,C,items);

            if(left.ub>bestV) pq.push(left);// 只有可能更新答案才加入到队列中

            // 选当前物品，右节点
            if(cur.W+items[cur.idx].weight<=C) {
                Node right=cur;
                right.W=cur.W+items[cur.idx].weight;
                right.V=cur.V+items[cur.idx].value;
                right.a[cur.idx]=1;
                right.idx=cur.idx+1;
                calcUb(right,C,items);
                if(right.ub>bestV) pq.push(right);// 只有可能更新答案才加入到队列中
            }
        }
    }

    // 输出决策
    vector<int> ans(N);
    for(int i=0; i<N; ++i) ans[items[i].index]=bestA[i];
    for(int x:ans) cout<<x<<'\n';
    return 0;
}

