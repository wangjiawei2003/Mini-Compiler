#include "SyntaxAnalysis.h"
#include "testLex.h"
#include "SyntaxAnalysis.cpp"


int main()
{
    // 打开源文件
    fp = fopen("input2.txt", "r");
    if (!fp)
    {
        printf("文件不存在");
        exit(1);
    }

    // 检查源代码是否以'PROGRAM'关键词和一个标识符开始
    getsym();
    if (sym != SYM_PROGRAM) // 假设SYM_PROGRAM是'PROGRAM'关键词的词法单元类型
    {
        error(30); // 假设错误30表示"Missing 'PROGRAM' keyword."
    }
    else
    {
        getsym();
        if (sym != SYM_IDENTIFIER)
        {
            error(31); // 假设错误31表示"Missing identifier after 'PROGRAM' keyword."
        }
        else
        {
            getsym();
        }
    }

    // <程序>:=<分程序>.

    block();

    if (err)
        printf("There are %d error(s) in PL/0 program.\n", err);
    else
        interpret();
    printCodeToFile("output.txt", 0, cx);
    return 0;
}