#include "SyntaxAnalysis.h"
#include "testLex.h"
#include "SyntaxAnalysis.cpp"


int main()
{
    // ��Դ�ļ�
    fp = fopen("input2.txt", "r");
    if (!fp)
    {
        printf("�ļ�������");
        exit(1);
    }

    // ���Դ�����Ƿ���'PROGRAM'�ؼ��ʺ�һ����ʶ����ʼ
    getsym();
    if (sym != SYM_PROGRAM) // ����SYM_PROGRAM��'PROGRAM'�ؼ��ʵĴʷ���Ԫ����
    {
        error(30); // �������30��ʾ"Missing 'PROGRAM' keyword."
    }
    else
    {
        getsym();
        if (sym != SYM_IDENTIFIER)
        {
            error(31); // �������31��ʾ"Missing identifier after 'PROGRAM' keyword."
        }
        else
        {
            getsym();
        }
    }

    // <����>:=<�ֳ���>.

    block();

    if (err)
        printf("There are %d error(s) in PL/0 program.\n", err);
    else
        interpret();
    printCodeToFile("output.txt", 0, cx);
    return 0;
}