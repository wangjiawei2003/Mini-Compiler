#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>


#define NRW 9			 // number of reserved words
#define MAXNUMLEN 14	 // maximum number of digits in numbers
#define NSYM 9			 // maximum number of symbols in array ssym and csym
#define MAXIDLEN 10		 // length of identifiers
#define TXMAX 500		 // length of identifier table
#define MAXLEVEL 3		 // 最大嵌套级别
#define MAXINS 8		 // maximum number of instructions
#define CXMAX 500		 // size of code array
#define MAXADDRESS 32767 // maximum address
#define STACKSIZE 200	 // maximum storage
#define QUADRUPLE_MAX 500 // 四元式的最大数量


// 定义类别码
enum symtype {
	SYM_NULL, SYM_IDENTIFIER, SYM_NUMBER,
	SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_DIVIDE,
	SYM_LPAREN, SYM_RPAREN, SYM_EQU, SYM_COMMA,
	SYM_PERIOD, SYM_SEMICOLON,
	SYM_NEQ, SYM_LES, SYM_LEQ, SYM_GTR, SYM_GEQ, SYM_ASSIGN,
	SYM_PROGRAM, SYM_BEGIN, SYM_END, SYM_CONST, SYM_VAR, SYM_WHILE, SYM_DO, SYM_IF, SYM_THEN
};


// 定义标识符的类别码，这个是在符号表的kind中使用的码，对应kind传入enter函数中
enum idtype
{
	ID_CONSTANT, //常量
	ID_VARIABLE, //变量
};

// 指令的功能码
enum opcode
{
	LIT, // 将常数加载到栈顶，指令格式：LIT 0, A
	OPR, // 执行运算或逻辑操作，指令格式：OPR L, A，其中 L 是操作的类型，A 是操作数
	LOD, // 将变量加载到栈顶，指令格式：LOD L, A，其中 L 是层次差，A 是变量的相对地址
	STO, // 将栈顶的值存储到变量中，指令格式：STO L, A，其中 L 是层次差，A 是变量的相对地址
	CAL, // 调用过程，指令格式：CAL L, A，其中 L 是被调用过程的层次差，A 是被调用过程的地址
	INT, // 分配空间，指令格式：INT 0, A，其中 A 是要分配的数据空间大小
	JMP, // 无条件跳转，指令格式：JMP 0, A，其中 A 是跳转的目标地址
	JPC, // 条件跳转，如果栈顶为零则跳转，指令格式：JPC 0, A，其中 A 是跳转的目标地址

};

//
enum operationCode {
	CONSTASSIGNMENT, // 赋值
	ASSIGNMENT,      // 赋值
	OPRA_EQU,             //比较，逻辑操作
	OPRA_NEQ,             //比较，逻辑操作
	OPRA_LES,             //比较，逻辑操作
	OPRA_LEQ,             //比较，逻辑操作
	OPRA_GTR,             //比较，逻辑操作
	OPRA_GEQ,             //比较，逻辑操作
	OPRA_JMP,             //跳转

	OPRA_ADD,             //加法运算
	OPRA_MIN,             //减法运算
	OPRA_MUL,             //乘法运算
	OPRA_DIV,             //除法运算
};



// 指令的操作码
enum oprcode
{
	OPR_RET,
	OPR_NEG,
	OPR_ADD,
	OPR_MIN,
	OPR_MUL,
	OPR_DIV,
	OPR_ODD,
	OPR_EQU,
	OPR_NEQ,
	OPR_LES,
	OPR_LEQ,
	OPR_GTR,
	OPR_GEQ
};


typedef struct
{
	char name[MAXIDLEN + 1]; // 变量名、过程名
	int kind;	   // 变量、过程
	short level;   // 嵌套级别/层次差
	short address; // 存储位置的相对地址
} variableTable;

typedef struct
{
	char name[MAXIDLEN + 1];
	int kind;
	int value;
} constantTable;

typedef struct
{
	int f; // 操作码
	int l; // 层次差
	int a; // 位移地址
} instruction;


// 四元式结构
typedef struct {
	int op;      // 操作码
	int arg1;    // 参数1
	int arg2;    // 参数2
	int result;  // 结果
} Quadruple;


typedef struct {
	int index;  //0 constant 常量，1 变量
	int value;
}Factor;
