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
extern int level; // 这是嵌套级别。函数可以嵌套，主程序是0层，在主程序中定义的过程是1层，最多三层。这个变量用于跟踪当前代码的嵌套级别。
extern int cx;	  // 这是将要生成的当前指令的索引。这个变量用于跟踪下一条指令的位置。
extern int tx;
extern int dx;    // 这是数据分配索引，也就是数据地址索引。这个变量用于跟踪下一个可用的数据存储位置。
extern instruction code[CXMAX];
extern constantTable table[TXMAX]; // 符号表table[500]，存储常量、变量



void error(int n);
void getsym();
void getch();