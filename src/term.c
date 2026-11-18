#include "term.h"

#include "fb.h"
#include "rect.h"
#include "font.h"
#include "info.h"
#include "timer.h"

int cursor_row = 0;
int cursor_col = 0;
int line_end[128] = {0};
int symb_xn = 0;
int symb_yn = 0;

void term_init(void)
{
   term_puts("[MOS] Init");
   term_newline_cli();
}

void term_cursor_blink(void)
{
    draw_block(XSTART+symb_xn, YSTART+symb_yn, WHITE);
    delay(400);
    draw_block(XSTART+symb_xn, YSTART+symb_yn, BLACK);
    delay(300);
}

void term_backspace(void)
{
    draw_block(XSTART+symb_xn, YSTART+symb_yn, BLACK);
    if (cursor_col > 0) {
        symb_xn -= XSTEP;
        cursor_col--;
        draw_block(XSTART+symb_xn, YSTART+symb_yn, WHITE);
    } else if (cursor_row > 0) {
        symb_yn -= YSTEP;
        int col = line_end[cursor_row-1];
        line_end[cursor_row--] = cursor_col;
        symb_xn = col * XSTEP;
        cursor_col = col;
        draw_block(XSTART+symb_xn, YSTART+symb_yn, WHITE);
    } else {
        draw_block(XSTART+symb_xn, YSTART+symb_yn, WHITE);
    }
}

void term_putc(unsigned char c)
{
    if (XSTART + symb_xn >= 1905)
        term_newline();
    draw_char(XSTART+symb_xn, YSTART+symb_yn, c, 0xf);
    symb_xn += XSTEP;
    draw_block(XSTART+symb_xn, YSTART+symb_yn, 0xf);
    cursor_col++;
}

void term_puts(const char *str)
{
    while (*str) {
        term_putc((unsigned char)*str++);
    }
}

void term_newline()
{
    draw_block(XSTART+symb_xn, YSTART+symb_yn, 0x0);
    int col = line_end[cursor_row+1];
    line_end[cursor_row++] = cursor_col;
    cursor_col = col;
    symb_xn = col * XSTEP;
    symb_yn += YSTEP;
    draw_block(XSTART+symb_xn, YSTART+symb_yn, 0xf);
}

void term_newline_cli()
{
    term_newline();
    term_puts("> ");
}

int term_set_border(int on)
{
    return draw_rect_frame(0, 0, display.width, display.height, on ? RED : BLACK);
}

void term_clear(void)
{
    for (unsigned int y = 0; y < display.height; y++) {
        for (unsigned int x = 0; x < display.width; x++) {
            draw_pixel(x, y, 0x0);
        }
    }
    symb_xn = 0;
    symb_yn = 0;

    for (unsigned int i = 0; i < cursor_row; i++) {
        line_end[i] = 0;
    }
    cursor_row = 0;
    cursor_col = 0;
}

void term_put_mac_address(void)
{
    char hex[] = "0123456789abcdef";
    term_putc(hex[board.mac & 0xf]);
    term_putc(hex[(board.mac >> 4) & 0xf]);
    term_putc(':');
    term_putc(hex[(board.mac >> 8) & 0xf]);
    term_putc(hex[(board.mac >> 12) & 0xf]);
    term_putc(':');
    term_putc(hex[(board.mac >> 16) & 0xf]);
    term_putc(hex[(board.mac >> 20) & 0xf]);
    term_putc(':');
    term_putc(hex[(board.mac >> 24) & 0xf]);
    term_putc(hex[(board.mac >> 28) & 0xf]);
    term_putc(':');
    term_putc(hex[(board.mac >> 32) & 0xf]);
    term_putc(hex[(board.mac >> 36) & 0xf]);
}

void term_put_uint32(unsigned int data)
{
    char hex[] = "0123456789abcdef";
    term_putc(hex[(data >> 28) & 0xf]);
    term_putc(hex[(data >> 24) & 0xf]);
    term_putc(hex[(data >> 20) & 0xf]);
    term_putc(hex[(data >> 16) & 0xf]);
    term_putc(hex[(data >> 12) & 0xf]);
    term_putc(hex[(data >> 8)  & 0xf]);
    term_putc(hex[(data >> 4)  & 0xf]);
    term_putc(hex[(data >> 0)  & 0xf]);
}
