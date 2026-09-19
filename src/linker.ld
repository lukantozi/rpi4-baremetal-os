ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text.boot : {
        KEEP(*(.text.boot))
    }

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    . = ALIGN(16);
    bss_start = .;

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(16);
    bss_end = .;
}
