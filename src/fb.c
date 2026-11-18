#include "fb.h"

#include "font.h"
#include "info.h"
#include "mail.h"
#include "rect.h"
#include "uart.h"
#include "utils.h"

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

int draw_pixel(int x, int y, unsigned int col)
{
    PIXEL_VALIDATE(x, y);
    int loc = (y * display.pitch) + (x * (32 / 8));
    *((unsigned int *)(display.buffer + loc)) = base16[col & 0x0f];
    return 1;
}

int draw_char(int x, int y, unsigned char c, unsigned int col)
{
    PIXEL_VALIDATE(x, y);
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
    return 1;
}

int draw_block(int x, int y, unsigned int col)
{
    PIXEL_VALIDATE(x, y);
    for (int yp = 0; yp < GLYPH_HEIGHT; yp++) {
        for (int xp = 0; xp < GLYPH_WIDTH; xp++) {
            draw_pixel(x+xp, y+yp, col);
        }
    }
    return 1;
}
