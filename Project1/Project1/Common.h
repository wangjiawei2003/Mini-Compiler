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
#define QUADRUPLE_MAX 500 // ��Ԫʽ���������


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
	LIT, // ���������ص�ջ����ָ���ʽ��LIT 0, A
	OPR, // ִ��������߼�������ָ���ʽ��OPR L, A������ L �ǲ��������ͣ�A �ǲ�����
	LOD, // ���������ص�ջ����ָ���ʽ��LOD L, A������ L �ǲ�βA �Ǳ�������Ե�ַ
	STO, // ��ջ����ֵ�洢�������У�ָ���ʽ��STO L, A������ L �ǲ�βA �Ǳ�������Ե�ַ
	CAL, // ���ù��̣�ָ���ʽ��CAL L, A������ L �Ǳ����ù��̵Ĳ�βA �Ǳ����ù��̵ĵ�ַ
	INT, // ����ռ䣬ָ���ʽ��INT 0, A������ A ��Ҫ��������ݿռ��С
	JMP, // ��������ת��ָ���ʽ��JMP 0, A������ A ����ת��Ŀ���ַ
	JPC, // ������ת�����ջ��Ϊ������ת��ָ���ʽ��JPC 0, A������ A ����ת��Ŀ���ַ

};

//
enum operationCode {
	CONSTASSIGNMENT, // ��ֵ
	ASSIGNMENT,      // ��ֵ
	OPRA_EQU,             //�Ƚϣ��߼�����
	OPRA_NEQ,             //�Ƚϣ��߼�����
	OPRA_LES,             //�Ƚϣ��߼�����
	OPRA_LEQ,             //�Ƚϣ��߼�����
	OPRA_GTR,             //�Ƚϣ��߼�����
	OPRA_GEQ,             //�Ƚϣ��߼�����
	OPRA_JMP,             //��ת

	OPRA_ADD,             //�ӷ�����
	OPRA_MIN,             //��������
	OPRA_MUL,             //�˷�����
	OPRA_DIV,             //��������
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


// ��Ԫʽ�ṹ
typedef struct {
	int op;      // ������
	int arg1;    // ����1
	int arg2;    // ����2
	int result;  // ���
} Quadruple;


typedef struct {
	int index;  //0 constant ������1 ����
	int value;
}Factor;
