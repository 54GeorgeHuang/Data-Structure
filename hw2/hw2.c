#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define MAX_EXPR 500
#define MAX_STACKS 5
#define LP 0
#define RP 1
#define PL 2
#define MI 3
#define TI 4
#define DI 5
#define EOS 6
#define OP 7

//typedef enum {lparen, rparen, plus, minus, times, divide, eos, operand} precedence;

char expr[MAX_EXPR] = "";
char expr_pf[MAX_EXPR] = "";
int isp[] = {0, 19, 12, 12, 13, 13, 0};
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

void int2String(char* symbol, char* symbolN, int* n)
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

int getToken(char* symbol, char* symbolN, int* n)
{
	*symbol = expr[(*n)++];
	if (*symbol == '+' || *symbol == '-')
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

void postfix(void)
{
	char symbol;
	char symbolN[50];
	int token;
	int n = 0;
	top = NULL;
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

int getToken2(char* symbol, int* n)
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

	token = getToken2(&symbol, &n);
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
		token = getToken2(&symbol, &n);
		
	}
	return popF();
}

int main(void)
{
	FILE* f1 = fopen("infix.txt", "r");
	FILE* f2 = fopen("postfix.txt", "w");
	//freopen("infix.txt", "r", stdin);

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
