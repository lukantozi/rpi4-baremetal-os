#ifndef INFO_H
#define INFO_H

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

void board_info(void);
extern Display display;
extern Board board;

#endif
