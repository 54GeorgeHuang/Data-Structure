#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_QUEUE_SIZE 100
#define MAX_IN_SIZE 100

typedef struct node* nodePointer;
typedef struct node
{
	char character;
	int frequency;
	nodePointer leftChild;
	nodePointer rightChild;
}node;
nodePointer priorityQueue[MAX_QUEUE_SIZE];

void input(char* s)
{
	fgets(s, MAX_IN_SIZE, stdin);
	//printf("%s\n", s);

	return;
}

void genNode(char* s)
{
	bool table[95];
	int count;
	memset(table, 0, sizeof(table));
	for(int i = 0; s[i] != '\0'; i++)
	{
		if(table[s[i] - 32])
			continue;
		count = 0;
		for(int j = i; s[j] != '\0'; j++)
		{
			if(s[i] == s[j])
				count++;
		}
		//printf("%c count = %d\n", s[i], count);
		table[s[i] - 32] = true;
	}
	return;
}

int main(void)
{
	freopen("text.txt", "r", stdin);
	char inString[MAX_IN_SIZE];
	input(inString);
	genNode(inString);


	return 0;
}
