#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <curses.h>

#define DEBUG 1
#define MAP_X 60
#define MAP_Y 20
#define MAX_ENEM 40

#define END 0
#define TELEPORT 1

#define DEAD_END 0
#define LEVEL_UP 1
#define NONE -1

#define _MIN(x,y) ((x) < (y) ? (x) : (y))
#define _MAP_RANGE(x,y,MAX_X,MAX_Y) ((x) < 0 ? 0 : ((x) > (MAX_X) - 1 ? 0 : ((y) < 0 ? 0 : ((y) > (MAX_Y) - 1 ? 0 : 1))))
#define _MOVE_RANGE(a,max) ((a) < 0 ? 0 : ((a) > (max) ? (max) : (a)))

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
int keysense(Player* player);
void enemy_move(Enemy* root, Player* player);
void teleport(Enemy* rppt,Player* player);
int survive_check(Enemy* root,Player* player);
int clear_check(Enemy* root,Player* player);
Enemy* level_up(Enemy* root,Player* player);

int main(void)
{
  srand((unsigned)time(NULL));

  int flag = NONE;
  int enemCount = 0,i;
  Player player;
  Enemy* pt;
  Enemy* root = (Enemy*)malloc(sizeof(Enemy));
  root->vector.x = 65535;
  root->vector.y = 65535;

  init_player(&player);

  enemCount = _MIN(player.level*5,MAX_ENEM);
  create_enemy(root,enemCount);

  list_sort(root);

  initscr();
  noecho();
  nonl();
  intrflush(stdscr,FALSE);
  keypad(stdscr,TRUE);
  curs_set(0);

  i = 0;


  while(1){
    clear();
    beep();
    disp(root,&player);
    refresh();
    flag= keysense(&player);

    if(flag == TELEPORT){
      teleport(root,&player);
      flag = NONE;
    }else if(flag == END){
      break;
    }
    flag = NONE;
    enemy_move(root,&player);
    list_sort(root);
    survive_check(root,&player);
    flag = clear_check(root,&player);
    if(flag == DEAD_END){
      break;
    }else if(flag == LEVEL_UP){
      root = level_up(root,&player);
    }
  }
  endwin();

  list_free(root);

  pt = root->next;

  /*while(pt != root){
    printf("x:%2d,y:%2d\n",pt->vector.x,pt->vector.y);
    pt = pt->next;
  }*/

  printf("score: %3d\n",player.score);
  exit(EXIT_SUCCESS);
}

void init_player(Player* player)
{
  player->level = 1;
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
      if(!_MAP_RANGE(horiz,verti,MAP_X,MAP_Y)){
          printw("#");
      }else if(pt->vector.x == horiz && pt->vector.y == verti){
        if(pt->scrap == false){
          printw("E");
        }else{
          printw("O");
        }
        while(pt->next->vector.x == pt->vector.x && pt->next->vector.y == pt->vector.y){
          pt = pt->next;
        }
        pt = pt->next;
      }else if(player->vector.x == horiz && player->vector.y == verti){
        printw("@");
      }else{
        printw(" ");
      }
    }
    printw("\n");
  }

  printw("\n\n\n");
  pt = root->next;

  while(pt != root){
    printw("x:%2d,y:%2d\n",pt->vector.x,pt->vector.y);
    pt = pt->next;
  }
}

int keysense(Player* player)
{
  int c = -1;
  int x = player->vector.x;
  int y = player->vector.y;

  while(c == -1){
    c = getch();
    switch(c){
      case '1':
        x = _MOVE_RANGE(x-1,MAP_X-1);
        y = _MOVE_RANGE(y+1,MAP_Y-1);
        break;
      case '2':
        y = _MOVE_RANGE(y+1,MAP_Y-1);
        break;
      case '3':
        x = _MOVE_RANGE(x+1,MAP_X-1);
        y = _MOVE_RANGE(y+1,MAP_Y-1);
        break;
      case '4':
        x = _MOVE_RANGE(x-1,MAP_X-1);
        break;
      case '6':
        x = _MOVE_RANGE(x+1,MAP_X-1);
        break;
      case '7':
        x = _MOVE_RANGE(x-1,MAP_X-1);
        y = _MOVE_RANGE(y-1,MAP_Y-1);
        break;
      case '8':
        y = _MOVE_RANGE(y-1,MAP_Y-1);
        break;
      case '9':
        x = _MOVE_RANGE(x+1,MAP_X-1);
        y = _MOVE_RANGE(y-1,MAP_Y-1);
        break;
      case ' ':
        return TELEPORT;
        break;
      case '0':
        break;
      case '+':
        return END;
        break;
      default:
        c = -1;
        break;
    }
  }

  player->vector.x = x;
  player->vector.y = y;
  return NONE;
}

void enemy_move(Enemy* root,Player* player)
{

  Enemy* pt = root->next;

  while(pt != root){
    if(pt->scrap == false){
      if(pt->vector.x < player->vector.x){
        (pt->vector.x)++;
      }else if(pt->vector.x > player->vector.x){
        (pt->vector.x)--;
      }

      if(pt->vector.y < player->vector.y){
        (pt->vector.y)++;
      }else if(pt->vector.y > player->vector.y){
        (pt->vector.y)--;
      }
    }
    pt = pt->next;
  }
}

void teleport(Enemy* root,Player* player)
{
  Enemy* pt = root->next;
  int x = 0,y = 0;
  int i;

  int vect = rand()%(MAP_X*MAP_Y);

  for(i = 0;i <= vect;i++){
    if(pt->vector.x == x && pt->vector.y == y){
      i--;
      pt = pt->next;
    }
    x++;
    if(x%MAP_X == 0){
      x = 0;y++;
    }
  }

  player->vector.x = x;
  player->vector.y = y;
}

int survive_check(Enemy* root,Player* player)
{

  Enemy* pt = root->next;
  while(pt->next != root){
    if(pt->vector.y == pt->next->vector.y){
      if(pt->vector.x == pt->next->vector.x){
        if(pt->scrap == false){
          pt->scrap = true;
          (player->score)++;
        }
        if(pt->next->scrap == false){
          pt->next->scrap = true;
          (player->score)++;
        }
      }
    }

    if(pt->vector.x == player->vector.x){
      if(pt->vector.y == player->vector.y){
        player->alive = false;
      }
    }
    pt = pt->next;
  }

  if(pt->vector.x == player->vector.x){
    if(pt->vector.y == player->vector.y){
      player->alive = false;
    }
  }
}

int clear_check(Enemy* root,Player* player)
{
  bool enem_flag = true;
  Enemy* pt = root->next;

  while(pt != root){
    if(pt->scrap == false)enem_flag =  false;
    pt = pt->next;
  }

  if(player->alive == false){
    return DEAD_END;
  }else if(enem_flag == true){
    return LEVEL_UP;
  }else{
    return NONE;
  }
}

Enemy* level_up(Enemy* root,Player* player)
{
  int enemCount;

  player->score += 50*player->level;

  (player->level)++;
  list_free(root);

  root = (Enemy*)malloc(sizeof(Enemy));
  root->vector.x = 65535;
  root->vector.y = 65535;

  enemCount = _MIN(player->level*5,MAX_ENEM);

  create_enemy(root,enemCount);

  list_sort(root);

  return root;
}
