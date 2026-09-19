#include "utils.h"

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

void gpio_activate(void)
{
    unsigned int selector;
    // retrieve register to control gpio pins 10-19
    selector = get32(GPFSEL1);

    // clear gpio 14's 3-bit function selector field
    selector &= ~(7u << 12); // clear 14:12 bits
    selector |=  (2u << 12); // set   14:12 bits to: 010

    // clear gpio 15's 3-bit function selector field
    selector &= ~(7u << 15); // clear 17:15 bits
    selector |=  (2u << 15); // set   17:15 bits to: 010

    put32(GPFSEL1, selector);
}

void pup_pdn_resistor_disable(void)
{
    unsigned int selector;
    // retrieve register to control gpio pup pdn resistors
    selector = get32(GPIO_PUP_PDN_CNTRL_REG0);

    // clear pup / pdn resistors for pins: 14 -- 29:28; 15 -- 31:30
    selector &= ~(15u << 28); // set bits 31:28 to 0000

    put32(GPIO_PUP_PDN_CNTRL_REG0, selector);
}

void mini_uart_enable(void)
{
    // enable access to mini uart registers
    put32(AUX_ENABLES, 1);

    // disable t/r while configuring mini uart
    put32(AUX_MU_CNTL_REG, 0);

    // disable t/r interrupts for now
    put32(AUX_MU_IER_REG, 0);

    // enable 8 bit mode
    put32(AUX_MU_LCR_REG, 3);

    // clear both FIFOs
    put32(AUX_MU_IIR_REG, 0xc6);

    // set RTS high to prevent receiver from receriving 0x00
    put32(AUX_MU_MCR_REG, 0);

    // set baud rate to 115200 (needs review)
    put32(AUX_BAUD_REG, 541);

    // enable t/r
    put32(AUX_MU_CNTL_REG, 3);
}

char uart_receive_char(void)
{
    // wait until FIFO holds at least 1 byte
    while (!(get32(AUX_MU_LSR_REG) & 0x01));
    return get32(AUX_MU_IO_REG) & 0xff;
}

void uart_send_char(char c)
{
    // wait until transmit FIFO is empty and transmit is idle
    // (finished shifting out last bit)
    while (!(get32(AUX_MU_LSR_REG) & 0x20));
    put32(AUX_MU_IO_REG, c);
}

void uart_send_string(const char *s) {
    while (*s) {
        uart_send_char(*s++);
    }
}

void uart_init(void)
{
    gpio_activate();
    pup_pdn_resistor_disable();
    mini_uart_enable();
}

void kernel_main(void)
{
    uart_init();
    uart_send_string("\r\n");
    uart_send_string("Booting...\r\n");
    uart_send_string("\r\r\n\n");
    uart_send_string("Still booting...\r\n");
    uart_send_string("Type: ");

    while (1) {
        uart_send_char(uart_receive_char());
    }
}
