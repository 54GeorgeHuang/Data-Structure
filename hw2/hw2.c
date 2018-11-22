#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define MAX_EXPR 500
#define LP 0 //left (
#define RP 1 //right )
#define PL 2 //plus
#define MI 3 //minus
#define TI 4 //times
#define DI 5 //divide
#define EOS 6 //end of string
#define OP 7 //operand

char expr[MAX_EXPR] = "";
char expr_pf[MAX_EXPR] = "";//expression for postfix
int isp[] = {0, 19, 12, 12, 13, 13, 0};//in stack precedence
int icp[] = {20, 19, 12, 12, 13, 13, 0};

typedef struct stack* stackPointer;
typedef struct stack { 
	int data;
	stackPointer link;
} stack;
stackPointer top;

void push(int item)
{
	stackPointer temp;
	temp = malloc(sizeof(*temp));
	assert(temp != NULL);

	temp->data = item;
	temp->link = top;
	top = temp;

	return;
}	

int pop()
{
	stackPointer temp = top;
	int item;

	assert(temp != NULL);
	item = temp->data;
	top = temp->link;
	free(temp);

	return item;
}

typedef struct stackF* stackFPointer;
typedef struct stackF { 
	double data;
	stackFPointer link;
} stackF;
stackFPointer topF;

void pushF(double item)
{
	stackFPointer temp;
	temp = malloc(sizeof(*temp));
	assert(temp != NULL);

	temp->data = item;
	temp->link = topF;
	topF = temp;
}	

double popF()
{
	stackFPointer temp = topF;
	double item;

	assert(temp != NULL);
	item = temp->data;
	topF = temp->link;
	free(temp);

	return item;
}

int getTokenPostFix(char* symbol, int* n)
{
	*symbol = expr_pf[(*n)++];

	switch (*symbol)
	{
		case '(' :
		 	return LP;
		case ')' :
			return RP;
		case '+' :
			return PL;
		case '-' :
			return MI;
		case '/' :
			return DI;
		case '*' :
			return TI;
		case '\0' :
			return EOS;
		case '\n' :
			return EOS;
		default :
			return OP;
	}
}

int getInt(char* symbol, int* n)
{
	int k = 0;

	while ( *symbol >= 48 && *symbol <= 57)
	{
		k *= 10;
		k += *symbol - '0';
		*symbol = expr_pf[(*n)++];
	}
	(*n)--;
	
	return k;
}

double eval(void)
{
	char symbol;
	int token;
	double op1, op2;
	int n = 0;
	topF = NULL;

	token = getTokenPostFix(&symbol, &n);
	while (token != EOS)
	{
		if (token == OP)
			pushF( (double)getInt(&symbol, &n));
		else
		{
			op2 = popF();
			op1 = popF();
			switch(token)
			{
				case PL:
					pushF(op1+op2);
					break;
				case MI:
					pushF(op1-op2);
					break;
				case TI:
					pushF(op1*op2);
					break;
				case DI:
					pushF(op1/op2);
					break;
			}
		}
		n++;
		token = getTokenPostFix(&symbol, &n);
	}
	return popF();
}

void append(char* s, char c)
{
	int len = strlen(s);
	assert(len+1 < MAX_EXPR);

	s[len] = c;
	s[len+1] = ' ';
	s[len+2] = '\0';

	return;
}

void printToken(int data)
{
	switch (data) 
	{
		case RP : 
			append(expr_pf, ')');
			return;
		case LP :
			append(expr_pf, '(');
			return;
		case PL :
			append(expr_pf, '+');
			return;
		case MI :
			append(expr_pf, '-');
			return;
		case TI :
			append(expr_pf, '*');
			return;
		case DI :
			append(expr_pf, '/');
			return;
		case EOS :
			return;
		default :
			return;
	}
}

void int2String(char* symbol, char* symbolN, int* n)//put multiple char(stands for int) to a string
{
	int k = 0;
	while ( *symbol >= 48 && *symbol <= 57)
	{
		symbolN[k++] = *symbol;
		*symbol = expr[(*n)++];
	}
	(*n)--;
	symbolN[k++] = ' ';
	symbolN[k] = '\0';

	return;
}

int getToken(char* symbol, char* symbolN, int* n)//classify character
{
	*symbol = expr[(*n)++];//get the current char

	if (*symbol == '+' || *symbol == '-')//plus/minus classify
	{
		int flag;
		for (flag = 1; *symbol == '+' || *symbol == '-'; *symbol = expr[(*n)++])
		{
			if (*symbol == '+') flag *= 1;
			else if (*symbol == '-') flag *= -1;
		}
		(*n)--;
		if (flag == 1) return PL;
		if (flag == -1) return MI;
	}

	switch (*symbol)
	{
		case '(' :
		 	return LP;
		case ')' :
			return RP;
		case '/' :
			return DI;
		case '*' :
			return TI;
		case '\n' :
			return EOS;
		default :
			int2String(symbol, symbolN, n);
			return OP;
	}
}

void postfix(void)
{
	char symbol;
	char symbolN[50];
	int token;
	int n = 0;
	top = NULL;//declare stack

	push(EOS);
	for (token = getToken(&symbol, symbolN, &n); token != EOS; token = getToken(&symbol, symbolN, &n))
	{
		if (token == OP)
			strcat(expr_pf, symbolN);
		else if (token == RP)
		{
			while (top->data != LP)
				printToken(pop());
			pop();
		}
		else
		{
			while (isp[top->data] >= icp[token])
				printToken(pop());
			push(token);
		}
	}
	while ( (token = pop()) != EOS)
		printToken(token);
}

int main(void)
{
	FILE* f1 = fopen("infix.txt", "r");
	FILE* f2 = fopen("postfix.txt", "w");

	while (fgets(expr, MAX_EXPR, f1) != NULL)
	{
		if (strcmp(expr, "\n") == 0)
			break;
		postfix();
		fprintf(f2, "%s", expr_pf);
		fprintf(f2, "%12.2lf\n", eval());
		memset(expr, 0, MAX_EXPR);
		memset(expr_pf, 0, MAX_EXPR);
	}

	return 0;
}
