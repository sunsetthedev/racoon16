#include <ncurses.h>

#define RB_HEIGHT 13
#define RB_WIDTH 12

void drawBox(int x, int y, int h, int w, const char *title) {
  mvhline(y, x, ACS_HLINE, w);

}



void RegWindow() { 
  WINDOW *regwin = newwin(RB_HEIGHT, RB_WIDTH, 1, 1);
  box(regwin, 0, 0);
  wrefresh(regwin);
  delwin(regwin);

}

void drawOuterBox() { box(stdscr, 0, 0); }

int main(int argc, char *argv[]) {
  // Program height and width
  initscr();
  int p_height, p_width;
  getmaxyx(stdscr, p_height, p_width);
  drawOuterBox();
  refresh();
  mvhline(p_height - 4, p_width - 1, ACS_HLINE, 0);
  mvprintw(p_height - 2, 1, "(S) Step (C) Continue (Q) Quit");
  refresh();

  RegWindow();

  refresh();
  getch();
  endwin();
}
