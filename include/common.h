#ifndef COMMON_H
#define COMMON_H

#ifndef UART_SWITCH
#define UART_SWITCH 0
#endif

#ifndef GENERIC
#define GENERIC 0
#endif

/* from boot.S */
extern void cpu123(void);
extern unsigned long spin_cpu1;
extern unsigned long spin_cpu2;
extern unsigned long spin_cpu3;

/* from entry.S */
extern unsigned long vectors;

#endif
