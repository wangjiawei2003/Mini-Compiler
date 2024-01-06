#pragma once
#include "Common.h"

// ���ļ�����ȫ�ֱ���

char ch = ' '; // last character read ���¶�ȡ�����ַ�
FILE* fp;	   // �ļ�ָ��

int err = 0;   //�������


// �޸�NRW�����PROGRAM�ؼ���
// ����ؼ���
const char* keyword[NRW + 1] = {
	"", /* place holder */
	"BEGIN","CONST", "DO", "END", "IF",
	"THEN", "VAR", "WHILE","PROGRAM"
};

// �ؼ��ֶ�Ӧ�������
int wsym[NRW + 1] = {
	SYM_NULL, SYM_BEGIN, SYM_CONST, SYM_DO, SYM_END,
	SYM_IF,SYM_THEN, SYM_VAR, SYM_WHILE, SYM_PROGRAM
};

// ����������ͽ��
char csym[NSYM + 1] = {
	' ', '+', '-', '*', '/', '(', ')', '=', ',', ';'
};

// ������ͽ����Ӧ�������
int ssym[NSYM + 1] = {
	SYM_NULL, SYM_PLUS, SYM_MINUS, SYM_TIMES, SYM_DIVIDE,
	SYM_LPAREN, SYM_RPAREN, SYM_EQU, SYM_COMMA, SYM_SEMICOLON
};

// PL/0���Ե�Ŀ������ָ��
const char* mnemonic[MAXINS] = {
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"
};

// ���ǵ�ǰ��ȡ���ŵ����͡����ƺ�ֵ����getsym()�б���ֵ��ÿ�ν�������ű�
int sym;			   // �ʷ������ó�һ��token�����ͣ���ǰ��ȡ���ŵ�����systype
char id[MAXIDLEN + 1]; // ��ǰ��ȡ���ŵ�����
int num;			   // ��ǰ��ȡ���ŵ�ֵ

// ������table���洢Ҫ�������ջ��ķ���
constantTable table[TXMAX]; // ���ű�table[500]���洢����������

// �洢���ս��ָ�������
instruction code[CXMAX];

int level = 0; // ����Ƕ�׼��𡣺�������Ƕ�ף���������0�㣬���������ж���Ĺ�����1�㣬������㡣����������ڸ��ٵ�ǰ�����Ƕ�׼���
int cx = 0;	   // ���ǽ�Ҫ���ɵĵ�ǰָ�������������������ڸ�����һ��ָ���λ�á�
int tx = 0;	   // ���Ǳ���������������ͨ�����ڸ��ٷ��ű���������ƽṹ�ĵ�ǰλ�á�
int dx = 0;	   // �������ݷ���������Ҳ�������ݵ�ַ����������������ڸ�����һ�����õ����ݴ洢λ�á�

// ���ڴ洢��Ԫʽ������
Quadruple quadruples[QUADRUPLE_MAX]; 
int quadIndex = 100; // ���ڸ��ٵ�ǰ��Ԫʽ������
Factor temp_factor[QUADRUPLE_MAX];
int temp_factor_id = 0;

//�������б�����Ϣ
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
	/* 32 */ "Ƕ�׼���������3��"
};

//��Ԫʽ����Ŀ�����ָ��
const char* codeIntrustion[20] = {
	":=",":=","j==","j!=","j<","j<=","j>","j>=","j","+","-","*","/"
};