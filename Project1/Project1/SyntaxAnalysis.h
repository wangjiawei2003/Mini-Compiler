#pragma once
#define _CRT_SECURE_NO_WARNINGS


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

char ch = ' '; // last character read ���¶�ȡ�����ַ�
FILE* fp;	   // �ļ�ָ��



int err = 0;   //�������



// ���������
enum symtype
{
	SYM_NULL,SYM_IDENTIFIER,SYM_NUMBER,SYM_PLUS,SYM_MINUS,SYM_TIMES,SYM_SLASH,SYM_EQU,
	SYM_NEQ,SYM_LES,SYM_LEQ,SYM_GTR,SYM_GEQ,SYM_LPAREN,SYM_RPAREN,SYM_COMMA,SYM_SEMICOLON,
	SYM_BECOMES,SYM_BEGIN,SYM_END,SYM_IF,SYM_THEN,SYM_WHILE,SYM_DO,SYM_CONST,
	SYM_VAR,
	SYM_PROGRAM
};
// �޸�NRW�����PROGRAM�ؼ���
// ����ؼ���
const char* word[NRW + 1] = {
	"", /* place holder */
	"BEGIN","CONST", "DO", "END", "IF",
	"THEN", "VAR", "WHILE","PROGRAM" };
// �ؼ��ֶ�Ӧ�������
int wsym[NRW + 1] = {
	SYM_NULL, SYM_BEGIN, SYM_CONST, SYM_DO, SYM_END,
	SYM_IF,SYM_THEN, SYM_VAR, SYM_WHILE, SYM_PROGRAM };

// ����������ͽ��
char csym[NSYM + 1] = {
	' ', '+', '-', '*', '/', '(', ')', '=', ',', ';' };
// ������ͽ����Ӧ�������
int ssym[NSYM + 1] = { //
	SYM_NULL, SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_SLASH,
	SYM_LPAREN, SYM_RPAREN, SYM_EQU, SYM_COMMA, SYM_SEMICOLON };

// �����ʶ��������룬������ڷ��ű��kind��ʹ�õ��룬��Ӧkind����enter������
enum idtype
{
	ID_CONSTANT, //����
	ID_VARIABLE, //����
};

// PL/0���Ե�Ŀ������ָ��
const char* mnemonic[MAXINS] = {
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC" };
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

// ���ǵ�ǰ��ȡ���ŵ����͡����ƺ�ֵ����getsym()�б���ֵ��ÿ�ν�������ű�
int sym;			   // ����
char id[MAXIDLEN + 1]; // ����
int num;			   // ֵ

// �Գ�������������Ϣ���д洢
typedef struct
{
	char name[MAXIDLEN + 1];
	int kind;
	int value;
} constantTable;

// �Ա�������������Ϣ���д洢
typedef struct
{
	char name[MAXIDLEN + 1]; // ��������������
	int kind;	   // ����������
	short level;   // Ƕ�׼���/��β�
	short address; // �洢λ�õ���Ե�ַ
} variableTable;

// ������table���洢Ҫ�������ջ��ķ���
constantTable table[TXMAX]; // ���ű�table[500]���洢����������

// Ŀ�����code�����ִ��������Ϣ����ת���ʹ洢
typedef struct
{
	int f; // ������
	int l; // ��β�
	int a; // λ�Ƶ�ַ
} instruction;

// �洢ָ�������
instruction code[CXMAX];

int level = 0; // ����Ƕ�׼��𡣺�������Ƕ�ף���������0�㣬���������ж���Ĺ�����1�㣬������㡣����������ڸ��ٵ�ǰ�����Ƕ�׼���
int cx = 0;	   // ���ǽ�Ҫ���ɵĵ�ǰָ�������������������ڸ�����һ��ָ���λ�á�
int tx = 0;	   // ���Ǳ���������������ͨ�����ڸ��ٷ��ű���������ƽṹ�ĵ�ǰλ�á�
int dx = 0;	   // �������ݷ���������Ҳ�������ݵ�ַ����������������ڸ�����һ�����õ����ݴ洢λ�á�

//������Ϣ����Ҫ�޸�
const char* err_msg[] =
{
	/*  0 */ "Fatal Error:Unknown character.\n",
	/*  1 */ "��ֵ����ó���\'=\'",
	/*  2 */ "��������ֵ�ڸ�ֵ���� ':='����",
	/*  3 */ "�ڱ�ʶ����������и�ֵ���� ':='",
	/*  4 */ "��const����һ��Ҫ�Ǳ�ʶ��",
	/*  5 */ "Ӧʹ��','��';'����",
	/*  6 */ "Incorrect procedure name.",
	/*  7 */ "Statement expected.",
	/*  8 */ "Follow the statement is an incorrect symbol.",
	/*  9 */ "'.' expected.",
	/* 10 */ "Ӧʹ��';'����",
	/* 11 */ "δ�������",
	/* 12 */ "�������ܸ�ֵ",
	/* 13 */ "Ӧʹ��':='����",
	/* 14 */ "There must be an identifier to follow the 'call'.",
	/* 15 */ "A constant or variable can not be called.",
	/* 16 */ "Ӧʹ��'then'����",
	/* 17 */ "Ӧʹ��';'���Ż�'end'����",
	/* 18 */ "Ӧʹ��'do'����",
	/* 19 */ "Incorrect symbol.",
	/* 20 */ "Relative operators expected.",
	/* 21 */ "Procedure identifier can not be in an expression.",
	/* 22 */ "û�� \')\'��ƥ��",
	/* 23 */ "The symbol can not be followed by a factor.",
	/* 24 */ "The symbol can not be as the beginning of an expression.",
	/* 25 */ "The number is too great.",
	/* 26 */ "The identifier is too long",
	/* 27 */ "�����˳�����",
	/* 28 */ "",
	/* 29 */ "",
	/* 30 */ "ȱ�ٳ����ײ�\'PROGRAM\'�ؼ���",
	/* 31 */ "ȱ�ٳ����ײ�\'PROGRAM\'�ؼ��ʺ���ı�ʶ��",
	/* 32 */ "Ƕ�׼���������3��" };

////�õ������з���
//void enter(int kind);
//int position(char* id);
//void constdeclaration();
//void vardeclaration(void);
//void gen(int x, int y, int z);
//void factor();
//void term();
//void expression();
//void condition();
//void statement();
//void printCode(int from, int to);
//int base(int stack[], int currentLevel, int levelDiff);
//void interpret();
//void block();
