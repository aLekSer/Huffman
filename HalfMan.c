// HalfMan.c : Defines the entry point for the console application.
//
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>

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
	uint64_t seq; //try to add array of long [8]
} code;

void append_file( FILE * write, FILE * read , code* codes) 
{
	int i = 0;
	uint64_t bits = 0;
	uint64_t mask = 1;
	int pointer = 0;
	char c = '\0';
	char buf[256];
	if (!(read && write))
		abort();
	while((c=fgetc(read)) != EOF)
	{
		mask = 1;
		for (i = 0; i < codes[c].len; bits++, i++)
		{
			if((bits%8) == 0)
			{
				pointer++;
			}
			buf[pointer] = buf[pointer] << 1;
			if(codes[c].seq & mask)
			{ 
				buf[pointer] = buf[pointer] | 1;
			}
			mask << 1;
		}

	}
	printf("Hi there!\n");
}

typedef struct {
	int up, one, zero;
	int val;
	char letter;
} element;
void write_file( FILE * write, element* prefix, uint64_t file_size ) 
{
	return;
}

int compare(const Node ** left, const Node ** right)
{
	return ((*left)->val < (*right)->val) - ((*left)->val > (*right)->val);
}
void build_tree(Node** sorted, int len) {}
void Free_Tree(Node** this_node)
{
	if(!this_node || !(*this_node))
		return;
	if((*this_node)->one)
		Free_Tree(&((*this_node)->one) );
	if((*this_node)->zero)
		Free_Tree(&((*this_node)->zero) );
	if (this_node)
		free(*this_node);
	*this_node = NULL;
}
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
int idx = 0;
//NULL pointer will be -1 index in array
int put_to_array(element * prefix, Node * root, int up)
{
	int pos = idx;
	idx++;
	prefix[pos].val = root->val;
	prefix[pos].letter = root->letter;
	prefix[pos].up = up;
		
	if(root->zero != NULL)
	{
		prefix[pos].zero = 
		  put_to_array(prefix, root->zero, pos);
	}
	else 
		prefix[pos].zero = -1;
	if(root->one != NULL)
	{
		prefix[pos].one = put_to_array(prefix, root->one, pos);
	}
	else 
		prefix[pos].one = -1;
	return pos;
}
int main(int argc, char* argv[])
{
	FILE * read, * write,  *write_dec;
	char c;
	int i , j, total;
	int freq[SIZE] = {0};
	Node *nodes[SIZE], *leaves[SIZE];
	code codes[SIZE] = {0};
	uint64_t file_size = 0;
	element prefix[2 * SIZE] = {0};
	int set = 0;
	Node * current = NULL;
	read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "r");
	write = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Encode.txt", "w");
	write_dec = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Decode.txt", "w");
	if(read)
		while((c=fgetc(read)) != EOF)
		{
			freq[c]++; file_size++;
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
		current->val = freq[i]; // this is to add randomization in tree
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
	put_to_array(prefix, nodes[0], 0, -1);
	printf("\nSorted Nodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	get_inv_codes((code**) &codes, leaves);
	write_file(write, prefix, file_size);
	read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "r");
	append_file(write, read, codes);
	printf("\nCodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("s%d %x ", codes[i].len, codes[i].seq);
	}
	printf("\n%i \n", sizeof(codes[i].seq));

	Free_Tree(&(nodes[0]));
	return 0;
}

