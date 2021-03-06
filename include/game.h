#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

extern int gameState;
extern int score;
extern int acceleration;
extern int ai_move;

enum ORIENTATION {
  RIGHT,
  LEFT,
  UP,
  DOWN
};

struct window {
  int startx;
  int starty;
  int width;
  int height;
  WINDOW *win;
};

struct paddle {
  int top;
  int bottom;
  int startx;
  int endx;
  int center;
};

extern struct paddle ai_pd;

struct pong {
  int x;
  int y;
  int o_rl;
  int o_ud;
};

struct render {
  struct pong *p;
  struct paddle *pd;
  struct window *w;
};

void create_win(struct window *);
void printPaddle(struct window *, struct paddle *);
void *renderBall(void *);
int collision(struct pong *, struct paddle *);

#endif
