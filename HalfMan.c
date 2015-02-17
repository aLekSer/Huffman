// HalfMan.c : Defines the entry point for the console application.
//
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define DEB1UG
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

#define BUF_SIZE 8
//encode file
void append_file( FILE * write, FILE * read , code* codes)
{
	int i = 0;
	uint64_t bits = 0;
	uint64_t mask = 1;
	int pointer = -1;
	unsigned int c = 0; //possibly source of error
	int ui = 0;
	char buf[BUF_SIZE] = {0};
	if (!(read && write))
		abort();
	while((ui = fgetc(read)) != EOF)
	{
		c = (unsigned int) ui;
		mask = 1;
		for (i = codes[c].len; i > 0 ; bits++, i--)
		{
			if((bits%8) == 0)
			{
				pointer++;
				if (pointer == BUF_SIZE)
				{
					fwrite(buf, sizeof(char), BUF_SIZE, write);
					memset(buf, 0, BUF_SIZE);
					pointer = 0;
				}
			}
			buf[pointer] = buf[pointer] << 1;
			if(codes[c].seq & mask)
			{
				buf[pointer] = buf[pointer] | 1;
			}
			mask = mask << 1;
		}
	}
	// for code to be complete and starts from MSB
	if((bits%8) != 0)
	{
		buf[pointer] = buf[pointer]  << (8 - (bits%8));
	}
	fwrite(buf, sizeof(char), pointer+1, write);
	fclose(write);
}

typedef struct {
	short int up, one, zero;
	//int val;
	char letter;
} element;

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
			if(((Node *)upper->one)->letter == current->letter)
			{
				codes[i].seq = (codes[i].seq << 1) + 1;
			}
			else if(((Node *)upper->zero)->letter == current->letter)
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
void write_file( FILE * write, element* prefix, uint64_t file_size )
{
	fwrite(&file_size, sizeof(file_size), 1, write);
	fwrite(&idx, sizeof(idx), 1, write);
	fwrite(prefix, sizeof(element), idx, write);
}
void read_prefix( FILE * read_file, element* tree, int* tree_size, uint64_t* file_size )
{
	int read_elements;
	fread(file_size, sizeof(*file_size), 1, read_file);
	fread(tree_size, sizeof(*tree_size), 1, read_file);
	read_elements = fread(tree, sizeof(element), *tree_size, read_file);
}

void decode( FILE * encoded, element* tree, size_t tree_size, FILE * decoded, int file_size)
{
	int ui = 0;
	unsigned int c = 0;
	int mask = 1;
	char buf[BUF_SIZE];
	char read_buf[BUF_SIZE];
	int readed;
	uint64_t total_read = 0;
	int buf_idx = 0;
	int i = 0, j = 0;
	int pos = 0;
	assert(tree[0].up == -1);
	while (1)
	{
		readed = fread(read_buf, sizeof(char), BUF_SIZE, encoded);
		for (j = 0; j < readed; j++)
		{
			c = read_buf[j];
			mask = 1<<7;
			for (i = 0; i <8; i++)
			{
				assert(pos < tree_size);
				if ((tree[pos].zero == -1) && (tree[pos].one == -1)) // leaf found
				{
					buf[buf_idx++] = tree[pos].letter;
					total_read++;
					if(buf_idx == BUF_SIZE) //TODO check why on long file decode is not equal to enc
					{
						fwrite(buf, sizeof(char), BUF_SIZE, decoded);
						buf_idx = 0;
					}
					if(total_read == file_size)
						break;
					pos = 0;
				}
				if (c & mask) // == 1
				{
					pos = tree[pos].one;
				}
				else
				{
					pos = tree[pos].zero;
				}
				assert(pos != -1);
				mask = mask >> 1;
			}
		}
		if((total_read == file_size) || (readed != BUF_SIZE))
		{
			/* this code seems to be unnecessary:
			if ((tree[pos].zero == -1) && (tree[pos].one == -1)) // leaf found
			{
				if(buf_idx == BUF_SIZE)
				{
					fwrite(buf, sizeof(char), BUF_SIZE, decoded);
					buf_idx = 0;
				}
				buf[buf_idx++] = tree[pos].letter;
				pos = 0;
			} */
			break;
		}
	}
	fwrite(buf, sizeof(char), file_size % BUF_SIZE, decoded);
	fclose(decoded);
}

//NULL pointer will be -1 index in array
int put_to_array(element * prefix, Node * root, int up)//, code* codes)
{
	int pos = idx;
	idx++;
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
int get_codes_from_array(element * prefix, code * codes)
{
	int i = 0, j = 0;
	int current, upper;
	for(; j < SIZE; j++)
	{
		//find leaf
		for(; i < 2 * SIZE; i++)
		{
			if(prefix[i].letter == j)
				break;
		}
		current = i;
		codes[i].len = 0;
		codes[i].seq = 0;
		while(prefix[current].up != -1)
		{
			upper = prefix[current].up;
			if(prefix[upper].one == current)
			{
				codes[i].seq = (codes[i].seq << 1) + 1;
			}
			else if(prefix[upper].zero == current)
			{
				codes[i].seq = codes[i].seq << 1;
			}
			codes[i].len ++;
			//something goes wrong
			if((prefix[upper].one != current) && (prefix[upper].zero != current))
			{
				abort();
			}
			current = prefix[current].up;
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
	FILE * read, * write,  *write_dec, *read_enc;
	int c;
	int i , j; //, total;
	int freq[SIZE] = {0};
	int to_encode = 1;
	unsigned int ui = 0;
	Node *nodes[SIZE], *leaves[SIZE];
	code codes[SIZE] = {0};
	uint64_t file_size = 0;
	element prefix[2 * SIZE] = {0};
	element dprefix[2 * SIZE] = {0};
	//element* tree = NULL; // possibly do malloc while decoding
	size_t tree_size = 0;
	int set = 0;
	Node * current = NULL;
    if(argc == 4)
    {
    	if (strcmp(argv[2], "-c") == 0)
    	{
    		to_encode = 1; //TRUE
    	}
    	if (strcmp(argv[2], "-x") == 0)
    	{
    		to_encode = 0; //FALSE
    	}
	    read = fopen(argv[1], "rb");
	    write = fopen(argv[3], "wb");
	    write_dec = fopen("Decode.txt", "wb");

    }
    else
    {
	    read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "rb");
	    write = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Encode.txt", "wb");
	    write_dec = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Decode.txt", "wb");
    }
	if(read)
		while((c=fgetc(read)) != EOF)
		{
			ui = (unsigned int) c;
			freq[ui]++; file_size++;
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
		current->val = freq[i]; // this was to add randomization in tree
		current->letter = i;
		leaves[i] = current;
		nodes[i] = current;
	}
	qsort(nodes, SIZE, sizeof(Node_p), compare);

#ifdef DEBUG
	for(i = 0; i < file_size; i++)
	{
		printf("%d ", nodes[i]->val);
	}
#endif
#ifdef DEBUG
	printf("\nSorted Nodes: \n");
	for(i = 0; i < file_size; i++)
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
	put_to_array(prefix, nodes[0], -1);
	get_codes_from_array(prefix, codes);
	printf("\nSorted Nodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		printf("%d ", nodes[i]->val);
	}
	//get_inv_codes(codes, leaves);
	write_file(write, prefix, file_size);
	if(argc == 4)
	{
		read = fopen(argv[1], "rb");
		read_enc = fopen(argv[3], "rb");
	}
	else
	{
	    read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "rb");
	    read_enc = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Encode.txt", "rb");
	}
	append_file(write, read, codes);
	read_prefix(read_enc, dprefix, &tree_size, & file_size);
	decode(read_enc, dprefix, tree_size, write_dec, file_size);
	printf("\nCodes: \n");
	for(i = 0; i < SIZE; i++)
	{
		if (codes[i].len != 0)
			printf("%c s %d %x ",(char) i, codes[i].len, codes[i].seq);
	}
	Free_Tree(&(nodes[0]));
	return 0;
}

