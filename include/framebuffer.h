#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define FB_REQ_OK               0x80000000
#define FB_ALLOC_BUFFER         0x00040001
#define FB_SET_PDISPLAY         0x00048003
#define FB_SET_VDISPLAY         0x00048004
#define FB_SET_DEPTH            0x00048005
#define FB_SET_ORDER            0x00048006
#define FB_GET_PITCH            0x00040008
#define FB_SET_OFFSET           0x00048009

void framebuffer_init(void);
void draw_pixel(int x, int y, unsigned char col);

#endif
