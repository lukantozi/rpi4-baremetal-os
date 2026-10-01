#include "common.h"
#include "peripherals.h"
#include "utils.h"

void gpio_activate(void)
{
    unsigned int selector;
    selector = get32(GPFSEL1); // retrieve register to control gpio pins 10-19
    selector &= ~(7u << 12);   // clear 14:12 bits
    selector |=  (2u << 12);   // set   14:12 bits to: 010 (gpio 14)
    selector &= ~(7u << 15);   // clear 17:15 bits
    selector |=  (2u << 15);   // set   17:15 bits to: 010 (gpio 15)
    put32(GPFSEL1, selector);
}

void pup_pdn_resistor_disable(void)
{
    unsigned int selector;
    selector = get32(GPIO_PUP_PDN_CNTRL_REG0); // retrieve register to control gpio pup pdn resistors
    selector &= ~(15u << 28);                  // clear pup / pdn resistors for pins: 14 -- 29:28; 15 -- 31:30
    put32(GPIO_PUP_PDN_CNTRL_REG0, selector);
}

void mini_uart_enable(void)
{
    put32(AUX_ENABLES, 1);                      // enable access to mini uart registers
    put32(AUX_MU_CNTL_REG, 0);                  // disable t/r while configuring mini uart
    put32(AUX_MU_IER_REG, 0);                   // disable t/r interrupts for now
    put32(AUX_MU_LCR_REG, 3);                   // enable 8 bit mode
    put32(AUX_MU_IIR_REG, 0xc6);                // clear both FIFOs
    put32(AUX_MU_MCR_REG, 0);                   // set RTS high to prevent receiver from receriving 0x00
    put32(AUX_BAUD_REG, AUX_BAUD(BAUD_RATE));   // set baud rate to 115200
    put32(AUX_MU_CNTL_REG, 3);                  // enable t/r
}

char uart_receive_char(void)
{
    while (!(get32(AUX_MU_LSR_REG) & 0x01)); // wait until FIFO holds at least 1 byte
    return get32(AUX_MU_IO_REG) & 0xff;
}

void uart_send_char(char c)
{
    while (!(get32(AUX_MU_LSR_REG) & 0x20)); // wait until finished shifting out last bit
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
