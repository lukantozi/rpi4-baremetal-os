#include "utils.h"
#include "uart.h"
#include "mail.h"
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

#if 0
void mailbox_setup_request_display(void)
{
    /* buffer header */
    mailbox[0] = 7 * 4;  // size in bytes
    mailbox[1] = 0;       // request(0)/response code (0x8..0 ok, 0x8..1 err)

    /* tags */
    mailbox[2] = FB_PDISPLAY_SET; // tag
    mailbox[3] = 8; // size in bytes
    mailbox[4] = 0; // request codes: (b31 clear; b30-0 reserved) / response codes: (b31 set; b30-b0: value in bytes)
    mailbox[5] = 1920; // width
    mailbox[6] = 1080; // height
}
#endif

void framebuffer_init(void)
{
    mailbox_setup_request_firmware();

    unsigned int mb_addr_req = (unsigned int)(long) mailbox;
    mailbox_write(MAIL_CHANNEL_ARM_VC, mb_addr_req);
    unsigned int mb_addr_resp = mailbox_read(MAIL_CHANNEL_ARM_VC);

    if (mb_addr_resp == mb_addr_req
            && mailbox[1] == FB_REQ_OK
            && mailbox[4] == (FB_REQ_OK + 0x4)) {
        uart0_send_string("[Frame buffer] Response OK\r\n");
        uart0_send_string("[Firmware revision] 0x");
        uart0_send_uint32(mailbox[5]);
        uart0_send_string("\r\n");
    }
    else {
        uart0_send_string("[Frame buffer] Response BAD\r\n");
    }
}
