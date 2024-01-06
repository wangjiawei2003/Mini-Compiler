#pragma once
#include "Common.h"

// 此文件定义全局变量

char ch = ' '; // last character read 最新读取到的字符
FILE* fp;	   // 文件指针

int err = 0;   //错误计数


// 修改NRW，添加PROGRAM关键词
// 定义关键字
const char* keyword[NRW + 1] = {
	"", /* place holder */
	"BEGIN","CONST", "DO", "END", "IF",
	"THEN", "VAR", "WHILE","PROGRAM"
};

// 关键字对应的类别码
int wsym[NRW + 1] = {
	SYM_NULL, SYM_BEGIN, SYM_CONST, SYM_DO, SYM_END,
	SYM_IF,SYM_THEN, SYM_VAR, SYM_WHILE, SYM_PROGRAM
};

// 定义运算符和界符
char csym[NSYM + 1] = {
	' ', '+', '-', '*', '/', '(', ')', '=', ',', ';'
};

// 运算符和界符对应的类别码
int ssym[NSYM + 1] = {
	SYM_NULL, SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_DIVIDE,
	SYM_LPAREN, SYM_RPAREN, SYM_EQU, SYM_COMMA, SYM_SEMICOLON
};

// PL/0语言的目标代码的指令
const char* mnemonic[MAXINS] = {
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"
};

// 这是当前读取符号的类型、名称和值，在getsym()中被赋值，每次将存入符号表
int sym;			   // 词法分析得出一个token的类型，当前读取符号的类型systype
char id[MAXIDLEN + 1]; // 当前读取符号的名称
int num;			   // 当前读取符号的值

// 这里用table来存储要放入符号栈里的符号
constantTable table[TXMAX]; // 符号表table[500]，存储常量、变量

// 存储最终结果指令的数组
instruction code[CXMAX];

int level = 0; // 这是嵌套级别。函数可以嵌套，主程序是0层，在主程序中定义的过程是1层，最多三层。这个变量用于跟踪当前代码的嵌套级别。
int cx = 0;	   // 这是将要生成的当前指令的索引。这个变量用于跟踪下一条指令的位置。
int tx = 0;	   // 这是表的索引。这个变量通常用于跟踪符号表或其他类似结构的当前位置。
int dx = 0;	   // 这是数据分配索引，也就是数据地址索引。这个变量用于跟踪下一个可用的数据存储位置。

// 用于存储四元式的数组
Quadruple quadruples[QUADRUPLE_MAX]; 
int quadIndex = 100; // 用于跟踪当前四元式的索引
Factor temp_factor[QUADRUPLE_MAX];
int temp_factor_id = 0;

//定义所有报错信息
const char* err_msg[] =
{
	/*  0 */ "Fatal Error:Unknown character.\n",
	/*  1 */ "赋值语句用成了\'=\'",
	/*  2 */ "必须由数值在赋值符号 ':='后面",
	/*  3 */ "在标识符后面必须有赋值符号 ':='",
	/*  4 */ "在const后面一定要是标识符",
	/*  5 */ "应使用','或';'符号",
	/*  6 */ "Incorrect procedure name.",
	/*  7 */ "Statement expected.",
	/*  8 */ "Follow the statement is an incorrect symbol.",
	/*  9 */ "'.' expected.",
	/* 10 */ "应使用';'符号",
	/* 11 */ "未定义变量",
	/* 12 */ "常量不能赋值",
	/* 13 */ "应使用':='符号",
	/* 14 */ "There must be an identifier to follow the 'call'.",
	/* 15 */ "A constant or variable can not be called.",
	/* 16 */ "应使用'then'符号",
	/* 17 */ "应使用';'符号或'end'符号",
	/* 18 */ "应使用'do'符号",
	/* 19 */ "Incorrect symbol.",
	/* 20 */ "Relative operators expected.",
	/* 21 */ "Procedure identifier can not be in an expression.",
	/* 22 */ "没有 \')\'做匹配",
	/* 23 */ "The symbol can not be followed by a factor.",
	/* 24 */ "The symbol can not be as the beginning of an expression.",
	/* 25 */ "The number is too great.",
	/* 26 */ "The identifier is too long",
	/* 27 */ "出错，退出程序",
	/* 28 */ "",
	/* 29 */ "",
	/* 30 */ "缺少程序首部\'PROGRAM\'关键词",
	/* 31 */ "缺少程序首部\'PROGRAM\'关键词后面的标识符",
	/* 32 */ "嵌套级别过大，最多3层"
};

//四元式生成目标代码指令
const char* codeIntrustion[20] = {
	":=",":=","j==","j!=","j<","j<=","j>","j>=","j","+","-","*","/"
};