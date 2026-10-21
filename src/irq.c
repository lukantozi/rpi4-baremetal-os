#include "utils.h"
#include "peripherals.h"
#include "timer.h"
#include "uart.h"

void enable_timer_irq(void)
{
    unsigned int target = read_reg32(GICD_ITARGETSR24);
    target &= ~(0xff << 8); // clear 15:8
    target |= (1u << 8);    // set bit 8 to point at cpu0

    write_reg32(GICD_ITARGETSR24, target);
    write_reg32(GICD_ISENABLER3,  (1u << GICD_ISENABLER3_BIT));

    unsigned int enabled = read_reg32(GICD_ISENABLER3);

    if (enabled & (1u << GICD_ISENABLER3_BIT))
        uart0_send_string("[System timer] IRQ enable reads 1\r\n");
    else
        uart0_send_string("[System timer] IRQ enable reads 0\r\n");
}

void enable_generic_timer_irq(void)
{
    write_reg32(GICD_ISENABLER0, (1u << GICD_ISENABLER0_BIT));
    // unsigned int enable = read_reg32(GICD_ISENABLER0); 

    // if (enable & (1u << 30))
    //     uart0_send_string("[GICD_ISENABLER0] read bit (30) enabled\r\n");
    // else
    //     uart0_send_string("[GICD_ISENABLER0] read bit (30) not enabled\r\n");
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long addr)
{
    (void)type; (void)esr; (void)addr;
    uart0_send_string("[Invalid Entry] Implement the printf already\r\n");
}

void handle_irq(void)
{
    //uart0_send_string("[entered handle_irq]\r\n");

    unsigned int iar = read_reg32(GICC_IAR);
    unsigned int iid = iar & 0x3ff; // extract 9:0 (interrupt id)

    while (iid < 1020) {
        if (iid == SYS_TIMER_IRQ1)
            timer_handle();
        else if (iid == GENERIC_TIMER_IRQ)
            timer_generic_handle();

        write_reg32(GICC_EOIR, iar); // hey gic, we're done

        iar = read_reg32(GICC_IAR);
        iid = iar & 0x3ff;
    }
}
