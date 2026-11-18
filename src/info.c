#include "info.h"

#include "fb.h"
#include "mail.h"
#include "term.h"
#include "uart.h"

Display display;
Board board;

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

        uart0_send_firmware_revision();
        uart0_send_mac_address();
        uart0_send_board_serial();
    } else {
        uart0_send_string("[Board Info] Response BAD\r\n");
    }
}
