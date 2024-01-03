#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
using namespace std;

// 使用CommonVar中的全局变量
extern char ch;
extern int err;
extern enum symtype;
extern FILE* fp;	   // 文件指针
extern const char* err_msg[];
extern int sym;			   // 类型
extern char id[MAXIDLEN + 1]; // 名称
extern int num;			   // 值
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
extern constantTable table[TXMAX]; // 符号表table[500]，存储常量、变量



void error(int n);
void getsym();
void getch();