#include "game.h"

int gameState;
int score;
int acceleration;

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
