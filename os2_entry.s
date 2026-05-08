.code32
.section .text.entry
.global os2_start
os2_start:
    cli
    
    # Paging ERZWUNGEN ausschalten
    mov %cr0, %eax
    and $0x7FFFFFFF, %eax
    mov %eax, %cr0

    # 1. PAGE TABLES BAUEN (auf 64 MB = 0x4000000)
    mov $0x4000000, %edi
    mov $0x6000, %ecx
    xor %eax, %eax
    rep stosb

    mov $0x4000000, %edi
    movl $0x4001003, (%edi)

    mov $0x4001000, %edi
    movl $0x4002003, 0(%edi)
    movl $0x4003003, 8(%edi)
    movl $0x4004003, 16(%edi)
    movl $0x4005003, 24(%edi)

    mov $0x4002000, %edi
    mov $0x83, %eax
    mov $2048, %ecx
fill_pds:
    movl %eax, (%edi)
    add $8, %edi
    add $0x200000, %eax
    loop fill_pds

    # SSE / XMM REGISTERS EINSCHALTEN
    mov %cr0, %eax
    and $0xFFFB, %ax
    or  $0x0002, %ax
    mov %eax, %cr0

    mov %cr4, %eax
    or $0x0600, %eax
    mov %eax, %cr4

    # 2. 64-BIT SCHALTER UMLEGEN
    mov $0x4000000, %eax
    mov %eax, %cr3

    mov %cr4, %eax
    or $0x20, %eax
    mov %eax, %cr4

    mov $0xC0000080, %ecx
    rdmsr
    or $0x100, %eax
    wrmsr

    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0

    # 3. DER SPRUNG NACH 64-BIT
    lgdt gdt64_ptr
    push $0x08
    push $long_mode_start
    lret

.code64
long_mode_start:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Neuen Stack laden
    movabsq $0x3000000, %rsp   
    mov %rbx, %rdi
    
    # Ab ins C++!
    movabsq $main, %rax
    call *%rax

halt:
    cli
    hlt
    jmp halt

.data
.align 16
gdt64:
    .quad 0
    .quad 0x00209A0000000000
    .quad 0x0000920000000000
gdt64_ptr:
    .word gdt64_ptr - gdt64 - 1
    .quad gdt64
	
.global isr_trap
isr_trap:
    # 0-31: Echte CPU-Exceptions (RAM-Crash, etc.)
    # Hier halten wir die CPU sicher an, damit sie nicht neu startet.
    cli
    hlt
    jmp isr_trap

.global isr_hw
isr_hw:
    # 32-255: Hardware-Interrupts (USB, Maus, Tastatur)
    # Wir retten kurz das A-Register, bestätigen den Empfang beim PIC
    # und springen butterweich in dein OS2 zurück!
    pushq %rax
    movb $0x20, %al
    outb %al, $0x20   # EOI an Master PIC
    outb %al, $0xA0   # EOI an Slave PIC
    popq %rax
    iretq