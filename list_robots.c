#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAP_X 60
#define MAP_Y 20
#define MAX_ENEM 40

#define DEBUG 0

#define MIN(x,y) ((x) < (y) ? (x) : (y))

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
void create_enemy(Enemy** enemy,int enemCount);
int list_free(Enemy* enemy);
void list_sort(Enemy** enemy);

int main(void)
{
  srand((unsigned)time(NULL));

  int enemCount = 0,i;
  Player player;
  Enemy* enemy;
  Enemy* pt;

  player.level = 1;
  
  while(1){
    enemCount = MIN(player.level*5,MAX_ENEM);
    create_enemy(&enemy,enemCount);
    break;
  }

  pt = enemy;
  
  while(pt != NULL){
    printf("x:%2d,y:%2d\n",pt->vector.x,pt->vector.y);
    pt = pt->next;
  }

  list_free(enemy);
  
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



void create_enemy(Enemy** enemy,int enemCount)
{
  int i;

  *enemy = (Enemy*)malloc(sizeof(Enemy));
  
  Enemy* pt = *enemy;
  Enemy* pre = NULL;

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
  pre->next = NULL;

  #ifdef DEBUG
  printf("[:ok, create_enemy/2]\n");
  #endif
  
}

int list_free(Enemy* enemy)
{

  Enemy* tmp;
  while(enemy != NULL){
    tmp = enemy;
    enemy = enemy->next;
    free(tmp);
  }

  return 0;
}

void list_sort(Enemy** enemy)
{
  Enemy* i,j;
    
  while(i != NULL){
    j = i;
    while(j->prev != NULL && prev_comp(j)){
      swap(j,j->prev);
    }
  }
}



bool prev_comp(Enemy* enemy)
{
  Enemy* pre = enemy->prev;

  if(enemy->vector.y < pre->vector.y){
    return true;
  }else if(enemy->vector.y == pre->vector.y){
    if(enemy->vector.x < pre->vector.x){
      return true;
    }
  }

  return false;
}

void swap(Enemy** a,Enemy** b)
{
  Enemy* pre = *a->prev;
  Enemy* next = *b->next;

  *a->next->prev = *b;
  *b->next = *a->next;

  *b->pre->next = *a;
  *a->pre = *b->pre;

  pre->next = *b;
  next->pre = *a;

  *a->next = next;
  *b->pre = pre;

}
