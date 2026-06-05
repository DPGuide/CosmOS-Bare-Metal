ENTRY(app_main)
SECTIONS
{
    . = 0x01100000;  /* BARE METAL FIX: Auf 64 Megabyte hochschieben! */
    .text : { *(.text.entry) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss : { *(.bss .bss.*) }
    /DISCARD/ : { *(.eh_frame) *(.comment) *(.note*) }
}
