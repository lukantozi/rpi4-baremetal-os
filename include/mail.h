#ifndef MAIL_H
#define MAIL_H

#define MAIL_CHANNEL_ARM_VC 8
#define MAIL_CHANNEL_VC_ARM 9

#define MAIL_BASE               (PBASE + 0xb880)
#define MAIL_READ               (MAIL_BASE + 0x00)
#define MAIL_STATUS             (MAIL_BASE + 0x18)
#define MAIL_WRITE              (MAIL_BASE + 0x20)

#define MAIL_FULL               0x80000000
#define MAIL_EMPTY              0x40000000

extern volatile unsigned int mailbox[36];
unsigned int mailbox_read(unsigned char channel_req);
void mailbox_write(unsigned char channel, unsigned int data);

#endif
