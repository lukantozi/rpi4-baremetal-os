#include "common.h"
#include "irq.h"
#include "peripherals.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

#define GENERIC 1

/* fake uart sync */
#define DELAY(s)                                       \
do {                                                   \
    for (volatile unsigned int i = 0; i < s; i++);     \
} while (0)                                            \

/* before implementing printf */
#define REPORT_EL(el, uart)                                           \
do {                                                                  \
    switch (el) {                                                     \
        case 0: uart##_send_string("[Current EL] EL0\r\n"); break;    \
        case 1: uart##_send_string("[Current EL] EL1\r\n"); break;    \
        case 2: uart##_send_string("[Current EL] EL2\r\n"); break;    \
        case 3: uart##_send_string("[Current EL] EL3\r\n"); break;    \
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
    uart0_send_string("[CPU1] Activated\r\n");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu2(void)
{
    DELAY(1000000);
    uart0_send_string("[CPU2] Activated\r\n");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu3(void)
{
    DELAY(10000000);
    uart0_send_string("[CPU3] Activated\r\n");
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
    mini_uart_send_string("[UART1] Enabled\r\n");
    REPORT_EL(curr_el, mini_uart);
    mini_uart_send_string("[Kernel] Booting\r\n");
    mini_uart_send_string("Type: ");
#else
    uart0_send_string("\r\n");
    uart0_send_string("[UART0] Enabled\r\n");
    REPORT_EL(curr_el, uart0);
    uart0_send_string("[CPU0] Activated\r\n");
    uart0_send_string("[Kernel] Booting\r\n");
#endif
}

void gic_init(void)
{
    write_reg32(GICC_PMR, 0xffu);                      
    write_reg32(GICD_CTLR, read_reg32(GICD_CTLR) | 1u);
    write_reg32(GICC_CTLR, read_reg32(GICC_CTLR) | 1u);
}

void kernel_main(void)
{
    uart_init();
    vector_init();
    boot_log();
    disable_irq();
#if GENERIC == 1 
    gic_init();
    enable_generic_timer_irq();
    timer_generic_arm(1);
#else // SYSTEM
    enable_timer_irq();
    timer_arm(1000000);
#endif
    __asm__ volatile("dsb sy" ::: "memory");
    enable_irq();
#if 0
    INIT_CPUS;
#else
    TYPE(uart0);
#endif
}
