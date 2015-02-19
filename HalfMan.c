// HalfMan.c : Defines the entry point for the console application.
//
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define DE1BUG
#define SIZE 256
#define MARK -1

typedef struct Node Node;
struct Node {
	struct Node * up, * one, *zero;
	int val;
	char letter;
};
typedef struct Node * Node_p;
typedef struct {
	int len;
	uint64_t seq; //try to add array of long [8]
    uint64_t inv_seq; //fragments that uses this code be source of er
} code;

#define BUF_SIZE 1024
//encode file
void append_file( FILE * write, FILE * read , code* codes)
{
	int i = 0;
	uint64_t bits = 0;
	uint64_t mask = 1;
    uint64_t symbol = 0;
    register char b = 0;
	register int pointer = -1;
	unsigned int c = 0; //possibly source of error
	int ui = 0;
	char buf[BUF_SIZE] = {0};
	if (!(read && write))
		abort();
	while((ui = fgetc(read)) != EOF)
	{
        c = (unsigned int) ui;
        i = (bits % 8);
        //TODO way to improve this give 20% increase on fibonachi sequences
        if ((codes[c].len <= ( 8 - i)))
        {
			if(i == 0)
			{
				++pointer;
                if (pointer == BUF_SIZE)
				{
					fwrite(buf, sizeof(char), BUF_SIZE, write);
                    memset(buf, BUF_SIZE, 0);
					pointer = 0;
				}
			}
            buf[pointer] = (buf[pointer] << codes[c].len) | codes[c].inv_seq;
            bits = bits + codes[c].len;
        }
        else
      {
          symbol = codes[c].seq;
            b = buf[pointer];
          i = codes[c].len;
		while (i)
		{
			if((bits%8) == 0)
			{
                buf[pointer] = b;
				++pointer;
                b = buf[pointer];
                if (pointer == BUF_SIZE)
				{
					fwrite(buf, sizeof(char), BUF_SIZE, write);
                    memset(buf, BUF_SIZE, 0);
					pointer = 0;
				}
			}
			b = (b << 1) | (symbol & 0x1);
			symbol = symbol >> 1;
            bits ++;
            i--;
		}
          buf[pointer] = b;
      }
	}
	// for code to be complete and starts from MSB
	if((bits%8) != 0)
	{
		buf[pointer] = buf[pointer]  << (8 - (bits%8));
	}
	fwrite(buf, sizeof(char), pointer+2, write);
	fclose(write);
}

typedef struct {
	short int /*up,*/ one, zero;
	//int val;
	char letter;
} element;

int compare(const void* left, const void* right)
{
	return ((*(Node **)left)->val < (*(Node **)right)->val) - ((*(Node **)left)->val > (*(Node **)right)->val);
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
	int i = 0, j = 0;
	Node * current, * upper;
	code* codes = arg_codes;
    uint64_t cur_seq = 0;
	for(; i < SIZE; i++)
	{
		codes[i].len = 0;
		codes[i].seq = 0;
		current = leaves[i];
		while(current->up != NULL)
		{
			upper = current->up;
			if((Node *)upper->one == current)
			{
				codes[i].seq = (codes[i].seq << 1) + 1;
			}
			else if((Node *)upper->zero == current)
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
        cur_seq = codes[i].seq;
        for(j=0; j<codes[i].len; j++)
        {
           codes[i].inv_seq = (codes[i].inv_seq << 1) | ( cur_seq & 0x1);
           cur_seq = cur_seq >> 1;
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

void decode( FILE * encoded, element* tree, int tree_size, FILE * decoded, uint64_t file_size)
{
	int ui = 0;
	register unsigned char c = 0;
	register unsigned char  mask = 1;
	char buf[BUF_SIZE];
	char read_buf[BUF_SIZE];
	int readed;
	uint64_t total_read = 0;
	int buf_idx = 0;
	int i = 0, j = 0;
	int pos = 0;
	//assert(tree[0].up == -1);
	if(file_size == 0)
	{
		fclose(decoded);
		return;
	}
	while (1)
	{
		readed = fread(read_buf, sizeof(char), BUF_SIZE, encoded);
		for (j = 0; j < readed; j++)
		{
			c = read_buf[j];
			mask = 1<<7;
			while (mask)
			{
				//assert(pos < tree_size);
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
				//assert(pos != -1);
				mask = mask >> 1;
			}
			if((total_read == file_size))
			{
				break;
			}
		}
		if((total_read == file_size))
		{
			break;
		}
	}
	//fwrite(buf, sizeof(char), file_size % BUF_SIZE, decoded);
	if (buf_idx != 0)
		fwrite(buf, sizeof(char), buf_idx, decoded);
	fclose(decoded);
}

//NULL pointer will be -1 index in array
int put_to_array(element * prefix, Node * root, int up)//, code* codes)
{
	int pos = idx;
	idx++;
	prefix[pos].letter = root->letter;

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
#define ARG_COMPRESS 1
#define ARG_FILE 2
int main(int argc, char* argv[])
{
	FILE * read, * write,  *write_dec, *read_enc;
	int c;
	int i , j; //, total;
	int freq[SIZE] = {0};
	int to_encode = 1;
	unsigned int ui = 0;
	Node *nodes[SIZE], *leaves[SIZE];
	code codes[SIZE] = {{0}};
	int is_zero_case = 0;
	code codes2[SIZE] = {{0}};
	uint64_t file_size = 0;
	element prefix[2 * SIZE] = {{0}};
	element dprefix[2 * SIZE] = {{0}};
	//element* tree = NULL; // possibly do malloc while decoding
	int tree_size = 0;
	int set = 0;
	Node * current = NULL;
    if(argc == 4)
    {
    	if (strcmp(argv[ARG_COMPRESS], "-c") == 0)
    	{
    		to_encode = 1; //TRUE
    	    read = fopen(argv[ARG_FILE], "rb");
    	    write = fopen(argv[3], "wb");
    	}
    	if (strcmp(argv[ARG_COMPRESS], "-x") == 0)
    	{
    		to_encode = 0; //FALSE
			read_enc = fopen(argv[ARG_FILE], "rb");
    	    write_dec = fopen(argv[3], "wb");
    	}
    }
    else
    {
	    read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "rb");
	    write = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Encode.txt", "wb");
	    write_dec = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Decode.txt", "wb");
    }
    if(to_encode)
    {
		if(read)
			while((c=fgetc(read)) != EOF)
			{
				ui = (unsigned int) c;
				freq[ui]++; file_size++;
			}
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
		// Empty file case
		if(j==0)
		{
			j = 1;
			nodes[0]->letter = 's';
		}
		//for one letter cases corner case
		if(j==1)
		{
			current = malloc(sizeof(Node));
			nodes[0]->up = (Node_p)current;
			current->zero = NULL;
			current->one = (Node_p)nodes[0];
			nodes[0] = current;
		}
		while(j != 1)
		{
			--j;
			if(j>=2)
				qsort(nodes, j+1, sizeof(Node_p), compare);
			current = malloc(sizeof(Node));
			nodes[j-1]->up = (Node_p)current;
			nodes[j]->up = (Node_p)current;
			current->val = nodes[j]->val + nodes[j-1]->val;
			current->zero = (Node_p)nodes[j];
			current->one = (Node_p)nodes[j-1];
			nodes[j-1] = current;
			nodes[j] = NULL;//->val = MARK;
		}
		// this is a root node
		nodes[0]->up = NULL;
		put_to_array(prefix, nodes[0], -1);
		/*code next;
		next.seq = -1;
		search_for_codes(prefix, codes, 0, &next);*/
		//get_codes_from_array(prefix, codes);
		get_inv_codes(codes2, leaves);
#ifdef DEBUG
		printf("\nPrefix: \n");
		for(i = 0; i < 2*SIZE; i++)
		{
			if (prefix[i].letter != 0)
				printf("%d ",(char) prefix[i].letter, codes[i].len, codes[i].seq);
		}
		printf("\nCodes2: \n");
		for(i = 0; i < SIZE; i++)
		{
			if (codes2[i].len != 0)
				printf("%c s%d  %x ",(char) i, codes2[i].len, codes2[i].seq);
		}
		printf("\n");
#endif
		if(argc == 4)
		{
			read = fopen(argv[ARG_FILE], "rb");
		}
		else
		{
			read = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Tmp.txt", "rb");
		}
		write_file(write, prefix, file_size);
		append_file(write, read, codes2);
		Free_Tree(&(nodes[0]));
    }
    else /* if (to_encode) //for tests in VS */
    {
		if(argc != 4)
		{
			read_enc = fopen("D:\\working\\Small work for student\\HalfMan\\HalfMan\\Debug\\Encode.txt", "rb");
		}
		if(read_enc)
		{
			read_prefix(read_enc, dprefix, &tree_size, & file_size);
			decode(read_enc, dprefix, tree_size, write_dec, file_size);
		}
		else
			assert(0);
    }
	return 0;
}

