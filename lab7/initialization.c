#include "stdio.h"
struct item {
  int a;
  float b;
  char * name;
};

int main(void)
{
  struct item i = {5, 2.2, "Jonny"};
  struct item j = {6}; // other NULL as in static declaration
  //designated init-zer, C99
  struct item l = { .name = "Bill", .a = 42};

  //compound literal
  struct item m;
  m = (struct item) {5, 2., "Eddy"};
  return 0;
}
