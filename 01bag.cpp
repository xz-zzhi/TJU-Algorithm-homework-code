#include <bits/stdc++.h>
using namespace std;

int N;
int M;

signed main(){
    cin>>N>>M;
    vector<vector<int>> dp(N+5,vector<int>(M+5,0));
    vector<int> val(N+5);
    vector<int> wei(N+5);
    vector<int> ans(N+5);
    vector<vector<int>> change(N+5,vector<int>(M+5,0));

    for(int i=1;i<=N;++i) cin>>val[i];
    for(int i=1;i<=N;++i) cin>>wei[i];

    for(int i=1;i<=N;++i){
        for(int j=0;j<=M;++j){
            if(j>=wei[i]){
                if(dp[i-1][j]>=dp[i-1][j-wei[i]]+val[i]){
                    change[i][j]=0;
                    dp[i][j]=dp[i-1][j];
                }
                else{
                    change[i][j]=1;
                    dp[i][j]=dp[i-1][j-wei[i]]+val[i];
                }
            }
            else{
                change[i][j]=0;
                dp[i][j]=dp[i-1][j];
            }
        }
    }

    int index=N;
    int c=M;
    while(index){
        ans[index]=change[index][c];
        if(change[index][c]){
            c-=wei[index];
            index--;
        }else{
            index--;
        }
    }

    for(int i=1;i<=N;++i){
        cout<<ans[i]<<endl;
    }
    
    return 0;
}
