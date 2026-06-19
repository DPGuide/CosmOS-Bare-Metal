#include <stdint.h>

// Wir brauchen nur die Größe, "is_free" und "next" sind bei einem Bump-Allocator überflüssig
struct MemoryBlock {
    unsigned long size;
};

extern "C" {
    // Globale Variable für den Offset, damit wir ihn von außen zurücksetzen können
    unsigned long stbi_heap_offset = 0;
    static unsigned char stbi_heap[1024 * 1024 * 15]; // 15 MB für große Bilder!

    // === WICHTIG: RAM WIEDER FREIGEBEN ===
    // Ruf diese Funktion in deinem OS auf, wenn du das Bild-Fenster schließt!
    void reset_image_heap() {
        stbi_heap_offset = 0;
    }

    void* malloc(unsigned long size) {
        // Wir brauchen Platz für die Daten UND den Header davor!
        unsigned long total_size = sizeof(MemoryBlock) + size;

        // Out-of-Memory Check! Verhindert OS-Abstürze
        if (stbi_heap_offset + total_size > sizeof(stbi_heap)) {
            return 0; 
        }

        // Header in den RAM schreiben
        MemoryBlock* block = (MemoryBlock*)&stbi_heap[stbi_heap_offset];
        block->size = size;

        // Offset weiterrücken
        stbi_heap_offset += total_size;

        // Pointer auf den Bereich HINTER dem Header zurückgeben
        return (void*)((unsigned char*)block + sizeof(MemoryBlock));
    }

    void free(void* ptr) {
        // Bei einem Bump-Allocator machen wir hier nichts.
        // Der RAM wird durch reset_image_heap() im Ganzen geleert.
    }
}

// Bare-Metal taugliche Memory-Funktionen (falls dein Compiler sie nicht eh schon linkt)
extern "C" void* my_memcpy(void* dest, const void* src, unsigned long n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (unsigned long i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

extern "C" void* my_memset(void* s, int c, unsigned long n) {
    unsigned char* p = (unsigned char*)s;
    for (unsigned long i = 0; i < n; i++) p[i] = (unsigned char)c;
    return s;
}

extern "C" int abs(int j) {
    return j < 0 ? -j : j;
}

void* my_stbi_realloc(void* ptr, unsigned long new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) { free(ptr); return 0; }
    
    // Jetzt funktioniert dieser Trick, weil malloc den Header wirklich angelegt hat!
    MemoryBlock* block = (MemoryBlock*)((unsigned char*)ptr - sizeof(MemoryBlock));
    if (block->size >= new_size) {
        return ptr;
    }
    
    void* new_ptr = malloc(new_size);
    if (!new_ptr) return 0;
    
    // Daten sicher kopieren
    my_memcpy(new_ptr, ptr, block->size);
    
    free(ptr);
    return new_ptr;
}

// === STB IMAGE KONFIGURATION ===
#define STBI_NO_STDIO
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_SIMD
#define STBI_NO_THREAD_LOCALS
#define STBI_ASSERT(x)
#define STBI_MALLOC malloc
#define STBI_REALLOC my_stbi_realloc
#define STBI_FREE free
#define STBI_MEMCPY my_memcpy
#define STBI_MEMSET my_memset

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

extern "C" unsigned char* decode_image(unsigned char* buffer, int len, int* w, int* h, int* comp, int req_comp) {
    // GANZ WICHTIG: Bevor wir ein NEUES Bild laden, löschen wir den alten Müll aus dem RAM!
    reset_image_heap();
    return stbi_load_from_memory(buffer, len, w, h, comp, req_comp);
}

extern "C" void free_image(unsigned char* ptr) {
    // stbi_image_free ruft intern STBI_FREE auf. Bei uns macht das nichts, 
    // aber es ist sauberer Programmierstil, es trotzdem aufzurufen.
    stbi_image_free(ptr);
}