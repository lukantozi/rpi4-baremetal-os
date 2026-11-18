#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define MB_SET_PDISPLAY         0x00048003
#define MB_SET_VDISPLAY         0x00048004
#define MB_SET_DEPTH            0x00048005
#define MB_SET_ORDER            0x00048006
#define MB_GET_PITCH            0x00040008
#define MB_SET_OFFSET           0x00048009

void fb_init(void);
int draw_pixel(int x, int y, unsigned int col);
int draw_char(int x, int y, unsigned char c, unsigned int col);
int draw_block(int x, int y, unsigned int col);

void uart0_send_string(const char *s);
#define PIXEL_VALIDATE(x, y)                                         \
do {                                                                 \
    if (x < 0 || x > 1920 || y < 0 || y > 1200) {                    \
        uart0_send_string("trying to draw pixel outside of fb\r\n"); \
        return -1;                                                   \
    }                                                                \
} while (0)

#endif
