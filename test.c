#include <stdio.h>
#include <stdlib.h>

#define _MOVE_RANGE(a,max) ((a) < 0 ? 0 : ((a) > (max) ? (max) : (a)))

int main(void){
  int x,y,res;
  
  while(1){
    
    scanf("%d",&x);
    scanf("%d",&y);
    
    x = _MOVE_RANGE(x,59);
    y = _MOVE_RANGE(y,19);
  
    printf("x: %2d, y: %2d\n",x,y);
  }
  
  return 0;
}