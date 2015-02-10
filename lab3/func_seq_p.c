#include <stdio.h>

void func(int a, int*  b)
{
	printf("%d %d\n", a, *b);
}

int main()
{
	int x = 0;
	func(x++, &x); // 0 1

	return 0;
}
