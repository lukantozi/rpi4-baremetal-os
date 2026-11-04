#ifndef MAIL_H
#define MAIL_H

#include "peripherals.h"

#define MAIL_CHANNEL_ARM_VC 8
#define MAIL_CHANNEL_VC_ARM 9

#define MAIL_BASE               (PBASE + 0xb880)
#define MAIL_READ               (MAIL_BASE + 0x00)
#define MAIL_STATUS             (MAIL_BASE + 0x18)
#define MAIL_WRITE              (MAIL_BASE + 0x20)

#define MAIL_FULL               0x80000000
#define MAIL_EMPTY              0x40000000

#define MB_REQ_OK               0x80000000
#define MB_GET_FIRMV            0x00000001
#define MB_GET_MACADDR          0x00010003
#define MB_GET_SERIAL           0x00010004
#define MB_ALLOC_BUFFER         0x00040001

extern volatile unsigned int mailbox[49];
unsigned int mailbox_read(unsigned char channel_req);
void mailbox_write(unsigned char channel, unsigned int data);

#endif
