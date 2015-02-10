// gcc -Wpadded
// author: leks 
// 10.02.2014
//
#include <stdio.h>

struct test {
  int b;
  short a;
  short c;
} s; //sizeof (s) == 8
struct test2 {
  short a;
  int b;
  short c;
} s2; // sizeof(s2) = 12

int main(void)
{
  printf("%zu\n", sizeof(s.a));
  printf("%zu\n", sizeof(s.b));
  printf("%zu\n", sizeof(s.c));
  printf("%zu\n", sizeof(s));
  printf("%zu\n", sizeof(s2.a));
  printf("%zu\n", sizeof(s2.b));
  printf("%zu\n", sizeof(s2.c));
  printf("%zu\n", sizeof(s2));

  return 0;
}
