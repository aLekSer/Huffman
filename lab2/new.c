#include <stdio.h>

int main(void)
{
	printf("value %zu\n", sizeof(0L));
	printf("value %zu\n", sizeof(3.0F));
	printf("value %zu\n", sizeof(40LL));
	printf("value %zu\n", sizeof(5.0));
	printf("value %zu\n", sizeof(5.0L));
	printf("value %zu\n", sizeof('x'));
	return 0;
}
