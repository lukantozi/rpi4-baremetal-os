#ifndef UART_H
#define UART_H

#include "peripherals.h"

#define GPIO_PUP_PDN_CNTRL_REG0 (GPIO_BASE + 0x0000e4)
#define GPIO_BASE               (PBASE     + 0x200000)
#define GPFSEL1                 (GPIO_BASE + 0x000004)
#define GPFSEL4                 (GPIO_BASE + 0x000010)
#define GPCLR1                  (GPIO_BASE + 0x00002c)

#define UART0_BASE              (PBASE      + 0x201000)
#define UART0_DR                UART0_BASE
#define UART0_FR                (UART0_BASE + 0x000018)
#define UART0_IBRD              (UART0_BASE + 0x000024)
#define UART0_FBRD              (UART0_BASE + 0x000028)
#define UART0_LCRH              (UART0_BASE + 0x00002c)
#define UART0_CR                (UART0_BASE + 0x000030)
#define UART0_IFLS              (UART0_BASE + 0x000034)
#define UART0_IMSC              (UART0_BASE + 0x000038)
#define UART0_RIS               (UART0_BASE + 0x00003c)
#define UART0_MIS               (UART0_BASE + 0x000040)

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

#define SYSTEM_CLOCK 500000000
#define BAUD_RATE 115200
#define AUX_BAUD(BAUD_RATE) (SYSTEM_CLOCK / ((BAUD_RATE) * 8) - 1)

#define FUARTCLK 48000000 // init_uart_clock
#define BAUDDIV(BAUD_RATE) (((double)(FUARTCLK) / (16 * (BAUD_RATE))))
#define IBRD ((int)(BAUDDIV(BAUD_RATE)))
#define FBRD ((int)(64 * ((BAUDDIV(BAUD_RATE)) - (IBRD)) + 0.5))

char mini_uart_receive_char(void);
void mini_uart_send_char(char c);
void mini_uart_send_string(const char *s);
char uart0_receive_char(void);
void uart0_send_char(char c);
void uart0_send_string(const char *s);
void uart0_send_uint32(unsigned int data);
void uart0_handle(void);
void uart_init(void);

#endif
