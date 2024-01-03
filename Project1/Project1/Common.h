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
#define MAXLEVEL 3		 // ���Ƕ�׼���
#define MAXINS 8		 // maximum number of instructions
#define CXMAX 500		 // size of code array
#define MAXADDRESS 32767 // maximum address
#define STACKSIZE 200	 // maximum storage

// ���������
enum symtype {
	SYM_NULL, SYM_IDENTIFIER, SYM_NUMBER,
	SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_DIVIDE,
	SYM_LPAREN, SYM_RPAREN, SYM_EQU, SYM_COMMA,
	SYM_PERIOD, SYM_SEMICOLON,
	SYM_NEQ, SYM_LES, SYM_LEQ, SYM_GTR, SYM_GEQ, SYM_ASSIGN,
	SYM_PROGRAM, SYM_BEGIN, SYM_END, SYM_CONST, SYM_VAR, SYM_WHILE, SYM_DO, SYM_IF, SYM_THEN
};


// �����ʶ��������룬������ڷ��ű��kind��ʹ�õ��룬��Ӧkind����enter������
enum idtype
{
	ID_CONSTANT, //����
	ID_VARIABLE, //����
};

// ָ��Ĺ�����
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

// ָ��Ĳ�����
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
	char name[MAXIDLEN + 1]; // ��������������
	int kind;	   // ����������
	short level;   // Ƕ�׼���/��β�
	short address; // �洢λ�õ���Ե�ַ
} variableTable;

typedef struct
{
	char name[MAXIDLEN + 1];
	int kind;
	int value;
} constantTable;

typedef struct
{
	int f; // ������
	int l; // ��β�
	int a; // λ�Ƶ�ַ
} instruction;