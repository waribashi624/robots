#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAP_X 60
#define MAP_Y 20
#define MAX_ENEM 40

#define MIN(x,y) ((x) < (y) ? (x) : (y))

extern char getChar(void);

typedef struct VECTOR{
  int x;
  int y;
}Vector;

typedef struct ENEMY{
  Vector vector;
  bool scrap;
  struct enemy *next;
  struct enemy *prev;
}Enemy;

typedef struct PLAYER{
  Vector vector;
  int level;
  int score;
  bool alive;
}Player;

void init_player(Player* player);
void create_enemy(Enemy** enemy,int enemCount);
//void level_up(Player *player);

int main(void)
{
  srand((unsigned)time(NULL));

  int enemCount = 0,i;
  Player player;
  Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
  //enemy->vector.x = 1;

  while(1){
    enemCount = MIN(player.level*5,MAX_ENEM);
    create_enemy(&enemy,enemCount);
    break;
  }

  for(i = 0;i < enemCount;i++){
    printf("[%2d] x:%2d ,y:%2d\n",i,enemy[i].vector.x,enemy[i].vector.y);
  }

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

  *enemy = (Enemy*)realloc(*enemy,sizeof(Enemy)*enemCount);

  for(i = 0;i < enemCount;i++){
    (*enemy)[i].vector.x = rand()%MAP_X;
    (*enemy)[i].vector.y = rand()%MAP_Y;
    (*enemy)[i].scrap = false;
  }
}
