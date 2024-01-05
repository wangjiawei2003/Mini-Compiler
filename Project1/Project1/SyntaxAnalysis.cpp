﻿#include "SyntaxAnalysis.h"


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
    strcpy(table[tx].name, id);
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
                emit(ASSIGNMENT, num, NULL, tx);
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

// 生成汇编指令 (generates (assembles) an instruction.)
void gen(int x, int y, int z){ 
    //最大指令数量 cx > 500
    if (cx > CXMAX){ 
        printf("Fatal Error: Program too long.\n");
        exit(1);
    }
    //三个变量：操作码、层次差和操作数。
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    // 指针移动，向后记录
    cx++;   
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
                temp_factor_id++;
                temp_factor[temp_factor_id].index = 1;
                temp_factor[temp_factor_id].value = i;
                // 变量是常量Const,
                case ID_CONSTANT:
                    gen(LIT, 0, table[i].value); // 把常数放到栈顶
                    break;
                case ID_VARIABLE:
                    vT = (variableTable*)&table[i];
                    cout << "vT:" << vT->address << endl;
                    gen(LOD, level - vT->level, vT->address); // 如果是变量，根据层次差和地址去栈里面找到这个值，把变量放到栈顶
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
        gen(LIT, 0, num); // 把常数放到栈顶

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

        if (addoperation == SYM_MINUS){
            gen(OPR, 0, OPR_NEG);
        }

    
    }
    // <项>
    term();
    temp_id_factor = temp_factor_id;

    // <加法运算符>
    while (sym == SYM_PLUS || sym == SYM_MINUS)
    {
        addoperation = sym;
        getsym();
        // <项>
        term();

        // 生成中间变量
        string temp = newtemp();
        tx++;
        strcpy(table[tx].name, temp.c_str());
        table[tx].kind = ID_VARIABLE;
        if (addoperation == SYM_PLUS){
            gen(OPR, 0, OPR_ADD);
            emit(OPRA_ADD, temp_factor_id, temp_id_factor, tx);
        }
        else{
            gen(OPR, 0, OPR_MIN);
            emit(OPRA_MIN, temp_factor_id, temp_id_factor, tx);
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
        if (muloperation == SYM_TIMES) {
            emit(OPRA_MUL, temp_factor_id, temp_id_factor, tx);
            //emit(OPRA_MUL, tempFactor.index, temp_factor.index, tx);
            gen(OPR, 0, OPR_MUL); // 将栈顶和次栈顶进行运算
        }
        else {
            emit(OPRA_DIV, temp_factor_id, temp_id_factor, tx);

            //emit(OPRA_DIV, tempFactor.index, temp_factor.index, tx);
            gen(OPR, 0, OPR_DIV);
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
    gen(JPC, 0, 0);   // 条件转移指令，栈顶为非真时跳转到a
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
    
    // <表达式>
    expression();

    // 根据比较关系生成指令
    switch (relationOperation){ 
        case SYM_EQU: // ==
            gen(OPR, 0, OPR_EQU);
            emit(OPRA_EQU, 0, 0, NULL);
            break;
        case SYM_NEQ: // !=
            gen(OPR, 0, OPR_NEQ);
            emit(OPRA_NEQ, 0, 0, NULL);
            break;
        case SYM_LES: // <
            gen(OPR, 0, OPR_LES);
            emit(OPRA_LES, 0, 0, NULL);
            break;
        case SYM_LEQ: // <=
            gen(OPR, 0, OPR_LEQ);
            emit(OPRA_LEQ, 0, 0, NULL);
            break;
        case SYM_GTR: // >
            gen(OPR, 0, OPR_GTR);
            emit(OPRA_GTR, 0, 0, NULL);
            break;
        case SYM_GEQ: // >=
            gen(OPR, 0, OPR_GEQ);
            emit(OPRA_GEQ, 0, 0, NULL);
            break;
        default:
            error(20);
    }
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
    expression(); // 分析赋值号右部表达式

    variableTable* vT;
    vT = (variableTable*)&table[i];
    if (i) {
        gen(STO, level - vT->level, vT->address); // 将栈顶内容存到刚刚识别到的变量里
        // 将其以四元式
        emit(ASSIGNMENT, num, NULL, i);

    }


}

// <循环语句>→WHILE <条件> DO <语句>
void circulationDeclaration(){
    getsym();
    int savedCx = cx;           // while循环的开始位置

    // <条件>
    condition();                // 处理while里的条件
    int savedCx_ = cx;          // while的do后的语句后的语句块的开始位置
    
    int savedCodePos = quadIndex;   // 记录当前指令号码
    cout << savedCodePos << endl;

    emit(OPRA_JMP, 0, 0, 0);        // 条件转移指令，转移位置暂时不知
    //gen(JPC, 0, 0);                  // 条件转移指令，转移位置暂时不知
    
    // DO
    if (sym == SYM_DO) {
        getsym();
    }
    else {
        error(18); // 'do' expected.
    }

    // <语句>
    statement();          // 分析do后的语句块
    emit(OPRA_JMP, 0, 0,-1);
    gen(JMP, 0, savedCx); // 无条件转移指令，跳转到cx1，再次进行逻辑判断
    //code[savedCx_].a = cx; // 填写刚才那个条件转移指令的跳转位置，while循环结束
    quadruples[savedCodePos].result = quadIndex;    // 填写刚才那个条件转移指令的跳转位置
}



int base(int stack[], int currentLevel, int levelDiff)
{
    int b = currentLevel;

    while (levelDiff--)
        b = stack[b];
    return b;
}

void interpret()
{
    int pc = 0;           // program counter
    int stack[STACKSIZE]; // 栈
    int top = 0;          // top of stack
    instruction i;        // instruction register

    cout << "开始执行PL/0程序" << endl;

    stack[1] = stack[2] = stack[3] = 0;
    do
    {
        printf("%d_", pc);
        i = code[pc++];
        switch (i.f)
        {
        case LIT:
            stack[++top] = i.a;
            break;
        case OPR:
            switch (i.a) // operator
            {
            case OPR_RET:
                top = i.l - 1;
                pc = stack[top + 3];
                break;
            case OPR_NEG:
                stack[top] = -stack[top];
                break;
            case OPR_ADD:
                top--;
                stack[top] += stack[top + 1];
                break;
            case OPR_MIN:
                top--;
                stack[top] -= stack[top + 1];
                break;
            case OPR_MUL:
                top--;
                stack[top] *= stack[top + 1];
                break;
            case OPR_DIV:
                top--;
                if (stack[top + 1] == 0)
                {
                    fprintf(stderr, "Runtime Error: Divided by zero.\n");
                    fprintf(stderr, "Program terminated.\n");
                    continue;
                }
                stack[top] /= stack[top + 1];
                break;
            case OPR_ODD:
                stack[top] %= 2;
                break;
            case OPR_EQU:
                top--;
                stack[top] = stack[top] == stack[top + 1];
                break;
            case OPR_NEQ:
                top--;
                stack[top] = stack[top] != stack[top + 1];
            case OPR_LES:
                top--;
                stack[top] = stack[top] < stack[top + 1];
                break;
            case OPR_GEQ:
                top--;
                stack[top] = stack[top] >= stack[top + 1];
            case OPR_GTR:
                top--;
                stack[top] = stack[top] > stack[top + 1];
                break;
            case OPR_LEQ:
                top--;
                stack[top] = stack[top] <= stack[top + 1];
            }
            break;
        case LOD:
            stack[++top] = stack[base(stack, i.l, i.l) + i.a];
            break;
        case STO:
            stack[base(stack, i.l, i.l) + i.a] = stack[top];
            top--;
            break;
        case INT:
            top += i.a;
            break;
        case JMP:
            pc = i.a;
            break;
        case JPC:
            if (stack[top] == 0)
                pc = i.a;
            top--;
            break;
        }
    } while (pc);

    cout << endl << "停止执行PL/0程序" << endl;
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
    //gen(OPR, 0, OPR_RET); // 从分程序返回（对于 0 层的程序来说，就是程序运行完成，退出）
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
    cout << "op: " << op;
    cout << " |  arg1: " << arg1;
    cout << " |  arg2: " << arg2;
    cout << " |  result: " << result << endl;
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

    fprintf(outFile, "printCode:\n");

    //SyntaxAnalysis.cpp
    //将quadIndex中元素输入文件
    int length = 0;
    for (int i = 0; i < QUADRUPLE_MAX; i++) {
        if (quadruples[i].op != NULL) {
            length++;
        }
    }

    for (int i = 0; i < length; i++)

    {

        fprintf(outFile, "%5d %s\t%d\t%d\t%s\n", i, codeIntrustion[quadruples[i].op], quadruples[i].arg1, quadruples[i].arg2, table[quadruples[i].result].name);

    }
    fprintf(outFile, "\n");

    fclose(outFile);
}
