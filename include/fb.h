#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define MB_SET_PDISPLAY         0x00048003
#define MB_SET_VDISPLAY         0x00048004
#define MB_SET_DEPTH            0x00048005
#define MB_SET_ORDER            0x00048006
#define MB_GET_PITCH            0x00040008
#define MB_SET_OFFSET           0x00048009

#define RECT_AMOUNT 10

// typedef struct Rects rect[RECT_AMOUNT];
//
// typedef struct {
//     Rects rects;
//     int size = 0;
// } Rects;

void fb_init(void);
void board_info(void);
int draw_pixel(int x, int y, unsigned int col);
int draw_rect(int x_st, int y_st, int width, int height, unsigned int col);
int draw_rect_frame(int xs, int ys, int xl, int yl, unsigned int col);
int draw_char(int x, int y, unsigned char c, unsigned int col);
void draw_char_right(unsigned char c);
int draw_block(int x, int y, unsigned int col);
void draw_char_process(unsigned char c);
void draw_string(const char *str);
void draw_cursor_newline(void);
void draw_clear(void);
void draw_color_band(void);
Rect create_rect(int xs, int ys, int xl, int yl, unsigned int col);
int move_rect_up(Rect *rect, int step);
int move_rect_down(Rect *rect, int step);
int move_rect_left(Rect *rect, int step);
int move_rect_right(Rect *rect, int step);

typedef struct {
    unsigned char *buffer;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int pixord;
} Display;

typedef struct {
    unsigned int firmware;
    unsigned long mac;
    unsigned long serial;
} Board;

extern Display display;
extern Board board;

extern int symb_xn;
extern int symb_yn;
extern int line_end[];
// extern Rects rects;

#define XSTEP 8
#define YSTEP 12
#define XSTART 5
#define YSTART 5
#define NEWLINE 0

#endif
