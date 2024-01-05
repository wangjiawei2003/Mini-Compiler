#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"
#include "LexicalAnalysis.h"

// ��������
void interpret();
void block();
void printCodeToFile(const char* filename, int from, int to);




void constDeclaration();         // <��������>��<��ʶ��>:=<�޷�������>
void assignmentDeclaration();    // <��ֵ���>��<��ʶ��>:=<���ʽ>


void expression();               // <���ʽ>��[+|-]�� | <���ʽ> <�ӷ������> <��>
void conditionDeclaration();     // <�������>��IF <����> THEN <���>
void circulationDeclaration();   // <ѭ�����>��WHILE <����> DO <���>
void combinedDeclaration();

void condition();
void statement();

void term();

void emit(int op, int arg1, int arg2, int  result);


extern Quadruple quadruples[QUADRUPLE_MAX];
extern int quadIndex; 
extern Factor temp_factor[QUADRUPLE_MAX];
extern int temp_factor_id;
