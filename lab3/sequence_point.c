#include <stdio.h>
int main(void)
{
int i = 0;
int x[20] = {0};
i = i++;
printf("i++ %d\n", i);
i = ++i;
printf("++i %d\n", i);
x[i] = i++;
printf("%d\n", i);
x[i++] = i;
printf("%d\n", i);

return 0;
}
