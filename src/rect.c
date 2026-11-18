#include "rect.h"

#include "fb.h"
#include "font.h"
#include "info.h"

Rect create_rect(int x_st, int y_st, int width, int height, unsigned int col)
{
    return (Rect){.x_st=x_st, .y_st=y_st, .width=width, .height=height, .col=col};
}

int draw_rect(int x_st, int y_st, int width, int height, unsigned int col)
{
    PIXEL_VALIDATE(x_st, y_st);
    PIXEL_VALIDATE(x_st + width, y_st + height);
    for (int x = x_st; x < x_st + width; x++) {
        for (int y = y_st; y < y_st + height; y++) {
            draw_pixel(x, y, col);
        }
    }
    return 1;
}

int draw_rect_frame(int xs, int ys, int xl, int yl, unsigned int col)
{
    PIXEL_VALIDATE(xs, ys);
    PIXEL_VALIDATE(xs + xl, ys + yl);

    for (int x = xs; x < xs + xl; x++) {
        draw_pixel(x, ys, col);
        draw_pixel(x, ys + yl - 1, col);
    }

    for (int y = ys; y < ys + yl; y++) {
        draw_pixel(xs, y, col);
        draw_pixel(xs + xl - 1, y, col);
    }

    return 1;
}

void draw_color_band(void)
{
    int colors = 16;
    int color_width = 20;
    int band_xpos = display.width - colors * color_width - 10;
    int band_height = 50;
    int band_ypos = 10;
    int next_xpos = band_xpos;

    for (int i = 15; i >= 0; i--) {
        draw_rect(next_xpos, band_ypos, color_width, band_height, i);
        next_xpos += color_width;
    }
    draw_rect_frame(1000, 100, 200, 200, 1);
    draw_rect_frame(band_xpos-1, 0, next_xpos+2, band_height+1, 1);
}

int clamp(int x, int y)
{
    return x > y ? y : x;
}

int move_rect_up(Rect *rect, int step)
{
    if (step < 0)
        return -1;
    int rheight = rect->height;
    int ryst = rect->y_st;
    step = clamp(step, rheight);
    step = clamp(step, ryst);
    if (step == 0)
        return -1;
    if (draw_rect(rect->x_st, rect->y_st - step, rect->width, step, rect->col) == 1) {
        if (draw_rect(rect->x_st, rect->y_st + rect->height - step, rect->width, step, BLACK) == 0)
            return -1;
        rect->y_st -= step;
        return 1;
    }
    return -1;
}

int move_rect_down(Rect *rect, int step)
{
    if (step < 0)
        return -1;
    int rect_down = rect->y_st + rect->height;
    int rect_to_bottom = display.height - rect_down;
    if (rect_down >= display.height)
        return -1;
    step = clamp(step, rect_to_bottom);
    if (draw_rect(rect->x_st, rect->y_st + rect->height, rect->width, step, rect->col) == 1) {
        if (draw_rect(rect->x_st, rect->y_st, rect->width, step, BLACK) == 0)
            return -1;
        rect->y_st += step;
        return 1;
    }
    return -1;
}

int move_rect_left(Rect *rect, int step)
{
    if (step < 0)
        return -1;
    int rwidth = rect->width;
    int rxst = rect->x_st;
    step = clamp(step, rwidth);
    step = clamp(step, rxst);
    if (step == 0)
        return -1;
    if (draw_rect(rxst - step, rect->y_st, step, rect->height, rect->col) == 1) {
        if (draw_rect(rxst + rwidth - step, rect->y_st, step, rect->height, BLACK) == 0)
            return -1;
        rect->x_st -= step;
        return 1;
    }
    return -1;
}

int move_rect_right(Rect *rect, int step)
{
    if (step < 0)
        return -1;
    int rect_right = rect->x_st + rect->width;
    int rect_to_right = display.width - rect_right;
    if (rect_right >= display.width)
        return -1;
    step = clamp(step, rect_to_right);
    if (draw_rect(rect->x_st + rect->width, rect->y_st, step, rect->height, rect->col) == 1) {
        if (draw_rect(rect->x_st, rect->y_st, step, rect->height, BLACK) == 0)
            return -1;
        rect->x_st += step;
        return 1;
    }
    return -1;
}
