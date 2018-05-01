/*
   sudo apt install libncurses5-dev
   gcc ncurses_test.c -lncurses

http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html
   */

#include <stdio.h>
#include <ncurses.h>

int main()
{

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    printw("Press q to Exit\n");

    bool run = true;

    while (run)
    {
        int ch = getch();

        switch (ch)
        {
        case 'a':
        {
            mvaddch(5, 5, 'a');
            move(1, 1);
            break;
        }
        case 'q':
        {
            run = false;
            break;
        }
        default:
        {
            printw("%c", ch);
        }
        }
    }

    refresh();
    endwin();

    return 0;
}
