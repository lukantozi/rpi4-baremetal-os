#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define PBASE 0xfe000000

#define GPFSEL1                 (PBASE + 0x200004)
#define GPFSEL4                 (PBASE + 0x200010)
#define GPCLR1                  (PBASE + 0x20002c)
#define GPIO_PUP_PDN_CNTRL_REG0 (PBASE + 0x2000e4)

#define AUX_ENABLES             (PBASE + 0x215004)
#define AUX_MU_IO_REG           (PBASE + 0x215040)
#define AUX_MU_IER_REG          (PBASE + 0x215044)
#define AUX_MU_IIR_REG          (PBASE + 0x215048)
#define AUX_MU_LCR_REG          (PBASE + 0x21504c)
#define AUX_MU_MCR_REG          (PBASE + 0x215050)
#define AUX_MU_LSR_REG          (PBASE + 0x215054)
#define AUX_MU_CNTL_REG         (PBASE + 0x215060)
#define AUX_BAUD_REG            (PBASE + 0x215068)

#endif
