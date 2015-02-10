#include <stdio.h>
int main()
{
	int a = 0, b =1;
	printf("%d\n", a+++++b); // ((a++)++)+b is not compilable rvalue - (a++)
	printf("%d\n", a+++ ++b); // (a++)+(++b) is compilable rvalue
}
