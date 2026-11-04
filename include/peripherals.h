#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define PBASE 0xfe000000
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

#endif
