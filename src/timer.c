#include "peripherals.h"
#include "timer.h"
#include "utils.h"
#include "uart.h"

unsigned int timer_read_counter(void)
{
    return read_reg32(SYS_TIMER_CLO);
}

void timer_arm(unsigned int interval)
{
    write_reg32(SYS_TIMER_CS, (1u << 1));

    unsigned int counter = timer_read_counter();
    write_reg32(SYS_TIMER_C1, (counter + interval));
    uart0_send_string("[System timer] Armed\r\n");
}

void timer_handle(void)
{
    write_reg32(SYS_TIMER_CS, (1u << 1)); // clear match detect and interrupt request
    uart0_send_string("[System timer] Handled\r\n");
}

void timer_test_polling(unsigned int interval)
{
    write_reg32(SYS_TIMER_CS, (1u << 1)); // clear match detect and interrupt request

    unsigned int counter = timer_read_counter();
    write_reg32(SYS_TIMER_C1, (counter + interval));
    while (!(read_reg32(SYS_TIMER_CS) & (1u << 1)));

    unsigned int pend = read_reg32(GICD_ISPENDR3);
    if (pend & (1u << 1))
        uart0_send_string("[GIC] pending\r\n");
    else
        uart0_send_string("[GIC] not pending\r\n");

    write_reg32(SYS_TIMER_CS, (1u << 1));
}

/* generic */
unsigned long timer_generic_read_counter(void)
{
    unsigned long counter;
    __asm__ volatile("mrs %0, cntpct_el0" : "=r" (counter));
    return counter;
}

unsigned long timer_generic_read_frequency(void)
{
    unsigned long frequency;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r" (frequency));
    return frequency;
}

void timer_generic_set_compare(unsigned long deadline)
{
    __asm__ volatile("msr cntp_cval_el0, %0" :: "r" (deadline));
}

unsigned int timer_generic_get_status(void)
{
    unsigned int status;
    __asm__ volatile("mrs %0, cntp_ctl_el0" : "=r" (status));
    return status & (1u << 2);
}

void timer_generic_enable(void)
{
    unsigned long cntpl_enable;
    __asm__ volatile("mrs %0, cntp_ctl_el0" : "=r" (cntpl_enable));
    cntpl_enable &= ~(1u << 1);
    cntpl_enable |= 1;
    __asm__ volatile("msr cntp_ctl_el0, %0" :: "r" (cntpl_enable));
}

void timer_generic_test_counter(unsigned int iterations)
{
    unsigned long counter = timer_generic_read_counter();

    for (unsigned volatile int i = 0; i < iterations; i++);
    unsigned long new_counter = timer_generic_read_counter();
    
    if (new_counter != counter)
        uart0_send_string("[Generic counter] read worked\r\n");
    else
        uart0_send_string("[Generic counter] read failed\r\n");
}

void timer_generic_test_polling(unsigned int interval)
{
    timer_generic_enable();
    unsigned long freq = timer_generic_read_frequency();
    unsigned long deadline = freq * interval;
    unsigned long counter = timer_generic_read_counter();
    uart0_send_string("[Generic timer] setting up\r\n");
    timer_generic_set_compare(deadline + counter);
    while (!timer_generic_get_status());
    uart0_send_string("[Generic timer] interval lasted 1 sec\r\n");
}

void timer_generic_test_frequency(void)
{
    unsigned long freq = timer_generic_read_frequency();
    // printf proving itself more and more useful
    if (freq == 62500000)
        uart0_send_string("[62.5Mhz]\r\n");
    else if (freq > 62500000)
        uart0_send_string("[> 62.5Mhz]\r\n");
    else
        uart0_send_string("[< 62.5Mhz]\r\n");
}

void timer_generic_arm(unsigned int sec)
{
    unsigned long freq = timer_generic_read_frequency();
    unsigned long now = timer_generic_read_counter();
    timer_generic_set_compare(now + freq * sec);
    timer_generic_enable();
    uart0_send_string("[Generic timer] Armed\r\n");
}

void timer_generic_handle(void)
{
    unsigned long cntpl_enable;
    __asm__ volatile("mrs %0, cntp_ctl_el0" : "=r" (cntpl_enable));
    cntpl_enable &= ~1ul;
    __asm__ volatile("msr cntp_ctl_el0, %0" :: "r" (cntpl_enable));
    uart0_send_string("\r\n[Generic timer] IRQ handled\r\n");
    uart0_send_string("Type: ");
}
