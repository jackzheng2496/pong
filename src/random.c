#include <ncurses.h>
#include <unistd.h>

enum ORIENTATION { RIGHT, LEFT };

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
    // printw("x %d y %d ", mx, my);
    struct pong p;

    p.x = mx / 2;
    p.y = my / 2;

    p.orientation = RIGHT;

    /*
     * well, this is suppose to remove cursor, 
     * but functionality is not available on my terminal :\
     */
    curs_set(0);

    while (1) {
        erase();

        if (p.x == (mx-2)) {
            p.orientation = LEFT;
        } else if (p.x == 0) {
            p.orientation = RIGHT;
        }

        move(p.y, p.x);
        addch('o');
        refresh();
        usleep(50000);
        //refresh();
        
        p.x += (p.orientation == RIGHT) ? 1 : -1;

        /*
        if (mov_x == (mx - 1)) {
            move(++mov_y, 0);
            mov_x = 0;
        }
        if (mov_y == (my - 1)) {
            mov_y = mov_x = 0;
            move(mov_y, mov_x);
        }
        move(mov_y, mov_x);
        addch('o');
        usleep(50000);
        refresh();
        mov_x++;
        */
        // sleep for a bit
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
