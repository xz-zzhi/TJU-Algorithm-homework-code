#!/usr/bin/env python3
import subprocess
import time
import pandas as pd
import numpy as np
import os
import argparse

# 超时阈值（秒）
TIMEOUT = 10

def parse_args():
    p = argparse.ArgumentParser(description='0/1 背包算法完整测试')
    p.add_argument('--outdir', default='datasets', help='数据集目录')
    return p.parse_args()

# 可执行文件与名称映射
executables = ['./01bag', './BAB01bag', './Backtracing01bag']
name_map = {
    './01bag': '动态规划',
    './BAB01bag': '分支限界',
    './Backtracing01bag': '回溯法'
}

# 实验数据集定义 A-L
environments = [
    {'label':'A','mode':'normal','N':200,'C':100},
    {'label':'B','mode':'normal','N':500,'C':1000},
    {'label':'C','mode':'heavy','N':200,'C':100},
    {'label':'D','mode':'heavy','N':500,'C':1000},
    {'label':'E','mode':'smallcap','N':200,'C':100},
    {'label':'F','mode':'smallcap','N':500,'C':1000},
    {'label':'G','mode':'ratio_constant','N':200,'C':100},
    {'label':'H','mode':'ratio_constant','N':500,'C':1000},
    {'label':'I','mode':'all_ones','N':40,'C':20},
    {'label':'J','mode':'ratio_constant','N':60,'C':30},
    {'label':'K','mode':'anti_greedy','N':50,'C':25},
    {'label':'L','mode':'deep_exhaust','N':30,'C':15},
]

# 数据生成

def generate_dataset(mode, N, C, seed=1):
    rng = np.random.default_rng(seed)
    if mode == 'normal':
        w = rng.integers(1,101,size=N); v = rng.integers(1,101,size=N)
    elif mode == 'heavy':
        w = rng.integers(C//2, C+1, size=N); v = rng.integers(1,101,size=N)
    elif mode == 'smallcap':
        w = rng.integers(1,20,size=N); v = rng.integers(1,101,size=N)
    elif mode == 'ratio_constant':
        w = rng.integers(1,C//2+1,size=N); v = (w*10).astype(int)
    elif mode == 'all_ones':
        w = np.ones(N, int); v = np.ones(N, int)
    elif mode == 'anti_greedy':
        half = N//2
        w1 = rng.integers(20,C+1,size=half); v1 = (w1*5).astype(int)
        w2 = rng.integers(1,5,size=N-half); v2 = rng.integers(1,5,size=N-half)
        w = np.concatenate([w1,w2]); v = np.concatenate([v1,v2])
    elif mode == 'deep_exhaust':
        w = np.ones(N,int); v = rng.integers(1,10,size=N)
        perm = rng.permutation(N); w = w[perm]; v = v[perm]
    else:
        raise ValueError(mode)
    return v, w

# Python DP 用于校验最优值

def python_dp(values, weights, C):
    N = len(values)
    dp = [0]*(C+1)
    for i in range(N):
        vi, wi = values[i], weights[i]
        for cap in range(C, wi-1, -1):
            dp[cap] = max(dp[cap], dp[cap-wi]+vi)
    return max(dp)

# 主流程
def main():
    args = parse_args()
    os.makedirs(args.outdir, exist_ok=True)
    records = []

    # 生成所有数据集文件
    for exp in environments:
        v, w = generate_dataset(exp['mode'], exp['N'], exp['C'])
        path = os.path.join(args.outdir, f"{exp['label']}.txt")
        with open(path,'w') as f:
            f.write(f"{exp['N']} {exp['C']}\n")
            f.write(' '.join(map(str,v))+'\n')
            f.write(' '.join(map(str,w))+'\n')
        exp['values'], exp['weights'] = v, w

    # 执行测试
    for exp in environments:
        path = os.path.join(args.outdir, f"{exp['label']}.txt")
        data = open(path,'rb').read()
        vlist, wlist = exp['values'], exp['weights']
        best_dp = python_dp(vlist, wlist, exp['C'])
        for exe in executables:
            start = time.perf_counter()
            try:
                proc = subprocess.run([exe], input=data,
                                      stdout=subprocess.PIPE,
                                      stderr=subprocess.PIPE,
                                      timeout=TIMEOUT, check=True)
                elapsed = time.perf_counter() - start
                out = proc.stdout.decode().split()
                # 解校验
                if len(out)!=exp['N'] or any(x not in ('0','1') for x in out):
                    status = 'wrong'
                    node_cnt = None
                else:
                    ans = list(map(int,out))
                    tw = sum(ai*wi for ai,wi in zip(ans,wlist))
                    tv = sum(ai*vi for ai,vi in zip(ans,vlist))
                    status = 'ok' if (tv==best_dp and tw<=exp['C']) else 'wrong'
                    # 从 stderr 提取节点计数
                    err = proc.stderr.decode()
                    node_cnt = None
                    for line in err.splitlines():
                        if line.startswith('NODE_CNT='):
                            node_cnt = int(line.split('=')[1])
                            break
            except subprocess.TimeoutExpired:
                elapsed = None; status = 'timeout'; node_cnt = None
            except subprocess.CalledProcessError:
                elapsed = None; status = 'error'; node_cnt = None
            records.append({
                'Dataset': exp['label'],
                'Solver': name_map[exe],
                'Time_s': elapsed,
                'Status': status,
                'NodeCount': node_cnt
            })

    df = pd.DataFrame(records)
    # 输出
    print("数据集说明：")
    for exp in environments:
        print(f" {exp['label']}: {exp['mode']}, N={exp['N']}, C={exp['C']}")
    print("\n执行状态：")
    print(df.pivot(index='Solver', columns='Dataset', values='Status'))
    print("\n运行时间(s)：")
    ok = df[df.Status=='ok']
    ttab = ok.pivot(index='Solver', columns='Dataset', values='Time_s')
    ttab['AVG'] = ttab.mean(axis=1)
    print(ttab.round(4))
    print("\n节点计数：")
    nc = df.pivot(index='Solver', columns='Dataset', values='NodeCount')
    print(nc)

if __name__ == '__main__':
    main()
