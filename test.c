#include <stdio.h>
#include <stdlib.h>

typedef struct Cell{
  int num;
}CELL;

void swap(CELL** a){
  int b = (*a)->num;
}

int main(void)
{
  CELL* cell = (CELL*)malloc(sizeof(CELL));
  cell->num = 3;
  swap(&cell);
  
  printf("%d\n",cell->num);
  exit(EXIT_SUCCESS);
}