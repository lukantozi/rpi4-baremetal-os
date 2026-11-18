#include "shell.h"

#include "info.h"
#include "rect.h"
#include "term.h"
#include "uart.h"
#include "utils.h"

unsigned char command[512] = {0};
unsigned int command_len = 0;

void clear_cmd(void)
{
    for (unsigned int i = 0; i < command_len; i++) {
        command[i] = 0;
    }
    command_len = 0;
}

int func_clear(int st)
{
    (void)st;
    uart0_send_string("from command process, should clear\r\n");
    term_clear();
    term_puts("> ");
    return 0;
}

int func_info(int st)
{
    (void)st;
    term_newline();
    term_puts("Board firmware: ");
    term_put_uint32(board.firmware);
    term_newline();
    term_puts("Board mac addr: ");
    term_put_mac_address();
    return 1;
}

static const Cmd_entry cmds[] = {
    {.label="clear", .func=func_clear},
    {.label="border on", .func=term_set_border, .arg=1},
    {.label="border off", .func=term_set_border, .arg=0},
    {.label="info", .func=func_info},
};

int process_cmd(unsigned char c)
{
    int len = sizeof(cmds) / sizeof(cmds[0]);
    for (int i = 0; i < len; i++) {
        if (strcmp(cmds[i].label, (const char*)command) == 1)
            return cmds[i].func(cmds[i].arg);
    }
    return -1;
}

void process_key(unsigned char c)
{
    switch (c) {
        case 0x7f: // backspace
            if (command_len) {
                command[--command_len] = 0;
                term_backspace();
            }
            break;
        case 0x0d: // \r
            uart0_send_string("\r\n");
            uart0_send_string((const char *)command);
            uart0_send_string("\r\n");
            if (process_cmd(c) != 0)
                term_newline_cli();
            clear_cmd();
            break;
        default: // write char
            command[command_len++] = c;
            term_putc(c);
    }
}

typedef struct {
    unsigned int arrows[4][3];
    int count;
} Arrow_detect;

Arrow_detect detect = {
    .arrows = {
        {0x1b, 0x5b, 0x41},
        {0x1b, 0x5b, 0x42},
        {0x1b, 0x5b, 0x43},
        {0x1b, 0x5b, 0x44}
    },
    .count = 0,
};

extern Rect *rect;

int detect_arrow(unsigned char c)
{
    int status = 0;
    if (detect.count < 2) {
        if (c == detect.arrows[0][detect.count]) {
            detect.count++;
            return DETECT;
        } else {
            detect.count = 0;
            return DETECT_NOT;
        }
    } else if (detect.count == 2) {
        status = DETECT;
        detect.count = 0;
        switch (c) {
            case 0x41:
                uart0_send_string("up\r\n");
                move_rect_up(rect, 30);
                break;
            case 0x42:
                uart0_send_string("down\r\n");
                move_rect_down(rect, 30);
                break;
            case 0x43:
                uart0_send_string("right\r\n");
                move_rect_right(rect, 30);
                break;
            case 0x44:
                uart0_send_string("left\r\n");
                move_rect_left(rect, 30);
                break;
            default:
                status = DETECT_NOT;
        }
    }
    return status;
}
