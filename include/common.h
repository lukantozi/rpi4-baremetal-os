#ifndef COMMON_H
#define COMMON_H

#define SYSTEM_CLOCK 500000000
#define BAUD_RATE 115200
#define AUX_BAUD(BAUD_RATE) (SYSTEM_CLOCK / ((BAUD_RATE) * 8) - 1)

#define FUARTCLK 48000000 // init_uart_clock
#define BAUDDIV(BAUD_RATE) (((double)(FUARTCLK) / (16 * (BAUD_RATE))))
#define IBRD ((int)(BAUDDIV(BAUD_RATE)))
#define FBRD ((int)(64 * ((BAUDDIV(BAUD_RATE)) - (IBRD)) + 0.5))

typedef enum {
    SELECT_UART0 = 0,
    SELECT_UART1 = 1,
} Su;

#endif
