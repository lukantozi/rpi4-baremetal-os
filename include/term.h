#ifndef TERM_H
#define TERM_H

#define GLYPH_NUM  128
#define GLYPH_BYTES  8
#define GLYPH_HEIGHT 8
#define GLYPH_WIDTH  8
#define GLYPH_PERL   1

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5
#define LBLUE 6
#define LGREY 7
#define DGREY 8
#define RED_ 9
#define GREEN_ 10
#define YELLOW_ 11
#define BLUE_ 12
#define PINK_ 13
#define LBLUE_ 14
#define WHITE 15

// https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h
extern const unsigned char font8x8_basic[GLYPH_NUM][GLYPH_BYTES];
extern const unsigned int base16[];

#endif
