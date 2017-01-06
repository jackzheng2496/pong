#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

enum ORIENTATION { RIGHT, LEFT, UP, DOWN };

struct window {
    int startx;
    int starty;
    int width;
    int height;
    WINDOW *win;
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
    keypad(stdscr, TRUE);

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

    p.x = mx / 2;
    p.y = my / 2 - 1;

    p.o_rl = RIGHT;
    p.o_ud = DOWN;

    /*
     * well, this is suppose to remove cursor, 
     * but functionality is not available on my terminal :\
     */
    curs_set(0);
    while (1) {
        erase();

        /* Gotta keep refreshing border */
        if (p.x == (win.width-1) || p.x == 0) {
            p.o_rl = (p.o_rl == RIGHT) ? LEFT : RIGHT;
        }

        if (p.y == (win.height-1) || p.y == 0) {
            p.o_ud = (p.o_ud == UP) ? DOWN : UP;
        }
        /*
        if (p.x == (win.width-1)) {
            p.o_rl = LEFT;
        } else if (p.x == 0) {
            p.o_rl = RIGHT;
        }

        if (p.y == (win.height-1)) {
            p.o_rl = LEFT;
        } else if (p.y == 0) {
            p.o_rl = RIGHT;
        }
        */

        create_win(&win);
        wborder(win.win, '|','|','-','-','+','+','+','+');
        mvwprintw(win.win, p.y, p.x, "%c", 'o');
        wrefresh(win.win);
        napms(50);

        delwin(win.win);

        /* Just bouncing back and forth right now */
        /* Do some ball movement here */
        if (p.o_ud == UP && p.o_rl == RIGHT) {
            p.x += 1;
            p.y -= 1;
        } else if (p.o_ud == UP && p.o_rl == LEFT) {
            p.x -= 1;
            p.y -= 1;
        } else if (p.o_ud == DOWN && p.o_rl == RIGHT) {
            p.x += 1;
            p.y += 1;
        } else if (p.o_ud == DOWN && p.o_rl == LEFT) {
            p.x -= 1;
            p.y += 1;
        }
        /*
        p.x += (p.o_rl == RIGHT) ? 1 : -1;
        p.y += (p.o_rl == RIGHT) ? 1 : -1;
        */
    }
    endwin();
    return 0;
}
