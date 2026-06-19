#ifndef ELF_LOADER_H
#define ELF_LOADER_H



// ELF 64-bit Header
typedef struct {
    uint8_t  e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

// ELF 64-bit Program Header
typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

#define PT_LOAD 1

// BARE METAL FIX: memcpy_elf and memset_elf so we don't rely on stdlib
static void memcpy_elf(void* dest, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (uint64_t i = 0; i < n; i++) d[i] = s[i];
}

static void memset_elf(void* s, uint8_t c, uint64_t n) {
    uint8_t* p = (uint8_t*)s;
    for (uint64_t i = 0; i < n; i++) p[i] = c;
}

// Parses an ELF loaded in RAM, copies PT_LOAD segments to their p_vaddr,
// and returns the entry point. Returns 0 if invalid.
static uint64_t parse_and_load_elf(uint8_t* elf_data) {
    Elf64_Ehdr* header = (Elf64_Ehdr*)elf_data;

    // Check Magic "\x7F ELF"
    if (header->e_ident[0] != 0x7F || 
        header->e_ident[1] != 'E' || 
        header->e_ident[2] != 'L' || 
        header->e_ident[3] != 'F') {
        return 0; // Not an ELF
    }

    // Must be 64-bit (Class 2)
    if (header->e_ident[4] != 2) return 0;

    Elf64_Phdr* phdrs = (Elf64_Phdr*)(elf_data + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (phdrs[i].p_type == PT_LOAD) {
            uint8_t* dest = (uint8_t*)phdrs[i].p_vaddr;
            uint8_t* src = elf_data + phdrs[i].p_offset;
            
            // Copy file data to memory
            if (phdrs[i].p_filesz > 0) {
                memcpy_elf(dest, src, phdrs[i].p_filesz);
            }
            
            // Zero out .bss (remaining memory size after file data)
            if (phdrs[i].p_memsz > phdrs[i].p_filesz) {
                uint64_t bss_size = phdrs[i].p_memsz - phdrs[i].p_filesz;
                memset_elf(dest + phdrs[i].p_filesz, 0, bss_size);
            }
        }
    }

    return header->e_entry;
}

#endif
