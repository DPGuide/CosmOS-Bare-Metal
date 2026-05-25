#include "schneider_lang.h"

/// BARE METAL FIX: Verstecke diesen Code vor dem 32-Bit Compiler!
#ifdef __x86_64__

/// =======================================================
/// BARE METAL PAGING: PAGE TABLE ENTRY FINDEN
/// Navigiert durch PML4 -> PDPT -> PD -> PT
/// =======================================================
uint64_t* find_pte_for_address(uint64_t pml4_phys, uint64_t virtual_addr) {
    uint64_t pml4_idx = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_idx = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_idx   = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_idx   = (virtual_addr >> 12) & 0x1FF;

    uint64_t* pml4 = (uint64_t*)(pml4_phys & ~0xFFFULL);
    _15 ((pml4[pml4_idx] & 1) EQ 0) _96 0;

    uint64_t* pdpt = (uint64_t*)(pml4[pml4_idx] & ~0xFFFULL);
    _15 ((pdpt[pdpt_idx] & 1) EQ 0) _96 0;
    
    _15 (pdpt[pdpt_idx] & 0x80) _96 &pdpt[pdpt_idx];

    uint64_t* pd = (uint64_t*)(pdpt[pdpt_idx] & ~0xFFFULL);
    _15 ((pd[pd_idx] & 1) EQ 0) _96 0;
    
    _15 (pd[pd_idx] & 0x80) _96 &pd[pd_idx];

    uint64_t* pt = (uint64_t*)(pd[pd_idx] & ~0xFFFULL);
    _96 &pt[pt_idx]; 
}

/// =======================================================
/// BARE METAL FIX: NX-BIT FÜR EINEN BEREICH ENTSPERREN
/// =======================================================
extern "C" _50 disable_nx_for_app(uint64_t virtual_addr, uint64_t size_in_bytes) {
    uint64_t cr3_val;
    __asm__ _192("mov %%cr3, %0" : "=r"(cr3_val));
    
    uint64_t start_page = virtual_addr & ~0xFFFULL;
    uint64_t end_page = (virtual_addr + size_in_bytes + 0xFFF) & ~0xFFFULL;

    _39 (uint64_t page = start_page; page < end_page; page += 0x1000) {
        uint64_t* pte = find_pte_for_address(cr3_val, page);
        
        _15 (pte != 0) {
            *pte &= ~(1ULL << 63); 
            __asm__ _192("invlpg (%0)" :: "r"(page) : "memory");
        }
    }
}
/// =======================================================
/// BARE METAL PAGING: DIE MATRIX-ERSCHAFFUNG
/// =======================================================

/// Ein extrem primitiver Bare-Metal Page Allocator. 
/// Er schnappt sich einfach immer die nächsten 4 KB RAM ab der 24 MB Grenze.
extern "C" uint64_t alloc_page() {
    static uint64_t next_free_page = 0x01800000; /// Start bei 24 MB
    uint64_t p = next_free_page;
    next_free_page += 4096;
    /// RAM zwingend nullen, sonst stürzt die CPU wegen Müll in den Tabellen ab!
    _39(int i=0; i<4096; i++) ((char*)p)[i] = 0; 
    _96 p;
}

/// Mapped eine physische Adresse auf eine virtuelle Adresse in einer spezifischen PML4!
extern "C" _50 map_virtual_to_physical(uint64_t pml4_phys, uint64_t v_addr, uint64_t p_addr) {
    uint64_t pml4_idx = (v_addr >> 39) & 0x1FF;
    uint64_t pdpt_idx = (v_addr >> 30) & 0x1FF;
    uint64_t pd_idx   = (v_addr >> 21) & 0x1FF;
    uint64_t pt_idx   = (v_addr >> 12) & 0x1FF;

    uint64_t* pml4 = (uint64_t*)pml4_phys;
    _15 ((pml4[pml4_idx] & 1) EQ 0) {
        pml4[pml4_idx] = alloc_page() | 0x07; /// Present, R/W, User!
    }

    uint64_t* pdpt = (uint64_t*)(pml4[pml4_idx] & ~0xFFFULL);
    _15 ((pdpt[pdpt_idx] & 1) EQ 0) {
        pdpt[pdpt_idx] = alloc_page() | 0x07;
    }

    uint64_t* pd = (uint64_t*)(pdpt[pdpt_idx] & ~0xFFFULL);
    _15 ((pd[pd_idx] & 1) EQ 0) {
        pd[pd_idx] = alloc_page() | 0x07;
    }

    uint64_t* pt = (uint64_t*)(pd[pd_idx] & ~0xFFFULL);
    /// Hier wird das finale Kabel gesteckt! Physische Adresse + Flags
    pt[pt_idx] = (p_addr & ~0xFFFULL) | 0x07; 
}

/// Klont den Kernel-Space und bereitet eine leere Matrix für eine neue App vor!
extern "C" uint64_t create_process_space() {
    uint64_t new_pml4 = alloc_page();
    
    uint64_t current_cr3;
    __asm__ _192("mov %%cr3, %0" : "=r"(current_cr3));
    
    /// Wir kopieren die KOMPLETTE Kernel-Matrix in die neue App-Matrix.
    /// Dadurch laufen deine Interrupts und Syscalls auch dann noch weiter, 
    /// wenn die App gerade rechnet!
    uint64_t* src = (uint64_t*)(current_cr3 & ~0xFFFULL);
    uint64_t* dst = (uint64_t*)new_pml4;
    _39(int i=0; i<512; i++) {
        dst[i] = src[i];
    }
    
    _96 new_pml4;
}

#endif /// Ende des 64-Bit Blocks
