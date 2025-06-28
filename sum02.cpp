#include<bits/stdc++.h>
using namespace std;
// int dp[N]
int N;
int M;

vector<int> step;

signed main(){
    cin>>N>>M;
    vector<vector<bool>> dp(N+5, vector<bool>(M+5, 0));
    vector<int> val(N+5);
    vector<int> res(N+5);
    vector<vector<int>> change(N+5,vector<int>(M+5,0));
    
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
                if(!dp[i][j]){
                    continue;
                }
                if(dp[i-1][j]){
                    change[i][j]=0;
                }else{
                    change[i][j]=1;
                }
            }else{
                dp[i][j]=dp[i-1][j];
                change[i][j]=0;
            }
        }
    }

    int c=M;
    while(!dp[N][c]){
        c--;
    }
    int index=N;
    while(index){
        if(change[index][c]){
            res[index]=1;
            c-=val[index];
            index--;
        }else{
            res[index]=0;
            index--;
        }
    }

    for(int i=1;i<=N;++i){
        cout<<res[i]<<endl;
    }

    return 0;
}
