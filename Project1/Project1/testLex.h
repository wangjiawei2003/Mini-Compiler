
#include "SyntaxAnalysis.h"
using namespace std;

// �ڳ���������ʱ�����Ӧ�Ĵ�����Ϣ�������ۼӴ�������� err ��ֵ��
void error(int n)
{
    err++;
    cout << "Error " << n << ": " << err_msg[n] << "\n";
}

void getch()
{
    ch = getc(fp);
}

void getsym()
{
    while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
        getch();
    }
    if (ch == EOF)
    {
        exit(0);
    }

    if (isalpha(ch))
    {
        char a[MAXIDLEN + 1];
        int k = 0;
        while ((isalpha(ch) || isdigit(ch)) && k < MAXIDLEN)
        {
            a[k++] = ch;
            getch();
        }

        a[k] = '\0';
        strcpy(id, a);

        int i = 1;
        for (; i <= NRW; i++)
        {
            if (strcmp(a, word[i]) == 0)
                break;
        }
        if (i <= NRW)
        {
            sym = wsym[i];
            cout << "��⵽�ؼ���: " << a << "\n";
        }
        else
        {
            sym = SYM_IDENTIFIER;
            cout << "��⵽��ʶ��: " << a << "\n";
        }
    }
    else if (isdigit(ch))
    {
        sym = SYM_NUMBER;
        int k = num = 0;
        while (isdigit(ch))
        {
            num = num * 10 + ch - '0';
            getch();
        }
        cout << "��⵽����: " << num << "\n";
    }
    else if (ch == ':')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_BECOMES;
            getch();
        }
        else
        {
            sym = SYM_NULL;
        }
        cout << "��⵽��ֵ���: :=\n";
    }
    else if (ch == '>')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_GEQ;
            getch();
        }
        else
        {
            sym = SYM_GTR;
        }
        cout << "��⵽�Ƚ�: >/>=\n";
    }
    else if (ch == '<')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_LEQ;
            getch();
        }
        else if (ch == '>')
        {
            sym = SYM_NEQ;
            getch();
        }
        else
        {
            sym = SYM_LES;
        }
        cout << "��⵽�Ƚ�: </<=/<>\n";
    }
    else
    {
        int i = 1;
        for (; i <= NSYM; i++)
        {
            if (ch == csym[i])
                break;
        }
        if (i <= NSYM)
        {
            sym = ssym[i];
            getch();
        }
        else
        {
            error(0);
            sym = SYM_NULL;
        }
        cout << "��⵽�����ַ�\n";
    }
}
