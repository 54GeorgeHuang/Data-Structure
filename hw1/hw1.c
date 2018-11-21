#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct
{
	int label;
	double x;
	double y;
} node;

typedef struct edge
{
	int label;
	int fir_node;
	int sec_node;
	struct edge* link;
} edge;

node* inputNode(int n)
{
	node* np = malloc(n * sizeof(node));
	assert(np != NULL);

	for (int i = 0; i < n; i++)
		scanf("%d %lf %lf", &((np+i)->label), &((np+i)->x), &((np+i)->y));

	return np;
}

edge* genEdge(int n, node* np)
{
	edge* head = malloc(sizeof(edge));
	assert(head != NULL);
	head->link = NULL;

	edge* now = head;
	edge* tmp = NULL;

	int	count = 0;
	for (int i = 0; i < n-1; i++)
		for (int j = i+1; j < n; j++)
			if (pow((np+i)->x - (np+j)->x, 2) + pow((np+i)->y - (np+j)->y, 2) <= 1)
				{
					tmp = malloc(sizeof(edge));
					assert(tmp != NULL);

					tmp->label = count++;
					tmp->fir_node = i;
					tmp->sec_node = j;
					tmp->link = NULL;

					now->link = tmp;
					now = tmp;
				}

	head->label = count;
	
	return head;
}

void outputEdge(edge* head)
{
	edge* now = head->link;

	printf("%d\n", head->label);
	for (int i = 0; i < head->label; i++, now = now->link)
		printf("%d %d %d\n", now->label, now->fir_node, now->sec_node);
	
	printf("\n");

	return;
}

edge* genPlanarGraph(int n, node* np, edge* head)
{
	edge* now = head->link;
	edge* tmp = head;

	int count = 0;
	for (int i = 0, flag = 0; i < head->label; i++, flag = 0)
	{
		double ctr_x = ((np+now->fir_node)->x + (np+now->sec_node)->x) / 2;
		double ctr_y = ((np+now->fir_node)->y + (np+now->sec_node)->y) / 2;
		double radius = pow(ctr_x - (np+now->fir_node)->x, 2) + pow(ctr_y - (np+now->fir_node)->y, 2);

		for (int j = 0; j < n; j++)
		{
			if (j == now->fir_node || j == now->sec_node)
				continue;

			if (pow(ctr_x - (np+j)->x, 2) + pow(ctr_y - (np+j)->y, 2) < radius)
			{
				flag = 1;
				count++;

				tmp->link = now->link;
				free(now);
				now = tmp->link;

				break;
			}
		}
		if (flag == 0)
		{
			now = now->link;
			tmp = tmp->link;
		}
	}

	head->label -= count;

	return head;
}

void freePointer(node* np, edge* head)
{
	free(np);

	edge* now = head;
	edge* tmp = NULL;

	for ( ; now->link != NULL; now = tmp)
	{
		tmp = now->link;
		free(now);
	}
	
	return;
}

int main(void)
{
//	inputNode;
//	genEdge;
//	outputEdge;
//	genPlanarGraph;
//	outputEdge;
//	freePointer;

	int n;

	freopen("node.txt", "r", stdin);
	scanf("%d", &n);
	
	node* np = inputNode(n);

	edge* head = genEdge(n, np);

	freopen("link.txt", "w", stdout);

	outputEdge(head);

	head = genPlanarGraph(n, np, head);

	outputEdge(head);


	freePointer(np, head);

	return 0;
}
