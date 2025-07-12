# 期末算法大作业

库中有所有用到的代码，还有一份实验报告

# TJU-Algorithm-homework-code

2.2 算法验证类

选择使用动态规划方法解决

01背包 -- 01bag.cpp

子集和数问题 -- sum01.cpp sum02.cpp

2.3 问题求解类

针对附件 1 中某一问题的至少 2 个数据集，利用动态规划、回溯及分支限界三种算法进行求解

要求：分析不同算法解决同一问题的复杂度，归纳不同求解算法的优缺点。


# 其他说明

子集和问题中给的数据集分为两种 --

对于给定的数据集找到所有的方案，对应代码sum01.cpp，实现中采用dfs的方式搜索找到全部方案，用时较长

对于给定的数据集只找到其中一种方案，对应代码sum02.cpp，实现中采用转移数组的方式快速找到一个方案，用时很短

01背包问题只要求找到其中的至少一种方案 --

对于只找到一种方案的代码对应 01bag.cpp

对于找到所有的方案考虑依然是采用与子集和问题中相同的思路使用dfs进行搜索


问题求解，选择解决01bag问题: 

01bag.cpp 对应动态规划

Backtracing01bag.cpp 对应回溯法

BAB01bag.cpp 对应分支界限法

# 选用数据集

4.1 0/1 包问题
• [Florida State University: 8 个数据集](https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/knapsack_01.html)

4.2 子集和数问题
• [Florida State University: 7 个数据集](https://people.sc.fsu.edu/~jburkardt/datasets/subset_sum/subset_sum.html)


