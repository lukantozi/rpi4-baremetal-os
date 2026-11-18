#include "cpu.h"

#include "term.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

extern void display_show();
extern void term_cursor_blink();

void cpu1(void)
{
    delay(80);
    uart0_send_string("[CPU1] Activated\r\n");
    display_show();
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu2(void)
{
    delay(100);
    uart0_send_string("[CPU2] Activated\r\n");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
}

void cpu3(void)
{
    delay(200);
    uart0_send_string("[CPU3] Activated\r\n");
    int curr_el = get_current_el();
    REPORT_EL(curr_el, uart0);
    uart0_send_string("Type: ");
    display_show();
    while (1) {
        term_cursor_blink();
    }
}
