#include "utils.h"
#include "uart.h"
#include "mail.h"
#include "term.h"
#include "framebuffer.h"
#include "peripherals.h"

void mailbox_setup_request_firmware(void)
{
    mailbox[0] = 8 * 4;      // total size in bytes
    mailbox[1] = 0;          // process request
    mailbox[2] = 0x00000001; // firmware revision
    mailbox[3] = 4;          // value in bytes
    mailbox[4] = 0;
    mailbox[5] = 0;
    mailbox[6] = 0;
    mailbox[7] = 0;
}

void mailbox_setup_request_display(void)
{
    /* buffer header */
    mailbox[0] = 35 * 4;  // size of mail in bytes
    mailbox[1] = 0;       // request(0)/response code (0x8..0 ok, 0x8..1 err)

    /* tags */
    mailbox[2] = FB_SET_OFFSET; // tag
    mailbox[3] = 8; // size in bytes
    mailbox[4] = 0; // request
    mailbox[5] = 0; // x
    mailbox[6] = 0; // y

    mailbox[7] = FB_SET_PDISPLAY;
    mailbox[8] = 8;
    mailbox[9] = 0;
    mailbox[10] = 800;
    mailbox[11] = 600;

    mailbox[12] = FB_SET_VDISPLAY;
    mailbox[13] = 8;
    mailbox[14] = 0;
    mailbox[15] = 800;
    mailbox[16] = 600;

    mailbox[17] = FB_SET_DEPTH;
    mailbox[18] = 4;
    mailbox[19] = 0;
    mailbox[20] = 32; // bits per pix

    mailbox[21] = FB_SET_ORDER;
    mailbox[22] = 4;
    mailbox[23] = 0;
    mailbox[24] = 0; // BRG*

    mailbox[25] = FB_ALLOC_BUFFER;
    mailbox[26] = 8;  // space for two responce words
    mailbox[27] = 0;  // req
    mailbox[28] = 16; // 16-byte fb align
    mailbox[29] = 0;  // second response word reserve

    mailbox[30] = FB_GET_PITCH;
    mailbox[31] = 4;
    mailbox[32] = 0;
    mailbox[33] = 0;

    mailbox[34] = 0; // last
}

unsigned char *buffer;
unsigned int w;
unsigned int h;
unsigned int pitch;
unsigned int pixord;

void framebuffer_init(void)
{
    mailbox_setup_request_display();

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
