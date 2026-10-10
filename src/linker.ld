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

    .cpu1stack : {
        stack_start = .;
        . = . + 4096;
        cpu1_stack = .;
    }

    .cpu2stack : {
        . = . + 4096;
        cpu2_stack = .;
    }

    .cpu3stack : {
        . = . + 4096;
        cpu3_stack = .;
    }
}
