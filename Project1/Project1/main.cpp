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
        // �����output.txt
        printCodeToFile("output.txt", 0, cx);


    // JUST for Test
    //cout << "----------------------" << endl;
    //for (int i = 0;i < 15;i++)
    //    cout <<i<<"   " << quadruples[i].op << "   "
    //    << quadruples[i].arg1 << "   "
    //    << quadruples[i].arg2 << "   "
    //    << quadruples[i].result << endl;

    //for (int i = 0;i < 15;i++)
    //    cout << i << "   " << temp_factor[i].index << "   " << temp_factor[i].value << endl;
    //   

    for (int i = 0;i < 15;i++)
        cout << i << "   " << table[i].kind << "   " << table[i].name << "      " << table[i].value << endl;

    return 0;
}