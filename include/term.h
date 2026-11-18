#ifndef TERM_H
#define TERM_H

void term_init(void);
void term_cursor_blink(void);
void term_backspace(void);
void term_backspace_last(void);
void term_putc(unsigned char c);
void term_puts(const char *str);
void term_newline(void);
void term_newline_cli(void);
int term_set_border(int on);
void term_clear(void);
void term_put_uint32(unsigned int data);
void term_put_mac_address(void);

extern int symb_xn;
extern int symb_yn;
extern int line_end[];

#define XSTEP 8
#define YSTEP 12
#define XSTART 5
#define YSTART 5
#define NEWLINE 0

#endif
