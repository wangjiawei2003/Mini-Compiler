#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"
#include "LexicalAnalysis.h"

// 函数声明
void interpret();
void block();
void printCodeToFile(const char* filename, int from, int to);




void constDeclaration();         // <常量定义>→<标识符>:=<无符号整数>
void assignmentDeclaration();    // <赋值语句>→<标识符>:=<表达式>


void expression();               // <表达式>→[+|-]项 | <表达式> <加法运算符> <项>
void conditionDeclaration();     // <条件语句>→IF <条件> THEN <语句>
void circulationDeclaration();   // <循环语句>→WHILE <条件> DO <语句>
void combinedDeclaration();

void condition();
void statement();

void term();

void emit(int op, int arg1, int arg2, int  result);


extern Quadruple quadruples[QUADRUPLE_MAX];
extern int quadIndex; 
extern Factor temp_factor[QUADRUPLE_MAX];
extern int temp_factor_id;
