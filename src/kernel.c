#include "common.h"
#include "peripherals.h"
#include "utils.h"

#define UART_SWITCH 0

void gpio_activate(void)
{
    unsigned int selector;
    selector = read_reg32(GPFSEL1); // retrieve register to control gpio pins 10-19
    selector &= ~(7u << 12);   // clear 14:12 bits
    selector &= ~(7u << 15);   // clear 17:15 bits

    // if uart is 1, we want to activate UART1, so we need
    // to flip it to get 010; if it's 0, then we want 1
    // so we obtain 100; not sure why i prefer it over
    // just 4u >> uart
    unsigned int func = 2u << (!(UART_SWITCH));

    // select alternate function 0 or 5 to activate UART0 or UART1 (mini uart)
    selector |= (func << 12);   // set   14:12 bits to: 100 or 010 (gpio 14)
    selector |= (func << 15);   // set   17:15 bits to: 100 or 010 (gpio 15)
    write_reg32(GPFSEL1, selector);
}

void pup_pdn_resistor_disable(void)
{
    unsigned int selector;
    selector = read_reg32(GPIO_PUP_PDN_CNTRL_REG0); // retrieve register to control gpio pup pdn resistors
    selector &= ~(15u << 28);                  // clear pup / pdn resistors for pins: 14 -- 29:28; 15 -- 31:30
    write_reg32(GPIO_PUP_PDN_CNTRL_REG0, selector);
}

void mini_uart_enable(void)
{
    write_reg32(AUX_ENABLES, 1);                      // enable access to mini uart registers
    write_reg32(AUX_MU_CNTL_REG, 0);                  // disable t/r while configuring mini uart
    write_reg32(AUX_MU_IER_REG, 0);                   // disable t/r interrupts for now
    write_reg32(AUX_MU_LCR_REG, 3);                   // enable 8 bit mode
    write_reg32(AUX_MU_IIR_REG, 0xc6);                // clear both FIFOs
    write_reg32(AUX_MU_MCR_REG, 0);                   // set RTS high to prevent receiver from receriving 0x00
    write_reg32(AUX_BAUD_REG, AUX_BAUD(BAUD_RATE));   // set baud rate to 115200
    write_reg32(AUX_MU_CNTL_REG, 3);                  // enable t/r
}

char mini_uart_receive_char(void)
{
    while (!(read_reg32(AUX_MU_LSR_REG) & 0x01)); // wait until FIFO holds at least 1 byte
    return read_reg32(AUX_MU_IO_REG) & 0xff;
}

void mini_uart_send_char(char c)
{
    while (!(read_reg32(AUX_MU_LSR_REG) & 0x20)); // wait until finished shifting out last bit
    write_reg32(AUX_MU_IO_REG, c);
}

void mini_uart_send_string(const char *s) {
    while (*s) {
        mini_uart_send_char(*s++);
    }
}
void uart0_enable(void)
{
    write_reg32(UART0_CR, 0);                              // disable uart0
    write_reg32(UART0_IBRD, IBRD);                         // set integer part of bauddiv
    write_reg32(UART0_FBRD, FBRD);                         // set fraction part of bauddiv
    write_reg32(UART0_LCRH, ((3u << 5) | (1u << 4)));      // set WLEN to 8 bits and enable fifo
    write_reg32(UART0_IMSC, 0);                            // disable interrupts
    write_reg32(UART0_CR, ((1u << 9) | (1u << 8) | 1));    // enable t/r and uart0
}

char uart0_receive_char(void)
{
    while (read_reg32(UART0_FR) & (1u << 4));
    return read_reg32(UART0_DR) & 0xff;
}

void uart0_send_char(char c)
{
    while (read_reg32(UART0_FR) & (1u << 5));
    write_reg32(UART0_DR, c);
}

void uart0_send_string(const char *s) {
    while (*s) {
        uart0_send_char(*s++);
    }
}

void uart_init(void)
{
    gpio_activate();
    pup_pdn_resistor_disable();
#if UART_SWITCH
    mini_uart_enable();
#else
    uart0_enable();
#endif
}

void kernel_main(void)
{
    uart_init();
#if UART_SWITCH
    mini_uart_send_string("\r\n");
    mini_uart_send_string("[Booting]\r\n");
    mini_uart_send_string("[Mini UART Enabled]\r\n");
    mini_uart_send_string("Type: ");
#else
    uart0_send_string("\r\n");
    uart0_send_string("[Booting]\r\n");
    uart0_send_string("[UART0 Enabled]\r\n");
    uart0_send_string("Type: ");
#endif

    while (1) {
#if UART_SWITCH
        mini_uart_send_char(mini_uart_receive_char());
#else
        uart0_send_char(uart0_receive_char());
#endif
    }
}
