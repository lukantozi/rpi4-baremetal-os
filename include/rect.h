#ifndef RECT_H
#define RECT_H

typedef struct {
    int x_st;
    int y_st;
    int width;
    int height;
    unsigned int col;
} Rect;

Rect create_rect(int xs, int ys, int xl, int yl, unsigned int col);
int draw_rect(int x_st, int y_st, int width, int height, unsigned int col);
int draw_rect_frame(int xs, int ys, int xl, int yl, unsigned int col);
void draw_color_band(void);
int move_rect_up(Rect *rect, int step);
int move_rect_down(Rect *rect, int step);
int move_rect_left(Rect *rect, int step);
int move_rect_right(Rect *rect, int step);

#endif
