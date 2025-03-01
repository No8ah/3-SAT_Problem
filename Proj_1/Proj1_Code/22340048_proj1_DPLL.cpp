#include <iostream>
using namespace std;

// 因为最多300个变量 --> define MAX_VARS = 300
#define MAX_VARS 300
// 因为最多1200个clause --> define MAX_CLAUSES = 1200
#define MAX_CLAUSES 1200

// 因为是3-SAT问题 --> literals[3] --> K-SAT问题 --> literals[k]
// 定义一个Clauses结构体 <--> 单个clause
struct Clause {
    int literals[3];  // 每个clause包含K=3个literal
};

// 初始化全局变量
    // clause个数m 和 表达式中clause中变量的个数n
    int m, n;
    // 存储clauses的AllClauses array(array中的每个ele都是struct Clauses) <--> 用于存储clause <--> consists of K=3个literal
    Clause AllClauses[MAX_CLAUSES];
    // 存储bool赋值的assignment array --> 1: true 、 0: false 、 -1: 未赋值
    // 实现小细节: "+1" 是为了让index与实际x_{index}进行对应
    int assignment[MAX_VARS + 1];
    // 实现小细节: "+1" 是为了让index与实际x_{index}进行对应
// 初始化全局变量

 

// 实现难点: why需要判断EmptyClause? --> 为了给DPLL()算法做准备 <--> 保证整个表达式没有EmptyClause --> 因为如果有EmptyClause <--> 说明没有可行解 <--> Anti-E.g. : ????
// 检查整个表达式是否有EmptyClause <--> 判断整个表达式 is satisfied or unsatisfied? --> true : 表示有EmptyClause 、 false : 表示没有EmptyClause
bool hasEmptyClause() {
    // 遍历m个clause
    for (int i = 0; i < m; i++) {
        // 首先假设第i个clause is allLiteralFalse = true <--> 第i个clause is EmptyClause
        bool allLiteralFalse = true;
        // 遍历第i个clause中的K=3个literal (K-SAT中K=3的缘故)
        for (int j = 0; j < 3; j++) {
            // 将第i个clause中的第j个literal存储到literal当中
            int literal = AllClauses[i].literals[j];
            // 实现小细节: var = a > 0 ? b : c --> var = abs(a) 
            // 获取literal的编号
                // 考虑:\neg x_8 --> -8 --> var = abs(-8) = 8
                // 考虑:    x_24 --> 24 --> var = abs(24)  = 24
            int var = abs(literal);
            // 实现小细节: var = a > 0 ? b : c --> var = abs(a) 

            // 对第i个clause的第j个literal进行判断
            // 实现难点:
            // 对于"已经进行bool赋值" --> 说明第i个clause is certain --> 我们判断对这个clause进行判断 --> if conditions is satisfied <--> 说明第i个clause is satisfied的
            if ( (literal > 0 && assignment[var] == 1) || (literal < 0 && assignment[var] == 0)) {
                // 已经有assignment S.t. 该clause is satisfied <--> allFalse设定为false <--> 说明这个clause is satisfied的 <--> 说明这个clause is not EmptyClause 
                allLiteralFalse = false;
                // 既然已经 S.t. 第i个clause满足了 --> 无需对第i个clause的第j+1个literal进行判断 --> 直接break出循环判断第i+1个clause --> ↓complexity↓
                break;
            }
            // 对于"未进行bool赋值" --> 说明第i个clause is uncertain --> 那我们暂且假设这个clause(第i个clause)is satisfied的 <--> 假定第i个clause is not EmptyClause
            if (assignment[var] == -1) {  // 第i个clause的第j个literal还未赋值 
                // 假定第i个clause is satisfied的 <--> 假定第i个clause不是EmptyClause <--> allFalse = false  
                allLiteralFalse = false;
                // 同理 --> break出for_loop判断第i+1个clause
                break;
            }
            // 此时第i个clause的第j个literal已经进行赋值 并且 不满足conditions --> 要么判断第j+1个literal回到上述步骤/要么已经是j_max --> 进行下一步判断
            // 实现难点:
        }
        // 此时已经遍历完了第i个clause的K=3个literal --> 因为allFalse始终没有改变 <--> 此时allFalse = true <--> 说明存在EmptyClause --> 这个EmptyClause == 第i个clause
        if (allLiteralFalse){
            // 既然存在EmptyClause <--> EmptyClause就是第i个clause <--> 说明hasEmptyClause(这里只说明EmptyClause的存在性) --> 自然return true
            return true;
        }
        // 此时已经遍历完了第i个clause的K=3个literal --> 是时候判断第i+1个clause了  
    }
    // 此时已经遍历完了m个clause --> 此时m个clause都不是EmptyClause <--> 表达式中根本就没有EmptyClause <--> hasnotEmptyClause --> 自然返回false
    return false;
}
// 此时已经判断完整个表达式了 <--> 有无Emptyclauses已经很清楚地知道了
// 实现难点: why需要判断EmptyClause? --> 为了给DPLL()算法做准备 <--> 保证整个表达式没有EmptyClause --> 因为如果有EmptyClause <--> 说明没有可行解 <--> Anti-E.g. : ????

// 实现难点: 此时整个表达式 is unsatisfied \cap 没有EmptyClause,那么如何进行处理? --> 1.单clause传播
// 单clause传播
bool unitPropagation() {
    // 初始化一个判断是否进行单clause传播的bool常量propagated
    bool propagated = false;// 首先肯定还未开始进行单clause传播 --> 初始化为false
    // 遍历m个clause
    for (int i = 0; i < m; i++) {
        // 初始化用于记录"clause中未进行bool赋值的literal个数" --< 注意当unassignedCount==1的时候 --> 一定要找出bool赋值S.t.整个clause为true
        int unassignedLiteralCounter = 0;
        // 初始化用于记录"当前clause中最后一个未赋值的literal"
        int lastUnassignedliteral = 0;
        // 初始化用于记录"S.t.当前clause satisfied的literal"
        int satisfiedLiteral = 0;
        // 遍历第i个clause的K=3个literal
        for (int j = 0; j < 3; j++) {
            // 将第i个clause的第j个literal存储到literal当中
            int literal = AllClauses[i].literals[j];
            // 获取literal的编号
            int num_of_var = abs(literal);

            // 实现难点:
            // 对于"未进行bool赋值" --> 说明第i个clause中未进行bool赋值的literal个数++
            if (assignment[num_of_var] == -1) {  
                // 第i个clause中未进行bool赋值的literal个数++
                unassignedLiteralCounter++;
                // 说明第i个clause的第j个literal是未进行bool赋值的 <--> lastUnassignedliteral(当前clause中最后一个未赋值的literal)就是literal
                lastUnassignedliteral = literal;
            } 
            // 实现难点:

            // 实现难点:
            // 对于"已经进行bool赋值" --> if conditions are satisfied <--> 如果第i个clause可以被satisfied
            else if ((literal > 0 && assignment[num_of_var] == 1) || (literal < 0 && assignment[num_of_var] == 0)) {
                satisfiedLiteral = literal;  
                break;
            }
            // 实现难点:
            // 此处第i个clause的第j个literal已经完全进行赋值并且未满足conditions
        }
        // 此时已经遍历并检验完第i个clause的K=3个literal

        // 实现难点:
        // 如果 第i个clause中未进行bool赋值的literal只有一个 && 第i个clause还是unsatisfied的
        if (unassignedLiteralCounter == 1 && satisfiedLiteral == 0) {
            // 实现小细节:
            int num_of_var = abs(lastUnassignedliteral);
            // 实现小细节:
            // 如果lastUnassignedliteral是正literal <--> 赋值为true --> 否则 <--> lastUnassignedliteral是负literal <--> 赋值为false
            assignment[num_of_var] = (lastUnassignedliteral > 0) ? 1 : 0;
            // 说明需要进行单clause传播
            propagated = true;
        }
        // 实现难点:
    }
    // 此时已经遍历完m个clause <--> 返回是否进行了单clause传播 : propagated = true <--> 进行了单clause传播 || propagated = false <--> 没有进行单clause传播
    return propagated;
}
// 实现难点: 此时整个表达式 is unsatisfied \cap 没有EmptyClause,那么如何进行处理? --> 1.单clause传播

// 实现难点: 此时我们已经完成了 1.单clause传播 , 那么进行下一步 2.Pure Literal Elimination
// 实现难点: if all clauses中某个变量只是以一种形式出现,我们应该怎样进行处理呢? --> Pure Literal Elimination
// pure literal elimination
void pureLiteralElimination() {
    // 记录正负literal的出现次数
    // 实现小细节: "+1" 是为了让index与实际x_{index}进行对应
    int positive[MAX_VARS + 1] = {0};  // 记录正literal出现次数
    int negative[MAX_VARS + 1] = {0};  // 记录负literal出现次数
    // 实现小细节: "+1" 是为了让index与实际x_{index}进行对应
    // record pos/neg literal <--> 统计正负literal的出现次数 
    // 遍历m个clause
    for (int i = 0; i < m; i++) {
        // 遍历第i个clause的K=3个literal
        for (int j = 0; j < 3; j++) {
            // 将第i个clause的第j个literal存储到literal当中
            int literal = AllClauses[i].literals[j];
            // 如果是第i个clause的第j个literal is 正literal
            if (literal > 0) {
                // 记录clause的第index=literal(注意是正literal)的counter++
                positive[literal]++;  
            } 
            // 如果是第i个clause的第j个literal is 负literal
            else {
                // 实现小细节: 因为literal是负数 --> 实际index = -literal/abs(literal)
                // 记录clause的第index=-literal(注意是负literal)的counter++
                negative[-literal]++;
                // 实现小细节: 因为literal是负数 --> 实际index = -literal/abs(literal)
            }
        }
    }
    
    // eliminate pure literal --> 将消除的literal体现在assignment当中
    for (int index = 1; index <= n; index++) {
        // 如果第var个变量只是以为正literal的形式出现 --> 只需将assignment[index]赋值为true即可
            // 考虑: x_{8}只是以x_{8}出现 <--> 不以\neg x_{8} 出现 --> 只需将x_{8}赋值为true即可
        // 实现小细节: 只以pos出现 <--> (positive[index] > 0 && negative[index] == 0) || 只以neg出现 <--> negative[index] > 0 && positive[index] == 0
        if (positive[index] > 0 && negative[index] == 0) {
            // 将assignment[index]赋值为true
            assignment[index] = 1; 
        }
        // 如果第var个变量只是以为负literal的形式出现 --> 只需将assignment[index]赋值为false即可 
        else if (negative[index] > 0 && positive[index] == 0) {
            // 将assignment[index]赋值为false
            assignment[index] = 0;  
        }
    }
}
// 实现难点: if all clauses中某个变量只是以一种形式出现,我们应该怎样进行处理呢? --> Pure Literal Elimination
// 实现难点: 此时我们已经完成了 1.单clause传播 , 那么进行下一步 2.Pure Literal Elimination


// 检查第clauseIndex个clause是否被满足
bool isCurrentClauseSatisfied(int clauseIndex) {
    // 遍历currentClause的K=3个literal
    for (int j = 0; j < 3; j++) {
        // 将currentClause的第j个literal存储到literal当中
        int literal = AllClauses[clauseIndex].literals[j];
        // 获取变量编号
        int var = abs(literal);  

        // 如果currentClause中的literal已经被满足 <--> if conditions are satisfied <--> clause is satisfied --> 自然return true
        if ((literal > 0 && assignment[var] == 1) || (literal < 0 && assignment[var] == 0)) {
            // conditions are satisfied --> return true
            return true;  
        }
        // 如果currentClause中的literal is not satisfied --> 判断第j+1个literal/进行下一步判断
    }
    // 此时已经遍历完currentClause的K=3个literal --> 此时currentClause一定是unsatisfied <--> 因为没有return true的缘故 --> 自然return false
    return false; 
}

// 检查整个表达式是否被满足
bool areAllClausesSatisfied() {
    // 遍历m个clause
    for (int i = 0; i < m; i++) {
        // 如果第i个clause是不可满足的 <--> clause_{i} is unsatisfied --> 自然return false
        if (!isCurrentClauseSatisfied(i)) { // 说明第i个clause是不可满足的 <--> currentClauseSatisfied(i)return的结果是false
            // 此处return false
            return false;
        }
    }
    // 此时已经遍历完了m个clause <--> 没有经历"return false"的过程 <--> 所有的clause are satisfied <--> allClausesSatisfied()return的结果是true
    return true;
}


// 实现难点: DPLL()算法
// DPLL 递归求解函数
bool DPLL() {
    // 检查是否所有clauses已被满足 <--> all clauses are satisfied ? √ --> 返回true
    if (areAllClausesSatisfied()){// 所有clauses都是satisfied的
        // 说明可以通过DPLL algo进行求解 --> 求解并且output出outcome
        return true;
    }
    // 此时说明not all clauses are satisfied --> 找出空clause

    // 如果EmptyClause存在 --> hasEmptyClause <--> 存在不可求解 --> return false
    if (hasEmptyClause()){// 存在EmptyClause <--> hasEmptyClause() return的结果是true
        // 存在不可求解 --> return false
        return false;
    }
    // 此时说明 表达式中不存在EmptyClause \cap 表达式中存在clause还未被满足 --> 那么就通过需要但clause传播和pureliteralelimination处理
    // 进行 单clause传播 --> 直到没有再进行单clause传播 --> 可以步入下一个phase <--> pureLiteralElimination phase
    while (unitPropagation());

    // 进行 pureliteralelimination
    pureLiteralElimination();

    // 实现难点:
    // 此时已经进行完了 单clause传播 phase 和 pureliteralelimination phase --> assignment还有部分为进行bool赋值
    // 找到一个未赋值的变量
    int var = -1;
    // 遍历n个变量 <--> 从 assignment[1] 到 assignment[n]
    for (int i = 1; i <= n; i++) {
        // 如果assignment[i]未进行赋值
        if (assignment[i] == -1) {
            // 此时说明assignment[index=i]还未进行赋值
            var = i;
            // break出for_loop进行下一步操作
            // 实现小细节:
            break;
            // 实现小细节:
        }
        // 此时说明 assignment[i]不等于-1 <--> 说明assignment[i]已经进行了bool赋值了(1/-1)
    }

    // 实现难点:
    // 此时说明所有的assignment都已经进行了bool赋值 <--> 无需进行后续的assign_bool操作了
    if (var == -1){
        return false;
    }  
    // 实现难点: 
    // 实现难点:

    // 此时说明assignment[index=i=var]还未进行bool赋值 --> 接下来我们进行bool赋值
    // 实现难点: 递归与回溯
    // 递归尝试赋值
    assignment[var] = 1; // 将assignment[index=i=var]赋值为true --> 下面重新进行DPLL algo判断
    // 我们现在进行递归回去进行DPLL algo判断 : allClausesSatisfied 、 hasEmptyClause 、 unitPropagation 、 pureLiteralElimination
    if (DPLL()){
        // 此时说明可以通过DPLL algo进行求解 --> 自然返回true
        return true;
    }
    // 实现难点: 递归与回溯
    // 此时说明assignment[index=i=var] = true是不能进行求解 --> 通过将assignment[index=i=var]赋值为false进行求解
    // 实现难点: 递归与回溯
    // 回溯，尝试赋值为 false
    assignment[var] = 0; // 将assignment[index=i=var]赋值为false
    if (DPLL()){
        // 同理
        return true;
    } 
    // 实现难点: 递归与回溯

    // 实现难点:
    // 回溯后无法找到解，恢复未赋值状态 <--> ??why??
    assignment[var] = -1;
    // 实现难点:

    return false;
}
// 实现难点: DPLL()算法

// 实现小细节: 函数封装
void findSolution(bool result){
    // 利用某个algo进行求解 --> if algo()返回的结果是true --> cout出结果
    if (result) {
        // 输出变量赋值
        for (int i = 1; i <= n; i++) {
            cout << assignment[i];
            // 前n-1个赋值的输出格式都是bool-空格
            if (i != n) cout << " ";// 最后一个赋值只需输出bool即可
            // 最终outcome就是:bool-空格-bool-……-bool-空格-bool(最后一个assignment)
        }
        // 此时已经完全输出outcome --> 换行
        cout << endl;
    }
    // 此时使用某个algo并未求解出outcome <--> algo()返回的结果是false --> cout无可行解
    else {
        cout << "No solution" << endl;
    }
}
// 实现小细节: 函数封装

// 实现小细节: 函数封装
void inputClauses(){
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        AllClauses[i].literals[0] = a;
        AllClauses[i].literals[1] = b;
        AllClauses[i].literals[2] = c;
    }
};
// 实现小细节: 函数封装
int main() {
    // 开始的时候还未对assignment进行赋值 --> 初始化变量赋值为未赋值状态 --> -1 : 未开始进行赋值 、 1 : 赋值为true 、 0 : 赋值为false
    for (int i = 0; i <= MAX_VARS; i++) {
        assignment[i] = -1;
    }
    // 输入变量数n 和 clause个数m
    cin >> n >> m;
    inputClauses();
    // 使用DPLL算法进行求解 --> 如果有solution --> cout出outcome || 如果没有solution --> cout出No solution
    findSolution(DPLL());// 当然我也可以将这个DPLL()改为"回溯算法"和"归结算法"
    return 0;
}
