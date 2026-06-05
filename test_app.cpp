typedef unsigned long long uint64_t;

uint64_t syscall(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t ret;
    asm volatile(
        "mov %1, %%rax\n"
        "mov %2, %%rdi\n"
        "mov %3, %%rsi\n"
        "mov %4, %%rdx\n"
        "int $0x80\n"
        "mov %%rax, %0\n"
        : "=r" (ret)
        : "r" (sys_num), "r" (arg1), "r" (arg2), "r" (arg3)
        : "rax", "rdi", "rsi", "rdx", "memory"
    );
    return ret;
}

void print(const char* text) {
    syscall(1, (uint64_t)text, 0, 0);
}

extern "C" void _start() {
    print("\n");
    print("==================================\n");
    print("HELLO FROM PACKAGE MANAGER APP!\n");
    print("SUCCESSFULLY LOADED VIA ELF-LOADER!\n");
    print("==================================\n");
    while(1) {
        syscall(0, 0, 0, 0); // yield
    }
}
