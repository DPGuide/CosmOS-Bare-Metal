#include <stdint.h>

void* malloc(unsigned long size);
void free(void* ptr);

struct MemoryBlock {
    unsigned long size;
    int is_free;
    MemoryBlock* next;
};

void* my_stbi_malloc(unsigned long size) {
    return malloc(size);
}

void my_stbi_free(void* ptr) {
    free(ptr);
}

void* my_stbi_realloc(void* ptr, unsigned long new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) { free(ptr); return 0; }
    
    MemoryBlock* block = (MemoryBlock*)((unsigned char*)ptr - sizeof(MemoryBlock));
    if (block->size >= new_size) {
        return ptr;
    }
    
    void* new_ptr = malloc(new_size);
    if (!new_ptr) return 0;
    
    unsigned char* d = (unsigned char*)new_ptr;
    unsigned char* s = (unsigned char*)ptr;
    for (unsigned long i = 0; i < block->size; i++) {
        *(volatile unsigned char*)&d[i] = *(volatile unsigned char*)&s[i];
    }
    
    free(ptr);
    return new_ptr;
}



extern "C" int abs(int j) {
    return j < 0 ? -j : j;
}

#define STBI_NO_STDIO
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_SIMD
#define STBI_NO_THREAD_LOCALS
#define STBI_ASSERT(x)
#define STBI_MALLOC my_stbi_malloc
#define STBI_REALLOC my_stbi_realloc
#define STBI_FREE my_stbi_free
#define STBI_MEMCPY memcpy
#define STBI_MEMSET memset

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

extern "C" unsigned char* decode_image(unsigned char* buffer, int len, int* w, int* h, int* comp, int req_comp) {
    return stbi_load_from_memory(buffer, len, w, h, comp, req_comp);
}

extern "C" void free_image(unsigned char* ptr) {
    stbi_image_free(ptr);
}
