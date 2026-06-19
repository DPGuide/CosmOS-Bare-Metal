#include <stdio.h>
#include <sys/mman.h>

int main() {
    void* p = mmap((void*)0x00100000, 256 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (p == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }
    printf("mmap success at %p\n", p);
    
    // test write to 0x0B500000
    volatile char* test = (char*)0x0B500000;
    *test = 'A';
    printf("Write success! %c\n", *test);
    return 0;
}
