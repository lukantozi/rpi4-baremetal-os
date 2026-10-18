#include "common.h"
#include "irq.h"
#include "peripherals.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

/* fake uart sync */
#define DELAY(s)                                       \
do {                                                   \
    for (volatile unsigned int i = 0; i < s; i++);     \
} while (0)                                            \

/* before implementing printf */
#define REPORT_EL(el, uart)                                           \
do {                                                                  \
    switch (el) {                                                     \
        case 0: uart##_send_string("[Exception level 0]\r\n"); break; \
        case 1: uart##_send_string("[Exception level 1]\r\n"); break; \
        case 2: uart##_send_string("[Exception level 2]\r\n"); break; \
        case 3: uart##_send_string("[Exception level 3]\r\n"); break; \
    }                                                                 \
} while(0)                                                            \

#define TYPE(uart)                                  \
do {                                                \
    uart##_send_string("Type: ");                   \
    while (1) {                                     \
        uart##_send_char(uart##_receive_char());    \
    }                                               \
} while (0)                                         \

void cpu1(void)
{
    DELAY(100000);
    uart0_send_string("[CPU1 Activated] ");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu2(void)
{
    DELAY(1000000);
    uart0_send_string("[CPU2 Activated] ");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu3(void)
{
    DELAY(10000000);
    uart0_send_string("[CPU3 Activated] ");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
    TYPE(uart0);
}

#define INIT_CPU(N, INSTR)                                                  \
do {                                                                        \
    write_addr64((unsigned long)&spin_cpu##N, (unsigned long)(INSTR));      \
    write_addr64(WAKE_CPU##N, (unsigned long)&cpu123);                      \
} while(0)                                                                  \

#define INIT_CPUS      \
do {                   \
    INIT_CPU(1, cpu1); \
    INIT_CPU(2, cpu2); \
    INIT_CPU(3, cpu3); \
} while (0)            \

void boot_log(void)
{
    int curr_el = get_current_el();
#if UART_SWITCH
    mini_uart_send_string("\r\n");
    mini_uart_send_string("[Mini UART Enabled]\r\n");
    REPORT_EL(curr_el, mini_uart);
    mini_uart_send_string("[Booting]\r\n");
    mini_uart_send_string("Type: ");
#else
    uart0_send_string("\r\n");
    uart0_send_string("[UART0 Enabled]\r\n");
    REPORT_EL(curr_el, uart0);
    uart0_send_string("[CPU0 Activated]\r\n");
    uart0_send_string("[Booting]\r\n");
#endif
}

void kernel_main(void)
{
    uart_init();
    vector_init();
    boot_log();

    enable_timer_irq();
    uart0_send_string("[arming one-shot timer]\r\n");
    timer_arm(1000000);
    __asm__ volatile("dsb sy" ::: "memory");
    enable_irq();
#if 0
    INIT_CPUS;
#else
    TYPE(uart0);
#endif
}
