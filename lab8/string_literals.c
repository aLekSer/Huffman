/* explore the executable with 'strings' util */

void main(void)
{
  char *s = "Hello world!";
  char * s2 = "Hello "
  "world "
  "from "
  "epam" "!";
  puts(s2);
  s[0] = 'h'; //could not modify due to standart
  puts(s);
}
