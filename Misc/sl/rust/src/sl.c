/**************************************************************************************************
 INCLUDES
**************************************************************************************************/

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**************************************************************************************************
 FUNCTIONS
**************************************************************************************************/

void sl_initialize()
{
    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);
}

int sl_draw(int y, int x, char c_char)
{
    return mvaddch(y, x, c_char);
}

void sl_wait()
{
    getch();
    refresh();
    usleep(40000);
}

void sl_finalize()
{
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}

int sl_get_cols()
{
    return COLS;
}

int sl_get_lines()
{
    return LINES;
}
