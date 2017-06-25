#include <stdio.h>
#include <stdlib.h>

#define MAP_RANGE(x,y,MAX_X,MAX_Y) ((x) < 0 ? 0 : ((x) > (MAX_X) - 1 ? 0 : ((y) < 0 ? 0 : ((y) > (MAX_Y) - 1 ? 0 : 1))))

int main(void){
  int x,y,res;
  
  while(1){
    
    scanf("%d",&x);
    scanf("%d",&y);
    
    res = MAP_RANGE(x,y,60,20);
  
    if(res){
      printf("true\n");
    }else{
      printf("else\n");
    }
  }
  
  return 0;
}