#include "LexicalAnalysis.h"


// �ڳ���������ʱ�����Ӧ�Ĵ�����Ϣ�������ۼӴ�������� err ��ֵ��
// n�����n������
void error(int n)
{
    err++;
    cout << "Error " << n << ": " << err_msg[n] << "\n";
}

void getch(){
    ch = getc(fp);
}



void getsym(){
	while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
	{
		ch = fgetc(fp);
	}
	if (isalpha(ch)) // ��ǰ����Ϊ��ĸ,��Ӧ��Ϊ�ؼ��ֻ��ʶ��
	{
		char a[MAXIDLEN + 1]; // ��ǰ��ȡ���ĵ���
		int k = 0;
		for (; (isalpha(ch) || isdigit(ch)) && k < MAXIDLEN; k++)
		{
			a[k] = ch;
			ch = fgetc(fp);
		}

		a[k] = '\0'; // �ַ�������ַ�����������ǽ�β����\0��һ��Ҫ���ϣ�
		// ����Ƿ�Ϊ�ؼ���
		int i = 1;
		for (; i <= NRW; i++)
		{

			if (strcmp(a, keyword[i]) == 0)
			{
				break;
			}

		}
		if (i <= NRW)
		{
			sym = wsym[i]; // symbol is a reserved word
		}
		else
		{
			sym = SYM_IDENTIFIER; // symbol is an identifier
			strcpy(id, a);

		}
		printf("(%d,%s)\n", sym, a);
	}
	else if (isdigit(ch))
	{ // symbol is a number.
		sym = SYM_NUMBER;
		int k = 0;
		num = 0;
		while (isdigit(ch))
		{
			num = num * 10 + ch - '0';
			ch = fgetc(fp);
			k++;
		}
		if (k > MAXNUMLEN)
			error(25); // The number is too great.
		else
		{
			printf("(%d,%d)\n", sym, num);
		}
	}
	else if (ch == ':')
	{
		ch = fgetc(fp);
		if (ch == '=')
		{
			sym = SYM_ASSIGN; // :=
			ch = fgetc(fp);
			printf("(%d,:=)\n", sym);
		}
		else
		{
			sym = SYM_NULL; // illegal?
		}
	}
	else if (ch == '>')
	{
		ch = fgetc(fp);
		if (ch == '=')
		{
			sym = SYM_GEQ; // >=
			ch = fgetc(fp);
			printf("(%d,>=)\n", sym);
		}
		else
		{
			sym = SYM_GTR; // >
			printf("(%d,=)\n", sym);
		}
	}
	else if (ch == '<')
	{
		ch = fgetc(fp);
		if (ch == '=')
		{
			sym = SYM_LEQ; // <=
			ch = fgetc(fp);
			printf("(%d,<=)\n", sym);
		}
		else if (ch == '>')
		{
			sym = SYM_NEQ; // <>
			ch = fgetc(fp);
		}
		else
		{
			sym = SYM_LES; // <
			printf("(%d,<)\n", sym);
		}
	}
	//else if (ch == '{')
	//{ //����ע��
	//	int end = 1;
	//	while (end)
	//	{
	//		ch = fgetc(fp);
	//		if (ch == '}')
	//			end = 0;
	//	}
	//	ch = fgetc(fp);
	//}
	else
	{ // other tokens : '+', '-', '*', '/', '(', ')', '=', ',', '.', ';'
		//�����ʶ��ؼ�����������
		int i = 1;
		for (; i <= NSYM; i++)
		{
			if (ch == csym[i])
				break;
		}
		if (i <= NSYM)
		{
			sym = ssym[i];
			printf("(%d,%c)\n", sym, ch);
			ch = fgetc(fp);
		}
		//��Ӧ�ó��ֵ��ַ�
		else if (ch != EOF)
		{

			printf("Fatal Error: Unknown character.\n");
			exit(1);
		}
	}
}


//void lexer(FILE* fp)
//{
//	ch = fgetc(fp);
//	int num_temp = 0;//read one effective symbol
//
//	while (ch != EOF && num_temp == 0)
//	{
//		//num_temp++;//delete this sentence ,then we can finish the lexer once a time 
//		while (isspace(ch)) {
//			ch = fgetc(fp);
//		}
//		if (ch == '{') // Comment
//		{
//			do
//			{
//				ch = fgetc(fp);
//			} while (ch != '}' && ch != EOF);
//			ch = fgetc(fp);
//
//		}
//
//		if (isalpha(ch)) // words or identifier
//		{
//			char words[MAXIDLEN + 1];
//			int len = 0;
//
//			//Check if there is still a letter or num 
//			while (isalnum(ch) && len < MAXIDLEN)
//			{
//				words[len++] = ch;
//				ch = fgetc(fp);
//			}
//
//			words[len] = '\0'; // Null-terminate the string
//			strcpy(id, words); //save the value of identifier 
//
//			// Check if it's a keyword than is reserved
//			int i = 0;
//			while (i <= NRW && strcmp(words, keyword[i]) != 0)
//			{
//				i++;
//			}
//			if (i <= NRW) {
//				sym = wsym[i];
//				printf("(%d,%s)\n", wsym[i], words);
//			}
//			else {
//				sym = SYM_IDENTIFIER;
//				printf("(%d,%s)\n", SYM_IDENTIFIER, words);
//			}
//		}
//		else if (isdigit(ch)) // Number
//		{
//			int num = 0;
//			int len = 0;
//
//			while (isdigit(ch) && len < MAXNUMLEN)
//			{
//				num = num * 10 + (ch - '0');
//				ch = fgetc(fp);
//				len++;
//			}
//
//			if (len == MAXNUMLEN)
//				error(25); // The number is too large
//			sym = SYM_NUMBER;
//			printf("(%d,%d)\n", SYM_NUMBER, num);
//		}
//		else if (ch == ':')
//		{
//			ch = fgetc(fp);
//			sym = (ch == '=') ? SYM_ASSIGN : SYM_NULL; // :=
//			if (ch == '=') ch = fgetc(fp);
//			printf("(%d,:=)\n", sym);
//		}
//		else if (ch == '>')
//		{
//			ch = fgetc(fp);
//			if (ch == '=')
//			{
//				sym = SYM_GEQ; // >=
//				ch = fgetc(fp);
//			}
//			else
//			{
//				sym = SYM_GTR; // >
//			}
//			printf("(%d,>=)\n", sym);
//		}
//		else if (ch == '<')
//		{
//			ch = fgetc(fp);
//			if (ch == '=')
//			{
//				sym = SYM_LEQ; // <=
//				ch = fgetc(fp);
//			}
//			else if (ch == '>')
//			{
//				sym = SYM_NEQ; // <>
//				ch = fgetc(fp);
//			}
//			else
//			{
//				sym = SYM_LES; // <
//			}
//			printf("(%d,<)\n", sym);
//		}
//		else
//		{
//			// other tokens : '+', '-', '*', '/', '(', ')', '=', ',', '.', ';'
//			int index = 0;
//
//			while (index <= NSYM && ch != csym[index]) {
//				index++;
//			}
//
//			if (index <= NSYM) {
//				sym = ssym[index];
//				printf("(%d,%c)\n", sym, ch);
//				ch = fgetc(fp);
//			}
//			// Unrecognized character
//			else {
//				fprintf(stderr, "Fatal Error: Unknown character %c\n", ch);
//				exit(1);
//			}
//		}
//
//	}
//	//printf("END");
//}