#include <ncurses.h>

int main()
{
    int ch;
    int mx, my;
    int cx, cy;
    /*
     * Initialization
     */
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    /* Find boundaries of the screen */
    getmaxyx(stdscr, my, mx);

    printw("x %d y %d ", mx, my);

    while ((ch = getch()) != 'q') {
        getyx(stdscr, cy, cx);
        
        if (cy == (my - 1)) {
            clear();
            move(0, 0);
        }

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
    }
    endwin();
    return 0;
}
