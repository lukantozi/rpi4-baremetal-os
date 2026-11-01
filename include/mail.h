#ifndef MAIL_H
#define MAIL_H

#define MAIL_CHANNEL_ARM_VC 8
#define MAIL_CHANNEL_VC_ARM 9

extern volatile unsigned int mailbox[36];
unsigned int mailbox_read(unsigned char channel_req);
void mailbox_write(unsigned char channel, unsigned int data);

#endif
