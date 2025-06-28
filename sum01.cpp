#include<bits/stdc++.h>
using namespace std;
// int dp[N]
int N;
int M;

vector<int> step;
void dfs(int index,int c,const vector<vector<bool>>& dp,vector<vector<int>>& res,const vector<int> &val){
    if(index==0){
        for(int i=N;i>=1;--i){
            res[i].push_back(step[N-i]);
        }
        return;
    }

    if(dp[index-1][c]){
        step.push_back(0);
        dfs(index-1,c,dp,res,val);
        step.pop_back();
    }
    if(c>=val[index]&&dp[index-1][c-val[index]]){
        step.push_back(1);
        dfs(index-1,c-val[index],dp,res,val);
        step.pop_back();
    }
}
signed main(){
    cin>>N>>M;
    vector<vector<bool>> dp(N+5, vector<bool>(M+5, 0));
    vector<int> val(N+5);
    vector<vector<int>> res(N+5);

    for(int i=1;i<=N;++i){
        cin>>val[i];
    }

    for(int i=0;i<N;++i){
        dp[i][0]=true;
    }

    for(int i=1;i<=N;++i){
        for(int j=1;j<=M;++j){
            if(j>=val[i]){
                dp[i][j]=(dp[i-1][j]|dp[i-1][j-val[i]]);
            }else{
                dp[i][j]=dp[i-1][j];
            }
        }
    }

    int maxx=M;
    while(!dp[N][maxx]){
        maxx--;
    }

    dfs(N,maxx,dp,res,val);

    for(int i=1;i<=N;++i){
        for(auto j:res[i]){
            cout<<j<<" ";
        }
        cout<<endl;
    }

    return 0;
}
