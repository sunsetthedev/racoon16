#include "../include/tui.h"
#include "string.h"

void draw_box(int y, int x, int h, int w, const char *title)
{
    mvhline(y, x, ACS_HLINE, w - 2);
    mvhline(y + h - 1, x, ACS_HLINE, w - 2);
    mvvline(y, x, ACS_VLINE, h);
    mvvline(y, x + w - 1, ACS_VLINE, h);
    mvaddch(y, x,             ACS_ULCORNER);
    mvaddch(y, x + w - 1,     ACS_URCORNER);
    mvaddch(y + h - 1, x,     ACS_LLCORNER);
    mvaddch(y + h - 1, x + w - 1, ACS_LRCORNER);

    if (title) {
        int tlen = strlen(title);
        int tx = x + (w - tlen - 4) / 2;
        mvaddch(y, tx,     ACS_LTEE);
        mvaddch(y, tx + tlen + 3, ACS_RTEE);
        mvprintw(y, tx + 2, " %s ", title);
    }
}
 void print_reg_line(int y, int x, const char *name, uint16_t val)
{
    attron(COLOR_PAIR(2));          /* cyan label */
    mvprintw(y, x, "%-4s", name);
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));          /* yellow value */
    printw(" %04X", val);
    attroff(COLOR_PAIR(3));
}


