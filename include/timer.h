#ifndef TIMER_H
#define TIMER_H

#include "peripherals.h"

#define SYS_TIMER_REG           (PBASE + 0x003000)
#define SYS_TIMER_CS            (SYS_TIMER_REG)
#define SYS_TIMER_CLO           (SYS_TIMER_REG + 0x04)
#define SYS_TIMER_C1            (SYS_TIMER_REG + 0x10)

unsigned int timer_read_counter(void);
void timer_arm(unsigned int interval);
void timer_handle(void);
void timer_generic_arm(unsigned int seconds);
void timer_generic_handle(void);

#endif
