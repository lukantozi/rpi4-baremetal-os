#ifndef IRQ_H
#define IRQ_H

#include "peripherals.h"

#define SYS_TIMER_IRQ1          97
#define GENERIC_TIMER_IRQ       30
#define UART0_IRQ               153 // 57 (vc uart0) + 96 
#define GICD_ISENABLER3_BIT     (SYS_TIMER_IRQ1 % 32)

#define GICD_ISENABLER0         (GICD_BASE + 0x100)
#define GICD_ITARGETSR24        (GICD_BASE + 0x800 + (4 * (SYS_TIMER_IRQ1 / 4)))
#define GICD_ISENABLER3         (GICD_BASE + 0x100 + (4 * (SYS_TIMER_IRQ1 / 32)))
#define GICD_ISENABLER0_BIT     (GENERIC_TIMER_IRQ % 32)
#define GICD_ISENABLER4         (GICD_BASE + 0x100 + (4 * (UART0_IRQ / 32)))
#define GICD_ITARGETSR38        (GICD_BASE + 0x800 + (4 * (UART0_IRQ / 4)))
#define UART0_IRQ_BIT           (UART0_IRQ % 32)
#define GICD_ISPENDR3           (GICD_BASE + 0x200 + (4 * (SYS_TIMER_IRQ1 / 32)))
#define GICD_ISPENDR4           (GICD_BASE + 0x200 + (4 * (UART0_IRQ / 32)))

#define GICC_IAR                (GICC_BASE + 0xc)
#define GICC_EOIR               (GICC_BASE + 0x10)

void enable_timer_irq(void);
void enable_generic_timer_irq(void);
void enable_uart0_irq(void);

#endif
