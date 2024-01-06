#include "SyntaxAnalysis.h"


// 用于产生新变量名，如t1，t2
string newtemp() {
    static int tim = 1;
    string digits[10] = { "0","1","2","3","4","5","6","7","8","9" };
    string s;
    if (tim < 10)
        s = "t" + digits[tim];
    else 
        s = "t" + digits[tim / 10] + digits[tim % 10];
    
    tim++;
    return s;
}


// 将标识符填入符号表 (enter object(constant, variable or procedre) into table.)
// 从下标 1 开始
void enter(int kind)
{

    variableTable* vT;
    tx++; //记录符号表当前索引，初始值设置为0
    table[tx].kind = kind;
    // 如果标识符是一个常量（ID_CONSTANT），那么检查其值是否超过最大（MAXADDRESS）。
    //      如果超过了，那么输出错误信息并将其值设置为0。然后将其值存储到符号表的当前位置。
    // 如果标识符是一个变量（ID_VARIABLE），那么将其层次设置为当前层次，
    //      并将其地址设置为当前的数据分配索引dx。然后将dx加1，为下一个变量腾出空间。
    switch (kind){
        case ID_CONSTANT:
            if (num > MAXADDRESS){
                // num太大
                error(25); //"The number is too great."
                num = 0;
            }
            table[tx].value = num;
            break;
        case ID_VARIABLE:
            strcpy(table[tx].name, id);

            //cout << "----------test------------" << endl;
            vT = (variableTable*)&table[tx];
            vT->level = level;
            vT->address = dx++;
            break;
    }
}

// 在符号表table中查找id的位置,没找到返回0
int position(char* id){
    int i;
    strcpy(table[0].name, id);
    i = tx + 1;
    while (strcmp(table[--i].name, id) != 0)
        ;
    return i;
}

// <常量定义>→<标识符>:=<⽆符号整数>
void constDeclaration()
{
    if (sym == SYM_IDENTIFIER)
    { 
        //全局变量id中存有已识别的标识符
        getsym();   
        if (sym == SYM_EQU || sym == SYM_ASSIGN){
            if (sym == SYM_EQU)
                error(1); // Found '=' when expecting ':='
            getsym();

            if (sym == SYM_NUMBER){
                enter(ID_CONSTANT); // 将标识符填入符号表
                emit(CONSTASSIGNMENT, table[tx].value, NULL, tx);
                getsym();
            }
            else{
                error(2); // 必须由数值在赋值符号 ':='后面
            }
        }
        else{
            error(3); // 再标识符后面必须有赋值符号 ':='
        }
    }
    else{
        error(4); // 在const后面一定要是标识符
    }
}


// <标识符>→<字⺟>{<字⺟> | <数字>}
void varDeclaration(void){
    if (sym == SYM_IDENTIFIER)
    {
        enter(ID_VARIABLE); // 将变量填入符号表
        getsym();
    }
    else{
        error(4);
    }
}


// 因子，把待运算的数放到栈中
// 把项和因子独立开处理解决了加减号与乘除号的优先级问题
//<因子>→<标识符> |<无符号整数> | (<表达式>)
void factor(){
    // 如果读到 <标识符>
    if (sym == SYM_IDENTIFIER){
        int i = position(id);  // 存储变量在符号表中位置
        // 如果未存储变量id
        if (i == 0){
            error(11);  // "未定义变量"
        }
        else
        {
            variableTable* vT;
            // 判断变量种类
            switch (table[i].kind){
                // 变量是常量Const,
                case ID_CONSTANT:
                    temp_factor_id++;
                    temp_factor[temp_factor_id].index = 1;
                    temp_factor[temp_factor_id].value = i;
                    break;
                case ID_VARIABLE:
                    temp_factor_id++;
                    temp_factor[temp_factor_id].index = 1;
                    temp_factor[temp_factor_id].value = i;
                    //vT = (variableTable*)&table[i];
                    //cout << "vT:" << vT->address << endl;

                    break;
            }
        }
        getsym();
    }
    // <无符号整数>
    else if (sym == SYM_NUMBER)
    {
        if (num > MAXADDRESS){
            error(25);
            num = 0;
        }
        temp_factor_id++;
        temp_factor[temp_factor_id].index = 0;
        temp_factor[temp_factor_id].value = num;
        enter(ID_CONSTANT);
        getsym();
    }
    // <表达式> 读到( 为开始
    else if (sym == SYM_LPAREN) 
    {
        getsym();
        // 递归调用表达式
        expression();
        // ) 为结尾
        if (sym == SYM_RPAREN){
            getsym();
        }
        else{
            error(22); // 没有 ')'做匹配
        }
    }
}


/*
*   对于表达式进行分析
*   <表达式>→[+|-]<项> | <表达式> <加法运算符> <项>
*   <项> (<加法运算符> <项>)^*
*/
void expression()
{
    int addoperation;
    int temp_id_factor;
    // [+|-]
    if (sym == SYM_PLUS || sym == SYM_MINUS){
        addoperation = sym;
        getsym();

        factor();

        // 负号特殊处理，正号忽略
        if (addoperation == SYM_MINUS){
            // gen(OPR, 0, OPR_NEG);
            //emit(OPRA_MIN, 0, 0, tx);
        }

    
    }


    // <项>
    term();
    // 进入term前进行记录
    int tx_1 = tx;

    // <加法运算符> <项>
    while (sym == SYM_PLUS || sym == SYM_MINUS)
    {

        addoperation = sym;
        getsym();


        temp_id_factor = temp_factor_id;
        // <项>
        term();

        int tx_2 = tx;
        // 生成中间变量
        string temp = newtemp();
        tx++;
        strcpy(table[tx].name, temp.c_str());
        table[tx].kind = ID_VARIABLE;
        if (addoperation == SYM_PLUS){
            emit(OPRA_ADD, temp_id_factor, temp_factor_id, tx);
        }
        else{
            emit(OPRA_MIN, temp_id_factor, temp_factor_id, tx);
        }
    }
}

// <项>→<因子> | <项><乘法运算符><因子>
void term()
{
    int muloperation;
    // <因子>
    factor();
    int temp_id_factor = temp_factor_id;

    while (sym == SYM_TIMES || sym == SYM_DIVIDE) {
        muloperation = sym; // 记录下当前运算符
        getsym();
        factor();

        // 生成中间变量 t0，t1...
        string temp = newtemp();
        tx++;
        strcpy(table[tx].name, temp.c_str());
        table[tx].kind = ID_VARIABLE;
        // 乘法
        if (muloperation == SYM_TIMES) {
            emit(OPRA_MUL, temp_id_factor, temp_factor_id, tx);
        }
        // 除法
        else {
            emit(OPRA_DIV, temp_id_factor, temp_factor_id, tx);

        }
    }
}



void conditionDeclaration() {
    getsym();
    condition();
    if (sym == SYM_THEN) {
        getsym();
    }
    else {
        error(16); // 'then' expected.
    }

    // THEN关键字后分析
    //int savedCx = cx;
    int savedPos = quadIndex;
    emit(OPRA_JMP, 0, 0, 0);
    statement();      // 递归调用
    //code[savedCx].a = cx; // 设置刚刚那个条件转移指令的跳转位置
    quadruples[savedPos].result = quadIndex;

}


// <条件>→<表达式> <关系运算符> <表达式>
void condition()
{

    // <表达式>
    expression();
     
    // <关系运算符>
    int relationOperation = sym; // 记录下当前运算符
    getsym();
    int temp_id = temp_factor_id;
    // <表达式>
    expression();

    int savedPos = quadIndex;
    // 根据比较关系生成指令
    switch (relationOperation){ 
        case SYM_EQU: // ==
            emit(OPRA_EQU, temp_id, temp_factor_id,  0);
            break;
        case SYM_NEQ: // !=
            emit(OPRA_NEQ, temp_id, temp_factor_id,  0);
            break;
        case SYM_LES: // <
            emit(OPRA_LES, temp_id, temp_factor_id,  0);
            break;
        case SYM_LEQ: // <=
            emit(OPRA_LEQ, temp_id, temp_factor_id, 0);
            break;
        case SYM_GTR: // >
            emit(OPRA_GTR, temp_id, temp_factor_id,  0);
            break;
        case SYM_GEQ: // >=
            emit(OPRA_GEQ, temp_factor_id, temp_id, 0);
            break;
        default:
            error(20);
    }

    // 执行完的后一句(不确定对不对)
    quadruples[savedPos].result = quadIndex + 1;
}


// <语句> → <赋值语句> | <条件语句> | <循环语句> | <复合语句> | <空语句>
void statement()
{
    int i = 0, savedCx, savedCx_;
    // <赋值语句>
    if (sym == SYM_IDENTIFIER){ 
        assignmentDeclaration();
    }
    
    // <条件语句>
    else if (sym == SYM_IF){
        conditionDeclaration();
    }

    //复合语句，顺序执行begin和end之间的语句就好
    else if (sym == SYM_BEGIN){ 
        combinedDeclaration();
    }

    // <循环语句>
    else if (sym == SYM_WHILE)
        circulationDeclaration();
}

// 复合语句
void combinedDeclaration() {
    getsym();
    statement(); // 递归调用
    while (sym == SYM_SEMICOLON) {
        if (sym == SYM_SEMICOLON) {
            getsym();
        }
        else {
            error(10);
        }
        statement();
    }
    if (sym == SYM_END)
    {

    }
    else {
        error(17); // ';' or 'end' expected.
    }

}



// 变量赋值判断
// <赋值语句>→<标识符>:=<表达式>
void assignmentDeclaration() {
    int i = position(id);

    //cout << "i:" << i;
    // 如果变量未定义，则报错
    if (i == 0) {
        error(11);
    }
    // 如果该符号不是变量，则报错
    else if (table[i].kind != ID_VARIABLE) {
        error(12);
    }

    // 获取下一个词
    getsym();

    // 如果是  :=  ，符合条件，获取下一个词
    if (sym == SYM_ASSIGN) {
        getsym();
    }
    // 不是 := 则报错："应使用':='符号"
    else {
        error(13);
    }
    int temp_id = tx;
    expression(); // 分析赋值号右部表达式

    if (i) {
        //cout << "-----------" << num << endl;
        //cout << "i:" << i << endl;
        //table[i].value = num;
        //cout << "---------" << temp_factor_id<<"-----------"<<i << endl;
        emit(ASSIGNMENT, tx, NULL, i);
    }


}

// <循环语句>→WHILE <条件> DO <语句>
void circulationDeclaration(){
    getsym();
    int savedCx = cx;           // while循环的开始位置

    int savedCodePosOri = quadIndex;   // 记录当前指令号码
    // <条件>
    condition();                // 处理while里的条件
    int savedCx_ = cx;          // while的do后的语句后的语句块的开始位置
    
    int savedCodePos = quadIndex;   // 记录当前指令号码
    //cout << savedCodePos << endl;

    emit(OPRA_JMP, 0, 0, 0);        // 条件转移指令，转移位置暂时不知
    
    // DO
    if (sym == SYM_DO) {
        getsym();
    }
    else {
        error(18); // 'do' expected.
    }

    // <语句>
    statement();          // 分析do后的语句块

    emit(OPRA_JMP, 0, 0, savedCodePosOri);
    quadruples[savedCodePos].result = quadIndex;    // 填写刚才那个条件转移指令的跳转位置
}



int base(int stack[], int currentLevel, int levelDiff)
{
    int b = currentLevel;

    while (levelDiff--)
        b = stack[b];
    return b;
}



/*
    一遍扫描，语法分析、语义分析、目标代码生成 一起完成
*/
void block()
{
    //后续变量定义主要用于代码生成
    int savedCx = cx;

    dx = 3;

    while (1){
        // 先看常量和变量定义
        // 最开始的嵌套级别是0
        if (level > MAXLEVEL){
            error(32); // There are too many levels.
        }

        // <常量声明部分>:=const<常量定义>{,<常量定义>};
        if (sym == SYM_CONST){
            getsym();
            if (sym == SYM_IDENTIFIER){
                constDeclaration();
                //逗号则，循环处理id1=num1,id2=num2,……
                while (sym == SYM_COMMA) {
                    getsym();
                    constDeclaration();
                }
                //常量定义结束
                
                //读到分号则结束分号
                if (sym == SYM_SEMICOLON){
                    getsym();
                }
                // 没有遇到逗号 ',' 又没有遇到 ';'，则报错
                else{
                    error(5); 
                }
            }
        }
        // <变量说明>→VAR<标识符>{，<标识符>};
        else if (sym == SYM_VAR){
            getsym();
            if (sym == SYM_IDENTIFIER){
                varDeclaration();

                while (sym == SYM_COMMA){
                    getsym();
                    varDeclaration();
                }
                if (sym == SYM_SEMICOLON){
                    getsym();
                }
                else{
                    error(5); 
                }
            }
        }
        else if (sym == SYM_NULL){
            // getsym(); // 出错的情况下跳过出错的符号
            // 不进行容错处理，遇到错误直接报错
            error(27);
            exit(1);
        }
        else{
            break;
        }
    }
    // 后续部分主要用于代码生成
    code[savedCx].a = cx; // 这时cx正好指向语句的开始位置，这个位置正是前面的 jmp 指令需要跳转到的位置

    //然后处理所有语句
    statement(); 

}





/*
* 产生四元式用于显示
* op: 对应operationCode
* arg1: 参数1
* arg2: 参数2
* result: 结果
 */
void emit(int op, int arg1, int arg2, int result) {
    quadruples[quadIndex].op = op;
    quadruples[quadIndex].arg1 = arg1;
    quadruples[quadIndex].arg2 = arg2;
    quadruples[quadIndex].result = result;
    quadIndex++;
    //cout << "op: " << op;
    //cout << " |  arg1: " << arg1;
    //cout << " |  arg2: " << arg2;
    //cout << " |  result: " << result << endl;
    return;
}

// 打印生成的三地址四元式中间代码
void printCodeToFile(const char* filename, int from, int to)
{
    FILE* outFile = fopen(filename, "w");
    if (!outFile) {
        cout << "文件打开失败" << filename;
        //fprintf(stderr, "文件打开失败\n", filename);
        exit(1);
    }

    //fprintf(outFile, "printCode:\n");
    printTol(outFile);

    fclose(outFile);
}


#include <iostream>
#include <string>

void printTol(FILE* outFile) {
    //fprintf(outFile, "printCode:\n");

    int length = 0;
    for (int i = 0; i < QUADRUPLE_MAX; i++) {
        if (quadruples[i].op != NULL) {
            length++;
        }
    }

    for (int i = 100; i < 100+length; i++) {
        string a1, a2, a3, a4;

        int arg1_id = quadruples[i].arg1;
        int arg2_id = quadruples[i].arg2;
        int result = 0;

        switch (quadruples[i].op) {
        case(OPRA_ADD):
        case(OPRA_MIN):
        case(OPRA_MUL):
        case(OPRA_DIV): 
            if (temp_factor[arg1_id].index == 1) {
                a1 = table[temp_factor[arg1_id].value].name;
            }
            if (temp_factor[arg1_id].index == 0) {
                a1 = to_string(temp_factor[arg1_id].value);
            }
            if (temp_factor[arg2_id].index == 1) {
                a2 = table[temp_factor[arg2_id].value].name;
            }
            if (temp_factor[arg2_id].index == 0) {
                a2 = to_string(temp_factor[arg2_id].value);
            }
            fprintf(outFile, "%-5d %-10s %-10s %-10s %-10s\n", i, codeIntrustion[quadruples[i].op], a1.c_str(), a2.c_str(), table[quadruples[i].result]);
            break;

        // 比较符号
        case(OPRA_EQU):
        case(OPRA_NEQ):
        case(OPRA_LES):
        case(OPRA_LEQ):
        case(OPRA_GTR):
        case(OPRA_GEQ):
            if (temp_factor[arg1_id].index == 1) {
                a1 = table[temp_factor[arg1_id].value].name;
            }
            if (temp_factor[arg1_id].index == 0) {
                a1 = to_string(temp_factor[arg1_id].value);
            }
            if (temp_factor[arg2_id].index == 1) {
                a2 = table[temp_factor[arg2_id].value].name;
            }
            if (temp_factor[arg2_id].index == 0) {
                a2 = to_string(temp_factor[arg2_id].value);
            }
            result = quadruples[i].result;
            //cout << "Debug: " << result << endl; // Print the value for debugging purposes
            fprintf(outFile, "%-5d %-10s %-10s %-10s %-10d\n", i, codeIntrustion[quadruples[i].op], a1.c_str(), a2.c_str(), result);
            break;

        case(ASSIGNMENT):
        case(CONSTASSIGNMENT):
            if (table[arg1_id].kind == 0)
                a1 = to_string(table[arg1_id].value);
                //a1 = to_string(temp_factor[quadruples[i].arg1].value);
            else
                a1 = table[arg1_id].name;

                //a1 = table[temp_factor[arg1_id].value].name;
            a2 = "_";
            fprintf(outFile, "%-5d %-10s % -10s % -10s %-10s\n", i, codeIntrustion[quadruples[i].op], a1.c_str(), a2.c_str(), table[quadruples[i].result].name);

            break;



        case(OPRA_JMP):
            a1 = "_";
            a2 = "_";
            fprintf(outFile, "%-5d %-10s %-10s %-10s %-10s\n", i, codeIntrustion[quadruples[i].op], a1.c_str(), a2.c_str(), to_string(quadruples[i].result).c_str());

            //fprintf(outFile, "%5d %-10_ %-10_ %-10s\n", i, codeIntrustion[quadruples[i].op], a1.c_str(), a2.c_str(), to_string(quadruples[i].result).c_str());
            break;
        }


    }

    fprintf(outFile, "\n");
}
