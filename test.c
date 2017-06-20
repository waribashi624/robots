#include <stdio.h>
#include <stdlib.h>

typedef struct ascii{
  int num;
}Ascii;

typedef struct bob{
  Ascii str;
}Bob;

void func(Bob** human){
  (*human)->str.num = 8;
}
  
int main()
{
  Bob* human = (Bob*)malloc(sizeof(Bob));
  
  func(&human);

  printf("%d\n",human->str.num);
  free(human);
  return 0;
}
