// HalfMan.c : Defines the entry point for the console application.
//
#include "stdlib.h"
#include "stdio.h"

#define DEBUG
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
	unsigned int seq;
} code;
int compare(const Node ** left, const Node ** right)
{
	return ((*left)->val < (*right)->val) - ((*left)->val > (*right)->val);
}
void build_tree(Node** sorted, int len) {}
void get_inv_codes(code* arg_codes, Node** leaves)
{
	int i = 0;
	Node * current, * upper;
	code* codes = arg_codes;
	for(; i < SIZE; i++)
	{
		codes[i].len = 0;
		codes[i].seq = 0;
		current = leaves[i];
		while(current->up != NULL)
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
			codes[i].len ++;
			//something goes wrong
			if((upper->one != current) && (upper->zero != current))
			{
				abort();
			}
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
	code codes[SIZE] = {0};
	int set = 0;
	Node * current = NULL;
	read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "r");
	if(read)
		while((c=fgetc(read)) != EOF)
		{
			freq[c]++;
		}
#ifdef DEBUG
	for (i = 0; i < SIZE; i++)
	{
		printf("%x ", freq[i]);
	}
#endif
	for(i = 0; i < SIZE; i++)
	{
		current = malloc(sizeof(Node));
		current->up = NULL;
		current->one = current->zero = NULL;
		current->val = freq[i];
		current->letter = i;
		leaves[i] = nodes[i] = current;
	}
#ifdef DEBUG
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
#endif
	qsort(nodes, SIZE, sizeof(Node_p), compare);
#ifdef DEBUG
	printf("\nSorted Nodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
#endif
	j = SIZE;
	for(i = 0; i < SIZE; i++)
	{
		if (nodes[i]->val == 0 && !set)
		{
			j = i;
			set = 1;
		}
		if(set)
			nodes[i]->up = NULL;
	}
	while(j != 1)
	{
		--j;
		//possibly wrong & operator in &nodes[j-2], removed
		if(j>=2)
			qsort(nodes + j-2, 3, sizeof(Node_p), compare);
		current = malloc(sizeof(Node));
		nodes[j-1]->up = current;
		nodes[j]->up = current;
		current->val = nodes[j]->val + nodes[j-1]->val;
		current->zero = nodes[j-1];
		current->one = nodes[j];
		nodes[j-1] = current;
		nodes[j]->val = MARK;
	}
	// this is a root node
	nodes[0]->up = NULL;
	printf("\nSorted Nodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	get_inv_codes((code**) &codes, leaves);
	printf("\nCodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("s%d %x ", codes[i].len, codes[i].seq);
	}
	printf("\n%i \n", sizeof(codes[i].seq));

	return 0;
}

