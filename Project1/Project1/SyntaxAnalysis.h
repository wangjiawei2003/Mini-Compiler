#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"
#include "LexicalAnalysis.h"

// º¯ÊýÉùÃ÷
void interpret();
void block();
void printCodeToFile(const char* filename, int from, int to);