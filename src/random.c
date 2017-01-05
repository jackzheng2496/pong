#include <ncurses.h>
#include <unistd.h>

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
    // printw("x %d y %d ", mx, my);
    int mov_x, mov_y;

    mov_x = mov_y = 0;

    while (1) {
    // while ((ch = getch()) != 'q') {
        erase();

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
        //printw("x %d y %d", mov_x, mov_y);
        //printw("cursor x %d cursor y %d", cx, cy);
        usleep(50000);
        refresh();
        mov_x++;
        //move(mov_y, mov_x);
        //mvaddch(mov_y, mov_x, 'o');
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
