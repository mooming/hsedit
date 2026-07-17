#include <ncurses.h>

void createFooter()
{
    int rows   = LINES;
    int cols   = COLS;
    int cur_y  = getcury(stdscr);
    int cur_x  = getcurx(stdscr);

    WINDOW* footerWin = newwin(0, rows - 1, cols, rows);
    wprintw(footerWin, "LINES: %d, COLS: %d", cur_y, cur_x);
    wrefresh(footerWin);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    mvprintw(0, 0, "Hello, ncurses!");
    createFooter();

    refresh();
    getch();

    endwin();

    return 0;
}
