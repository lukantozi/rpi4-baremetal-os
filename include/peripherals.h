#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define PBASE 0xfe000000
//#define PBASE *(volatile unsigned int *)0xfe000000

#define GPIO_BASE               (PBASE     + 0x200000)
#define GPFSEL1                 (GPIO_BASE + 0x000004)
#define GPFSEL4                 (GPIO_BASE + 0x000010)
#define GPCLR1                  (GPIO_BASE + 0x00002c)
#define GPIO_PUP_PDN_CNTRL_REG0 (GPIO_BASE + 0x0000e4)

#define UART0_BASE              (PBASE      + 0x201000)
#define UART0_DR                UART0_BASE
#define UART0_FR                (UART0_BASE + 0x000018)
#define UART0_IBRD              (UART0_BASE + 0x000024)
#define UART0_FBRD              (UART0_BASE + 0x000028)
#define UART0_LCRH              (UART0_BASE + 0x00002c)
#define UART0_CR                (UART0_BASE + 0x000030)
#define UART0_IMSC              (UART0_BASE + 0x000038)

#define AUX_BASE                (PBASE    + 0x215000)
#define AUX_ENABLES             (AUX_BASE + 0x000004)
#define AUX_MU_IO_REG           (AUX_BASE + 0x000040)
#define AUX_MU_IER_REG          (AUX_BASE + 0x000044)
#define AUX_MU_IIR_REG          (AUX_BASE + 0x000048)
#define AUX_MU_LCR_REG          (AUX_BASE + 0x00004c)
#define AUX_MU_MCR_REG          (AUX_BASE + 0x000050)
#define AUX_MU_LSR_REG          (AUX_BASE + 0x000054)
#define AUX_MU_CNTL_REG         (AUX_BASE + 0x000060)
#define AUX_BAUD_REG            (AUX_BASE + 0x000068)

#endif
