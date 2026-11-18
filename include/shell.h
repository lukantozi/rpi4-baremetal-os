#ifndef SHELL_H
#define SHELL_H

#define DETECT_NOT 0
#define DETECT 1

void process_key(unsigned char c);
int detect_arrow(unsigned char c);

typedef struct {
    const char *label;
    int (*func)(int);
    int arg;
} Cmd_entry;

#endif
