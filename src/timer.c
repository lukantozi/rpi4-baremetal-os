#include "peripherals.h"
#include "timer.h"
#include "utils.h"
#include "uart.h"

unsigned int timer_read_counter(void)
{
    return read_reg32(SYS_TIMER_CLO);
}

void timer_test_polling(unsigned int interval)
{
    write_reg32(SYS_TIMER_CS, (1u << 1)); // clear match detect and interrupt request

    unsigned int counter = timer_read_counter();
    write_reg32(SYS_TIMER_C1, (counter + interval));
    while (!(read_reg32(SYS_TIMER_CS) & (1u << 1)));

    unsigned int pend = read_reg32(GICD_ISPENDR3);
    if (pend & (1u << 1))
        uart0_send_string("[GIC pending]\r\n");
    else
        uart0_send_string("[GIC not pending]\r\n");

    write_reg32(SYS_TIMER_CS, (1u << 1));
}

void timer_arm(unsigned int interval)
{
    write_reg32(SYS_TIMER_CS, (1u << 1));

    unsigned int counter = timer_read_counter();
    write_reg32(SYS_TIMER_C1, (counter + interval));
}

void timer_handle(void)
{
    write_reg32(SYS_TIMER_CS, (1u << 1)); // clear match detect and interrupt request
    uart0_send_string("[timer IRQ handled]\r\n");
}
