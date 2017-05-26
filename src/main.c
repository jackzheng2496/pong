#include "game.h"

int gameState;
int score;
int acceleration;

struct ai {
  int ai_y;
  struct window *win;
  struct pong *p;
};

void *renderAI(void *vargs)
{
  struct ai *p_ai = (struct ai *) vargs;
  while (gameState) {
    p_ai->ai_y = p_ai->p->y;
  }
  return NULL;
}

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

    // Create another thread for opponent 'AI' paddle
    pthread_t ai;
    struct ai p_ai;

    p_ai.ai_y = ai_y;
    p_ai.win = &win;
    p_ai.p = &p;

    pthread_create(&ai, NULL, renderAI, (void*)&p_ai);

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
        mvwprintw(win.win, p_ai.ai_y, mx-5, "%c", 'P');

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
