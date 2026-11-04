#include "mail.h"
#include "peripherals.h"
#include "utils.h"
#include "uart.h"

volatile unsigned int mailbox[49] __attribute__((aligned(16)));

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
