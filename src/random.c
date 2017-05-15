#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

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
    box(window->win, 0, 0);                              
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
  if (p->x == 3 && (p->y >= pd->top && p->y <= pd->bottom))
    return 1;
  return 0;
}

void *renderBall(void *vargs)
{
  struct render *r = (struct render *) vargs;
  struct paddle *pd = r->pd;
  struct pong *p = r->p;
  struct window *win = r->w;

  while (1) {

    // TODO: Collision detection here

    if (collision(p, pd) || p->x == (win->width-1) || p->x == 0) {
        p->o_rl = (p->o_rl == RIGHT) ? LEFT : RIGHT;
    }

    if (p->y == (win->height-1) || p->y == 0) {
        p->o_ud = (p->o_ud == UP) ? DOWN : UP;
    }

    p->x += (p->o_rl == RIGHT) ? 1 : -1;
    p->y += (p->o_ud == UP) ? -1 : 1;

    napms(50);
  }

  return NULL;
}

int main()
{
    int ch;
    int mx, my;
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
    
    WINDOW *my_win;

    struct render r;
    struct window win;

    win.height = my - 2;
    win.width = mx - 2;
    win.starty = 1;
    win.startx = 1;

    struct pong p;
    struct paddle pd = {-3+my/2, 3+my/2};

    p.x = mx / 2;
    p.y = my / 2 - 1;

    p.o_rl = RIGHT;
    p.o_ud = DOWN;

    /*
     * well, this is suppose to remove cursor, 
     * but functionality is not available on my terminal :\
     */
    curs_set(0);
    create_win(&win);
    //cbreak();
    //nodelay(win.win,0); 

    // Create another thread for rendering ball movement
    r.p = &p;
    r.w = &win;
    r.pd = &pd;
    pthread_t ball;
    pthread_create(&ball, NULL, renderBall, (void*)&r);
    char c = 'N';
    while (1) {
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
        printPaddle(&win, &pd);
        wborder(win.win, '|','|','-','-','+','+','+','+');
        wrefresh(win.win);
    }
    pthread_join(ball,NULL); 
    endwin();
    return 0;
}
