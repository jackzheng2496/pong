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

void delete_win(struct window *window)
{
    wborder(window->win, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(window->win);
    delwin(window->win);
}

struct pong {
    int x;
    int y;
    int o_rl;
    int o_ud;
};

struct render {
  struct pong *p;
  struct window *w; 
};

void printPaddle(struct window *window, struct paddle *pd)
{
  for (int i = pd->top; i <= pd->bottom; i++) {
    mvwprintw(window->win, i, 1, "%c", 'T');
  }
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
    char c = 'N';
    while (1) {
        c = getch();
        //erase();

        /* Gotta keep refreshing border */
        
        if (p.x == (win.width-1) || p.x == 0) {
            p.o_rl = (p.o_rl == RIGHT) ? LEFT : RIGHT;
        }

        if (p.y == (win.height-1) || p.y == 0) {
            p.o_ud = (p.o_ud == UP) ? DOWN : UP;
        }
        
// Co-creator - Norman Le
        wborder(win.win, '|','|','-','-','+','+','+','+');
        mvwprintw(win.win, p.y, p.x, "%c", 'o');
        printPaddle(&win, &pd);
        wrefresh(win.win);
        napms(50);
        
        switch (c) {
        case 'w':
          if (pd.top - 1 > 0) {
            pd.top -= 1;
            pd.bottom -= 1;
          }
          break;
        case 's':
          if (pd.bottom + 1 < my-3) {
            pd.top += 1;
            pd.bottom += 1;
          }
          break;
        default:
          break;
        }
        
        wclear(win.win);
        /* Just bouncing back and forth right now */
        /* Do some ball movement here */
        
        p.x += (p.o_rl == RIGHT) ? 1 : -1;
        p.y += (p.o_ud == UP) ? -1 : 1;
    }
    //pthread_join(ball,NULL); 
    endwin();
    return 0;
}
