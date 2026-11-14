#include "common.h"
#include "fb.h"
#include "mail.h"
#include "peripherals.h"
#include "term.h"
#include "uart.h"
#include "utils.h"

unsigned int mb_ind;
int cursor_row = 1;
int cursor_col = 0;
int line_end[128] = {0};
int symb_xn = 0;
int symb_yn = 12;

void mailbox_begin(void)
{
    mb_ind = 0;
    mailbox[mb_ind++] = 0;  // set in the end
    mailbox[mb_ind++] = 0;  // request(0)/display code (0x8..0 ok, 0x8..1 err)
}

unsigned int mailbox_add_tag(unsigned int tag, unsigned int bytes)
{
    mailbox[mb_ind++] = tag;
    mailbox[mb_ind++] = bytes;
    mailbox[mb_ind++] = 0; // request
    unsigned int curr_ind = mb_ind;

    unsigned int words = (bytes + 3) / 4; // for mac
    for (unsigned int i = 0; i < words; i++) {
        mb_ind++;
    }

    return curr_ind;
}

void mailbox_end(void)
{
    mailbox[mb_ind++] = 0; // last
    mailbox[0] = mb_ind * 4;
}

Display display;
Board board;

void mac_address_show(void)
{
    char hex[] = "0123456789abcdef";
    uart0_send_string("[MAC address] ");
    uart0_send_char(hex[board.mac & 0xf]);
    uart0_send_char(hex[(board.mac >> 4) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(board.mac >> 8) & 0xf]);
    uart0_send_char(hex[(board.mac >> 12) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(board.mac >> 16) & 0xf]);
    uart0_send_char(hex[(board.mac >> 20) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(board.mac >> 24) & 0xf]);
    uart0_send_char(hex[(board.mac >> 28) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(board.mac >> 32) & 0xf]);
    uart0_send_char(hex[(board.mac >> 36) & 0xf]);
    uart0_send_string("\r\n");
}

void firmware_revision_show(void)
{
    uart0_send_string("[Firmware revision] 0x");
    uart0_send_uint32(board.firmware);
    uart0_send_string("\r\n");
}

void display_offset_show(void)
{
    uart0_send_string("[Offset (x, y)] ");
    uart0_send_string("(0x");
    uart0_send_uint32(display.width);
    uart0_send_string(", 0x");
    uart0_send_uint32(display.height);
    uart0_send_string(")");
    uart0_send_string("\r\n");
}

void board_serial_show(void)
{
    uart0_send_string("[Serial] 0x");
    uart0_send_uint32((unsigned int) (board.serial >> 32));
    uart0_send_uint32((unsigned int) board.serial);
    uart0_send_string("\r\n");
}

void fb_init(void)
{
    mailbox_begin();

    unsigned int offset = mailbox_add_tag(MB_SET_OFFSET, 8);
    mailbox[offset] = 0;   // x
    mailbox[offset+1] = 0; // y

    unsigned int phys = mailbox_add_tag(MB_SET_PDISPLAY, 8);
    mailbox[phys] = 1920;
    mailbox[phys+1] = 1200;

    unsigned int virt = mailbox_add_tag(MB_SET_VDISPLAY, 8);
    mailbox[virt] = 1920;
    mailbox[virt+1] = 1200;

    unsigned int depth = mailbox_add_tag(MB_SET_DEPTH, 4);
    mailbox[depth] = 32; // bits per pix

    unsigned int ord = mailbox_add_tag(MB_SET_ORDER, 4);
    mailbox[ord] = 0; // BGR*

    unsigned int buf = mailbox_add_tag(MB_ALLOC_BUFFER, 8);
    mailbox[buf] = 16;   // 16-char fb align
    mailbox[buf+1] = 0;  // second display word reserve

    unsigned int ptch = mailbox_add_tag(MB_GET_PITCH, 4);
    mailbox[ptch] = 0;

    mailbox_end();

    unsigned int mb_addr_req = (unsigned int)(long) mailbox;
    mailbox_write(MAIL_CHANNEL_ARM_VC, mb_addr_req);
    unsigned int mb_addr_resp = mailbox_read(MAIL_CHANNEL_ARM_VC);

    if (mb_addr_resp == mb_addr_req && mailbox[depth] == 32 && mailbox[buf]) {
        uart0_send_string("[Frame Buffer] Response OK\r\n");
        display.width = mailbox[phys];
        display.height = mailbox[phys+1];
        display.pixord = mailbox[ord];
        display.buffer = (unsigned char *)(long) mailbox[buf];
        display.pitch = mailbox[ptch];
        display_offset_show();
    }
    else {
        uart0_send_string("[Frame Buffer] Response BAD\r\n");
    }
}

void board_info(void)
{
    mailbox_begin();
    unsigned int firm = mailbox_add_tag(MB_GET_FIRMV, 4);
    mailbox[firm] = 0;

    unsigned int mac = mailbox_add_tag(MB_GET_MACADDR, 6);
    mailbox[mac] = 0; // first 4 bytes
    mailbox[mac+1] = 0; // second 2 bytes + padding

    unsigned int ser = mailbox_add_tag(MB_GET_SERIAL, 8);
    mailbox[ser] = 0; // low 32 bits
    mailbox[ser+1] = 0; // high 32 bits

    mailbox_end();

    unsigned int mb_addr_req = (unsigned int)(long) mailbox;
    mailbox_write(MAIL_CHANNEL_ARM_VC, mb_addr_req);
    unsigned int mb_addr_resp = mailbox_read(MAIL_CHANNEL_ARM_VC);

    if (mb_addr_resp == mb_addr_req
            && mailbox[1] == MB_REQ_OK
            && mailbox[firm-1] == (MB_REQ_OK | 0x4)
            && mailbox[mac-1] == (MB_REQ_OK | 0x6)
            /*&& mailbox[ser-1] == (MB_REQ_OK | 0x8)*/) {
        uart0_send_string("[Board Info] Response OK\r\n");
        board.firmware = mailbox[firm];
        board.mac = mailbox[mac] | ((unsigned long)(mailbox[mac+1] & 0xffff) << 32);
        board.serial = ((unsigned long)mailbox[ser+1] << 32) | mailbox[ser];

        firmware_revision_show();
        mac_address_show();
        board_serial_show();
    } else {
        uart0_send_string("[Board Info] Response BAD\r\n");
    }
}

void draw_pixel(int x, int y, unsigned char col)
{
    int loc = (y * display.pitch) + (x * (32 / 8));
    *((unsigned int *)(display.buffer + loc)) = base16[col & 0x0f];
}

void draw_rect(int xs, int ys, int xl, int yl, unsigned int col)
{
    for (int x = xs; x < xs + xl; x++) {
        for (int y = ys; y < ys + yl; y++) {
            draw_pixel(x, y, col);
        }
    }
}

void draw_char(int x, int y, unsigned char c, unsigned int col)
{
    int offset = (c % GLYPH_NUM) * GLYPH_BYTES;
    unsigned char *symbol = (unsigned char *)&font8x8_basic + offset;
    const unsigned int fg = col & 0x0f;
    const unsigned int bg = (col & 0xf0) >> 4;
    unsigned char px_col;

    for (int yp = 0; yp < GLYPH_HEIGHT; yp++) {
        for (int xp = 0; xp < GLYPH_WIDTH; xp++) {
            px_col = *symbol & (1 << xp) ? fg : bg;
            draw_pixel(x+xp, y+yp, px_col);
        }
        symbol++;
    }
}

void draw_block(int x, int y, unsigned int col)
{
    for (int yp = 0; yp < GLYPH_HEIGHT; yp++) {
        for (int xp = 0; xp < GLYPH_WIDTH; xp++) {
            draw_pixel(x+xp, y+yp, col);
        }
    }
}

void draw_char_erase(void)
{
    symb_xn -= XSTEP;
    cursor_col--;
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
}

void draw_char_erase_last(void)
{
    symb_yn -= YSTEP;
    int col = line_end[cursor_row-1];
    line_end[cursor_row--] = cursor_col;
    symb_xn = col * XSTEP;
    cursor_col = col;
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
}

void draw_cursor_newline(void)
{
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0x0);
    int col = line_end[cursor_row+1] * XSTEP;
    line_end[cursor_row++] = cursor_col;
    cursor_col = NEWLINE;
    symb_x = XSTART;
    symb_x = col ? col : symb_x;
    symb_xn = NEWLINE;
    symb_yn += YSTEP;
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
}

void draw_char_right(unsigned char c)
{
    draw_char(symb_x+symb_xn, symb_y+symb_yn, c, 0xf);
    symb_xn += XSTEP;
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
    cursor_col++;
}

void draw_char_process(unsigned char c)
{
    switch (c) {
        case 0x7f: // backspace
            draw_block(symb_x+symb_xn, symb_y+symb_yn, 0x0);
            if (symb_x + symb_xn != 5) {
                draw_char_erase();
                return;
            }
            if (symb_y + symb_yn == 5) {
                draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
                return;
            }
            draw_char_erase_last();
            break;
        case 0x0d: // \r
            draw_cursor_newline();
            break;
        default: // write char
            draw_char_right(c);
    }
}

void draw_string(int x, int y, char *str, unsigned int col)
{
    while (*str) {
        draw_char(x, y, (unsigned char)*str, col);
        x += GLYPH_WIDTH;
        str++;
    }
}
