#ifndef TERM_H
#define TERM_H

#define GLYPH_NUM  128
#define GLYPH_BYTES  8
#define GLYPH_HEIGHT 8
#define GLYPH_WIDTH  8

// https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h
extern const unsigned char font8x8_basic[GLYPH_NUM][GLYPH_BYTES];
extern const unsigned int base16[];

#endif
