#include "common.h"
#include "fb.h"
#include "irq.h"
#include "mail.h"
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
    uart0_send_string("Type: ");
    while (1) {
    }
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

void uart0_send_uint32_test(void)
{
    unsigned int test = 0x1234abcd;
    uart0_send_string("[Test] ");
    uart0_send_uint32(test);
    uart0_send_string("\r\n");
}

int symb_x = XSTART;
int symb_y = YSTART;

void draw_chars_outside_range(void)
{
    draw_char(1921, 200, 'c', 0x0);
    draw_char(400, 1300, 'l', 0x0);
    draw_char(-1, 400, 'b', 0x0);
    draw_char(120, -4, 'c', 0x0);
    draw_char(-5, -4, 'n', 0x0);
}

void draw_chars_stuff(void)
{
   draw_string("[MOS] Init");
   draw_cursor_newline();
}

void cursor_blink(void)
{
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0xf);
    DELAY(200000000);
    draw_block(symb_x+symb_xn, symb_y+symb_yn, 0x0);
    DELAY(200000000);
}

void kernel_main(void)
{
    disable_irq();
    uart_init();
    vector_init();
    boot_log();
    fb_init();
    board_info();
    gic_init();
#if GENERIC == 1
    enable_generic_timer_irq();
    timer_generic_arm(1);
#else // SYSTEM
    enable_timer_irq();
    timer_arm(1000000);
#endif
    enable_uart0_irq();
    __asm__ volatile("dsb sy" ::: "memory");
    enable_irq();
#if 0
    INIT_CPUS;
#else
    uart0_send_uint32(display.width);
    uart0_send_string("\r\n");
    draw_rect(10, 200, 50, 50, 1);
    draw_chars_stuff();
    // draw_chars_outside_range();
    uart0_send_string("Type: ");
    draw_rect_frame(0, 0, 1920, 1200, 1);
    draw_color_band();
    while (1) {
        cursor_blink();
    }
#endif
}
