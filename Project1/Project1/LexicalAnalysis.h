#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
using namespace std;

// ʹ��CommonVar�е�ȫ�ֱ���
extern char ch;
extern int err;
extern enum symtype;
extern FILE* fp;	   // �ļ�ָ��
extern const char* err_msg[];
extern int sym;			   // ����
extern char id[MAXIDLEN + 1]; // ����
extern int num;			   // ֵ
extern const char* keyword[NRW + 1];
extern int wsym[NRW + 1];
extern char csym[NSYM + 1];
extern int ssym[NSYM + 1];
extern const char* mnemonic[MAXINS];
extern int level;
extern int cx;	
extern int tx;
extern int dx;
extern instruction code[CXMAX];
extern constantTable table[TXMAX]; // ���ű�table[500]���洢����������



void error(int n);
void getsym();
void getch();