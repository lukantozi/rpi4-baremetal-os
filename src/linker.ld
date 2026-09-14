SECTIONS
{
    .text.boot : { *(.text.boot) }
    .text      : { *(.text) }
    .rodata    : { *(.rodata) }
    .data      : { *(.data) }
    . = ALIGN(8);
    bss_start = .;
    .bss : { *(.bss*) }
    bss_end = .;
}
