// HalfMan.c : Defines the entry point for the console application.
//
#include "stdlib.h"
#include "stdio.h"

#define SIZE 256
#define MARK -1

typedef struct Node* Node_p;
typedef struct {
	Node_p up, one, zero;
	int val;
	char letter;
} Node;
typedef struct {
	int len;
	int seq;
} code;
int compare(const Node ** left, const Node ** right)
{
	return ((*left)->val < (*right)->val) - ((*left)->val > (*right)->val);
}
void build_tree(Node** sorted, int len) {}
void get_codes(code** arg_codes, Node** leaves)
{
	int i = 0;
	Node * current, * upper;
	code* codes = *arg_codes;
	for(; i < SIZE; i++)
	{
		codes[i].len = 0;
		codes[i].seq = 0;
		current = leaves[i];
		while(current->up)
		{
			upper = current->up;
			if(upper->one == current)
			{
				codes[i].seq = (codes[i].seq << 1) + 1;
			}
			if(upper->zero == current)
			{
				codes[i].seq = codes[i].seq << 1;
			}
			else abort();
			current = current->up;
		}
	}
}
int main(int argc, char* argv[])
{
	FILE * read;
	char c;
	int i , j, total;
	int freq[SIZE] = {0};
	Node *nodes[SIZE], *leaves[SIZE];
	code codes[SIZE];
	Node * current = NULL;
	read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "r");
	if(read)
		while((c=fgetc(read)) != EOF)
		{
			freq[c]++;
		}
	for (i = 0; i < SIZE; i++)
	{
		printf("%x ", freq[i]);
	}
	for(i = 0; i < SIZE; i++)
	{
		current = malloc(sizeof(Node));
		current->up = NULL;
		current->one = current->zero = NULL;
		current->val = freq[i];
		current->letter = i;
		leaves[i] = nodes[i] = current;
	}
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	printf("Sorted Nodes: ");
	qsort(nodes, SIZE, sizeof(Node_p), compare);
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	j = SIZE;
	while(j != 1)
	{
		--j;
		if(j>=2)
			qsort(&nodes[j-2], 3, sizeof(Node_p), compare);
		current = malloc(sizeof(Node));
		nodes[j-1]->up = nodes[j]->up = current;
		current->val = nodes[j]->val + nodes[j-1]->val;
		current->zero = nodes[j-1];
		current->one = nodes[j];
		nodes[j-1] = current;
		nodes[j]->val = MARK;
	}
	nodes[j]->up = NULL;
	printf("Sorted Nodes: ");
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	get_codes(&codes, leaves);
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", codes[i].seq);
	}

	return 0;
}

