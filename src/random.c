#include <ncurses.h>
#include <unistd.h>

enum ORIENTATION { RIGHT, LEFT };

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
    int orientation;
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

    /* Find boundaries of the screen */
    getmaxyx(stdscr, my, mx);
    
    WINDOW *my_win;

    struct window win;

    win.height = my - 2;
    win.width = mx - 2;
    win.starty = 1;
    win.startx = 1;

    create_win(&win);
    delete_win(&win);

    struct pong p;

    p.x = mx / 2;
    p.y = my / 2 - 1;

    p.orientation = RIGHT;

    /*
     * well, this is suppose to remove cursor, 
     * but functionality is not available on my terminal :\
     */
    curs_set(0);
    while (1) {
        erase();

        /* Gotta keep refreshing border */

        if (p.x == (win.width-2)) {
            p.orientation = LEFT;
        } else if (p.x == 1) {
            p.orientation = RIGHT;
        }

        create_win(&win);
        //delete_win(&win); 
        wborder(win.win, '|','|','-','-','+','+','+','+');
        mvwprintw(win.win, p.y, p.x, "%c", 'o');
        wrefresh(win.win);
        //move(p.y, p.x);
        //addch('o');
        //refresh();
        napms(40);
 /*
        move(p.y, p.x);
        addch('o');
        refresh();
        napms(50);

        create_win(&win);
        delete_win(&win);
*/
        delwin(win.win);
        p.x += (p.orientation == RIGHT) ? 1 : -1;
        
        /*
        switch (ch) {
        case KEY_UP:
            printw("up ");
            break;
        case KEY_DOWN:
            printw("down ");
            break;
        default:
            printw("%c ", ch);
            break;
        }
        refresh();
        */
    }
    endwin();
    return 0;
}
