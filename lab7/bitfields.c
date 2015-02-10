#include <stdbool.h>
#include <stdio.h>

struct page {
  char* name;
  int is_writbale:1; // or unsigned int or bool
  int is_readable:1;
  int is_executable:1;
  int :4; //padding
  unsigned int value:3;
  int address:14;
  int :0; // force alignment to a new word
  int next_page:14;
  signed int t1:20;
  unsigned int t2:20;
  bool is_valid:1; // since C99
  long t3:42; // GCC extension
};

int main(void)
{
  struct page p1; // all that connected with bit fields is implementation defined behaviour
  p1.value = 5; //15; //this produce warning -Woverflow
  printf("%zu\n", p1.value);
  return 0;
}
