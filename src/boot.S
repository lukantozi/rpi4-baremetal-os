// following values are derived from AArch64-Reference-Manual

// system control reg EL1 (page 2654)
#define SCTLR_MMU_DISABLED ((3 << 28) | (3 << 22) | (1 << 20) | (1 << 11))

// HCR_EL2, hypervisor config reg EL2 (page 2492)
#define HCR_VALUE (1 << 31)

// SPSR_EL3, saved program status register EL2 (Page 384)
#define SPSR_VALUE (7 << 6 | 5)

// derived values stop here

.section ".text.boot"

    .globl _start
_start:
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, .core 
    b       .wait

.wait:
    wfe
    b       .wait

.core:
    adr     x0, bss_start
    adr     x1, bss_end
    subs    x1, x1, x0
    b.le    .skip_memzero

.memzero:
    str     xzr, [x0], #8
    subs    x1, x1, #8
    b.gt    .memzero

.skip_memzero:
    ldr     x0, =SCTLR_MMU_DISABLED
    msr     sctlr_el1, x0
    ldr     x0, =HCR_VALUE
    msr     hcr_el2, x0
    ldr     x0, =SPSR_VALUE
    msr     spsr_el2, x0
    adr     x0, el1
    msr     elr_el2, x0
    eret

el1:
    mov     sp, 0x400000
    bl      kernel_main
    dsb     sy // fluish memory accesses
    sev

.hang:
    wfe
    b       .hang

    .globl cpu123
cpu123:
    mrs     x1, mpidr_el1
    and     x1, x1, #3            // extract cpu num
    ldr     x2, =stack_start
    lsl     x3, x1, #12           // multiply number of cpu by 4096 (for stack location)
    add     sp, x2, x3            // move from start of the stack by cpu_num*4096
    adr     x4, spin_cpu0
load:
    ldr     x5, [x4, x1, lsl #3]  // load what's on current cpu's address
    cbnz    x5, run               // one-shot address function
    wfe
    b       load
run:
    blr     x5

park:
    wfe
    b       park

    .balign 8
    .globl spin_cpu0
spin_cpu0:
	.quad 0

    .globl spin_cpu1
spin_cpu1:
	.quad 0

    .globl spin_cpu2
spin_cpu2:
	.quad 0

    .globl spin_cpu3
spin_cpu3:
    .quad 0
