#include "common.h"
#include "cpu.h"
#include "fb.h"
#include "font.h"
#include "info.h"
#include "irq.h"
#include "mail.h"
#include "peripherals.h"
#include "rect.h"
#include "shell.h"
#include "term.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

/* before implementing printf */
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
    uart0_send_string("\r\n");
    uart0_send_string("Type: ");
#endif
}

void gic_init(void)
{
    write_reg32(GICC_PMR, 0xffu);                      
    write_reg32(GICD_CTLR, read_reg32(GICD_CTLR) | 1u);
    write_reg32(GICC_CTLR, read_reg32(GICC_CTLR) | 1u);
}

Rect *rect;

void display_show(void)
{
    term_init();
    draw_rect(10, 200, 50, 50, GREEN);
    *rect = create_rect(100, 100, 200, 200, BLUE);
    draw_rect(rect->x_st, rect->y_st, rect->width, rect->height, rect->col);
    draw_rect_frame(0, 0, display.width, display.height, 1);
    draw_color_band();
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
    display_show();
    while (1) {
        term_cursor_blink();
    }
#endif
}
