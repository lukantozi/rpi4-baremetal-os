#ifndef COMMON_H
#define COMMON_H

#ifndef UART_SWITCH
#define UART_SWITCH 0
#endif

#define SYSTEM_CLOCK 500000000
#define BAUD_RATE 115200
#define AUX_BAUD(BAUD_RATE) (SYSTEM_CLOCK / ((BAUD_RATE) * 8) - 1)

#define FUARTCLK 48000000 // init_uart_clock
#define BAUDDIV(BAUD_RATE) (((double)(FUARTCLK) / (16 * (BAUD_RATE))))
#define IBRD ((int)(BAUDDIV(BAUD_RATE)))
#define FBRD ((int)(64 * ((BAUDDIV(BAUD_RATE)) - (IBRD)) + 0.5))

#define WAKE_CPU1 0xe0
#define WAKE_CPU2 0xe8
#define WAKE_CPU3 0xf0

/* from boot.S */
extern void cpu123(void);
extern unsigned long spin_cpu1;
extern unsigned long spin_cpu2;
extern unsigned long spin_cpu3;

/* from entry.S */
extern unsigned long vectors;

#endif
