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
    mov     sp, 0x400000
    bl      kernel_main

.hang:
    wfe
    b       .hang
