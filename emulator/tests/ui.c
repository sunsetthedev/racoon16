/*
 * emulator_ui.c  —  ncurses front-end for a 16-bit CPU emulator
 *
 * Build:  gcc -o emulator_ui emulator_ui.c -lncurses
 * Run:    ./emulator_ui
 *
 * Minimum terminal size: 130 columns × 50 rows
 * Linux only (ncurses).
 */

#define _GNU_SOURCE
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

/* ──────────────────────────────────────────────────────────────────
 *  CPU & memory types
 * ────────────────────────────────────────────────────────────────── */

#define MEM_SIZE   0x8000          /* 32 KB  (0x0000 … 0x7FFF)        */
#define NUM_REGS   8               /* S0 … S7                       */
#define REG_NAMES  {"S0","S1","S2","S3","S4","S5","S6","S7"}

typedef struct {
    uint16_t regs[NUM_REGS];       /* S0 … S7  (16-bit each)         */
    uint16_t pc;                   /* program counter                */
    uint16_t sp;                   /* stack pointer                  */
    uint8_t  flags;                /* bit-field: Z N H C …           */
    int      running;
    uint64_t cycles;
    double   speed_mhz;
} CPU;

static uint8_t memory[MEM_SIZE];   /* 8-bit cells                    */
static CPU     cpu;

/* ──────────────────────────────────────────────────────────────────
 *  Layout constants  (tweak to taste)
 * ────────────────────────────────────────────────────────────────── */

#define MIN_COLS  130
#define MIN_ROWS  20

/* panel heights / widths are computed from LINES / COLS at runtime   */

/* ─────────────────────────────────────────────────────────────────
 *  Helpers
 * ────────────────────────────────────────────────────────────────── */

static void draw_box(int y, int x, int h, int w, const char *title)
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

static void print_reg_line(int y, int x, const char *name, uint16_t val)
{
    attron(COLOR_PAIR(2));          /* cyan label */
    mvprintw(y, x, "%-4s", name);
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));          /* yellow value */
    printw(" %04X", val);
    attroff(COLOR_PAIR(3));
}

/* ──────────────────────────────────────────────────────────────────
 *  Panel renderers
 * ────────────────────────────────────────────────────────────────── */

static void draw_registers(int y, int x, int h, int w)
{
    draw_box(y, x, h, w, "Registers");

    const char *names[] = REG_NAMES;
    int cy = y + 1;
    int cx = x + 1;

    for (int i = 0; i < NUM_REGS; i++) {
        if (i < NUM_REGS / 2)
            print_reg_line(cy + i, cx, names[i], cpu.regs[i]);
        else
            print_reg_line(cy + (i - NUM_REGS / 2), cx + 14,
                           names[i], cpu.regs[i]);
    }

    /* PC / SP / Flags on the left side */
    int ly = cy + (NUM_REGS / 2) + 1;
    print_reg_line(ly,     cx, "PC", cpu.pc);
    print_reg_line(ly + 1, cx, "SP", cpu.sp);

    attron(COLOR_PAIR(2));
    mvprintw(ly + 2, cx, "Flags:");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(1));
    printw(" -Z----N-");   /* example flag string */
    attroff(COLOR_PAIR(1));
}

/* ── Disassembly ─────────────────────────────────────────────────── */

static void draw_disassembly(int y, int x, int h, int w)
{
    draw_box(y, x, h, w, "Disassembly");

    /* Fake a few instructions around PC for demonstration */
    uint16_t addr = cpu.pc;
    int cy = y + 1;

    static const char *fake_ops[][3] = {
        {"007A", "28 F9",      "JR  Z,$0075"},
        {"007C", "E5",         "PUSH HL"},
        {"007D", "2A 41 20",   "LD  HL,($2041)"},
        {"0080", "23",         "INC HL"},
        {"0081", "7D",         "LD  A,L"},
        {"0082", "FE 3F",      "CP  $3F"},
    };

    for (int i = 0; i < 6 && (cy + i) < y + h - 1; i++) {
        int row = cy + i;
        attron(COLOR_PAIR(4));  /* green highlight for current */
        if (i == 0) {
            mvprintw(row, x + 1, ">");
        } else {
            mvprintw(row, x + 1, " ");
        }
        attroff(COLOR_PAIR(4));

        attron(COLOR_PAIR(3));
        mvprintw(row, x + 3, "%s", fake_ops[i][0]);
        attroff(COLOR_PAIR(3));

        attron(COLOR_PAIR(1));
        mvprintw(row, x + 10, "%-10s", fake_ops[i][1]);
        attroff(COLOR_PAIR(1));

        attron(COLOR_PAIR(5));  /* white mnemonic */
        mvprintw(row, x + 22, "%s", fake_ops[i][2]);
        attroff(COLOR_PAIR(5));
    }
}

/* ── Metrics ─────────────────────────────────────────────────────── */

static void draw_metrics(int y, int x, int h, int w)
{
    draw_box(y, x, h, w, "Metrics");

    int cy = y + 1;
    int cx = x + 2;

    attron(COLOR_PAIR(2)); mvprintw(cy++, cx, "── Z80 ──"); attroff(COLOR_PAIR(2));
    mvprintw(cy++, cx, "Speed: %.2f MHz", cpu.speed_mhz);
    mvprintw(cy++, cx, "Cycles: %llu", (unsigned long long)cpu.cycles);
    mvprintw(cy++, cx, "RAM: 3%% used");
    mvprintw(cy++, cx, "Stack: 0 words");
    mvprintw(cy++, cx, "ROM: 8 KB");
    cy++;

    attron(COLOR_PAIR(2)); mvprintw(cy++, cx, "── I/O ──"); attroff(COLOR_PAIR(2));
    mvprintw(cy++, cx, "InBuf: 0 chars");
    mvprintw(cy++, cx, "Term: 80x24");
    mvprintw(cy++, cx, "INT: IM1 EI");
    cy++;

    attron(COLOR_PAIR(2)); mvprintw(cy++, cx, "── Host ──"); attroff(COLOR_PAIR(2));
    mvprintw(cy++, cx, "CPU: 100.0%%");
    mvprintw(cy++, cx, "Mem: 16.7 MB");
}

/* ── Memory hex dump ─────────────────────────────────────────────── */

static void draw_memory(int y, int x, int h, int w)
{
    draw_box(y, x, h, w, "Memory");

    /* Show 8 rows × 16 bytes = 128 bytes starting near PC */
    uint16_t base = (cpu.pc & 0xFFF0);   /* align to 16-byte boundary */
    int cy = y + 1;
    int cx = x + 1;

    for (int row = 0; row < 8 && (cy + row) < y + h - 1; row++) {
        uint16_t addr = base + row * 16;

        /* address */
        attron(COLOR_PAIR(3));
        mvprintw(cy + row, cx, "%04X:", addr);
        attroff(COLOR_PAIR(3));

        /* hex bytes */
        int bx = cx + 6;
        for (int col = 0; col < 16; col++) {
            uint16_t a = addr + col;
            if (a < MEM_SIZE) {
                uint8_t b = memory[a];
                if (a == cpu.pc)
                    attron(COLOR_PAIR(4));   /* highlight PC byte */
                else
                    attron(COLOR_PAIR(1));
                printw("%02X ", b);
                attroff(COLOR_PAIR(a == cpu.pc ? 4 : 1));
            } else {
                printw("   ");
            }
        }

        /* ASCII column */
        int ax = bx + 48;
        mvaddch(cy + row, ax - 1, '|');
        for (int col = 0; col < 16; col++) {
            uint16_t a = addr + col;
            char ch = (a < MEM_SIZE) ? memory[a] : '.';
            if (ch < 32 || ch > 126) ch = '.';
            mvaddch(cy + row, ax + col, ch);
        }
    }
}

/* ── Terminal ────────────────────────────────────────────────────── */

#define TERM_LINES 14

static void draw_terminal(int y, int x, int h, int w)
{
    draw_box(y, x, h, w, "Terminal");

    int cy = y + 1;
    int cx = x + 1;
    int max_lines = h - 2;

    attron(COLOR_PAIR(5));
    mvprintw(cy++, cx, "Z80 SBC By Grant Searle");
    mvprintw(cy++, cx, "");
    mvprintw(cy++, cx, "Memory top?");
    mvprintw(cy++, cx, "Z80 BASIC Ver 4.7b");
    mvprintw(cy++, cx, "Copyright (C) 1978 by Microsoft");
    mvprintw(cy++, cx, "56958 Bytes free");
    mvprintw(cy++, cx, "Ok");
    mvprintw(cy++, cx, "10 PRINT \"HELLO WORLD\"");
    mvprintw(cy++, cx, "20 GOTO 10");
    mvprintw(cy++, cx, "RUN");
    attroff(COLOR_PAIR(5));

    /* cursor */
    if (cy < y + h - 1)
        mvaddch(cy, cx + 3, ACS_BLOCK | A_BLINK);
}

/* ── Status bar ──────────────────────────────────────────────────── */

static void draw_statusbar(int y, int w)
{
    attron(COLOR_PAIR(2));
    mvprintw(y, 1, "F5:Run  F6:Step  F7:Pause  F8:Reset  "
                   "PgUp/Dn:Mem  Home:MemPC  F12:Quit");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(4));
    mvprintw(y + 1, 1, "● RUNNING");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(1));
    mvprintw(y + 1, 16, "Cycles: %llu", (unsigned long long)cpu.cycles);
    mvprintw(y + 1, 50, "Mem: $%04X", cpu.pc);
    attroff(COLOR_PAIR(1));
}

/* ──────────────────────────────────────────────────────────────────
 *  Main layout
 * ────────────────────────────────────────────────────────────────── */

static void draw_all(void)
{
    int rows = LINES, cols = COLS;

    /* enforce minimum size */
    if (rows < MIN_ROWS || cols < MIN_COLS) {
        clear();
        attron(COLOR_PAIR(6));
        mvprintw(rows / 2, (cols - 50) / 2,
                 "Terminal too small! Need %dx%d (have %dx%d)",
                 MIN_COLS, MIN_ROWS, cols, rows);
        attroff(COLOR_PAIR(6));
        refresh();
        return;
    }

    erase();

    /* Top row:  Registers | Disassembly | Metrics */
    int top_h = 11;
    int reg_w  = 26;
    int met_w  = 26;
    int dis_w  = cols - reg_w - met_w;

    draw_registers   (1, 1,          top_h, reg_w);
    draw_disassembly (1, 1 + reg_w,  top_h, dis_w);
    draw_metrics     (1, cols - met_w - 1, top_h, met_w);

    /* Memory row */
    int mem_y = 1 + top_h + 1;
    int mem_h = 11;
    draw_memory(mem_y, 1, mem_h, cols - 2);

    /* Terminal row */
    int term_y = mem_y + mem_h + 1;
    int term_h = rows - term_y - 4;   /* leave room for status bar */
    draw_terminal(term_y, 1, term_h, cols - 2);

    /* Status bar */
    draw_statusbar(rows - 3, cols);

    refresh();
}

/* ──────────────────────────────────────────────────────────────────
 *  Initialise emulator state (demo values)
 * ───────────────────────────────────────────────────────────────── */

static void init_cpu(void)
{
    memset(&cpu, 0, sizeof(cpu));
    cpu.pc = 0x007A;
    cpu.sp = 0xFFC3;
    cpu.regs[0] = 0x0042;   /* S0 */
    cpu.regs[1] = 0x044E;   /* S1 */
    cpu.regs[2] = 0x215C;   /* S2 */
    cpu.regs[3] = 0x20A9;   /* S3 */
    cpu.regs[4] = 0x0000;   /* S4 */
    cpu.regs[5] = 0x0000;   /* S5 */
    cpu.regs[6] = 0x0000;   /* S6 */
    cpu.regs[7] = 0x0000;   /* S7 */
    cpu.flags = 0x02;       /* N flag set */
    cpu.running = 1;
    cpu.cycles = 74625007481ULL;
    cpu.speed_mhz = 962.60;

    /* Sprinkle some demo bytes into memory around PC */
    uint8_t demo[] = {
        0x28, 0xF9, 0xE5, 0x2A, 0x41, 0x20,
        0x23, 0x7D, 0xFE, 0x3F, 0xC3, 0xBA,
        0x00, 0x00, 0x00, 0x00, 0xC3, 0xA0,
    };
    for (size_t i = 0; i < sizeof(demo); i++)
        if (cpu.pc + i < MEM_SIZE)
            memory[cpu.pc + i] = demo[i];
}

/* ──────────────────────────────────────────────────────────────────
 *  Main
 * ────────────────────────────────────────────────────────────────── */

int main(void)
{
    /* ── ncurses setup ─────────────────────────────────────────── */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);          /* non-blocking getch */
    timeout(100);                   /* 100 ms refresh rate */

    if (has_colors()) {
        start_color();
        use_default_colors();
        /* pair(id, fg, bg) */
        init_pair(1, COLOR_WHITE,  -1);   /* default text       */
        init_pair(2, COLOR_CYAN,   -1);   /* labels / headers   */
        init_pair(3, COLOR_YELLOW, -1);   /* hex values         */
        init_pair(4, COLOR_GREEN,  -1);   /* highlight / PC     */
        init_pair(5, COLOR_WHITE,  -1);   /* terminal text      */
        init_pair(6, COLOR_RED,    -1);   /* error / warning    */
    }

    init_cpu();

    /* ── main loop ─────────────────────────────────────────────── */
    int ch;
    while (1) {
        draw_all();

        ch = getch();
        switch (ch) {
        case KEY_F(12):
        case 'q':
        case 'Q':
            goto cleanup;

        case KEY_F(5):   cpu.running = 1;          break;
        case KEY_F(6):   cpu.cycles += 4;          break;   /* single step */
        case KEY_F(7):   cpu.running = 0;          break;
        case KEY_F(8):   init_cpu();               break;
        case KEY_HOME:   cpu.pc = 0x0000;          break;
        case KEY_PPAGE:  if (cpu.pc >= 0x10) cpu.pc -= 0x10; break;
        case KEY_NPAGE:  if (cpu.pc < MEM_SIZE - 0x10) cpu.pc += 0x10; break;

        case KEY_RESIZE:
            /* ncurses auto-updates LINES/COLS; next draw_all() adapts */
            break;

        default:
            break;
        }

        /* Simulate cycle counter ticking while "running" */
        if (cpu.running)
            cpu.cycles += 1000;
    }

cleanup:
    endwin();
    return 0;
}
