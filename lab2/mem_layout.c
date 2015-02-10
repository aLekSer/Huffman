#include <stdio.h>
#include <stdlib.h>

#define SIZE (1024 * 1024 * 3 )
#define SIZE2 (1024 * 1024  )

//char a[SIZE];
int main()
{
//static char b[SIZE]= {1};
static char b22[SIZE];
char b1[SIZE2]= {1};
int i;
for (i=0; i< SIZE; i++)
{
//	a[i] =  0xcc;
//	b[i] = 0xEE;
	b1[i] = 0xEE;
b22[i] = 0xBA;
}
printf("I am here");
}
