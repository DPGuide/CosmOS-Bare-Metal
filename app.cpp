/// app.cpp - Kugelsichere Bouncing Box

extern "C" void app_main() {
    long long x = 400;
    long long y = 300;
    long long dx = 1;
    long long dy = 1;
    long long color = 0x00FF00; /// Hacker-Grün!
    
    while(1) {
        /// Pixel zeichnen (Syscall 3)
        /// Die Register werden dem Compiler knallhart vorgegeben, damit er sie nicht wegoptimiert!
        __asm__ volatile("mov $3, %%rax \n mov %0, %%rdi \n mov %1, %%rsi \n mov %2, %%rdx \n int $0x80" : : "r"(x),   "r"(y),   "r"(color) : "rax", "rdi", "rsi", "rdx");
        __asm__ volatile("mov $3, %%rax \n mov %0, %%rdi \n mov %1, %%rsi \n mov %2, %%rdx \n int $0x80" : : "r"(x+1), "r"(y),   "r"(color) : "rax", "rdi", "rsi", "rdx");
        __asm__ volatile("mov $3, %%rax \n mov %0, %%rdi \n mov %1, %%rsi \n mov %2, %%rdx \n int $0x80" : : "r"(x),   "r"(y+1), "r"(color) : "rax", "rdi", "rsi", "rdx");
        __asm__ volatile("mov $3, %%rax \n mov %0, %%rdi \n mov %1, %%rsi \n mov %2, %%rdx \n int $0x80" : : "r"(x+1), "r"(y+1), "r"(color) : "rax", "rdi", "rsi", "rdx");
        
        x += dx;
        y += dy;
        
        /// Wand-Kollision (Fix: Harte Zuweisung verhindert Steckenbleiben)
        if (x <= 10) dx = 1;
        if (x >= 790) dx = -1;
        if (y <= 10) dy = 1;
        if (y >= 590) dy = -1;
        
        /// Kurze Bremse
        for(volatile int i = 0; i < 50000; i++) {}
        
        /// Yield (Syscall 0)
        __asm__ volatile("mov $0, %%rax \n int $0x80" : : : "rax");
    }
}