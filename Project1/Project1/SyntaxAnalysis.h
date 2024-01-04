#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"
#include "LexicalAnalysis.h"

// º¯ÊýÉùÃ÷
void interpret();
void block();
void printCodeToFile(const char* filename, int from, int to);
void expression();
void circulationDeclaration();
void conditionDeclaration();


extern Quadruple quadruples[QUADRUPLE_MAX];
extern int quadIndex; 


