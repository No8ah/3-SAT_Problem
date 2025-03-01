#include <iostream>
using namespace std;

// 题目已经告知: 变量数n <= 300 、 表达式数m <= 1200
// 那么我们直接将这两个常数define为max_……

#define MAX_VARS 300  // 最大变量数量n_max=300 <--> n <= n_max=300
#define MAX_CLAUSES 1200  // 最大clause个数m_max=1200 <--> m <= m_max=1200

struct Clause {
    int literals[3];  // 每个clause包含三个literal
};

int m,n;
// 存储表达式(存储的变量就是Clause结构体 <--> 存储每个单独的clause)的数组
Clause AllClauses[MAX_CLAUSES];// 因为clause最多1200个 --> 直接利用全局变量进行初始化即可
// 赋值array <--> assignment --> 1:true、0:false、-1:未赋值  
int assignment[MAX_VARS + 1];  

// 检查 "某个clause" 在当前赋值下是否为真
bool isCurrentClauseSatisfied(int clauseIndex) {
    // 实现小细节:Clauses& <--> what if clause = AllClauses[clauseIndex] <--> 相当于赋值 <--> Copy-Paste work --> or space and time complexity↑
    Clause& clause = AllClauses[clauseIndex];// 将AllClauses中的第clauseIndex个表达式引用为clause
    // 实现小细节:Clauses& <--> what if clause = AllClauses[clauseIndex] <--> 相当于赋值 <--> Copy-Paste work --> or space and time complexity↑
    // 实现'problem
    bool HasUnassigned = false;
    // 实现'problem
    // 遍历表达式中存储的三个整数
    for (int i = 0; i < 3; i++) {
        // 首先获取表达式中存储的第i个整数
        int literal = clause.literals[i];
        // 实现难点:获取变量的编号 <--> 获取变量的绝对值
            // 44 -->      x_44 --> var=44
            // -8 --> \neg x_8  --> var=8
        int num_of_var = literal > 0 ? literal : -literal;
        // 实现难点:获取变量的编号 <--> 获取变量的绝对值

        // 实现小细节: 我们现在需要access assignment[var]处的bool赋值
        int bool_val = assignment[num_of_var];
        // 实现小细节: 我们现在需要access assignment[var]处的bool赋值

        // 实现难点:
        // 如果val == -1 <--> 说明assignment[var]处未进行赋值 --> 可以跳过 <--> 无法进行判断
        if (bool_val == -1){
            // 实现'problem
            HasUnassigned = true;
            // 实现'problem
            continue;
        }  // 未赋值，无法判断
        // 实现难点:
        // literal:表示输入的整数是否\neg || val:表示实际的bool赋值
            // 考虑:x_1 \cup \neg x_2 --> x_1取T or x_2取F 即可满足要求
            // 因为: \cup             --> 只需一个满足True即可continue
        // 如果no overline && boolean == 1 || 如果overline && boolean == 0 --> 以为cup的缘故 --> 说明该表达式满足conditon <--> 直接return true即可
        if ((literal > 0 && bool_val == 1) || (literal < 0 && bool_val == 0)) {
            return true;  // 表达式为真
        }
        // 否则 <--> 说明该表达式的outcome是false --> 该表达式is not satisfied --> return false
    }
    // 此时三个都已经判断完毕了 --> 都没有返回true --> 三个都不满足 --> 返回false

    // 实现'problem
    if(HasUnassigned){
        return false;
    }
    // 实现'problem
    return false;  // 表达式为假
}

// 检查 "所有表达式" 是否都为真
bool areAllClausesSatisfied() {
    // 遍历每个表达式 并 进行判断
    for (int i = 0; i < m; i++) {
        // 如果某个表达式的outcome不为true
        if (!isCurrentClauseSatisfied(i)) {
            // 自然返回false <--> 直接break出循环了
            return false;
        }
        // 否则 <--> 某个表达式的outcome为true --> 继续判断
    }
    // 此时所有的表达式都为true <--> 我们已经找到了一组可行解 --> 返回true
    return true;
}

// 实现难点:
// 回溯求解函数
bool Solve(int varIndex) {
    // 实现难点:
    // 如果所有的变量都已经赋值完毕 --> 判断每个表达式是否都为true
    if (varIndex > n) {
        // 如果所有表达式的outcome都为true
        if (areAllClausesSatisfied()) {
            // 输出变量赋值
            for (int i = 1; i <= n; i++) {
                cout << assignment[i];
                // 实现小细节: 如果不是最后的assignment --> 输出空格" "
                if (i != n){
                    cout << " ";
                } 
                // 实现小细节: 如果不是最后的assignment --> 输出空格" "

                // 否则 <--> 如果是最后的assignment --> 直接输出数值即可
            }
            // 此时已经输出完所有的赋值结果 --> 换行
            cout << endl;
            // 说明求解成功了 --> 自然返回true
            return true;
        }
        // 否则 <--> 存在表达式 S.t. outcome不为真 <--> 存在false <--> 不满足赋值 --> 自然返回false <--> 表示还未找到可行解 --> 回溯并尝试其它赋值可能性
        return false;
    }
    // 实现难点:
    // 否则 <--> 存在变量还未被赋值 --> 我们继续进行变量赋值 --> 通过递归进行赋值

    // 实现难点: 如何完全赋值2^varIndex个可能性 --> 利用递归控制递归深度
        // 假设3个var: x_1 x_2 x_3 --> 1 1 1 --> 1 1 0  
    // 将assignment[varIndex]赋值为true
    assignment[varIndex] = 1;
    if (Solve(varIndex + 1)){
        return true;
    }
    //将assignment[varIndex]赋值为false
    assignment[varIndex] = 0;
    if (Solve(varIndex + 1)){
        return true;
    } 
    // 回溯，恢复未赋值状态
    assignment[varIndex] = -1;
    // 实现难点: 如何完全赋值2^varIndex个可能性 --> 利用递归控制递归深度
    return false;
}
// 实现难点:

int main() {
    // 初始化变量赋值为未赋值状态
    for (int i = 0; i <= MAX_VARS; i++) {
        assignment[i] = -1;// 目前还没开始赋值 <--> 还在初始化阶段
    }

    // 输入 变量数n 和 表达式个数m
    cin >> n >> m;

    // 输入子句信息
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        // 将数据写入表达式中存储整数的数组
        AllClauses[i].literals[0] = a;
        AllClauses[i].literals[1] = b;
        AllClauses[i].literals[2] = c;
    }

    // 开始求解:solve(1) --> 如果找得到可行解 --> 直接会在solve(varIndex)里面输出可行解
    if (!Solve(1)) {
        // 如果没有找到的话 --> 输出无可行解 <--> cout "No solution"
        cout << "No solution" << endl;
    }

    return 0;
}
