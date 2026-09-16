#ifndef TUI_H
#define TUI_H

#include "cpu.h"
#include <ncurses.h>

void draw_border(WINDOW *win, int height, int width);
void draw_memory(WINDOW *win, uint16_t start_addr, uint16_t end_addr,
                 int num_bytes);
void draw_registers(WINDOW *win, CPU *cpu);

void draw_box(int y, int x, int h, int w, const char *title);
#endif // TUI_H
