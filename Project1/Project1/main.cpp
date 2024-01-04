#include "SyntaxAnalysis.h"
#include"CommonVar.h"

// 项目入口
int main()
{
    fp = fopen("input.txt", "r");
    // 文件不存在
    if (!fp){
        printf("文件不存在");
        exit(1);
    }

    // 检查源代码是否以'PROGRAM'关键词和一个标识符开始
    getsym();
    // SYM_PROGRAM是'PROGRAM'关键词的词法单元类型
    if (sym != SYM_PROGRAM) {
        error(30); // 错误30表示"Missing 'PROGRAM' keyword."
    }
    else{
        getsym();
        if (sym != SYM_IDENTIFIER){
            error(31); // 错误31表示"Missing identifier after 'PROGRAM' keyword."
        }
        else{
            getsym();
        }
    }

    // <程序>:=<分程序>.

    block();

    if (err)
        cout << "There are " << err << "error(s) in PL / 0 program." << endl;
    else
        ;
        //interpret();

    // 输出至output.txt
    printCodeToFile("output.txt", 0, cx);
    return 0;
}