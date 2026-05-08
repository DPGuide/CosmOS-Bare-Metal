.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set VIDEO,    1<<2
.set FLAGS,    ALIGN | MEMINFO | VIDEO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0
.long 0     # 0 = set linear graphics mode
.long 800   # width
.long 600   # height
.long 32    # depth (32-bit color)

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB Stack für den Kernel
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    # Stack aufsetzen
    mov $stack_top, %esp
    
    # Parameter für kernel_main auf den Stack pushen (von rechts nach links!)
    push %ebx  # Multiboot Info Pointer (HIER STECKEN DIE VESA INFOS DRIN!)
    push %eax  # Magic Number (0x2BADB002)
    
    # Springe in deinen C++ Code
    call kernel_main
    
    cli
1:  hlt
    jmp 1b
    .section .note.GNU-stack,"",@progbits
	