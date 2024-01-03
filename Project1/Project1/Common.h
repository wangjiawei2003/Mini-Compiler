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
	LIT,
	OPR,
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC
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