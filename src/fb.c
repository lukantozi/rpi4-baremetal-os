#include "utils.h"
#include "uart.h"
#include "mail.h"
#include "term.h"
#include "fb.h"
#include "peripherals.h"

void mailbox_setup_request(void)
{
    /* buffer header */
    mailbox[0] = 49 * 4;  // size of mail in chars
    mailbox[1] = 0;       // request(0)/response code (0x8..0 ok, 0x8..1 err)

    /* tags */
    mailbox[2] = MB_SET_OFFSET; // tag
    mailbox[3] = 8; // size in chars
    mailbox[4] = 0; // request
    mailbox[5] = 0; // x
    mailbox[6] = 0; // y

    mailbox[7] = MB_SET_PDISPLAY;
    mailbox[8] = 8;
    mailbox[9] = 0;
    mailbox[10] = 800;
    mailbox[11] = 600;

    mailbox[12] = MB_SET_VDISPLAY;
    mailbox[13] = 8;
    mailbox[14] = 0;
    mailbox[15] = 800;
    mailbox[16] = 600;

    mailbox[17] = MB_SET_DEPTH;
    mailbox[18] = 4;
    mailbox[19] = 0;
    mailbox[20] = 32; // bits per pix

    mailbox[21] = MB_SET_ORDER;
    mailbox[22] = 4;
    mailbox[23] = 0;
    mailbox[24] = 0; // BRG*

    mailbox[25] = MB_ALLOC_BUFFER;
    mailbox[26] = 8;  // space for two responce words
    mailbox[27] = 0;  // req
    mailbox[28] = 16; // 16-char fb align
    mailbox[29] = 0;  // second response word reserve

    mailbox[30] = MB_GET_PITCH;
    mailbox[31] = 4;
    mailbox[32] = 0;
    mailbox[33] = 0;

    mailbox[34] = MB_GET_FIRMV;
    mailbox[35] = 4;
    mailbox[36] = 0;
    mailbox[37] = 0;

    mailbox[38] = MB_GET_MACADDR;
    mailbox[39] = 6;
    mailbox[40] = 0;
    mailbox[41] = 0; // first 4 bytes
    mailbox[42] = 0; // second 2 bytes + padding

    mailbox[43] = MB_GET_SERIAL;
    mailbox[44] = 8;
    mailbox[45] = 0;
    mailbox[46] = 0; // low 32 bits
    mailbox[47] = 0; // high 32 bits

    mailbox[48] = 0; // last
}

unsigned char *buffer;
unsigned int w;
unsigned int h;
unsigned int pitch;
unsigned int pixord;
unsigned int firmware;
unsigned long mac;
unsigned long serial;

void mac_address_show(void)
{
    char hex[] = "0123456789abcdef";
    uart0_send_string("[MAC address] ");
    uart0_send_char(hex[mac & 0xf]);
    uart0_send_char(hex[(mac >> 4) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(mac >> 8) & 0xf]);
    uart0_send_char(hex[(mac >> 12) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(mac >> 16) & 0xf]);
    uart0_send_char(hex[(mac >> 20) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(mac >> 24) & 0xf]);
    uart0_send_char(hex[(mac >> 28) & 0xf]);
    uart0_send_string(":");
    uart0_send_char(hex[(mac >> 32) & 0xf]);
    uart0_send_char(hex[(mac >> 36) & 0xf]);
    uart0_send_string("\r\n");
}

void firmware_revision_show(void)
{
    uart0_send_string("[Firmware revision] 0x");
    uart0_send_uint32(firmware);
    uart0_send_string("\r\n");
}

void display_offset_show(void)
{
    uart0_send_string("[Offset (x, y)] ");
    uart0_send_string("(0x");
    uart0_send_uint32(w);
    uart0_send_string(", 0x");
    uart0_send_uint32(h);
    uart0_send_string(")");
    uart0_send_string("\r\n");
}

void board_serial_show(void)
{
    uart0_send_string("[Serial] 0x");
    uart0_send_uint32((unsigned int) (serial >> 32));
    uart0_send_uint32((unsigned int) serial);
    uart0_send_string("\r\n");
}

void fb_init(void)
{
    mailbox_setup_request();

    unsigned int mb_addr_req = (unsigned int)(long) mailbox;
    mailbox_write(MAIL_CHANNEL_ARM_VC, mb_addr_req);
    unsigned int mb_addr_resp = mailbox_read(MAIL_CHANNEL_ARM_VC);

    if (mb_addr_resp == mb_addr_req
            && mailbox[20] == 32 // pixels
            && mailbox[28]) {
        uart0_send_string("[Frame buffer] Response OK\r\n");
        w = mailbox[10];
        h = mailbox[11];
        pixord = mailbox[24];
        buffer = (unsigned char *)(long) mailbox[28];
        pitch = mailbox[33];
        firmware = mailbox[37];
        mac = mailbox[41] | ((unsigned long)(mailbox[42] & 0xffff) << 32);
        serial = ((unsigned long)mailbox[47] << 32) | mailbox[46];

        firmware_revision_show();
        mac_address_show();
        display_offset_show();
        board_serial_show();
    }
    else {
        uart0_send_string("[Frame buffer] Response BAD\r\n");
    }
}

void draw_pixel(int x, int y, unsigned char col)
{
    int loc = (y * pitch) + (x * (32 / 8));
    *((unsigned int *)(buffer + loc)) = base16[col & 0x0f];
}

void draw_rect(int st, int xl, int yl, unsigned int col)
{
    for (int x = st; x < xl + st; x++) {
        for (int y = st; y < yl + st; y++) {
            draw_pixel(x, y, col);
        }
    }
}
