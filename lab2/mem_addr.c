#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

extern char etext, edata, end;

int main( int argc, char *argv[])
{
	printf("First\n");
	printf(" program text(etext): %20p\n ", &etext);
	printf(" init data (edata):  %20p\n", &edata);
	printf(" uninit data (eend): %20p\n",  &end);
	printf(" program break (brk): %20p\n",  sbrk(0));
	printf(" Address of argc: %20p\n", &argc);
	printf(" Address of errno: %20p\n", &errno);
	printf(" Address of printf %20p\n", printf);
	printf(" Address of mem allocated:  %20p\n", malloc(16));

	return 0;
}
