#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int gameState;
int score;
int acceleration;

enum ORIENTATION { RIGHT, LEFT, UP, DOWN };

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
};

void create_win(struct window *window)
{
    window->win = newwin(window->height, window->width,
                         window->starty, window->startx);
    box(window->win, window->startx, window->starty);                              
    wrefresh(window->win);
}

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

void printPaddle(struct window *window, struct paddle *pd)
{
  mvwprintw(window->win, pd->top-1, 2, "%c", '-');
  for (int i = pd->top; i <= pd->bottom; i++) {
    mvwprintw(window->win, i, 1, "%c", '|');
    mvwprintw(window->win, i, 3, "%c", '|');
  }
  mvwprintw(window->win, pd->bottom+1, 2, "%c", '-');
}

int collision(struct pong *p, struct paddle *pd)
{
  return p->x == 3 && (p->y >= pd->top && p->y <= pd->bottom);
}

void *renderBall(void *vargs)
{
  struct render *r = (struct render *) vargs;
  struct paddle *pd = r->pd;
  struct pong *p = r->p;
  struct window *win = r->w;

  while (1) {

    // TODO: Collision detection here
    // Ball movement should behave differently if it hits paddle
    if (collision(p,pd)) {
      p->o_rl = (p->o_rl == RIGHT) ? LEFT : RIGHT;
      if (++score % 2 == 0)
        acceleration -= 10;
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

    // Control how fast the ball moves
    napms(acceleration);
  }

  return NULL;
}

int main()
{
    int ch;
    int mx, my;
    char c;
    pthread_t ball;
    WINDOW *my_win;
    WINDOW *score_win;
    /*
     * Initialization
     */
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr,true);
    /* Removes cursor from terminal .. Find some way to put it back */
    fputs("\e[?25l", stdout);

    /* Find boundaries of the screen */
    getmaxyx(stdscr, my, mx);

    struct window win = {
      .height = my-2,
      .width = mx-2,
      .starty = 1,
      .startx = 1
    };

    struct pong p = {
      .x = mx>>1,
      .y = (my>>1)-1,
      .o_rl = RIGHT,
      .o_ud = DOWN
    };

    struct paddle pd = {
      .top = -3+(my>>1), 
      .bottom = 3+(my>>1)
    };

    struct render r = {
      .p = &p,
      .w = &win,
      .pd = &pd
    };

    acceleration = 50;
    gameState = 1;
    c = 'N';
    /*
     * well, this is suppose to remove cursor, 
     * but functionality is not available on my terminal :\
     */
    curs_set(0);
    create_win(&win);

    // Create another thread for rendering ball movement
    pthread_create(&ball, NULL, renderBall, (void*)&r);

    while (gameState) {
        c = getch();
// Co-creator - Norman Le
        switch (c) {
        case 'w':
          if (pd.top - 2 > 0) {
            pd.top -= 1;
            pd.bottom -= 1;
          }
          break;
        case 's':
          if (pd.bottom + 1 < my-4) {
            pd.top += 1;
            pd.bottom += 1;
          }
          break;
        default:
          break;
        }
       
        wclear(win.win);
        mvwprintw(win.win, p.y, p.x, "%c", 'O');
        mvwprintw(win.win, 2, -3+(mx >> 1), "Score : %d", score);
        printPaddle(&win, &pd);
        wborder(win.win, '|','|','-','-','+','+','+','+');
        wrefresh(win.win);
    }
    pthread_join(ball,NULL); 
    wclear(win.win);
    mvwprintw(win.win, 1, 0, "Final Score : %d", score);
    mvwprintw(win.win, 0, 0, "Game Over (press 'q' to quit)");
    wrefresh(win.win);
    while ((c = getch()) != 'q')
      ;
    endwin();
    return 0;
}
