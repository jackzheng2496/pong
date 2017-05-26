#include "game.h"

int gameState;
int score;
int acceleration;
int ai_move;

extern struct paddle ai_pd;

int main()
{
    int ch;
    int mx, my;
    int ai_y;
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

    ai_y = (my>>1)-1;

    struct paddle pd = {
      .top = -3+(my>>1), 
      .bottom = 3+(my>>1),
      .startx = 1,
      .endx = 3
    };

    ai_pd.top = ai_y-3;
    ai_pd.bottom = ai_y+3;
    ai_pd.startx = mx-6;
    ai_pd.endx = mx-4;

    struct render r = {
      .p = &p,
      .w = &win,
      .pd = &pd
    };

    acceleration = 50;
    gameState = 1;
    ai_move = 1;
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
        
        // Rendering pong ball
        wclear(win.win);
        mvwprintw(win.win, p.y, p.x, "%c", 'O');
        mvwprintw(win.win, 2, -3+(mx >> 1), "Score : %d", score);

        // Rendering user paddle
        printPaddle(&win, &pd);

        // Rendering AI paddle
        //mvwprintw(win.win, ai_y, mx-5, "%c", 'P');
        printPaddle(&win, &ai_pd);

        // Update position of paddle (random)
        //ai_y = p.y;

        if (ai_move) {
          if (p.y-3 > 0 && p.y+3 < my-4) {
            ai_pd.top = p.y-3;
            ai_pd.bottom = p.y+3;
          } else if (p.y-3 <= 0) {
            ai_pd.top = 2;
            ai_pd.bottom = ai_pd.top+6;
          } else {
            ai_pd.bottom = my-5;
            ai_pd.top = ai_pd.bottom-6;
          }
        }
        // Rendering window
        wborder(win.win, '|','|','-','-','+','+','+','+');

        // Refresh window to get new frame
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
