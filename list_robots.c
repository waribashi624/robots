#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAP_X 60
#define MAP_Y 20
#define MAX_ENEM 40

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAP_RANGE(x,y,MAX_X,MAX_Y) ((x) < 0 ? 0 : ((x) > (MAX_X) - 1 ? 0 : ((y) < 0 ? 0 : ((y) > (MAX_Y) - 1 ? 0 : 1))))

extern char getChar(void);

typedef struct VECTOR{
  int x;
  int y;
}Vector;

typedef struct ENEMY{
  Vector vector;
  bool scrap;
  struct ENEMY *next;
  struct ENEMY *prev;
}Enemy;

typedef struct PLAYER{
  Vector vector;
  int level;
  int score;
  bool alive;
}Player;

void init_player(Player* player);
void create_enemy(Enemy* root,int enemCount);
int list_free(Enemy* root);
void list_sort(Enemy* root);
bool comp(Enemy* a,Enemy* b);
void swap(Enemy* a,Enemy* b);
void disp(Enemy* root,Player* player);

int main(void)
{
  srand((unsigned)time(NULL));

  int enemCount = 0,i;
  Player player;
  Enemy* pt;
  Enemy* root = (Enemy*)malloc(sizeof(Enemy));

  player.level = 1;
  player.vector.x = 0;
  player.vector.y = 0;
  
  while(1){
    enemCount = MIN(player.level*5,MAX_ENEM);
    create_enemy(root,enemCount);
    break;
  }

  pt = root->next;
  
  while(pt != root){
    printf("x:%2d,y:%2d\n",pt->vector.x,pt->vector.y);
    pt = pt->next;
  }
  printf("test\n");
  
  pt = root->next;
  
  list_sort(root);
  
  pt = root->next;
  
   while(pt != root){
    printf("x:%2d,y:%2d\n",pt->vector.x,pt->vector.y);
    pt = pt->next;
  }
  
  disp(root,&player);

  list_free(root);
  
  exit(EXIT_SUCCESS);
}

void init_player(Player* player)
{
  player->level = 0;
  player->vector.x = 0;
  player->vector.y = 0;
  player->score = 0;
  player->alive = true;
}



void create_enemy(Enemy* root,int enemCount)
{
  int i;
  
  Enemy* pt = (Enemy*)malloc(sizeof(Enemy));
  Enemy* pre = root;
  root->next = pt;
  
  #ifdef DEBUG
  printf("enemCount = %2d\n",enemCount);
  #endif
  
  for(i = 0;i < enemCount;i++){
    pt->vector.x = rand()%MAP_X;
    pt->vector.y = rand()%MAP_Y;
    pt->scrap = false;
    pt->prev = pre;
    pt->next = (Enemy*)malloc(sizeof(Enemy));
    pre = pt;
    pt = pt->next;
    

    #ifdef DEBUG
    printf("%d\n",i);
    #endif
  }

  free(pt);
  pre->next = root;
  root->prev = pre;

  #ifdef DEBUG
  printf("[:ok, create_enemy/2]\n");
  #endif
  
}

int list_free(Enemy* root)
{
  Enemy* enemy = root->next;
  Enemy* tmp;
  while(enemy != root){
    tmp = enemy;
    enemy = enemy->next;
    free(tmp);
  }
  
  free(root);

  return 0;
}

void list_sort(Enemy* root)
{
  Enemy* i,*j,*min;
  i = root->next;
    
  while(i != root){
    j = i;
    min = i;
    while(j != root){
      if(comp(j,min)){
        min = j;
      }
      j = j->next;
    }
    swap(i,min);
    i = min->next;
  }
}



bool comp(Enemy* a,Enemy* b)
{
  if(a->vector.y < b->vector.y){
    return true;
  }else if(a->vector.y == b->vector.y){
    if(a->vector.x < b->vector.x){
      return true;
    }
  }

  return false;
}

void swap(Enemy* a,Enemy* b)
{
  Enemy* pre = a->prev;
  Enemy* next = b->next;
  if(a == b){
    return;  
  }else if(a->next != b){
    a->next->prev = b;
    b->next = a->next;

    b->prev->next = a;
    a->prev = b->prev;

    pre->next = b;
    next->prev = a;

    a->next = next;
    b->prev = pre;
  }else{
    
    a->prev->next = b;
    b->next->prev = a;
    
    a->prev = b;
    b->next = a;
    
    a->next = next;
    b->prev = pre;
  }
}

void disp(Enemy* root,Player* player)
{
  int horiz,verti;
  Enemy* pt = root->next;
  
  for(verti = -1;verti <= MAP_Y;verti++){
    for(horiz = -1;horiz <= MAP_X;horiz++){
      if(!MAP_RANGE(horiz,verti,MAP_X,MAP_Y)){
          printf("#");
      }else if(pt->vector.x == horiz && pt->vector.y == verti){
        if(pt->scrap == true){
          printf("O");
        }else{
          printf("E");
        }
        pt = pt->next;
      }else if(player->vector.x == horiz && player->vector.y == verti){
        printf("@");
      }else{
        printf(" ");
      }
    }
    printf("\n");
  }
}
