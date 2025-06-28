import subprocess
import time
import psutil
import pandas as pd
import numpy as np

# ========== 可调参数 ==========
executables = ["01bag", "BAB01bag", "Backtracing01bag"]
seeds = list(range(1, 8))   # 种子 1-7
N = 30                       # 物品数量
C = 500                      # 背包容量
# =============================

records = []

for seed in seeds:
    # 1. 生成随机数据集
    rng = np.random.default_rng(seed)
    values = rng.integers(1, 101, size=N)
    weights = rng.integers(1, 101, size=N)
    input_data = (
        f"{N} {C}\n" +
        " ".join(map(str, values)) + "\n" +
        " ".join(map(str, weights)) + "\n"
    ).encode()

    for exe in executables:
        # 2. 启动子进程
        start_time = time.perf_counter()
        proc = psutil.Popen([f"./{exe}"], stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        proc.stdin.write(input_data)
        proc.stdin.close()

        # 3. 轮询内存使用，记录峰值
        max_rss = 0
        while proc.is_running():
            try:
                mem = proc.memory_info().rss
                if mem > max_rss:
                    max_rss = mem
            except psutil.NoSuchProcess:
                break

        proc.wait()
        end_time = time.perf_counter()

        # 4. 记录结果
        records.append({
            "Seed": seed,
            "Executable": exe,
            "Time_s": end_time - start_time,
            "Mem_KB": max_rss // 1024
        })

# 构建 DataFrame 并显示
df = pd.DataFrame(records)
df["Time_s"] = df["Time_s"].round(4)
df["Mem_KB"] = df["Mem_KB"].astype(int)

import ace_tools as tools; tools.display_dataframe_to_user("Run Performance per Seed and Executable", df)

# 提示
print(
    "说明：表格展示了 7 个随机数据集（种子 1-7），规模 N=30, C=500。\n"
    "可修改脚本顶部的 `executables`, `seeds`, `N`, `C` 来调整测试程序、数据集个数和规模。"
)

