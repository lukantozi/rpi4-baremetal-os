#include "mail.h"

#include "info.h"
#include "peripherals.h"
#include "uart.h"
#include "utils.h"

volatile unsigned int mailbox[49] __attribute__((aligned(16)));
extern Display display;
unsigned int mb_ind;

// reference: https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
unsigned int mailbox_read(unsigned char channel_req)
{
    while (1) { // wait until we get something from channel
        while ((read_reg32(MAIL_STATUS) & MAIL_EMPTY) != 0); // wait for data
        unsigned int data = read_reg32(MAIL_READ);
        unsigned char channel_resp = data & 0xf;

        if (channel_resp == channel_req)
            return data & ~(0xf);
    }
}

void mailbox_write(unsigned char channel, unsigned int mb_addr)
{
    while ((read_reg32(MAIL_STATUS) & MAIL_FULL) != 0); // wait for space
    write_reg32(MAIL_WRITE, ((mb_addr & (~0xf)) | (channel & 0xf)));
}

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
