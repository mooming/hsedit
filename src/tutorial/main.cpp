#include <ncurses.h>

static WINDOW* footerWin = nullptr;

void createFooter()
{
    int rows = LINES;
    int cols = COLS;
    int cur_y, cur_x;
    getyx(stdscr, cur_y, cur_x);

    footerWin = newwin(5, 40, 10, 5);
    wprintw(footerWin, "LINES: %d, COLS: %d, cursor: (%d, %d)", rows, cols, cur_y, cur_x);
    wrefresh(footerWin);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    mvprintw(0, 0, "Hello, ncurses!");
    refresh();

    createFooter();
    getch();

    delwin(footerWin);
    endwin();

    return 0;
}
