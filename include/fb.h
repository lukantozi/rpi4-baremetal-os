#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define MB_SET_PDISPLAY         0x00048003
#define MB_SET_VDISPLAY         0x00048004
#define MB_SET_DEPTH            0x00048005
#define MB_SET_ORDER            0x00048006
#define MB_GET_PITCH            0x00040008
#define MB_SET_OFFSET           0x00048009

void fb_init(void);
void draw_pixel(int x, int y, unsigned char col);
void draw_rect(int st, int xl, int yl, unsigned int col);

#endif
