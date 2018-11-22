#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#define MAX_QUEUE_SIZE 100
#define MAX_IN_SIZE 100

typedef struct node* nodePointer;
typedef struct node
{
	char character;
	int frequency;
	int height;
	nodePointer leftChild;
	nodePointer rightChild;
	nodePointer parent;
}node;
nodePointer priorityQueue;
nodePointer root = NULL;

void input(char* s)
{
	fgets(s, MAX_IN_SIZE, stdin);
	//printf("%s\n", s);

	return;
}

nodePointer getLastParent(nodePointer start, int* h)
{
	(*h)++;
	if( start->leftChild == NULL || start->rightChild == NULL)
		return start;
	else
	{
		int l_h = 0, r_h = 0;
		nodePointer left = getLastParent(start->leftChild, &l_h);
		nodePointer right = getLastParent(start->rightChild, &r_h);
		if( l_h == r_h)
		{
			(*h) += l_h;
			return left;
		}
		else
		{
			(*h) += r_h;
			return right;
		}
	}
}

void push(char c, int f)
{
	nodePointer tmp = (nodePointer )malloc(sizeof(node));
	assert(tmp != NULL);
		tmp->character = c;
		tmp->frequency = f;
		tmp->parent = NULL;
		tmp->leftChild = NULL;
		tmp->rightChild = NULL;
	if(root == NULL)
	{
		root = tmp;
	}
	else
	{
		int h = 0;
		nodePointer parentP = getLastParent(root, &h);
		tmp->parent = parentP;
		if(parentP->leftChild == NULL)
			parentP->leftChild = tmp;
		else
			parentP->rightChild = tmp;
		nodePointer now = tmp;
		while((now != root) && (now->frequency < now->parent->frequency))
		{
			parentP = now->parent;
			if(parentP->parent->leftChild == parentP)
			{
				parentP->parent->leftChild = now;
				now->parent = parentP->parent;
			}
			else
			{
				parentP->parent->rightChild = now;
				now->parent = parentP->parent;
			}
			if(parentP->leftChild == now)
			{
				now->leftChild = parentP;
				parentP->parent = now;
				now->rightChild = parentP->rightChild;
				if(parentP->rightChild != NULL)
					parentP->rightChild->parent = now;
				parentP->leftChild = NULL;
				parentP->rightChild = NULL;
			}
			else
			{
				now->rightChild = parentP;
				parentP->parent = now;
				now->leftChild = parentP->leftChild;
				if(parentP->leftChild != NULL)
					parentP->leftChild->parent = now;
				parentP->leftChild = NULL;
				parentP->rightChild = NULL;
			}

			now = now->parent;
		}
	
	}
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
		push(s[i], count);
		table[s[i] - 32] = true;
	}
	return;
}

nodePointer pop(nodePointer start)
{
	if(root == NULL)
	{
		fprintf(stderr, "the heap is empty\n");
		exit(EXIT_FAILURE);
	}
	nodePointer left = start->leftChild;
	nodePointer right = start->rightChild;
	nodePointer tmp = getLastLeaf();
	while(tmp->frequency > left->frequency || tmp->frequency > right->frequency)
	if(tmp->parent->leftChild == tmp)
		tmp->parent->leftChild = NULL;
	else
		tmp->parent->rightChild = NULL;
	tmp->parent = NULL;
	tmp->leftChild = start->leftChild;
	tmp->rightChild = start->rightChild;
	start->leftChild->parent = tmp;
	start->rightChild->parent = tmp;


}

void printTree(nodePointer start, int space)
{
	if(start == NULL)
		return;

	space += 3;

	printTree(start->rightChild, space);

	printf("\n");
	for( int i = 3; i < space; i++)
		printf(" ");
	printf("%c\n", start->character);

	printTree(start->leftChild, space);
	
	return;
}

int main(void)
{
	freopen("text.txt", "r", stdin);
	char inString[MAX_IN_SIZE];
	input(inString);
	genNode(inString);
	//printTree(root, 0);


	return 0;
}
