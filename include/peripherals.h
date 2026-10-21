#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define PBASE 0xfe000000

#define GPIO_BASE               (PBASE     + 0x200000)
#define GPFSEL1                 (GPIO_BASE + 0x000004)
#define GPFSEL4                 (GPIO_BASE + 0x000010)
#define GPCLR1                  (GPIO_BASE + 0x00002c)
#define GPIO_PUP_PDN_CNTRL_REG0 (GPIO_BASE + 0x0000e4)

#define UART0_BASE              (PBASE      + 0x201000)
#define UART0_DR                UART0_BASE
#define UART0_FR                (UART0_BASE + 0x000018)
#define UART0_IBRD              (UART0_BASE + 0x000024)
#define UART0_FBRD              (UART0_BASE + 0x000028)
#define UART0_LCRH              (UART0_BASE + 0x00002c)
#define UART0_CR                (UART0_BASE + 0x000030)
#define UART0_IMSC              (UART0_BASE + 0x000038)

#define AUX_BASE                (PBASE    + 0x215000)
#define AUX_ENABLES             (AUX_BASE + 0x000004)
#define AUX_MU_IO_REG           (AUX_BASE + 0x000040)
#define AUX_MU_IER_REG          (AUX_BASE + 0x000044)
#define AUX_MU_IIR_REG          (AUX_BASE + 0x000048)
#define AUX_MU_LCR_REG          (AUX_BASE + 0x00004c)
#define AUX_MU_MCR_REG          (AUX_BASE + 0x000050)
#define AUX_MU_LSR_REG          (AUX_BASE + 0x000054)
#define AUX_MU_CNTL_REG         (AUX_BASE + 0x000060)
#define AUX_BAUD_REG            (AUX_BASE + 0x000068)

#define SYS_TIMER_REG           (PBASE + 0x003000)
#define SYS_TIMER_CS            (SYS_TIMER_REG)
#define SYS_TIMER_CLO           (SYS_TIMER_REG + 0x04)
#define SYS_TIMER_C1            (SYS_TIMER_REG + 0x10)

/*
 * arm gic architecture specification (page 93)
 *
 * For interrupt ID m, when DIV and MOD are the integer division and modulo operations:
 *   the corresponding GICD_ISENABLER number, n, is given by n = m DIV 32
 *   the offset of the required GICD_ISENABLER is (0x100 + (4*n))
 *   the bit number of the required Set-enable bit in this register is m MOD 32.
 */
#define GICD_BASE               0xff841000 // bcm2711.dtsi (43-63)
#define GICC_BASE               0xff842000
#define GICD_CTLR               (GICD_BASE)
#define GICC_CTLR               (GICC_BASE)
#define GICC_PMR                (GICC_BASE + 0x004)

#define SYS_TIMER_IRQ1          97
#define GENERIC_TIMER_IRQ       30
#define GICD_ISENABLER3_BIT     (SYS_TIMER_IRQ1 % 32)

#define GICD_ISENABLER0         (GICD_BASE + 0x100)
#define GICD_ISENABLER0_BIT     (GENERIC_TIMER_IRQ % 32)

#define GICD_ISENABLER3         (GICD_BASE + 0x100 + (4 * (SYS_TIMER_IRQ1 / 32)))
#define GICD_ISPENDR3           (GICD_BASE + 0x200 + (4 * (SYS_TIMER_IRQ1 / 32)))
#define GICD_ITARGETSR24        (GICD_BASE + 0x800 + (4 * (SYS_TIMER_IRQ1 / 4)))

#define GICC_IAR                (GICC_BASE + 0xc)
#define GICC_EOIR               (GICC_BASE + 0x10)

#endif
