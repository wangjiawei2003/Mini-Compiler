#include "SyntaxAnalysis.h"


// 将标识符填入符号表 (enter object(constant, variable or procedre) into table.)
// 从下标 1 开始
void enter(int kind)
{
    variableTable* vT;
    tx++; //记录符号表当前索引，初始值设置为0
    strcpy(table[tx].name, id);
    table[tx].kind = kind;
    // 如果标识符是一个常量（ID_CONSTANT），那么检查其值是否超过最大地址（MAXADDRESS）。
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

// 在符号表table中查找id的位置
int position(char* id)
{
    int i;
    strcpy(table[0].name, id);
    i = tx + 1;
    while (strcmp(table[--i].name, id) != 0)
        ;
    return i;
}

// <常量定义>→<标识符>:=<⽆符号整数>
void constdeclaration()
{
    if (sym == SYM_IDENTIFIER)
    { 
        //全局变量id中存有已识别的标识符
        getsym();   
        /* 可能不对*****************************/
        if (sym == SYM_EQU || sym == SYM_ASSIGN)
        {
            if (sym == SYM_EQU)
                error(1); // Found '=' when expecting ':='
            getsym();
            if (sym == SYM_NUMBER)
            {
                enter(ID_CONSTANT); // 将标识符填入符号表
                getsym();
            }
            else
            {
                error(2); // 必须由数值在赋值符号 ':='后面
            }
        }
        else
        {
            error(3); // 再标识符后面必须有赋值符号 ':='
        }
    }
    else
    {
        error(4); // 在const后面一定要是标识符
    }
}

// <标识符>
void vardeclaration(void)
{
    if (sym == SYM_IDENTIFIER)
    {
        enter(ID_VARIABLE); // 将变量填入符号表
        getsym();
    }
    else
    {
        error(4);
    }
}

// 生成汇编指令 (generates (assembles) an instruction.)
void gen(int x, int y, int z)
{ //生成汇编指令
    //最大指令数量
    if (cx > CXMAX)
    { // cx > 500
        printf("Fatal Error: Program too long.\n");
        exit(1);
    }
    // printf("gen code[%d] f=%s l=%d a=%d\n",cx,mnemonic[x],y,z);
    //三个变量：操作码、层次差和操作数。
    code[cx].f = x;
    code[cx].l = y;
    code[cx++].a = z;
}

// 因子，把待运算的数放到栈中
// 把项和因子独立开处理解决了加减号与乘除号的优先级问题
void factor()
{
    void expression();//expression后面定义，要使用先声明一下
    int i;//存储找到的变量id位置
    if (sym == SYM_IDENTIFIER)//是标识符
    {
        if ((i = position(id)) == 0)
        {
            error(11); 
        }
        else
        {
            variableTable* vT;
            switch (table[i].kind)
            {
            case ID_CONSTANT:
                gen(LIT, 0, table[i].value); // 把常数放到栈顶
                break;
            case ID_VARIABLE:
                vT = (variableTable*)&table[i];
                gen(LOD, level - vT->level, vT->address); // 如果是变量，根据层次差和地址去栈里面找到这个值，把变量放到栈顶
                break;
            }
        }
        getsym();
    }
    else if (sym == SYM_NUMBER)//是整数
    {
        if (num > MAXADDRESS)
        {
            error(25);
            num = 0;
        }
        gen(LIT, 0, num); // 把常数放到栈顶
        getsym();
    }
    else if (sym == SYM_LPAREN) // (为开始
    {
        getsym();
        expression();          // 递归调用表达式
        if (sym == SYM_RPAREN) // )为结尾
        {
            getsym();
        }
        else
        {
            error(22); // 没有 ')'做匹配
        }
    }
}

// 项（乘除运算）
void term()
{
    int muloperation;
    factor();
    while (sym == SYM_TIMES || sym == SYM_DIVIDE)
    {
        muloperation = sym; // 记录下当前运算符
        getsym();
        factor();
        if (muloperation == SYM_TIMES)
        {
            gen(OPR, 0, OPR_MUL); // 将栈顶和次栈顶进行运算
        }
        else
        {
            gen(OPR, 0, OPR_DIV);
        }
    }
}

// <表达式>→[+|-]项 | <表达式> <加法运算符> <项>
void expression()
{
    int addoperation;
    if (sym == SYM_PLUS || sym == SYM_MINUS)
    {
        addoperation = sym;
        getsym();
        factor();
        if (addoperation == SYM_MINUS)
        {
            gen(OPR, 0, OPR_NEG);
        }
        term();
    }
    else
    {
        term();
    }
    while (sym == SYM_PLUS || sym == SYM_MINUS)
    {
        addoperation = sym;
        getsym();
        term();
        if (addoperation == SYM_PLUS)
        {
            gen(OPR, 0, OPR_ADD);
        }
        else
        {
            gen(OPR, 0, OPR_MIN);
        }
    }
}

// <条件>
void condition()
{
    expression();
    int relationOperation = sym; // 记录下当前运算符
    getsym();
    expression();
    switch (relationOperation)
    { // 根据比较关系生成指令
    case SYM_EQU: // ==
        gen(OPR, 0, OPR_EQU);
        break;
    case SYM_NEQ: // !=
        gen(OPR, 0, OPR_NEQ);
        break;
    case SYM_LES: // <
        gen(OPR, 0, OPR_LES);
        break;
    case SYM_LEQ: // <=
        gen(OPR, 0, OPR_LEQ);
        break;
    case SYM_GTR: // >
        gen(OPR, 0, OPR_GTR);
        break;
    case SYM_GEQ: // >=
        gen(OPR, 0, OPR_GEQ);
        break;
    default:
        error(20);
    }
}


// <语句>::= <赋值语句> | <条件语句> | <循环语句> | <复合语句> | null
void statement()
{
    int i = 0, savedCx, savedCx_;

    if (sym == SYM_IDENTIFIER)
    { // <赋值语句>→<标识符>:=<表达式>

        if ((i = position(id)) == 0)
        {
            error(11); 
        }
        else if (table[i].kind != ID_VARIABLE)
        {
            error(12); 
        }
        getsym();
        if (sym == SYM_ASSIGN) // :=
        {
            getsym();
        }
        else
        {
            error(13); // :=
        }
        expression(); // 算出赋值号右部表达式的值
        variableTable* vT;
        vT = (variableTable*)&table[i];
        if (i)
        {
            gen(STO, level - vT->level, vT->address); // 将栈顶内容存到刚刚识别到的变量里
        }
    }
    
    else if (sym == SYM_IF)
    {
        getsym();
        condition();
        if (sym == SYM_THEN)
        {
            getsym();
        }
        else
        {
            error(16); // 'then' expected.
        }
        savedCx = cx;
        gen(JPC, 0, 0);   // 条件转移指令，栈顶为非真时跳转到a
        statement();      // 递归调用
        code[savedCx].a = cx; // 设置刚刚那个条件转移指令的跳转位置
    }
    else if (sym == SYM_BEGIN)
    { //复合语句，顺序执行begin和end之间的语句就好
        getsym();
        statement(); // 递归调用
        while (sym == SYM_SEMICOLON)
        {
            if (sym == SYM_SEMICOLON)
            {
                getsym();
            }
            else
            {
                error(10);
            }
            statement();
        }
        if (sym == SYM_END)
        {
           
        }
        else
        {
            error(17); // ';' or 'end' expected.
        }
    }
    else if (sym == SYM_WHILE)
    {
        getsym();
        savedCx = cx;   // while循环的开始位置
        condition();    // 处理while里的条件
        savedCx_ = cx;  // while的do后的语句后的语句块的开始位置
        gen(JPC, 0, 0); // 条件转移指令，转移位置暂时不知

        if (sym == SYM_DO)
        {
            getsym();
        }
        else
        {
            error(18); // 'do' expected.
        }
        statement();          // 分析do后的语句块
        gen(JMP, 0, savedCx); // 无条件转移指令，跳转到cx1，再次进行逻辑判断
        code[savedCx_].a = cx; // 填写刚才那个条件转移指令的跳转位置，while循环结束
    }
}

//打印最后的pl0程序
void printCodeToFile(const char* filename, int from, int to)
{
    FILE* outFile = fopen(filename, "w");
    if (!outFile){
        fprintf(stderr, "文件打开失败\n", filename);
        exit(1);
    }

    fprintf(outFile, "printCode:\n");
    for (int i = from; i < to; i++)
    {
        fprintf(outFile, "%5d %s\t%d\t%d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
    fprintf(outFile, "\n");

    fclose(outFile);
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
    int stack[STACKSIZE]; // 假想栈
    int top = 0;          // top of stack
    instruction i;        // instruction register

    printf("开始执行PL/0程序\n");

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

    printf("\n停止执行PL/0程序\n");
}



// 一遍扫描，语法分析、语义分析、目标代码生成 一起完成
void block()
{
    //后续变量定义主要用于代码生成
    int savedCx = cx;

    dx = 3; // 分配3个单元供运行期间存放静态链SL、动态链DL和返回地址RA
    //在这段代码中，dx = 3;是为了在运行时分配3个存储单元。这三个存储单元用于存放以下信息：
    /// 静态链SL：存放的是定义该过程所对应的上一层过程，最近一次运行时的活动记录的起始单元1。
    /// 动态链DL：存放的是调用该过程前正在运行过程的活动记录的起始单元。过程返回时当前活动记录要被撤销，此时需要动态链信息来修改基址寄存器b的内容1。
    /// 返回地址RA：记录该过程返回后应该执行的下一条指令地址，即调用该过程的指令执行时指令地址寄存器p的内容加11。


    gen(JMP, 0, 0); // 跳转到分程序的开始位置，由于当前还没有知道在何处开始，所以jmp的目标暂时填为0


    while (1){
        //最开始的嵌套级别是0
        if (level > MAXLEVEL){
            error(32); // There are too many levels.
        }
        // <常量声明部分>:=const<常量定义>{,<常量定义>};
        if (sym == SYM_CONST)
        {
            getsym();
            if (sym == SYM_IDENTIFIER)
            {
                constdeclaration();
                //逗号
                while (sym == SYM_COMMA) //循环处理id1=num1,id2=num2,……
                {
                    getsym();
                    constdeclaration();
                }
                //常量定义结束
                //分号
                if (sym == SYM_SEMICOLON)
                {
                    getsym();
                }
                else
                {
                    error(5); // 没有遇到逗号 ',' 又没有遇到 ';'
                }
            }
        }
        // <变量说明>→VAR<标识符>{，<标识符>};
        else if (sym == SYM_VAR)
        {
            getsym();
            if (sym == SYM_IDENTIFIER)
            {
                vardeclaration();

                while (sym == SYM_COMMA){
                    getsym();
                    vardeclaration();
                }
                if (sym == SYM_SEMICOLON){
                    getsym();
                }
                else{
                    error(5); 
                }
            }
        }
        else if (sym == SYM_NULL)
        {
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
    gen(INT, 0, dx); // 为主程序在运行栈中开辟数据区，开辟 dx 个空间，作为这个分程序的第1条指令
    statement(); // 语句
    gen(OPR, 0, OPR_RET); // 从分程序返回（对于 0 层的程序来说，就是程序运行完成，退出）
}