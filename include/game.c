#include "game.h"

int gameState;
int score;
int acceleration;
int ai_move;

struct paddle ai_pd;

void create_win(struct window *window)
{
  window->win = newwin(window->height, window->width,
                       window->starty, window->startx);
  box(window->win, window->startx, window->starty);
  wrefresh(window->win);
}

void printPaddle(struct window *win, struct paddle *pd)
{
  /*
  mvwprintw(win->win, pd->top-1, pd->startx+1, "%c", '-');
  for (int i = pd->top; i <= pd->bottom; i++) {
    mvwprintw(win->win, i, pd->startx, "%c", '|');
    mvwprintw(win->win, i, pd->endx, "%c", '|');
  }
  mvwprintw(win->win, pd->bottom+1, pd->startx+1, "%c", '-');
  */
  mvwprintw(win->win, pd->top-1, pd->startx+1, "%c", '-');
  for (int i = pd->top; i <= pd->bottom; i++) {
    mvwprintw(win->win, i, pd->startx, "%c", '|');
    mvwprintw(win->win, i, pd->endx, "%c", '|');
  }
  mvwprintw(win->win, pd->bottom+1, pd->startx+1, "%c", '-');
}

void *renderBall(void *vargs)
{
  struct render *r = (struct render *) vargs;
  struct paddle *pd = r->pd;
  struct pong *p = r->p;
  struct window *win = r->w;

  srand(time(NULL));

  int col = -1;
  int rng;

  while (1) {
    // TODO: Collision detection here
    // Ball movement should behave differently if it hits paddle
    if ((col = collision(p,pd))) {
      p->o_rl = (p->o_rl == RIGHT) ? LEFT : RIGHT;

      if (col == 1) {
        if (++score % 2 == 0) {
          acceleration -= 5;
        }
        rng = rand() % 21;
        ai_move = (rng >= 10) ? 1 : 0;
      }
    }

    // Did not hit paddle, end game
    if (p->x == 0) {
      gameState = 0;
      break;
    }

    if (p->x == (win->width-1)) {
      p->o_rl = (p->o_rl == RIGHT) ? LEFT : RIGHT;
    }

    if (p->y == (win->height-1) || p->y == 0) {
      p->o_ud = (p->o_ud == UP) ? DOWN : UP;
    }

    p->x += (p->o_rl == RIGHT) ? 1 : -1;
    p->y += (p->o_ud == UP) ? -1 : 1;

    napms(acceleration);
  }
  return NULL;
}

int collision(struct pong *p, struct paddle *pd)
{
  if (p->x == 3 && (p->y >= pd->top-1 && p->y <= pd->bottom+1))
    return 1;
  else if (p->x == ai_pd.startx && (p->y >= ai_pd.top-1 && p->y <= ai_pd.bottom+1))
    return 2;
  else
    return 0;
}
