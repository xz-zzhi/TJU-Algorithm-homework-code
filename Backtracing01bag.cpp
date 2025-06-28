#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Item {
    int index;
    ll weight,value;
    double ratio;
};

int N;
ll W_cap;
vector<Item> items;
vector<int> currX,bestX;
ll currWeight=0,currValue=0,bestValue=0;

//策略：选剩余物品单价最高的，把能放的物品放进背包，如果碰到某个物品放不下，就按它的单价填满背包的剩余空间
double bound(int i,ll remW) {
    double b=currValue;
    ll w=remW;
    for(int j=i; j<N; ++j) {
        if(items[j].weight<=w) {
            w-=items[j].weight;
            b+=items[j].value;
        }
        else {
            b+=items[j].ratio*w;
            break;
        }
    }
    return b;
}

void backtrack(int i) {
    if(i==N) {
        if(currValue>bestValue) {
            bestValue=currValue;
            bestX=currX;
        }
        return;
    }

    if(currWeight+items[i].weight<=W_cap) {
        currX[i]=1;
        currWeight+=items[i].weight;
        currValue+=items[i].value;
        backtrack(i+1);
        currWeight-=items[i].weight;
        currValue-=items[i].value;
    }

    if(bound(i+1,W_cap-currWeight)>bestValue) { // 不可能更新最终答案则进行剪枝
        currX[i]=0;
        backtrack(i+1);
    }
}

bool cmp(Item x,Item y){
    return x.ratio>y.ratio;
}

int main() {
    cin>>N>>W_cap;
    items.resize(N);
    for(int i=0; i<N; ++i) {
        items[i].index=i;
        cin>>items[i].value;
    }
    for(int i=0; i<N; ++i) {
        cin>>items[i].weight;
    }
    for(auto& it:items) {
        it.ratio=(double)it.value/it.weight;
    }

    sort(items.begin(),items.end(),cmp);//按照单价排序

    currX.assign(N,0);
    bestX.assign(N,0);
    backtrack(0);

    vector<int> ans(N,0);
    for(int i=0; i<N; ++i) {
        if(bestX[i]) {
            ans[items[i].index]=1;
        }
    }

    for(int i=0; i<N; ++i) {
        cout<<ans[i]<<"\n";
    }

    return 0;
}

