// gcc -Wpadded
// author: leks 
// 10.02.2014
//
#include <stdio.h>

struct __attribute__((__packed__)) test {
  short a;
  int b;
  short c;
} s;

int main(void)
{
  printf("%zu\n", sizeof(s.a));
  printf("%zu\n", sizeof(s.b));
  printf("%zu\n", sizeof(s.c));
  printf("%zu\n", sizeof(s));

  return 0;
}
