#include "SyntaxAnalysis.h"
#include"CommonVar.h"

// ��Ŀ���
int main()
{
    fp = fopen("input.txt", "r");
    // �ļ�������
    if (!fp){
        printf("�ļ�������");
        exit(1);
    }

    // ���Դ�����Ƿ���'PROGRAM'�ؼ��ʺ�һ����ʶ����ʼ
    getsym();
    // SYM_PROGRAM��'PROGRAM'�ؼ��ʵĴʷ���Ԫ����
    if (sym != SYM_PROGRAM) {
        error(30); // ����30��ʾ"Missing 'PROGRAM' keyword."
    }
    else{
        getsym();
        if (sym != SYM_IDENTIFIER){
            error(31); // ����31��ʾ"Missing identifier after 'PROGRAM' keyword."
        }
        else{
            getsym();
        }
    }

    // <����>:=<�ֳ���>.

    block();

    if (err)
        cout << "There are " << err << "error(s) in PL / 0 program." << endl;
    else
        ;
        //interpret();

    // �����output.txt
    printCodeToFile("output.txt", 0, cx);
    return 0;
}