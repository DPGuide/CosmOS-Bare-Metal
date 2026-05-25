/// kernel_main.cpp (Cosmos OS V2 - Windows & Live RTC FIXED)
#include "boot_info.h"
#include "schneider_lang.h"
#include "cosmos_partition.h"
#include "arcade.h"
#include <stdint.h>
#include <stddef.h>
// --- GLOBALE BILDSCHIRM-VARIABLEN ---
uint32_t screen_w = 800;
uint32_t screen_h = 600;
uint32_t screen_pitch = 800 * 4; 
uint32_t* fb = nullptr; // Dein echter Framebuffer
uint32_t* bb = (uint32_t*)0x02000000;

bool is_app_running = false;
// --- BARE METAL EXTERNS ---
// Sag der kernel_main.cpp, dass diese Variable in der cosmos_ahci.cpp existiert!
extern uint32_t global_ahci_abar;
struct HBA_PORT; 
extern HBA_PORT* get_active_ahci_port();
extern _44 ahci_read(HBA_PORT* port, _89 startlba, void* target_ram_address);
// disable_nx ist extern "C" - wir nutzen native 64-Bit Typen für saubere Pointer-Casts!
extern "C" void disable_nx_for_app(unsigned long long virtual_addr, unsigned long long size_in_bytes);
/// ==========================================
/// OS2 NATIVE 64-BIT NETWORK SCANNER
/// ==========================================
uint32_t os2_net_bar0 = 0; 
uint16_t os2_net_vendor = 0;
uint16_t os2_net_device = 0;

uint32_t os2_pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    __asm__ volatile("outl %0, %w1" : : "a"(address), "Nd"(0xCF8));
    uint32_t ret;
    __asm__ volatile("inl %w1, %0" : "=a"(ret) : "Nd"(0xCFC));
    return ret;
}

void os2_smart_scan() {
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint32_t slot = 0; slot < 32; slot++) {
            uint32_t vendor_device = os2_pci_read(bus, slot, 0, 0);
            if (vendor_device != 0xFFFFFFFF) {
                uint32_t class_reg = os2_pci_read(bus, slot, 0, 0x08);
                uint8_t class_code = (class_reg >> 24) & 0xFF;
                
                /// 0x02 = Network Controller!
                if (class_code == 0x02) {
                    os2_net_vendor = vendor_device & 0xFFFF;
                    os2_net_device = vendor_device >> 16;
                    os2_net_bar0 = os2_pci_read(bus, slot, 0, 0x10) & 0xFFFFFFF0;
                    return; /// Stoppt beim ersten Netzwerkchip
                }
            }
        }
    }
}
void internal_test_task() {
    while(1) {
        // Direkter Video-RAM Zugriff (0xB8000 ist fast immer gemappt)
        char* vga = (char*)0xB8000;
        vga[0] = 'T'; // 'T' für Task
        vga[1] = 0x0A; // Hellgrün
        
        // Pause, um die CPU nicht zu grillen
        for(volatile int i = 0; i < 5000000; i++) {}
        
        // Abgeben
        __asm__ volatile("int $0x80" : : "a"(0LL)); 
    }
}

/// ==========================================
/// BARE METAL FIX: GLOBALE HARDWARE-TIMER & SCHEDULER
/// ==========================================
volatile uint64_t system_ticks = 0; 
uint64_t last_window_click = 0;

/// HIER WIRD SIE ECHT DEFINIERT (Kein extern mehr!)
int task_quantum = 0; 

/// Freiwillige CPU-Abgabe
void yield() {
    task_quantum = 15; 
    
    /// BARE METAL FIX: sti schließt die Interrupts wieder auf!
    /// So kann der Hardware-Timer uns wecken, auch wenn wir aus 
    /// einem System-Call kommen!
    __asm__ volatile("sti \n\t hlt"); 
}

/// ==========================================
/// DER WORKER TASK
/// ==========================================
void dynamic_task_worker() {
    while(1) {
        /// Dieser Task wird per Klick zur Laufzeit in den RAM geladen!
        volatile int z = 0;
        z++;
        yield(); 
    }
}
/// ==========================================
/// BARE METAL FIX: 64-BIT TASK SCHEDULER & STACKS
/// ==========================================
struct Task {
    uint64_t rsp;          /// 64-Bit Pointer für den Stack
    uint8_t stack[8192];   /// 8 KB eigener Arbeitsspeicher pro Task
    bool active;
};

Task tasks[4];             
int current_task = 0;      
int num_tasks = 1;         

// BARE METAL FIX: Eine Struktur, die exakt den Stack abbildet,
// wie ihn deine pit_isr vor dem iretq erwartet!
// BARE METAL FIX: Die 16 Bytes, die den Triple Fault verursacht haben!
struct InterruptFrame {
    // Exakt die 15 Register deiner Naked-ISR
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    
    // Die 5 Register, die iretq frisst
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));
void sys_idle_task() {
    while(1) {
        // Hier kannst du Dinge tun, die immer laufen sollen,
        // auch wenn keine App geladen ist.
        // Wenn du nichts tun willst:
        __asm__ volatile("hlt"); // CPU schlafen legen, bis ein Interrupt kommt
    }
}

void create_task(void (*entry_point)()) {
    // 1. Timer anhalten, Lebensgefahr!
    __asm__ volatile("cli"); 
    
    if (num_tasks >= 4) {
        __asm__ volatile("sti");
        return;
    }
    
    int id = num_tasks; 
    
    // ==========================================
    // BARE METAL FIX: DER KUGELSICHERE STACK
    // Keine .bss Arrays mehr! Wir nutzen 18 MB (0x01200000)
    // Jeder Task bekommt 1 volles Megabyte Abstand.
    // ==========================================
    uint64_t stack_base = 0x01200000 + (id * 0x100000); 
    stack_base &= ~0xF; // Perfektes 16-Byte Alignment
    
    InterruptFrame* frame = (InterruptFrame*)(stack_base - sizeof(InterruptFrame));
    
    // Alles mit Nullen füllen, um Müll aus dem RAM zu entfernen
    uint8_t* frame_ptr = (uint8_t*)frame;
    for (uint32_t i = 0; i < sizeof(InterruptFrame); i++) {
        frame_ptr[i] = 0;
    }
    
    // 64-Bit Variablen für die Segment-Register (verhindert 16-Bit Müll!)
    uint64_t current_cs = 0, current_ss = 0;
    __asm__ volatile("mov %%cs, %0" : "=r"(current_cs));
    __asm__ volatile("mov %%ss, %0" : "=r"(current_ss));
    
    // Frame befüllen
    frame->rip = (uint64_t)entry_point;
    frame->cs = current_cs;        
    frame->ss = current_ss;        
    
    // BARE METAL FIX: Absolut saubere Flags erzwingen! (IF=1, Reserved=1)
    // Keine zufälligen Bits mehr aus dem Kernel übernehmen.
    frame->rflags = 0x0202;  
    
    // Den Stack-Pointer für das C++ ABI auf -8 ausrichten
    frame->rsp = stack_base - 8; 
    
    // Task eintragen und scharf schalten
    tasks[id].rsp = (uint64_t)frame;
    tasks[id].active = true;
    num_tasks++; 
    
    // 2. Timer wieder an! Auf in die Schlacht!
    __asm__ volatile("sti"); 
}

extern "C" uint64_t schedule(uint64_t old_rsp) {
    system_ticks++; 
    
    tasks[current_task].rsp = old_rsp;
    
    current_task++;
    if (current_task >= num_tasks || !tasks[current_task].active) {
        current_task = 0;
    }
    
    return tasks[current_task].rsp;
}

/// ==========================================
/// BARE METAL FIX: 64-BIT NAKED PIT ISR
/// ==========================================
__attribute__((naked)) void pit_isr() {
    __asm__ volatile(
        "pushq %rax \n" "pushq %rcx \n" "pushq %rdx \n" "pushq %rbx \n"
        "pushq %rbp \n" "pushq %rsi \n" "pushq %rdi \n"
        "pushq %r8 \n" "pushq %r9 \n" "pushq %r10 \n" "pushq %r11 \n"
        "pushq %r12 \n" "pushq %r13 \n" "pushq %r14 \n" "pushq %r15 \n"

        /// BARE METAL FIX: EOI SOFORT SENDEN!
        /// Wir sagen dem PIC, dass wir den Interrupt erhalten haben,
        /// BEVOR wir den Task-Switch durchführen.
        "movb $0x20, %al \n"
        "outb %al, $0x20 \n"

        "movq %rsp, %rdi \n" 
        "andq $-16, %rsp \n" /// 16-Byte Align für C++
        "call schedule \n"   
        "movq %rax, %rsp \n" /// Stack-Wechsel zu Task X

        "popq %r15 \n" "popq %r14 \n" "popq %r13 \n" "popq %r12 \n"
        "popq %r11 \n" "popq %r10 \n" "popq %r9 \n" "popq %r8 \n"
        "popq %rdi \n" "popq %rsi \n" "popq %rbp \n" "popq %rbx \n"
        "popq %rdx \n" "popq %rcx \n" "popq %rax \n"

        "iretq \n"
    );
}
/// ==========================================
/// BARE METAL FIX: SAVE AS & FOLDER GLOBALS
/// ==========================================
extern void run_cosmos_script(char* file_buffer, int file_size);
extern char term_buffer[15][64];
int save_step = 0; 
char save_filename[32] = "NEWFILE"; 
int save_name_idx = 7; 
char new_folder_name[32] = "NEWDIR"; 
int folder_name_idx = 6;
/// ==========================================
/// BARE METAL FIX: USER SPACE RAM (64 KB pro Task)
/// ==========================================
/// aligned(4096) sorgt dafür, dass die Programme sauber auf RAM-Seiten-Grenzen liegen!
uint8_t user_programs[4][65536] __attribute__((aligned(4096)));
/// ==========================================
/// BARE METAL TOOL: STRING CONCAT (DER KLEBER)
/// ==========================================
void str_cat(char* dest, const char* src) {
    /// 1. Gehe zum Ende des ersten Textes (bis zur Null)
    while (*dest) {
        dest++;
    }
    /// 2. Kopiere den zweiten Text genau dort hin
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    /// 3. Setze am Ende die neue Null-Terminierung
    *dest = 0;
}
/// ==========================================
/// BARE METAL FIX: LOKALER PCI-READER (BYPASS LINKER)
/// ==========================================
uint32_t pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    
    /// Direkter CPU-Hardware-Zugriff (Inline Assembler) -> Braucht keine externen Dateien!
    __asm__ volatile("outl %0, %w1" : : "a"(address), "Nd"(0xCF8));
    uint32_t result;
    __asm__ volatile("inl %w1, %0" : "=a"(result) : "Nd"(0xCFC));
    
    return result;
}
struct MirrorEntry {
    uint32_t bus, dev, func;
    uint32_t vendor, device;
    uint64_t bar0;
    char name[32];
};
MirrorEntry mirror_list[32];
int mirror_count = 0;
bool show_oracle = false;

extern _50 pci_scan_all();
extern _44 xhci_bot_read_sectors(_184 slot_id, _43 lba, _89 dest_ram);
extern void xhci_poll_events_and_mouse();
extern void (*usb_mouse_callback)(int, int, int);
extern "C" _50 check_incoming();
extern "C" _50 net_check_link();
extern "C" _50 send_arp_ping();


/// ==========================================
/// BARE METAL FIX: DER UNIVERSAL-LESE-ADAPTER (FINAL)
/// ==========================================
extern int selected_drive_idx; 
extern int ahci_read_sectors(uint32_t lba, uint64_t dest_ram); 
extern _44 xhci_bot_read_sectors(_184 slot_id, uint32_t lba, uint64_t dest_ram);

_44 disk_read_auto(uint32_t lba, uint64_t dest_ram) {
    _39(int i=0; i<512; i++) {
        ((char*)dest_ram)[i] = 0;
    }

    _15(selected_drive_idx == 99) {
        _96 xhci_bot_read_sectors(1, lba, dest_ram);
    } _41 {
        _96 ahci_read_sectors(lba, dest_ram);
    }
}
/// ==========================================
/// BARE METAL FIX: DER UNIVERSAL-SCHREIB-ADAPTER
/// ==========================================
extern int ahci_write_sectors(uint32_t lba, uint64_t src_ram); 
extern _44 xhci_bot_write_sectors(_184 slot_id, uint32_t lba, uint64_t src_ram);

_44 disk_write_auto(uint32_t lba, uint64_t src_ram) {
    _15(selected_drive_idx == 99) {
        _96 xhci_bot_write_sectors(1, lba, src_ram);
    } _41 {
        _96 ahci_write_sectors(lba, src_ram);
    }
}
/// BARE METAL FIX: Den RAM für Texte restlos reinigen!
void mem_set(void* ptr, uint8_t value, uint32_t num) {
    uint8_t* p = (uint8_t*)ptr;
    while (num--) *p++ = value;
}
/// ==========================================
/// 1. HEAP ALLOCATOR (64-BIT GEFIXT)
/// ==========================================
MemoryBlock* heap_head = nullptr;
void init_heap() { heap_head = (MemoryBlock*)0x03000000; heap_head->size = 1024 * 1024 * 32; heap_head->is_free = 1; heap_head->next = nullptr; }
void* malloc(size_t size) { MemoryBlock* curr = heap_head; while (curr != nullptr) { if (curr->is_free == 1 && curr->size >= size) { curr->is_free = 0; return (void*)((uint8_t*)curr + sizeof(MemoryBlock)); } curr = curr->next; } return nullptr; }
void free(void* ptr) { if (ptr == nullptr) return; MemoryBlock* block = (MemoryBlock*)((uint8_t*)ptr - sizeof(MemoryBlock)); block->is_free = 1; }
void* operator new(size_t size) { return malloc(size); }
void* operator new[](size_t size) { return malloc(size); }
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t size) noexcept { free(ptr); }
/// ==========================================
/// 2. BARE METAL PORTS & MOUSE DRIVER
/// ==========================================
inline uint8_t inb(uint16_t port) { uint8_t ret; asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }
inline void outb(uint16_t port, uint8_t val) { asm volatile("outb %0, %1" : : "a"(val), "Nd"(port)); }
inline uint32_t inl(uint16_t port) { uint32_t ret; asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }
inline void outl(uint16_t port, uint32_t val) { asm volatile("outl %0, %1" : : "a"(val), "Nd"(port)); }
inline void outw(uint16_t port, uint16_t val) { asm volatile("outw %0, %1" : : "a"(val), "Nd"(port)); }
_50 mouse_wait(_184 type) { _89 t = 100000; _114(t--) { _15(type EQ 0 AND (inb(0x64)&1)) _96; _15(type EQ 1 AND !(inb(0x64)&2)) _96; } }
_50 mouse_write(_184 w) { mouse_wait(1); outb(0x64, 0xD4); mouse_wait(1); outb(0x60, w); }
_184 mouse_read() { mouse_wait(0); _96 inb(0x60); }
_50 init_mouse() { mouse_wait(1); outb(0x64,0xA8); mouse_wait(1); outb(0x64,0x20); mouse_wait(0); _184 s=inb(0x60)|2; mouse_wait(1); outb(0x64,0x60); mouse_wait(1); outb(0x60,s); mouse_write(0xF6); mouse_read(); mouse_write(0xF4); mouse_read(); }
/// ==========================================
/// 3. REAL-TIME CLOCK (RTC) BARE METAL
/// ==========================================
_43 rtc_h, rtc_m, rtc_day, rtc_mon, rtc_year;
_184 bcd2bin(_184 b) { _96 ((b >> 4) * 10) + (b & 0xF); }
_50 read_rtc() { 
    outb(0x70, 4); rtc_h = bcd2bin(inb(0x71)); 
    outb(0x70, 2); rtc_m = bcd2bin(inb(0x71)); 
    outb(0x70, 7); rtc_day = bcd2bin(inb(0x71)); 
    outb(0x70, 8); rtc_mon = bcd2bin(inb(0x71)); 
    outb(0x70, 9); rtc_year = bcd2bin(inb(0x71)); 
    rtc_h = (rtc_h + 1) % 24; /// Zeitzonen-Korrektur (CET)
}
/// ==========================================
/// 4. ENGINE GLOBALS & DATA STRUCTURES
/// ==========================================
uint32_t frame = 0;
_43 mouse_x = 400, mouse_y = 300; _44 mouse_down = _86, mouse_just_pressed = _86;
bool mouse_right_down = false; // Speichert, ob rechts gedrückt ist
int mouse_sub_x = 40000;
int mouse_sub_y = 30000;
/// Deine einstellbare Sensitivität! 
/// 10 = 1.0 (Normal) | 50 = 5 (Halb) | 100 = 10 (Extrem langsam)
int mouse_sens = 75; /// <-- Versuch es mal mit 30 (0.3) für die Diva!
/// ==========================================
/// BARE METAL FIX: DIE USB-MAUS SCHNITTSTELLE
/// ==========================================
void update_mouse_position(int dx, int dy, int btn) {
    
    /// BARE METAL FIX: Der USB-Werte-Wrap!
    /// Wir zwingen die 0-255 Werte hart in den negativen Bereich (-128 bis +127)
    int real_dx = (signed char)dx;
    int real_dy = (signed char)dy;
    
    /// 1. Die echte Mausbewegung mit Sensitivität multiplizieren
    mouse_sub_x += (real_dx * mouse_sens);
    mouse_sub_y += (real_dy * mouse_sens);
    
    /// 2. Den echten Bildschirm-Pixel berechnen
    int new_x = mouse_sub_x / 100;
    int new_y = mouse_sub_y / 100;

    /// 3. Achsen abriegeln UND Sub-Pixel-Konto zurücksetzen!
    if (new_x < 0) { new_x = 0; mouse_sub_x = 0; }
    if (new_x > 799) { new_x = 799; mouse_sub_x = 799 * 100; }
    
    if (new_y < 0) { new_y = 0; mouse_sub_y = 0; }
    if (new_y > 599) { new_y = 599; mouse_sub_y = 599 * 100; }

    /// 4. An dein System übergeben
    mouse_x = new_x;
    mouse_y = new_y;

    /// 5. Klick-Logik (Links)
    if (btn & 1) { 
        if (!mouse_down) mouse_just_pressed = _128; 
        else mouse_just_pressed = _86;
        mouse_down = _128;
    } else {
        mouse_down = _86;
        mouse_just_pressed = _86;
    }

    // ==========================================
    // BARE METAL FIX: RECHTSKLICK AUSLESEN!
    // ==========================================
    if (btn & 2) {
        mouse_right_down = true;
    } else {
        mouse_right_down = false;
    }
}
_72 _184 m_packet[3]; _72 _43 m_ptr = 0;
_44 galaxy_open = _86; _43 galaxy_expansion = 0;
_43 input_cooldown = 0; _44 click_consumed = _86;
struct Window { _43 id; _30 title[16]; _43 x, y, w, h; _44 open, minimized, fullscreen; _89 color; _30 content[2048]; _43 cursor_pos; };
struct Planet { _43 ang; _43 dist; _30 name[8]; _43 cur_x, cur_y; }; 
struct Star { _43 x, y, z, type, speed; };
_43 win_z[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
Window windows[13]; 
Planet planets[5];
Star stars[200];
_43 drag_win = -1; _43 drag_off_x = 0; _43 drag_off_y = 0; _43 resize_win = -1; _44 z_blocked = _86;
_72 _89 rng_seed = 123456789;
_89 random() { rng_seed = (rng_seed * 1103515245 + 12345) & 0x7FFFFFFF; _96 rng_seed; }
_43 int_sqrt(_43 n) { _43 x=n, y=1; _114(x>y){x=(x+y)/2; y=n/x;} _96 x; }
_71 _43 sin_lut[256] = { 1, 2, 4, 7, 9, 12, 14, 17, 19, 21, 24, 26, 28, 30, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 56, 58, 60, 61, 63, 64, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 79, 80, 81, 81, 82, 82, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 83, 83, 83, 82, 82, 81, 81, 80, 79, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 68, 67, 66, 64, 63, 61, 60, 58, 56, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 30, 28, 26, 24, 21, 19, 17, 14, 12, 9, 7, 4, 2, 1, -1, -2, -4, -7, -9, -12, -14, -17, -19, -21, -24, -26, -28, -30, -33, -35, -37, -39, -41, -43, -45, -47, -49, -51, -53, -55, -56, -58, -60, -61, -63, -64, -66, -67, -68, -70, -71, -72, -73, -74, -75, -76, -77, -78, -79, -79, -80, -81, -81, -82, -82, -83, -83, -83, -84, -84, -84, -84, -84, -84, -84, -83, -83, -83, -82, -82, -81, -81, -80, -79, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -68, -67, -66, -64, -63, -61, -60, -58, -56, -55, -53, -51, -49, -47, -45, -43, -41, -39, -37, -35, -33, -30, -28, -26, -24, -21, -19, -17, -14, -12, -9, -7, -4, -2, -1 };
_43 Cos(_43 a) { _96 sin_lut[(a + 64) % 256]; }
_43 Sin(_43 a) { _96 sin_lut[a % 256]; }
_50 str_cpy(_30* d, _71 _30* s) { _114(*s) *d++ = *s++; *d=0; }
_43 str_len(_71 _30* s) { _43 l=0; _114(*s++)l++; _96 l; }
_44 is_over(_43 mx, _43 my, _43 ox, _43 oy, _43 r) { _96 (mx-ox)*(mx-ox) + (my-oy)*(my-oy) < r*r; }
_44 is_over_rect(_43 mx, _43 my, _43 x, _43 y, _43 w, _43 h) { _96 (mx >= x AND mx <= x+w AND my >= y AND my <= y+h); }
/// ==========================================
/// BARE METAL FIX: HEX-KONVERTER FÜR DEBUGGING
/// ==========================================
void byte_to_hex(unsigned char byte, char* str) {
    const char* hex_digits = "0123456789ABCDEF";
    str[0] = hex_digits[(byte >> 4) & 0x0F];
    str[1] = hex_digits[byte & 0x0F];
    str[2] = 0; /// Null-Terminierung für den String
}
/// TEXT ENGINE HELPER FÜR FENSTER
bool str_equal(const char* s1, const char* s2) {
    while(*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*(const unsigned char*)s1 == *(const unsigned char*)s2);
}
bool str_starts(const char* full, const char* prefix) {
    while(*prefix) { if(*prefix++ != *full++) return false; }
    return true;
}
void print_win(Window* win, const char* text) {
    while (*text && win->cursor_pos < 2000) { win->content[win->cursor_pos++] = *text++; }
    win->content[win->cursor_pos] = 0;
}
void hex_to_str(uint32_t val, char* buf) {
    const char hex_chars[] = "0123456789ABCDEF"; buf[0] = '0'; buf[1] = 'x'; buf[10] = '\0';
    for(int i = 7; i >= 0; i--) { buf[i + 2] = hex_chars[val & 0xF]; val >>= 4; }
}
/// ==========================================
/// BARE METAL FIX: CFS & AHCI GLOBALS
/// ==========================================
uint32_t active_ahci_bar5 = 0;
uint32_t active_sata_port = 0;
uint32_t detected_ports[8]; 
int detected_port_count = 0;
int selected_drive_idx = -1;
/// ==========================================
/// OS2 CFS SYSTEM V2 (With Persistent Folders)
/// ==========================================
struct CFS_DIR_ENTRY { 
    uint8_t type;          /// 1 Byte: 0 = Leer, 1 = Datei, 2 = Ordner
    char filename[11];     /// 11 Byte: Name (8.3 Format)
    uint16_t start_lba;    /// 2 Byte: Start Sektor
    uint16_t file_size;    /// 2 Byte: Dateigröße in KB
    uint8_t parent_idx;    /// 1 Byte: NEU! Wem gehört diese Datei? (255 = Root)
    uint8_t reserved;      /// 1 Byte: Padding, um auf glatte 18 Bytes zu kommen (Alignment)
} __attribute__((packed)); /// Exakt 18 Bytes pro Eintrag!
/// BARE METAL FIX: is_folder Flag wieder hinzugefügt!
struct FileEntry { uint8_t exists; char name[12]; uint16_t size; uint16_t start_lba; uint8_t is_folder; uint8_t parent_idx; };
FileEntry cfs_files[28];
uint32_t active_file_lba = 0;
uint32_t active_file_idx = 0;
/// 255 bedeutet: Wir sind im Root-Verzeichnis (Ganz oben)
uint8_t current_folder_id = 255;
_44 dsk_mgr_opened = _86; /// BARE METAL FIX: Speichert, ob wir im Datei-Explorer sind!
/// NEU: Speicher für die Laufwerksgröße
uint32_t drive_total_gb = 0;
uint32_t drive_used_kb = 0;
bool is_mounted = false;
void int_to_str(uint32_t n, char* s) { if(n==0){s[0]='0';s[1]=0;return;} int i=0; uint32_t t=n; while(t>0){t/=10;i++;} s[i]=0; while(n>0){s[--i]=(n%10)+'0';n/=10;} }
/// ==========================================
/// BARE METAL FIX: 64-BIT KEYBOARD IDT
/// ==========================================
struct IDTEntry { 
    uint16_t offset_low; 
    uint16_t selector; 
    uint8_t ist; 
    uint8_t type_attr; 
    uint16_t offset_mid; 
    uint32_t offset_high; 
    uint32_t zero; 
} __attribute__((packed));

struct IDTPtr { uint16_t limit; uint64_t base; } __attribute__((packed));
IDTEntry idt[256]; IDTPtr idt_ptr;

void set_idt_gate(int n, uint64_t handler, uint8_t flags = 0x8E) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08; // Kernel Code Segment
    idt[n].ist = 0;
    idt[n].type_attr = flags;
    idt[n].offset_mid = (handler >> 16) & 0xFFFF; 
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF; 
    idt[n].zero = 0; 
}
/// BARE METAL FIX: Maske auf 0xFE (11111110). NUR der Timer darf durch!
void remap_pic() { outb(0x20, 0x11); outb(0xA0, 0x11); outb(0x21, 0x20); outb(0xA1, 0x28); outb(0x21, 0x04); outb(0xA1, 0x02); outb(0x21, 0x01); outb(0xA1, 0x01); outb(0x21, 0xFE); outb(0xA1, 0xFF); }
struct interrupt_frame;
int sys_selected_item = 0; 
int sys_max_items = 5; 
uint8_t key_scancode = 0;
bool key_ready = false;

void init_pit(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36); /// Befehl an den PIT: Kanal 0, Lobyte/Hibyte, Modus 3 (Rechteckwelle)
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

__attribute__((interrupt, target("general-regs-only"))) 
void dummy_isr(struct interrupt_frame* frame) { 
    outb(0x20, 0x20); 
}

__attribute__((interrupt, target("general-regs-only"))) 
void keyboard_isr(struct interrupt_frame* frame) { 
    key_scancode = inb(0x60); 
    key_ready = true; 
    outb(0x20, 0x20); 
}
_30 get_ascii_qwertz(_184 sc) {
    _30 k_low[] = { 0,27,'1','2','3','4','5','6','7','8','9','0',0,0,'\b','\t','q','w','e','r','t','z','u','i','o','p',0,0,'\n',0,'a','s','d','f','g','h','j','k','l',0,0,0,0,0,'y','x','c','v','b','n','m',',','.','-',0,0,0,' ' };
    _15 (sc < sizeof(k_low)) _96 k_low[sc]; _96 0;
}
/// ==========================================
/// BARE METAL FIX: AHCI SATA DRIVER (32-BIT)
/// ==========================================
/// Die Hardware-Struktur eines SATA-Ports im Arbeitsspeicher
struct HBA_PORT {
    uint32_t clb, clbu, fb, fbu, is, ie, cmd, res0, tfd, sig, ssts, sctl, serr, sact, ci, sntf, fbs, res1[11], vendor[4];
};
/// Das Haupt-Gehirn des AHCI Controllers
struct HBA_MEM {
    uint32_t cap, ghc, is, pi, vs, ccc_ctl, ccc_pts, em_loc, em_ctl, cap2, bohc;
    uint8_t res[0xA0 - 0x2C];
    uint8_t vendor[0x100 - 0xA0];
    HBA_PORT ports[32];
};
uint32_t pci_read(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    return inl(0xCFC);
}
void pci_write(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset, uint32_t val) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    outl(0xCFC, val);
}
/// ==========================================
/// BARE METAL FIX: KUGELSICHERER AHCI TREIBER
/// ==========================================
struct HBA_CMD_HEADER { uint16_t flags; uint16_t prdtl; uint32_t prdbc; uint32_t ctba; uint32_t ctbau; uint32_t res1[4]; };
struct HBA_PRDT_ENTRY { 
    uint32_t dba; 
    uint32_t dbau; 
    uint32_t res1; /// Hier von rsv0 auf res1 geändert
    uint32_t dbc; 
};
struct HBA_CMD_TBL { uint8_t cfis[64]; uint8_t acmd[16]; uint8_t rsv[48]; HBA_PRDT_ENTRY prdt_entry[1]; };
void ahci_init_port(HBA_PORT* port, int port_no) {
    HBA_MEM* hba = (HBA_MEM*)active_ahci_bar5;
    hba->ghc |= (1 << 31); /// AHCI Global Enable
    hba->is = 0xFFFFFFFF;
    port->cmd &= ~1; int w=0; while((port->cmd & (1<<15)) && w++<100000);
    port->cmd &= ~(1<<4); w=0; while((port->cmd & (1<<14)) && w++<100000);
    uint32_t base = 0x00800000 + (port_no * 0x10000);
    for(int i=0; i<0x10000; i++) ((uint8_t*)base)[i] = 0;
    port->clb = base; port->clbu = 0;
    port->fb = base + 0x400; port->fbu = 0;
    HBA_CMD_HEADER* cmdh = (HBA_CMD_HEADER*)port->clb;
    for(int i=0; i<32; i++) { 
        cmdh[i].prdtl = 1; 
        cmdh[i].ctba = base + 0x1000 + (i * 0x100); 
        cmdh[i].ctbau = 0; 
    }
    port->ie = 0xFFFFFFFF; port->serr = 0xFFFFFFFF; port->is = 0xFFFFFFFF;   
    port->cmd |= (1<<4); port->cmd |= 1; 
}
int ahci_rw(uint32_t lba, uint64_t buffer_addr, int is_write) { 
    if(active_ahci_bar5 == 0) return 0;
    HBA_PORT* port = &((HBA_MEM*)active_ahci_bar5)->ports[active_sata_port];
    port->is = 0xFFFFFFFF; 
    
    /// 64-BIT FIX: Upper (clbu) und Lower (clb) 32-Bit kombinieren!
    uint64_t clb_full = port->clb | ((uint64_t)port->clbu << 32);
    HBA_CMD_HEADER* cmdheader = (HBA_CMD_HEADER*)clb_full;
    
    cmdheader[0].flags = 5 | (is_write ? (1 << 6) : 0) | (1 << 16); 
    cmdheader[0].prdtl = 1;
    cmdheader[0].prdbc = 0;
    
    /// 64-BIT FIX: ctbau und ctba kombinieren!
    uint64_t ctba_full = cmdheader[0].ctba | ((uint64_t)cmdheader[0].ctbau << 32);
    HBA_CMD_TBL* cmdtbl = (HBA_CMD_TBL*)ctba_full;
    
    for(int i=0; i<128; i++) ((uint8_t*)cmdtbl)[i] = 0;
    
    /// 64-BIT FIX: RAM-Adresse in zwei 32-Bit Blöcke zerteilen für dba und dbau
    cmdtbl->prdt_entry[0].dba = (uint32_t)(buffer_addr & 0xFFFFFFFF); 
    cmdtbl->prdt_entry[0].dbau = (uint32_t)((buffer_addr >> 32) & 0xFFFFFFFF);
    cmdtbl->prdt_entry[0].dbc = 511; 
    cmdtbl->prdt_entry[0].res1 = 0; 
    
    uint8_t* fis = (uint8_t*)cmdtbl->cfis;
    fis[0] = 0x27; fis[1] = 0x80; fis[2] = is_write ? 0xCA : 0xC8; 
    fis[4] = lba & 0xFF; fis[5] = (lba >> 8) & 0xFF; fis[6] = (lba >> 16) & 0xFF;
    fis[7] = 0x40 | ((lba >> 24) & 0x0F); fis[12] = 1; 
    
    /// Timeout für 64-Bit CPU Geschwindigkeiten
    uint32_t spin = 0; 
    while((port->tfd & (0x80 | 0x08)) && spin++ < 50000000);
    if(spin >= 50000000) return 0;
    
    port->ci = 1; 
    
    spin = 0;
    while(1) { 
        if((port->ci & 1) == 0) break; 
        if(port->is & (1 << 30)) return 0; 
        if(spin++ > 100000000) return 0; 
    }
    if(port->tfd & 0x01) return 0;
    return 1;
}
/// Namen auf Sectors (Mehrzahl) geändert
int ahci_read_sectors(uint32_t lba, uint64_t buffer_addr) { return ahci_rw(lba, buffer_addr, 0); }
int ahci_write_sectors(uint32_t lba, uint64_t buffer_addr) { return ahci_rw(lba, buffer_addr, 1); }
int ahci_identify(uint32_t buffer_addr) {
    if(active_ahci_bar5 == 0) return 0;
    HBA_PORT* port = &((HBA_MEM*)active_ahci_bar5)->ports[active_sata_port];
    port->is = 0xFFFFFFFF; port->serr = 0xFFFFFFFF;
    HBA_CMD_HEADER* cmdheader = (HBA_CMD_HEADER*)port->clb;
    cmdheader[0].flags = 5; cmdheader[0].prdtl = 1;
    HBA_CMD_TBL* cmdtbl = (HBA_CMD_TBL*)cmdheader[0].ctba;
    for(int i=0; i<80; i++) ((uint8_t*)cmdtbl)[i] = 0;
    cmdtbl->prdt_entry[0].dba = buffer_addr; cmdtbl->prdt_entry[0].dbau = 0;
    cmdtbl->prdt_entry[0].res1 = 0; /// Geändert auf res1
    cmdtbl->prdt_entry[0].dbc = 511;
    uint8_t* fis = (uint8_t*)cmdtbl->cfis;
    fis[0] = 0x27; fis[1] = 0x80; fis[2] = 0xEC; 
    int spin = 0; while((port->tfd & (0x80 | 0x08)) && spin < 1000000) spin++;
    port->ci = 1;
    while(1) { if((port->ci & 1) == 0) break; if(port->is & (1<<30)) return 0; }
    return 1;
}
/// ==========================================
/// BARE METAL FIX: SYSTEM CONTROL & INFO
/// ==========================================
// BARE METAL MAGIC: Compiler-reservierter RAM für den SATA-Chip
__attribute__((aligned(4096))) uint8_t bare_metal_ahci_mem[0x10000];
/// BARE METAL FIX: Brücke zur net.cpp für DHCP!
extern "C" void send_dhcp_discover();
extern _30 ip_address[32];
extern _30 net_mask[32];
extern _30 gateway_ip[32];
char user_name[32] = "COSMOS"; 
char cpu_brand[49] = "SCANNING CPU...";
uint8_t sys_lang = 0; 
uint8_t sys_theme = 0;
/// BARE METAL FIX: Der globale Status-String ist zurück!
char cmd_status[256] = "SYSTEM READY";
/// NEU: Textspeicher für die klickbare Hardware-Liste
char hw_storage[256] = "PRESS TO SCAN";
char hw_net[256]     = "PRESS TO SCAN";
char hw_gpu[256]     = "PRESS TO SCAN";
char hw_usb[256]     = "PRESS TO SCAN";
void get_cpu_brand() { 
    uint32_t a, b, c, d; 
    
    /// BARE METAL FIX: Auch die erste Abfrage muss abgesichert werden!
    __asm__ volatile (
        "pushq %%rbx \n\t"
        "cpuid \n\t"
        "popq %%rbx \n\t"
        : "=a"(a)
        : "0"(0x80000000)
        : "rcx", "rdx"
    );
    
    if(a < 0x80000004) { str_cpy(cpu_brand,"GENERIC X86"); return; } 
    
    char* s = cpu_brand; 
    for(uint32_t i=0x80000002; i<=0x80000004; i++){
        /// BARE METAL FIX: Der kugelsichere 64-Bit CPUID Aufruf!
        __asm__ volatile (
            "pushq %%rbx \n\t"
            "cpuid \n\t"
            "movl %%ebx, %1 \n\t"
            "popq %%rbx \n\t"
            : "=a"(a), "=r"(b), "=c"(c), "=d"(d)
            : "0"(i)
        );
        *(uint32_t*)s=a; s+=4; *(uint32_t*)s=b; s+=4; *(uint32_t*)s=c; s+=4; *(uint32_t*)s=d; s+=4;
    } 
    cpu_brand[48]=0;
    
    int write_idx = 0; int space_count = 0;
    for(int i=0; i<48; i++) {
        char ch = cpu_brand[i]; if(ch == 0) break;
        if(ch >= 'a' && ch <= 'z') ch -= 32; 
        if((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-') {
            cpu_brand[write_idx++] = ch; space_count = 0;
        } else if(ch == ' ' || ch == '@') {
            if(space_count == 0 && write_idx > 0) cpu_brand[write_idx++] = ' ';
            space_count++;
        }
    }
    cpu_brand[write_idx] = 0;
}
void system_reboot() { outb(0x64, 0xFE); }
void system_shutdown() {
    outw(0xB004, 0x2000); /// QEMU / Bochs Power-Off
    outw(0x4004, 0x3400); /// VirtualBox Power-Off
    while(1) asm volatile("cli; hlt"); /// CPU anhalten
}
/// BARE METAL FIX: Echter Hardware-Scanner (Scannt ALLE Funktionen 0-7)
void scan_pci_class(uint8_t target_class, char* out_buf, const char* prefix) {
    for(uint16_t b=0; b<256; b++) {
        for(uint16_t s=0; s<32; s++) {
            for(uint16_t f=0; f<28; f++) { /// <-- WICHTIG: Laptop-Chips liegen oft auf Func 1-7!
                uint32_t vd = pci_read(b,s,f,0);
                if((vd & 0xFFFF) != 0xFFFF) {
                    uint32_t cls = pci_read(b,s,f,8);
                    if(((cls >> 24) & 0xFF) == target_class) {
                        str_cpy(out_buf, prefix);
                        int len = str_len(out_buf);
                        out_buf[len++] = ' '; out_buf[len++] = '[';
                        char hex[12]; hex_to_str(vd, hex);
                        for(int i=0; i<10; i++) out_buf[len++] = hex[i];
                        out_buf[len++] = ']'; out_buf[len] = 0;
                        return; /// Erster Treffer wird genommen!
                    }
                }
            }
        }
    }
    str_cpy(out_buf, "NOT FOUND ON PCI BUS");
}
void scan_pci_drives(Window* dsk_win) {
    print_win(dsk_win, "COSMOS AHCI SCANNER V6\n--------------------------\n");
    detected_port_count = 0; selected_drive_idx = -1;
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint16_t slot = 0; slot < 32; slot++) {
            for (uint16_t func = 0; func < 8; func++) {
                uint32_t vd = pci_read(bus, slot, func, 0);
                if ((vd & 0xFFFF) != 0xFFFF) {
                    uint32_t class_sub = pci_read(bus, slot, func, 8);
                    if (((class_sub >> 24) & 0xFF) == 0x01 && ((class_sub >> 16) & 0xFF) == 0x06) {
                        /// BARE METAL FIX: PCI Bus Mastering zwingend aktivieren!
                        uint32_t cmd = pci_read(bus, slot, func, 0x04);
                        /// Bit 1: Memory Space, Bit 2: Bus Master (DMA) erzwingen
                        pci_write(bus, slot, func, 0x04, cmd | 0x06);
                        uint32_t bar5 = pci_read(bus, slot, func, 0x24) & 0xFFFFFFF0;
                        active_ahci_bar5 = bar5;
                        HBA_MEM* hba = (HBA_MEM*)bar5;
                        for(int i = 0; i < 32; i++) {
                            if(hba->pi & (1 << i)) {
                                uint32_t ssts = hba->ports[i].ssts;
                                if((ssts & 0x0F) == 3 && ((ssts >> 8) & 0x0F) == 1) {
                                    if(detected_port_count < 8) {
                                        detected_ports[detected_port_count++] = i;
                                        print_win(dsk_win, "PORT ");
                                        char p_str[2] = {(char)('0' + i), 0}; print_win(dsk_win, p_str);
                                        /// Echte Hardware-Signatur auslesen!
                                        if(hba->ports[i].sig == 0x00000101) print_win(dsk_win, ": SATA HDD\n");
                                        else if(hba->ports[i].sig == 0xEB140101) print_win(dsk_win, ": CD/DVD ROM\n");
                                        else print_win(dsk_win, ": UNKNOWN\n");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (detected_port_count == 0) print_win(dsk_win, "NO DRIVES FOUND.\n");
    else print_win(dsk_win, "\nSELECT A DRIVE TO MOUNT.\n");
}
_50 focus_window(_43 id) { 
    _43 found_at = -1; 
    _39(_43 i=0; i<13; i++) _15(win_z[i] EQ id) found_at = i; 
    _15(found_at EQ -1) _96;
    _39(_43 i=found_at; i<12; i++) win_z[i] = win_z[i+1]; 
    win_z[12] = id; 
}

/// GANZ OBEN IN DER DATEI (Globaler Speicher für den Cursor)
_43 v_cx = 400;
_43 v_cy = 300;
uint64_t last_click_time = 0;
/// ==========================================
/// BARE METAL FIX: SAFE KEYBOARD & MOUSE POLLING (PS/2 LEGACY)
/// ==========================================
_50 handle_input() { 
    mouse_just_pressed = _86;
    
    /// Wir lesen bis zu 32 Bytes aus dem Puffer, damit Maus und Tastatur sich nicht stauen
    for(int i = 0; i < 32; i++) {
        _184 st = inb(0x64);
        
        /// Ist überhaupt ein Byte im Puffer? (Bit 0)
        if ((st & 1) == 0) break; 
        
        _184 d = inb(0x60);
        
        /// Ist es ein Maus-Byte? (Bit 5 im Status-Register)
        if (st & 0x20) { 
            m_packet[m_ptr] = d;
            
            /// WICHTIGER BARE-METAL SCHUTZ: Das erste Byte eines Maus-Pakets 
            /// muss immer Bit 3 gesetzt haben (Sync-Bit). 
            /// Fehlt es, sind die Daten verrutscht und wir verwerfen das Byte!
            if (m_ptr == 0 && !(d & 0x08)) {
                continue; 
            }
            
            m_ptr++;
            
            /// Wenn wir 3 Bytes gesammelt haben, werten wir das Paket aus
            if (m_ptr == 3) {
                m_ptr = 0;
                
                int dx = m_packet[1];
                int dy = m_packet[2];
                
                /// PS/2 Vorzeichen korrigieren (9-Bit auf 32-Bit Integer erweitern)
                if (m_packet[0] & 0x10) dx |= 0xFFFFFF00;
                if (m_packet[0] & 0x20) dy |= 0xFFFFFF00;
                
                /// Y-Achse invertieren (PS/2 zählt nach oben, unser Bildschirm nach unten)
                dy = -dy;
                
                /// Tasten auswerten (Bit 0 = Links, Bit 1 = Rechts)
                int btn = m_packet[0] & 0x07;
                
                /// Ab in deine Grafik-Engine!
                update_mouse_position(dx, dy, btn);
            }
        } else {
            /// Es ist ein reines Tastatur-Byte!
            key_scancode = d;
            key_ready = _128;
        }
    }
}

/// ==========================================
/// 5. GRAPHICS ENGINE & SHADER 
/// ==========================================
// ==========================================
// COSMOS GL - BARE METAL 3D ENGINE V1
// ==========================================

// 1. STRUKTUREN (Der Raum)
struct Vec3 { float x, y, z; };
struct Vec2 { int x, y; };
// ==========================================
// BARE METAL TRIGONOMETRIE (x87 FPU)
// ==========================================
float bare_sin(float x) {
    float res;
    __asm__ volatile("fsin" : "=t"(res) : "0"(x));
    return res;
}

float bare_cos(float x) {
    float res;
    __asm__ volatile("fcos" : "=t"(res) : "0"(x));
    return res;
}

// ==========================================
// 3D ROTATION (Matrix-Multiplikation Light)
// ==========================================
Vec3 RotateY(Vec3 p, float angle) {
    float s = bare_sin(angle);
    float c = bare_cos(angle);
    return { p.x * c - p.z * s, p.y, p.x * s + p.z * c };
}

Vec3 RotateX(Vec3 p, float angle) {
    float s = bare_sin(angle);
    float c = bare_cos(angle);
    return { p.x, p.y * c - p.z * s, p.y * s + p.z * c };
}

// 2. GLOBALE KAMERA
float camera_fov = 500.0f;     
float camera_z_offset = 600.0f;
// ==========================================
// KAMERA-GEDÄCHTNIS (Außerhalb der Main-Loop!)
// ==========================================
float global_cam_rot_x = 0.0f;
float global_cam_rot_y = 0.0f;
int last_mouse_x = 400; // Startwert (Mitte)
int last_mouse_y = 300; // Startwert (Mitte) 

// 3. PIXEL-ZEICHNER (Mit Hardware-Pitch-Schutz!)
void PutPixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= screen_w || y >= screen_h) return; 
    uint8_t* pixel_addr = (uint8_t*)bb + (y * screen_pitch) + (x * 4);
    *(uint32_t*)pixel_addr = color;
}

// 4. PROJEKTION (3D Welt -> 2D Monitor)
// ==========================================
// BARE METAL 3D PROJEKTION (Aufgerüstet)
// ==========================================
Vec2 Project3D(Vec3 point, int cx, int cy) {
    float z = point.z + camera_z_offset;
    
    // Verhindern, dass Objekte hinter der Kamera gezeichnet werden (Division durch Null / Clipping)
    if (z <= 0.1f) return { -1, -1 }; 
    
    // Die Perspektiven-Division (3D -> 2D)
    float projected_x = (point.x / z) * camera_fov;
    float projected_y = (point.y / z) * camera_fov;
    
    // Wir projizieren es relativ zum übergebenen Zentrum (cx, cy)!
    return { cx + (int)projected_x, cy - (int)projected_y };
}
// ==========================================
// BARE METAL 3D: DER DREIECKS-RASTERIZER
// ==========================================

// ==========================================
// BARE METAL 3D: DER DREIECKS-RASTERIZER (Kugelsicher!)
// ==========================================

// Wir nutzen float für die Mathematik. Das verhindert Integer-Overflows 
// und ignoriert eventuelle 'unsigned' Probleme aus deiner Vec2 Struct!
float EdgeFunction(Vec2 a, Vec2 b, int px, int py) {
    return ((float)px - (float)a.x) * ((float)b.y - (float)a.y) - ((float)py - (float)a.y) * ((float)b.x - (float)a.x);
}

void DrawTriangle(Vec2 v0, Vec2 v1, Vec2 v2, uint32_t color) {
    // Sicheres Casting auf int
    int v0x = (int)v0.x; int v0y = (int)v0.y;
    int v1x = (int)v1.x; int v1y = (int)v1.y;
    int v2x = (int)v2.x; int v2y = (int)v2.y;

    // 1. Bounding Box berechnen
    int min_x = v0x; if (v1x < min_x) min_x = v1x; if (v2x < min_x) min_x = v2x;
    int min_y = v0y; if (v1y < min_y) min_y = v1y; if (v2y < min_y) min_y = v2y;
    int max_x = v0x; if (v1x > max_x) max_x = v1x; if (v2x > max_x) max_x = v2x;
    int max_y = v0y; if (v1y > max_y) max_y = v1y; if (v2y > max_y) max_y = v2y;
    
    // 2. RAM-Schutzpanzer! 
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= screen_w) max_x = screen_w - 1;
    if (max_y >= screen_h) max_y = screen_h - 1;
    
    // 3. Pixel-Schleife feuern!
    for (int y = min_y; y <= max_y; y++) {
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + (y * screen_pitch));
        for (int x = min_x; x <= max_x; x++) {
            
            float w0 = EdgeFunction(v1, v2, x, y);
            float w1 = EdgeFunction(v2, v0, x, y);
            float w2 = EdgeFunction(v0, v1, x, y);
            
            // Wenn der Pixel drin ist, zeichnen!
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                row_ptr[x] = color;
            }
        }
    }
}

void DrawTriangle3D(Vec3 p0, Vec3 p1, Vec3 p2, uint32_t color, int cx, int cy) {
    Vec2 v0 = Project3D(p0, cx, cy);
    Vec2 v1 = Project3D(p1, cx, cy);
    Vec2 v2 = Project3D(p2, cx, cy);
    if (v0.x == -1 || v1.x == -1 || v2.x == -1) return;
    DrawTriangle(v0, v1, v2, color);
}

// 5. 2D-LINIEN-ZEICHNER (Bresenham Algorithmus)
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

// 6. 3D-LINIEN-ZEICHNER (Die Brücke zwischen 3D und 2D)
// ==========================================
// BARE METAL 3D: LINIEN-ZEICHNER (Update)
// ==========================================
void DrawLine3D(Vec3 p1, Vec3 p2, uint32_t color, int cx, int cy) {
    Vec2 v1 = Project3D(p1, cx, cy);
    Vec2 v2 = Project3D(p2, cx, cy);
    
    // Z-Clipping: Wenn ein Punkt hinter der Kamera ist (-1), zeichne die Linie nicht!
    if (v1.x == -1 || v2.x == -1) return;
    
    // Hier ruft er deine 2D DrawLine Funktion auf (die du schon hast)
    DrawLine(v1.x, v1.y, v2.x, v2.y, color); 
}
_50 Put(_43 x, _43 y, _89 c) { _15(x<0 OR x>=800 OR y<0 OR y>=600) _96; bb[y*800+x]=c; }
_50 PutAlpha(_43 x, _43 y, _89 c) { _15(x<0 OR x>=800 OR y<0 OR y>=600) _96; _89 bg = bb[y*800+x]; _89 s1 = ((c & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); bb[y*800+x] = ((s1 & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); }
_50 Swap() {
    // ==========================================
    // BARE METAL TURBO: 64-BIT MEMORY COPY
    // Kopiert 8 Bytes pro Taktzyklus statt nur 1 Byte!
    // ==========================================
    uint64_t* dst = (uint64_t*)fb;
    uint64_t* src = (uint64_t*)bb;
    
    // Gesamtanzahl der 64-Bit Blöcke berechnen
    uint32_t total_blocks = (screen_h * screen_pitch) / 8;
    
    for (uint32_t i = 0; i < total_blocks; i++) {
        dst[i] = src[i];
    }
}
_50 DrawRoundedRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c) { _39(_43 iy=0;iy<rh;iy++)_39(_43 ix=0;ix<rw;ix++){ _44 corn=_86; _15(ix<r AND iy<r AND (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=_128; _15(ix>rw-r AND iy<r AND (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=_128; _15(ix<r AND iy>rh-r AND (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; _15(ix>rw-r AND iy>rh-r AND (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; _15(!corn) Put(x+ix,y+iy,c); } }
_50 DrawGlassRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c) { 
    _43 cr = (c >> 16) & 0xFF; _43 cg = (c >> 8) & 0xFF; _43 cb = c & 0xFF;
    _39(_43 iy=0; iy<rh; iy++) {
        _39(_43 ix=0; ix<rw; ix++) {
            _44 corn=_86; 
            _15(ix<r AND iy<r AND (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=_128; 
            _15(ix>rw-r AND iy<r AND (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=_128; 
            _15(ix<r AND iy>rh-r AND (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; 
            _15(ix>rw-r AND iy>rh-r AND (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128;
            _15(!corn) {
                _43 sx = x + ix; _43 sy = y + iy;
                _15(sx >= 0 AND sx < 800 AND sy >= 0 AND sy < 600) {
                    _89 bg = bb[sy * 800 + sx];
                    _43 bg_r = (bg >> 16) & 0xFF; _43 bg_g = (bg >> 8) & 0xFF; _43 bg_b = bg & 0xFF;
                    _43 f_r = (bg_r + cr) >> 1; _43 f_g = (bg_g + cg) >> 1; _43 f_b = (bg_b + cb) >> 1;
                    bb[sy * 800 + sx] = (f_r << 16) | (f_g << 8) | f_b;
                }
            }
        }
    }
}
_50 DrawChar(_43 x, _43 y, _30 c, _89 col, _44 bold) { 
    _72 _71 _184 f_u[] = { 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22, 0x7F,0x41,0x41,0x22,124, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x09,0x01, 0x3E,0x41,0x49,0x49,0x7A, 0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41, 0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x0C,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E, 0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31, 0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x3F,0x40,0x38,0x40,0x3F, 0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07, 0x61,0x51,0x49,0x45,0x43 };
    _72 _71 _184 f_n[] = { 0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4B,0x31, 0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03, 0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1E };
    _71 _184* ptr = 0;
    _15(c >= 'A' AND c <= 'Z') ptr = &f_u[(c-'A')*5]; _41 _15(c >= '0' AND c <= '9') ptr = &f_n[(c-'0')*5];
    _41 _15(c EQ ':') { _72 _184 s[]={0,0x36,0x36,0,0}; ptr=s; }
    _41 _15(c EQ '.') { _72 _184 s[]={0,0x60,0x60,0,0}; ptr=s; }
    _15(!ptr) _96;
    _89 glow_col = (col < 0x555555) ? 0xFFFFFF : 0x000000;
    _39(_43 m=0;m<5;m++){ 
        _184 l=ptr[m]; 
        _39(_43 n=0;n<7;n++) {
            _15((l>>n)&1) { 
                PutAlpha(x+m-1, y+n-1, glow_col); PutAlpha(x+m, y+n-1, glow_col); PutAlpha(x+m+1, y+n-1, glow_col);
                PutAlpha(x+m-1, y+n,   glow_col);                                 PutAlpha(x+m+1, y+n,   glow_col);
                PutAlpha(x+m-1, y+n+1, glow_col); PutAlpha(x+m, y+n+1, glow_col); PutAlpha(x+m+1, y+n+1, glow_col);
                _15(bold) PutAlpha(x+m+2, y+n, glow_col);
            } 
        } 
    }
    _39(_43 m=0;m<5;m++){ _184 l=ptr[m]; _39(_43 n=0;n<7;n++) _15((l>>n)&1) { Put(x+m, y+n, col); _15(bold) Put(x+m+1, y+n, col); } } 
}
_50 Text(_43 x, _43 y, _71 _30* s, _89 col, _44 bold) { 
    _15(!s) _96; _43 ox = x;
    _114(*s) { 
        _15(*s EQ '\n') { y += 15; x = ox; s++; continue; }
        DrawChar(x,y,*s++,col,bold); x+=(bold?7:6); 
    } 
}
_50 TextC(_43 cp, _43 y, _71 _30* s, _89 col, _44 bold) { _15(!s) _96; _43 l=0; _114(s[l])l++; Text(cp-(l*(bold?7:6))/2, y, s, col, bold); }
_50 DrawAeroCursor(_43 mx, _43 my) {
    _72 _71 _30* c_map[17] = {
        "*", "**", "*.*", "*..*", "*...*", "*....*", "*.....*", "*......*",
        "*.......*", "*........*", "*.........*", "*......****", "*...*..*",
        "*..* *..*", "*.* *..*", "** *..*", "        **"
    };
    _39(_43 y = 0; y < 17; y++) { _43 len = str_len(c_map[y]); _39(_43 x = 0; x < len; x++) _15(c_map[y][x] NEQ ' ') PutAlpha(mx + x + 3, my + y + 4, 0x000000); }
    _39(_43 y = 0; y < 17; y++) { _43 len = str_len(c_map[y]); _39(_43 x = 0; x < len; x++) { _15(c_map[y][x] EQ '*') Put(mx + x, my + y, 0x000000); _41 _15(c_map[y][x] EQ '.') Put(mx + x, my + y, 0xFFFFFF); } }
}
_50 DrawDenseGalaxy(_43 cx, _43 cy, _43 exp) {
    _15 (exp <= 5) _96; _43 max_radius = (400 * exp) / 320; _89 l_seed = 123456; 
    _39(_43 i = 0; i < 8000; i++) {
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; _43 rand_val1 = l_seed % max_radius;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; _43 rand_val2 = l_seed % 30;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; _43 rand_val3 = l_seed % 256; 
        _43 d = rand_val1; _15(d < 50) continue; 
        _43 scatter_x = (l_seed % 7) - 3; l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; _43 scatter_y = (l_seed % 7) - 3;
        _43 angle = 0; _43 intensity_mod = 1;
        /// BARE METAL FIX: frame / 4 ersetzt durch reines frame!
        _15(i % 3 EQ 0) { _43 target_d = 60 + ((d / 50) * 50); d = target_d + (rand_val2 - 15); angle = (rand_val3 - frame + 256) % 256; intensity_mod = 2; }
        /// BARE METAL FIX: frame / 3 ersetzt durch reines frame!
        _41 { d = (d * d) / max_radius; _15(d < 50) d = 50 + (l_seed % 20); angle = (((i % 2) * 128) + (d / 2) - frame + 256) % 256; }
        _43 final_d = d + (rand_val2 - 15); _43 final_a = (angle + (l_seed % 10) - 5 + 256) % 256;
        _43 px = cx + (Cos(final_a) * final_d) / 84 + scatter_x; _43 py = cy + (Sin(final_a) * final_d * 3 / 4) / 84 + scatter_y;
        _15(px < 0 OR px >= 800 OR py < 0 OR py >= 600) continue;
        _43 r = 0, g = 0, b = 0;
        _15(d < 100) { r = 255; g = 180 - d; b = 60; } _41 _15(d < 180) { r = 160 - (d - 100); g = 50; b = 255; } _41 { r = 20; g = 30; b = 255 - (d - 180); }
        _89 bg = bb[py * 800 + px]; _43 bg_r = (bg >> 16) & 0xFF; _43 bg_g = (bg >> 8) & 0xFF; _43 bg_b = bg & 0xFF;
        _43 intensity = 255 - (int_sqrt(rand_val2*rand_val2) * 8); _15(intensity < 0) intensity = 0; intensity = (intensity * intensity_mod) / 2;
        _15(i % 100 EQ 0) { r = 255; g = 255; b = 255; intensity = 255; }
        _43 f_r = bg_r + (r * intensity / 256); _15(f_r > 255) f_r = 255; _43 f_g = bg_g + (g * intensity / 256); _15(f_g > 255) f_g = 255; _43 f_b = bg_b + (b * intensity / 256); _15(f_b > 255) f_b = 255;
        bb[py * 800 + px] = (f_r << 16) | (f_g << 8) | f_b;
    }
}
_50 DrawActiveSun(_43 cx, _43 cy, _43 radius) {
    _43 r2 = radius * radius; 
    _43 glow_radius = radius + 40; // Mehr Platz für lange, spitze Feuerstrahlen!
    _43 glow_r2 = glow_radius * glow_radius;
    
    _43 start_y = -glow_radius; _43 end_y = glow_radius;
    _43 start_x = -glow_radius; _43 end_x = glow_radius;
    
    _15(cy + start_y < 0) start_y = -cy;
    _15(cy + end_y >= screen_h) end_y = screen_h - 1 - cy;
    _15(cx + start_x < 0) start_x = -cx;
    _15(cx + end_x >= screen_w) end_x = screen_w - 1 - cx;

    float time = frame * 0.05f;
    float s_time = bare_sin(time);
    float c_time = bare_cos(time);
    float boil_offset = frame * 0.2f; 

    _39(_43 y = start_y; y <= end_y; y++) {
        _43 screen_y = cy + y;
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + (screen_y * screen_pitch));

        _39(_43 x = start_x; x <= end_x; x++) {
            _43 dist_sq = x*x + y*y; 
            _15(dist_sq <= glow_r2) {
                _43 screen_x = cx + x; 
                uint32_t* pixel = &row_ptr[screen_x]; 

                _15(dist_sq <= r2) {
                    // ==========================================
                    // DER KERN (Drehendes Magma)
                    // ==========================================
                    _43 z = int_sqrt(r2 - dist_sq);
                    float rx = x * c_time - z * s_time;
                    float rz = x * s_time + z * c_time;
                    
                    _43 nz = (z * 255) / radius; 
                    _43 r = nz; 
                    _43 g = (nz * 170) / 255; 
                    _43 b = (nz * 20) / 255;  
                    
                    *pixel = (r << 16) | (g << 8) | b; 
                } _41 {
                    // ==========================================
                    // BARE METAL FIX: RADIALE FEUERSTRAHLEN!
                    // ==========================================
                    _43 dist = int_sqrt(dist_sq);
                    
                    // 1. Demoscene-Trick: "Pseudo-Winkel" (0 bis 8) rasend schnell berechnen!
                    float px = (float)x; float py = (float)y;
                    float ax = (px > 0) ? px : -px;
                    float ay = (py > 0) ? py : -py;
                    float p_angle = 0;
                    
                    if (ax > ay) p_angle = ay / (ax + 0.0001f);
                    else p_angle = 2.0f - ax / (ay + 0.0001f);
                    
                    if (px < 0 && py >= 0) p_angle = 4.0f - p_angle;
                    else if (px < 0 && py < 0) p_angle = 4.0f + p_angle;
                    else if (px >= 0 && py < 0) p_angle = 8.0f - p_angle;
                    
                    // 2. Erzeuge radiale Spikes durch Überlagerung von Sinus-Wellen auf dem Winkel!
                    // p_angle * 12.0f steuert die ANZAHL der Strahlen.
                    float wave1 = bare_sin(p_angle * 12.0f + time * 3.0f);
                    float wave2 = bare_cos(p_angle * 7.0f - boil_offset);
                    float flare = wave1 * wave2; 
                    
                    if (flare < 0) flare = 0; // Spitzen dürfen nur nach außen wachsen, nicht nach innen!
                    
                    // 3. Spikes bis zu 25 Pixel nach außen schießen lassen!
                    _43 dynamic_glow = radius + 2 + (int)(flare * 25.0f);
                    
                    // Sicherheitsnetz gegen CPU-Freezes (Div/0)
                    _15(dynamic_glow <= radius) dynamic_glow = radius + 1; 
                    
                    _15 (dist <= dynamic_glow) {
                        _43 divisor = dynamic_glow - radius;
                        _43 alpha = 255 - ((dist - radius) * 255 / divisor); 
                        alpha = (alpha * alpha) / 255; // Macht die Flammen spitzenmäßig scharf!
                        
                        _15(alpha > 0) {
                            uint32_t bg = *pixel; 
                            
                            // ==========================================
                            // ECHTES FEUER (255 Rot, 120 Grün, 0 Blau)
                            // ==========================================
                            _43 final_r = (255 * alpha + ((bg >> 16) & 0xFF) * (255 - alpha)) / 255; 
                            _43 final_g = (120 * alpha + ((bg >> 8) & 0xFF) * (255 - alpha)) / 255; 
                            _43 final_b = (  0 * alpha + (bg & 0xFF) * (255 - alpha)) / 255;
                            
                            *pixel = (final_r << 16) | (final_g << 8) | final_b;
                        }
                    }
                }
            }
        }
    }
}
// ==========================================
// BARE METAL 3D SOLAR SYSTEM
// ==========================================

// ==========================================
// BARE METAL 3D SOLAR SYSTEM
// ==========================================
struct RenderObj {
    int type;           // 0 = Sonne, 1 = Planet
    Vec3 pos;           // 3D Position im Raum
    float radius;       // Echter Radius
    uint32_t color;     // Grundfarbe
};

void DrawPlanet3D(Vec3 pos, float radius, uint32_t color, int cx, int cy) {
    float z = pos.z + camera_z_offset;
    if (z <= 0.1f) return; // Planet ist hinter der Kamera!

    Vec2 center = Project3D(pos, cx, cy);
    if (center.x == -1) return;
    
    // Die Perspektive: Radius wird kleiner, je weiter weg der Planet ist!
    int screen_r = (int)((radius / z) * camera_fov);
    if (screen_r <= 0) return;

    int start_y = -screen_r; int end_y = screen_r;
    int start_x = -screen_r; int end_x = screen_r;
    
    if (center.y + start_y < 0) start_y = -center.y;
    if (center.y + end_y >= screen_h) end_y = screen_h - 1 - center.y;
    if (center.x + start_x < 0) start_x = -center.x;
    if (center.x + end_x >= screen_w) end_x = screen_w - 1 - center.x;

    int r2 = screen_r * screen_r;
    uint32_t cr = (color >> 16) & 0xFF;
    uint32_t cg = (color >> 8) & 0xFF;
    uint32_t cb = color & 0xFF;

    for (int y = start_y; y <= end_y; y++) {
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + ((center.y + y) * screen_pitch));
        for (int x = start_x; x <= end_x; x++) {
            int dist_sq = x*x + y*y;
            if (dist_sq <= r2) {
                // Simpler Fake-3D-Schatten (Kugel-Wölbung)
                int pz = int_sqrt(r2 - dist_sq);
                float light = (float)pz / (float)screen_r; 
                
                uint32_t final_r = (uint32_t)(cr * light);
                uint32_t final_g = (uint32_t)(cg * light);
                uint32_t final_b = (uint32_t)(cb * light);
                
                row_ptr[center.x + x] = (final_r << 16) | (final_g << 8) | final_b;
            }
        }
    }
}
_50 DrawOrganicPlanet(_43 cx, _43 cy, _43 radius, _89 base_col) {
    _43 r2 = radius * radius; _43 glow_radius = radius + 8; _43 glow_r2 = glow_radius * glow_radius;
    _43 base_r = (base_col >> 16) & 0xFF; _43 base_g = (base_col >> 8) & 0xFF; _43 base_b = base_col & 0xFF;
    _39(_43 y = -glow_radius; y <= glow_radius; y++) {
        _39(_43 x = -glow_radius; x <= glow_radius; x++) {
            _43 dist_sq = x*x + y*y; _43 screen_x = cx + x; _43 screen_y = cy + y;
            _15(screen_x < 0 OR screen_x >= 800 OR screen_y < 0 OR screen_y >= 600) _101;
            _15(dist_sq <= r2) {
                _43 nz = int_sqrt(r2 - dist_sq) * 255 / radius; _43 edge_dist = 255 - nz;
                _43 light_x = x + (radius / 2); _43 light_y = y + (radius / 2); _43 l_dist_sq = light_x*light_x + light_y*light_y; _43 diffuse = 0;
                _15(l_dist_sq < r2) diffuse = 255 - (int_sqrt(l_dist_sq) * 255 / radius);
                _43 noise = (((x + radius) * 17) + ((y + radius) * 31)) % 20; _43 banding = (Sin(((y + radius) * 100) / radius) + 64) / 8;
                _43 r = (base_r * nz) / 255; _43 g = (base_g * nz) / 255; _43 b = (base_b * nz) / 255;
                r += (diffuse * base_r) / 256; g += (diffuse * base_g) / 256; b += (diffuse * base_b) / 256;
                _43 rim = (edge_dist * edge_dist) / 255; r += (rim * base_r) / 512; g += (rim * base_g) / 512; b += (rim * base_b) / 512;
                r = (r * (220 + noise + banding)) / 256; g = (g * (220 + noise + banding)) / 256; b = (b * (220 + noise + banding)) / 256;
                _15(r > 255) r = 255; _15(g > 255) g = 255; _15(b > 255) b = 255;
                bb[screen_y * 800 + screen_x] = (r << 16) | (g << 8) | b;
            } _41 _15 (dist_sq <= glow_r2 AND dist_sq > r2) {
                _43 alpha = 255 - ((int_sqrt(dist_sq) - radius) * 255 / (glow_radius - radius)); alpha = (alpha * alpha) / 255;
                _15(alpha > 0) {
                    _89 bg = bb[screen_y * 800 + screen_x];
                    _43 final_r = (base_r * alpha + ((bg >> 16) & 0xFF) * (255 - alpha)) / 255; _43 final_g = (base_g * alpha + ((bg >> 8) & 0xFF) * (255 - alpha)) / 255; _43 final_b = (base_b * alpha + (bg & 0xFF) * (255 - alpha)) / 255;
                    bb[screen_y * 800 + screen_x] = (final_r << 16) | (final_g << 8) | final_b;
                }
            }
        }
    }
}
// 1. Dein App-Fenster Puffer (mit volatile!)
volatile uint32_t app_window_buffer[2500] = {0}; 
volatile bool app_window_active = false;

// 2. Deine kompilierte App als Hex-Array (Der echte Flummi)
unsigned char app_bin[] = {
  0xf3, 0x0f, 0x1e, 0xfa, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x48,
  0x85, 0xc0, 0x0f, 0x84, 0x0c, 0x01, 0x00, 0x00, 0x41, 0x57, 0x49, 0x89,
  0xc3, 0x41, 0xba, 0x01, 0x00, 0x00, 0x00, 0xbe, 0x05, 0x00, 0x00, 0x00,
  0x41, 0x56, 0x41, 0xb9, 0x05, 0x00, 0x00, 0x00, 0x45, 0x31, 0xf6, 0x48,
  0x8d, 0x88, 0x10, 0x27, 0x00, 0x00, 0x41, 0x55, 0x49, 0xc7, 0xc7, 0xfe,
  0xff, 0xff, 0xff, 0x41, 0x54, 0x55, 0xbd, 0x01, 0x00, 0x00, 0x00, 0x53,
  0x48, 0x89, 0xea, 0x48, 0x83, 0xec, 0x20, 0x90, 0x48, 0x89, 0xcf, 0x4c,
  0x89, 0xd8, 0x4c, 0x29, 0xdf, 0x83, 0xe7, 0x04, 0x74, 0x12, 0x49, 0x8d,
  0x43, 0x04, 0x41, 0xc7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0xc8,
  0x74, 0x18, 0x66, 0x90, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83,
  0xc0, 0x08, 0xc7, 0x40, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0xc8,
  0x75, 0xea, 0x45, 0x6b, 0xe1, 0x32, 0x48, 0x89, 0x34, 0x24, 0x89, 0xf7,
  0x44, 0x89, 0xcd, 0x4c, 0x89, 0x4c, 0x24, 0x08, 0x44, 0x8d, 0x6e, 0x04,
  0x45, 0x8d, 0x41, 0x04, 0x83, 0xff, 0x31, 0x42, 0x8d, 0x34, 0x27, 0x89,
  0xe8, 0x41, 0x0f, 0x96, 0xc1, 0x83, 0xf8, 0x31, 0x77, 0x12, 0x45, 0x84,
  0xc9, 0x74, 0x0d, 0x48, 0x63, 0xde, 0x49, 0x8d, 0x1c, 0x9b, 0xc7, 0x03,
  0x00, 0xff, 0x00, 0x00, 0x83, 0xc0, 0x01, 0x83, 0xc6, 0x32, 0x44, 0x39,
  0xc0, 0x75, 0xde, 0x83, 0xc7, 0x01, 0x44, 0x39, 0xef, 0x75, 0xc9, 0x48,
  0x8b, 0x34, 0x24, 0x4c, 0x8b, 0x4c, 0x24, 0x08, 0x4c, 0x01, 0xd6, 0x49,
  0x01, 0xd1, 0x48, 0x85, 0xf6, 0x7e, 0x5a, 0x48, 0x83, 0xfe, 0x2e, 0x4d,
  0x0f, 0x4d, 0xd7, 0x4d, 0x85, 0xc9, 0x7e, 0x46, 0x49, 0x83, 0xf9, 0x2e,
  0x49, 0x0f, 0x4d, 0xd7, 0xc7, 0x44, 0x24, 0x1c, 0x00, 0x00, 0x00, 0x00,
  0x8b, 0x44, 0x24, 0x1c, 0x3d, 0x4f, 0xc3, 0x00, 0x00, 0x7e, 0x11, 0x4c,
  0x89, 0xf0, 0xcd, 0x80, 0xe9, 0x33, 0xff, 0xff, 0xff, 0x0f, 0x1f, 0x00,
  0xf3, 0x90, 0xeb, 0xfc, 0xf3, 0x90, 0x8b, 0x44, 0x24, 0x1c, 0x83, 0xc0,
  0x01, 0x89, 0x44, 0x24, 0x1c, 0x8b, 0x44, 0x24, 0x1c, 0x3d, 0x4f, 0xc3,
  0x00, 0x00, 0x7e, 0xe8, 0xeb, 0xd5, 0xba, 0x02, 0x00, 0x00, 0x00, 0xeb,
  0xbb, 0x41, 0xba, 0x02, 0x00, 0x00, 0x00, 0xeb, 0xa6
};
void diagnose_ahci_ports() {
    if (global_ahci_abar == 0) {
        str_cpy(cmd_status, "ERR: ABAR IST NULL");
        return;
    }

    volatile uint32_t* abar = (volatile uint32_t*)global_ahci_abar;

    // --- BARE METAL FIX: Nur aufwecken, wenn er WIRKLICH schläft! ---
    // Wir prüfen erst, ob Bit 31 schon gesetzt ist (wie in QEMU).
    if ((abar[0] & (1 << 31)) == 0) {
        abar[0] = abar[0] | (1 << 31); // GHC.AE setzen
        for(volatile int i=0; i<100000; i++); 
    }

    // Jetzt erst lesen wir PI (Offset 0x0C, Index 3)
    uint32_t pi = abar[3]; 
    // ==========================================
    // TEST 1: KÖNNEN WIR DEN CHIP ÜBERHAUPT LESEN?
    // ==========================================
    if (pi == 0 || pi == 0xFFFFFFFF) {
        // Wenn PI 0 ist, lesen wir leeren Arbeitsspeicher!
        // Das bedeutet: Deine 64-Bit Page-Tables blockieren den Zugriff auf die Hardware.
        str_cpy(cmd_status, "ERR: MMIO/PAGING BLOCKIERT (PI=0)");
        return;
    }
    
    int active_ports_count = 0;
    
    // ==========================================
    // TEST 2: CHIP ANTWORTET, ABER WO IST DIE FESTPLATTE?
    // ==========================================
    for (int i = 0; i < 32; i++) {
        if (pi & (1 << i)) { // Mainboard sagt: "Diesen Port gibt es physikalisch"
            active_ports_count++;
            
            volatile uint32_t* port_regs = abar + 64 + (i * 32);
            uint32_t ssts = port_regs[10]; // SATA Status Register
            uint8_t det = ssts & 0x0F;     // Device Detection Bits
            
            if (det == 3) {
                str_cpy(cmd_status, "HDD AUF PORT GEFUNDEN!");
                return; 
            }
        }
    }
    
    // Wenn er hier ankommt, hat er Ports gescannt, aber an keinem hing eine Platte!
    if (active_ports_count > 0) {
        str_cpy(cmd_status, "ERR: PORTS DA, ABER HDD FEHLT!");
    } else {
        str_cpy(cmd_status, "ERR: KEINE PORTS VORHANDEN");
    }
}
unsigned int app_bin_len = 333;
// Unser unzerstörbarer Puffer im Code-Segment
__attribute__((section(".text"), aligned(4096))) uint8_t safe_app_buffer[65536] = {0};

// 1. Die neue Multi-Read Funktion aus cosmos_ahci.cpp anmelden!
extern _44 ahci_read_multi(HBA_PORT* port, _89 startlba, _43 count, _50* target_ram_address);

bool load_and_run_bin(uint32_t start_lba, uint32_t sector_count) {
    if (num_tasks >= 4) return false;
    
    uint64_t target_ram = 0x01100000; 
    uint8_t* ram = (uint8_t*)target_ram;
    
    HBA_MEM* hba = (HBA_MEM*)active_ahci_bar5;
    if (hba == nullptr) return false;

    str_cpy(cmd_status, "LADE FLUMMI...");

    HBA_PORT* port = &hba->ports[active_sata_port];

    for(uint32_t i=0; i < 512 * sector_count; i++) ram[i] = 0;
    
    // Wir lesen DIREKT von LBA 10000. Kein Radar mehr nötig!
    if (ahci_read_multi(port, start_lba, sector_count, (void*)target_ram) == 0) {
        str_cpy(cmd_status, "ERR: LESEFEHLER!");
        return false;
    }
    
    // ==========================================
    // BARE METAL FIX: CPU CACHE & TLB FLUSH
    // Zwingt die echte CPU, den Code zu akzeptieren und verhindert Freezes!
    // ==========================================
    __asm__ volatile("wbinvd" ::: "memory");
    
    disable_nx_for_app(target_ram, 8192); 
    
    __asm__ volatile(
        "mov %%cr3, %%rax \n"
        "mov %%rax, %%cr3 \n"
        ::: "rax", "memory"
    );
    
    // ==========================================
    // SIGNATUR PRÜFEN
    // ==========================================
    if (ram[0] == 0xF3 && ram[1] == 0x0F && ram[2] == 0x1E && ram[3] == 0xFA) {
        
        create_task((void (*)()) target_ram); 
        is_app_running = true; 
        
        str_cpy(cmd_status, "BINGO! FLUMMI LÄUFT!");
        return true;
    }
    
    str_cpy(cmd_status, "ERR: FALSCHE SIGNATUR AUF DISK!");
    return false; 
}
/// ==========================================
/// BARE METAL FIX: CMD Processor & App Toggles
/// ==========================================
/// Forward-Deklarationen (sagen C++, dass diese Dinge existieren)
_50 focus_window(_43 id);
extern char cpu_brand[49];
void system_reboot();
extern void system_init_usb();
extern _43 xhci_bot_get_capacity(_184 slot_id); /// BARE METAL FIX: Das Orakel-Radar für SCSI anmelden!
_50 toggle_app(_43 id) {
    Window* win = &windows[id];
    _15(win->open AND !win->minimized AND win_z[12] EQ win->id) { win->minimized = _128; } 
    _41 { win->open = _128; win->minimized = _86; focus_window(win->id); }
}
extern _44 key_new;
/// ==========================================
/// BARE METAL FIX: SYSTEM CALL DISPATCHER (Wiederhergestellt!)
/// ==========================================
volatile char last_app_key = 0; 

extern "C" uint64_t syscall_dispatcher(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    if (sys_num == 0) { yield(); return 0; }
    else if (sys_num == 1) { print_win(&windows[5], (char*)arg1); return 0; }
    else if (sys_num == 2) { uint64_t key = last_app_key; last_app_key = 0; return key; }
    else if (sys_num == 3) { Put(arg1, arg2, arg3); return 0; }
    
    // ==========================================
    // BARE METAL FIX: Syscall 4 - RAM Fenster an App übergeben!
    // ==========================================
    else if (sys_num == 4) { 
        app_window_active = true; 
        return (uint64_t)(volatile void*)app_window_buffer; 
    }
    
    return 0;
}

__attribute__((naked)) void syscall_isr() {
    __asm__ volatile(
        "pushq %rbx \n" 
        "pushq %rcx \n" "pushq %rdx \n" "pushq %rsi \n" "pushq %rdi \n"
        "pushq %r8 \n" "pushq %r9 \n" "pushq %r10 \n" "pushq %r11 \n"
        
        "movq %rdx, %rcx \n" "movq %rsi, %rdx \n" 
        "movq %rdi, %rsi \n" "movq %rax, %rdi \n" 
        
        /// BARE METAL FIX: Dynamische und absolute 16-Byte Ausrichtung!
        "pushq %rbp \n"         /// Alten Base-Pointer sichern
        "movq %rsp, %rbp \n"    /// Aktuellen Stack retten
        "andq $-16, %rsp \n"    /// Stack gnadenlos auf 16-Byte zwingen!
        
        "call syscall_dispatcher \n"
        
        "movq %rbp, %rsp \n"    /// Stack exakt wiederherstellen
        "popq %rbp \n"          /// Base-Pointer zurückholen
        
        "popq %r11 \n" "popq %r10 \n" "popq %r9 \n" "popq %r8 \n"
        "popq %rdi \n" "popq %rsi \n" "popq %rdx \n" "popq %rcx \n"
        "popq %rbx \n" 
        "iretq \n"
    );
}
/// ==========================================
/// BARE METAL PCI SCANNER (gefixt für OS2!)
/// ==========================================
uint32_t e1000_mmio_base = 0; 

void pci_scan_all() {
    /// Wir nutzen jetzt überall uint32_t, damit der Compiler deine Original-Funktion nimmt!
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint32_t slot = 0; slot < 32; slot++) {
            
            /// Explizite (uint32_t) Casts für die Nullen
            uint32_t vendor_device = pci_read(bus, slot, (uint32_t)0, (uint32_t)0);
            
            if (vendor_device != 0xFFFFFFFF) {
                uint32_t vendor = vendor_device & 0xFFFF;
                uint32_t device = vendor_device >> 16;
                
                /// 8086 = Intel, 100E = E1000
                if (vendor == 0x8086 && device == 0x100E) {
                    
                    /// Hier auch nochmal explizit (uint32_t) angeben
                    uint32_t bar0 = pci_read(bus, slot, (uint32_t)0, (uint32_t)0x10);
                    e1000_mmio_base = bar0 & 0xFFFFFFF0; 
                    return; 
                }
            }
        }
    }
}

/// Brücke zu deinem eigenen DHCP-Code aus OS1
extern void send_dhcp_discover();

void read_mac_address(char* mac_text_buffer) {
    /// 1. Die Register RAL und RAH aus dem Mainboard-Speicher lesen
    uint32_t ral = *(volatile uint32_t*)(e1000_mmio_base + 0x5400);
    uint32_t rah = *(volatile uint32_t*)(e1000_mmio_base + 0x5404);

    /// 2. Die 6 Bytes der MAC-Adresse extrahieren
    uint8_t mac[6];
    mac[0] = ral & 0xFF;
    mac[1] = (ral >> 8) & 0xFF;
    mac[2] = (ral >> 16) & 0xFF;
    mac[3] = (ral >> 24) & 0xFF;
    mac[4] = rah & 0xFF;
    mac[5] = (rah >> 8) & 0xFF;

    /// 3. In einen wunderschönen String formatieren (XX:XX:XX:XX:XX:XX)
    int pos = 0;
    for (int i = 0; i < 6; i++) {
        char hex_byte[3];
        byte_to_hex(mac[i], hex_byte);
        
        mac_text_buffer[pos++] = hex_byte[0];
        mac_text_buffer[pos++] = hex_byte[1];
        
        if (i < 5) mac_text_buffer[pos++] = ':'; /// Doppelpunkt nach jedem Byte, außer am Ende
    }
    mac_text_buffer[pos] = 0; /// Wichtig: String sauber beenden!
}
void dynamic_task_worker(); /// Forward-Deklaration für den Spawner
/// Brücke: Sag dem Compiler, dass diese Funktion weiter unten kommt!
void sleep_ms(uint32_t ms);
char cmd_input_buf[64];
int cmd_input_idx = 0;
void process_cmd(char* input, Window* cmd_win) {
    /// Befehl nochmal auf dem Bildschirm ausgeben (Echo)
    print_win(cmd_win, "C:\\> "); print_win(cmd_win, input); print_win(cmd_win, "\n");
    if(str_equal(input, "CLS")) {
        cmd_win->cursor_pos = 0; cmd_win->content[0] = 0;
    } 
    else if(str_starts(input, "ECHO ")) {
        print_win(cmd_win, input + 5); print_win(cmd_win, "\n");
    } 
    else if(str_equal(input, "DIR")) {
        print_win(cmd_win, "--- CFS DIRECTORY ---\n");
        int count = 0;
        for(int i=0; i<28; i++) {
            if(cfs_files[i].exists) {
                print_win(cmd_win, cfs_files[i].name);
                print_win(cmd_win, "   [FILE]\n");
                count++;
            }
        }
        if(count == 0) print_win(cmd_win, "NO FILES FOUND (MOUNT DRIVE FIRST)\n");
    } 
    else if(str_equal(input, "SYSINFO")) {
        print_win(cmd_win, "OS: COSMOS V2 (32-BIT PROTECTED MODE)\nCPU: ");
        print_win(cmd_win, cpu_brand); print_win(cmd_win, "\n");
    }
	/// ==========================================
    /// DIE VERMISSTEN HARDWARE- & INFO-BEFEHLE
    /// ==========================================
    else if(str_equal(input, "MEM")) {
        print_win(cmd_win, "RAM  : 8192 MB TOTAL\n");
        print_win(cmd_win, "USED : 14 MB (KERNEL)\n");
        print_win(cmd_win, "FREE : 8178 MB AVAILABLE\n");
    }
	/// ==========================================
    /// BARE METAL PARTITIONS-SCANNER (PORT-BRUTEFORCE)
    /// ==========================================
    else if(str_equal(input, "PART")) {
        print_win(cmd_win, "SCANNING ALL SATA PORTS (0-5)...\n");
        int total_found = 0;
        
        /// Wir feuern unseren Scanner auf die ersten 6 Ports ab!
        for (uint8_t port = 0; port < 6; port++) {
            
            /// Unser Scanner aus cosmos_partition.cpp setzt den gpt_partition_count
            /// bei jedem neuen Port-Scan automatisch wieder auf 0!
            scan_partitions(port); 
            
            /// Wenn er Partitionen gefunden hat, wissen wir: Da ist eine Platte!
            if (gpt_partition_count > 0) {
                print_win(cmd_win, "--------------------------------\n");
                print_win(cmd_win, "DRIVE FOUND ON SATA PORT ");
                char portStr[4]; int_to_str(port, portStr); print_win(cmd_win, portStr);
                print_win(cmd_win, "\n");
                
                /// Wir listen alle gefundenen Partitionen dieser Platte auf
                for (int p = 0; p < gpt_partition_count; p++) {
                    print_win(cmd_win, "-> PART ");
                    char numStr[4]; int_to_str(p + 1, numStr); print_win(cmd_win, numStr);
                    print_win(cmd_win, " | START LBA: ");
                    
                    /// Hex-Konvertierung der 64-Bit LBA
                    char lbaStr[15]; 
                    byte_to_hex((gpt_partition_starts[p] >> 24) & 0xFF, lbaStr);
                    byte_to_hex((gpt_partition_starts[p] >> 16) & 0xFF, lbaStr + 2);
                    byte_to_hex((gpt_partition_starts[p] >> 8) & 0xFF, lbaStr + 4);
                    byte_to_hex(gpt_partition_starts[p] & 0xFF, lbaStr + 6);
                    lbaStr[8] = 0;
                    
                    print_win(cmd_win, lbaStr);
                    print_win(cmd_win, "\n");
                    total_found++;
                }
            }
        }
        
        if (total_found == 0) {
            print_win(cmd_win, "ERR: NO PARTITIONS FOUND ON ANY PORT.\n");
        } else {
            print_win(cmd_win, "--------------------------------\n");
        }
    }
    /// ECHTER HARDWARE NET-BEFEHL
    else if(str_equal(input, "NET")) {
        print_win(cmd_win, "ETH0 : INTEL PRO/1000 E1000\n");
        
        /// Echte MAC-Adresse von der Hardware holen!
        char real_mac[20];
        read_mac_address(real_mac);
        
        /// Zusammenbauen und ausdrucken
        print_win(cmd_win, "MAC  : ");
        print_win(cmd_win, real_mac);
        print_win(cmd_win, "\n");
        
        print_win(cmd_win, "STAT : WAITING FOR LINK...\n");
    }
	/// ==========================================
    /// NATIVE 64-BIT NET BEFEHL (ENTSCHÄRFT!)
    /// ==========================================
    else if(str_equal(input, "NET")) {
        if (os2_net_bar0 == 0) {
            print_win(cmd_win, "SCANNING PCI BUS...\n");
            os2_smart_scan(); 
        }
        
        if (os2_net_bar0 == 0) {
            print_win(cmd_win, "ERR  : NO NETWORK CONTROLLER FOUND!\n");
        } else {
            char v_str[5], d_str[5];
            byte_to_hex(os2_net_vendor >> 8, v_str); byte_to_hex(os2_net_vendor & 0xFF, v_str + 2); v_str[4] = 0;
            byte_to_hex(os2_net_device >> 8, d_str); byte_to_hex(os2_net_device & 0xFF, d_str + 2); d_str[4] = 0;
            
            print_win(cmd_win, "NIC  : ID ");
            print_win(cmd_win, v_str); print_win(cmd_win, "-"); print_win(cmd_win, d_str);
            print_win(cmd_win, "\n");

            if (os2_net_vendor == 0x8086 && os2_net_device == 0x100E) {
                print_win(cmd_win, "TYPE : INTEL E1000 (LEGACY)\n");
                /// BARE METAL SICHERHEIT: Der direkte RAM-Zugriff auf 0x5400 ist vorerst deaktiviert!
                print_win(cmd_win, "MAC  : READING DISABLED (TRIPLE FAULT PREVENTION)\n");
            } 
            else if (os2_net_vendor == 0x10EC) {
                print_win(cmd_win, "TYPE : REALTEK CHIPSET\n");
            } 
            else if (os2_net_vendor == 0x8086) {
                print_win(cmd_win, "TYPE : MODERN INTEL CHIPSET\n");
            } 
            else {
                print_win(cmd_win, "TYPE : UNKNOWN ADAPTER\n");
            }
        }
    }
    /// ==========================================
    /// DHCP TRIGGER
    /// ==========================================
    else if(str_equal(input, "DHCP")) {
        if (os2_net_bar0 != 0) {
            print_win(cmd_win, "SENDING DHCP DISCOVER BROADCAST...\n");
            print_win(cmd_win, "WAITING FOR DHCP OFFER...\n");
        } else {
            print_win(cmd_win, "ERR: RUN 'NET' COMMAND FIRST\n");
        }
    }
    /// ==========================================
    /// IP BEFEHL (ENTSCHÄRFT!)
    /// ==========================================
    else if(str_equal(input, "IP")) {
        print_win(cmd_win, "INTERFACE : ETH0\n");
        /// BARE METAL SICHERHEIT: Alle Zugriffe auf externe OS1-Variablen deaktiviert!
        print_win(cmd_win, "IPv4 ADDR : OFFLINE (TRIPLE FAULT PREVENTION)\n");
        print_win(cmd_win, "SUBNET    : OFFLINE\n");
        print_win(cmd_win, "GATEWAY   : OFFLINE\n");
        print_win(cmd_win, "DNS       : 8.8.8.8\n");
    }
    /// ==========================================
    /// UPDATE: HELP BEFEHL
    /// ==========================================
    else if(str_equal(input, "HELP")) {
        print_win(cmd_win, "AVAILABLE COMMANDS:\n");
        print_win(cmd_win, " SYSINFO, MEM, NET, IP, DIR, CLS\n"); /// IP hinzugefügt!
        print_win(cmd_win, " MKDIR [NAME], ECHO [TEXT]\n");
        print_win(cmd_win, " START, RUNAPP, REBOOT\n");
    }
    else if(str_equal(input, "HELP")) {
        print_win(cmd_win, "AVAILABLE COMMANDS:\n");
        print_win(cmd_win, " SYSINFO, MEM, NET, DIR, CLS\n");
        print_win(cmd_win, " MKDIR [NAME], ECHO [TEXT]\n");
        print_win(cmd_win, " START, RUNAPP, REBOOT\n");
    }
    else if(str_starts(input, "MKDIR ")) {
        print_win(cmd_win, "SPINNING UP SATA DRIVE...\n");
        
        uint32_t buf_dir = 0x00901000; 
        
        /// NEU: Nur 2 Parameter (LBA 1002 und Puffer-Adresse)
        ahci_read_sectors(1002, buf_dir);
        
        sleep_ms(50); 
        
        CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
        bool found = false;
        
        for (int i = 0; i < 8; i++) {
            if (dir[i].type == 0) { 
                dir[i].type = 2; 
                dir[i].file_size = 0; 
                dir[i].start_lba = 0;
                
                for(int n=0; n<11; n++) dir[i].filename[n] = 0;
                char* new_name = input + 6; /// Input + 6 überspringt "MKDIR "
                
                for(int n=0; n<10 && new_name[n] != 0 && new_name[n] != '\n' && new_name[n] != '\r'; n++) {
                    dir[i].filename[n] = new_name[n];
                }
                
                /// NEU: Nur 2 Parameter
                ahci_write_sectors(1002, buf_dir);
                
                sleep_ms(50);
                
                print_win(cmd_win, "HDD WRITE OK: "); 
                print_win(cmd_win, dir[i].filename);
                print_win(cmd_win, "\n");
                
                found = true; 
                break;
            }
        }
        if (!found) print_win(cmd_win, "ERROR: SATA ROOT DIR FULL!\n");
    }
    /// ==========================================
    /// NEU: DER TASK-SPAWNER BEFEHL
    /// ==========================================
    else if(str_equal(input, "START")) {
        if (num_tasks < 4) {
            create_task(dynamic_task_worker);
            print_win(cmd_win, "BACKGROUND TASK SPAWNED.\n");
        } else {
            print_win(cmd_win, "ERROR: TASK LIMIT REACHED (4/4).\n");
        }
    }
    /// ==========================================
    /// NEU: DER FESTPLATTEN-LOADER
    /// ==========================================
    else if(str_equal(input, "RUNAPP")) {
        if (num_tasks < 4) {
            // BARE METAL FIX: Wir starten den echten Hex-Code aus dem RAM!
            create_task((void(*)())app_bin);
            
            print_win(cmd_win, "LOADING APP DIRECTLY FROM KERNEL RAM...\n");
        } else {
            print_win(cmd_win, "ERROR: TASK LIMIT REACHED.\n");
        }
    }
    /// ==========================================
    else if(str_equal(input, "REBOOT")) {
        system_reboot();
    } 
    else if(input[0] != 0) {
        print_win(cmd_win, "UNKNOWN COMMAND OR BAD SYNTAX.\n");
    }
}
void sleep_ms(uint32_t ms) {
    uint64_t target_ticks = system_ticks + ms;
    
    while (system_ticks < target_ticks) {
        /// Legt die CPU physisch schlafen. Sie wacht nur für eine Mikrosekunde auf, 
        /// wenn der PIT (oder die Maus) einen Interrupt abfeuert, prüft die Zeit 
        /// und schläft sofort weiter!
        __asm__ volatile("hlt"); 
    }
}
void background_task() {
    while(1) {
        // Unsichtbare Aufgaben (später für Netzwerk, etc.)
        volatile int x = 0;
        x++;
        
        // Unbedingt abgeben, sonst frisst er 100% CPU!
        __asm__ volatile("int $0x80" : : "a"(0LL)); 
    }
}
// --- GANZ OBEN IN DER DATEI (Außerhalb von main!) ---

extern uint32_t global_ahci_abar;

void bare_metal_port_init(int port_no) {
    if (port_no < 0) return;
    volatile uint32_t* abar = (volatile uint32_t*)global_ahci_abar;
    volatile uint32_t* port_regs = abar + 64 + (port_no * 32); 
    
    // 1. Motor aus
    port_regs[6] &= ~0x00000001; 
    port_regs[6] &= ~0x00000010; 
    
    // ANTI-FREEZE: Notausgang nach 1.000.000 Zyklen!
    int timeout = 1000000;
    while ((port_regs[6] & (1 << 14)) && timeout--) { __asm__ volatile("pause"); }
    timeout = 1000000;
    while ((port_regs[6] & (1 << 15)) && timeout--) { __asm__ volatile("pause"); }

    str_cpy(cmd_status, "PORT REBASE (64-BIT RAM)!");

    // 2. WICHTIG: 64-Bit Variable nutzen
    uint64_t base_addr = (uint64_t)&bare_metal_ahci_mem[0];
    
    // RAM komplett säubern
    volatile char* mem = (volatile char*)base_addr;
    for (int i = 0; i < 0x10000; i++) mem[i] = 0; 

    // 3. Pointer berechnen
    uint64_t clb = base_addr;
    uint64_t fb  = base_addr + 0x1000;
    uint64_t ctb = base_addr + 0x2000;
    
    // 4. BARE METAL MAGIC: Die 64-Bit Adresse in Low und High aufteilen
    port_regs[0] = (uint32_t)(clb & 0xFFFFFFFF); 
    port_regs[1] = (uint32_t)(clb >> 32);        
    
    port_regs[2] = (uint32_t)(fb & 0xFFFFFFFF);  
    port_regs[3] = (uint32_t)(fb >> 32);         

    volatile uint32_t* cmd_list = (volatile uint32_t*)clb;
    cmd_list[0] = (1 << 16) | 5; 
    cmd_list[1] = 0;
    cmd_list[2] = (uint32_t)(ctb & 0xFFFFFFFF); 
    cmd_list[3] = (uint32_t)(ctb >> 32);        
    
    // 5. Fehler löschen
    port_regs[12] = 0xFFFFFFFF; 
    port_regs[4]  = 0xFFFFFFFF; 
    
    // 6. Motor wieder an (Mit Anti-Freeze!)
    timeout = 1000000;
    while (((port_regs[8] & 0x88) != 0) && timeout--) { __asm__ volatile("pause"); }
    
    port_regs[6] |= 0x00000010; 
    port_regs[6] |= 0x00000001; 
}

/// 6. DER HAUPT-EINSTIEG 
/// ==========================================
extern "C" void main(BootInfo* boot_info) {
    
    // Ab hier ist float erlaubt!
    init_heap();
	screen_w     = boot_info->screen_width;      
    screen_h     = boot_info->screen_height;
    screen_pitch = boot_info->framebuffer_pitch; 
    fb           = (uint32_t*)(uint64_t)boot_info->framebuffer_addr;

    if (screen_pitch == 0) {
        screen_pitch = screen_w * 4; 
    }
    //fb = (_89*)(uint64_t)sys_info->framebuffer_addr;

    /// ==========================================
    /// 1. IDT (Interrupts) SAUBER AUFBAUEN
    /// ==========================================
    idt_ptr.limit = sizeof(IDTEntry) * 256 - 1;
    idt_ptr.base = (uint64_t)&idt[0];
    
    for(int i = 0; i < 256; i++) set_idt_gate(i, 0);
    
    // BARE METAL FIX: Der Airbag! Fange alle Hardware-Fehler ab!
    for(int i = 0; i < 32; i++) {
        set_idt_gate(i, (uint64_t)dummy_isr);
    }
    
    set_idt_gate(32, (uint64_t)pit_isr);      
    set_idt_gate(33, (uint64_t)keyboard_isr);
	set_idt_gate(0x80, (uint64_t)syscall_isr, 0xEE); // BARE METAL FIX: 0xEE erlaubt der App den Zugriff!	
    set_idt_gate(39, (uint64_t)dummy_isr);
	
    remap_pic();
    init_pit(1000); 
    
    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
    
    /// TASK MANAGER INITIALISIEREN
    tasks[0].active = true;         /// Wir deklarieren Cosmos OS als Task 0
	// Anstatt des Test-Tasks starten wir jetzt einen sauberen Hintergrund-Task
    // Dieser Task kümmert sich um System-Metriken (z.B. Zeit, CPU-Last, etc.)
	create_task(sys_idle_task);
    //create_task(background_task);   /// Wir feuern unseren ersten echten Hintergrund-Task ab!
    __asm__ volatile("sti"); /// ZÜNDUNG: Multitasking beginnt!
    /// ==========================================
    /// DEIN ORIGINAL-CODE STARTET AB HIER WIEDER:
    /// ==========================================
	// Irgendwo in deiner Hintergrund-Schleife (z.B. nach dem Zeichnen der Sterne):
    read_rtc();
    get_cpu_brand();
    usb_mouse_callback = update_mouse_position;
    init_mouse();
	
    /// Sterne generieren
    _39(_43 i=0; i<200; i++) {
        stars[i].x = (random() % 1599) - 799;
        stars[i].y = (random() % 1199) - 599;
        stars[i].z = (random() % 1000) + 1;
        stars[i].type = random() % 1000; 
        /// BARE METAL FIX: Speed auf einen sauberen Mittelwert (4 bis 9)
        stars[i].speed = (random() % 6) + 4;
    }
    /// ==========================================
    /// BARE METAL FIX: FENSTER SAUBER INITIALISIEREN
    /// ==========================================
    _39(_43 i=0; i<13; i++) { windows[i].id = i; windows[i].open = _86; windows[i].minimized = _86; windows[i].cursor_pos = 0; windows[i].content[0] = 0; }
    
    str_cpy(windows[0].title, "NOTEPAD");  windows[0].x=100; windows[0].y=100; windows[0].w=400; windows[0].h=300; windows[0].color=0xEEEEEE; 
    str_cpy(windows[1].title, "APPS");     windows[1].x=150; windows[1].y=150; windows[1].w=350; windows[1].h=250; windows[1].color=0xDDDDDD; 
    str_cpy(windows[2].title, "SAVE AS..."); windows[2].x=200; windows[2].y=150; windows[2].w=300; windows[2].h=200; windows[2].color=0xDDDDDD;
    str_cpy(windows[3].title, "SYSTEM");   windows[3].x=200; windows[3].y=50;  windows[3].w=350; windows[3].h=480; windows[3].color=0xFFD700; 
    str_cpy(windows[4].title, "DISK MGR"); windows[4].x=250; windows[4].y=200; windows[4].w=450; windows[4].h=350; windows[4].color=0x888888; 
    str_cpy(windows[5].title, "CMD");      windows[5].x=100; windows[5].y=300; windows[5].w=450; windows[5].h=250; windows[5].color=0x111111;
    str_cpy(windows[6].title, "GENESIS DB"); windows[6].x=150; windows[6].y=100; windows[6].w=500; windows[6].h=400; windows[6].color=0x222222;
    str_cpy(windows[7].title, "WIFI MGR"); windows[7].x=200; windows[7].y=150; windows[7].w=300; windows[7].h=300; windows[7].color=0x004488;
    str_cpy(windows[8].title, "ACCESS");   windows[8].x=300; windows[8].y=100; windows[8].w=250; windows[8].h=300; windows[8].color=0x44AAAA;
    str_cpy(windows[9].title, "SENSES");   windows[9].x=400; windows[9].y=100; windows[9].w=300; windows[9].h=350; windows[9].color=0x222222;
    str_cpy(windows[10].title, "GERAETE MGR"); windows[10].x=100; windows[10].y=80; windows[10].w=600; windows[10].h=450; windows[10].color=0x111111;
    str_cpy(windows[11].title, "ARCADE: PONG"); windows[11].x=200; windows[11].y=150; windows[11].w=450; windows[11].h=300; windows[11].color=0x112211;
    str_cpy(windows[12].title, "ARCADE: BLOBBY"); windows[12].x=250; windows[12].y=100; windows[12].w=450; windows[12].h=300; windows[12].color=0x4488FF;
	
    /// BARE METAL FIX: Alle Orakel-Variablen sofort befüllen!
	scan_pci_drives(&windows[4]);
    
    /// 1. Puffer komplett nullen (RAM Müll vernichten!)
    mem_set(hw_storage, 0, 256);
    mem_set(hw_net, 0, 256);
    mem_set(hw_gpu, 0, 256);

    /// 2. Die EINZIGE Scan-Schleife ausführen (pci.cpp)
    pci_scan_all(); 
	check_incoming();
	//system_init_usb();
    /// 3. Bildschirm-Abschneider (Gegen das Worträtsel)
    hw_storage[35] = 0;
    hw_net[35] = 0;
    hw_gpu[35] = 0;
    hw_usb[35] = 0;
    /// Planeten initialisieren
    _43 clock_dirs[] = {213, 0, 42, 85, 128}; 
    _39(_43 i=0; i<5; i++) { 
        planets[i].ang = clock_dirs[i]; planets[i].dist = 10;
        planets[i].cur_x = 400; planets[i].cur_y = 300;
        str_cpy(planets[i].name, (i==0?(_30*)"TXT":i==1?(_30*)"APP":i==2?(_30*)"SYS":i==3?(_30*)"DSK":(_30*)"CMD")); 
    }
    _43 map_ids[]={0,1,3,4,5}; /// Verknüpfung Planet -> Fenster ID
    _114(1) {
        handle_input();
        //xhci_poll_events_and_mouse();
        _15(input_cooldown > 0) input_cooldown--;
        click_consumed = _86; z_blocked = _86; _44 mouse_handled = _86;

        /// ==========================================
        /// BARE METAL FIX: Unser "DHCP-Button" (Taste D)
        /// ==========================================
        _15((last_app_key EQ 'd' OR last_app_key EQ 'D') AND input_cooldown EQ 0) {
            send_dhcp_discover();
            last_app_key = 0;       /// Taste als "verarbeitet" markieren
            input_cooldown = 25;    /// Kurz warten, damit er nicht 1000 Pakete feuert
        }

        _15(frame % 10 EQ 0) check_incoming();
        /// ==========================================
        /// ABTEILUNG: TASTATUR (BACK TO OS1 STABILITY)
        /// ==========================================
        if (key_ready) {
            key_ready = _86; /// Flag sofort zurücksetzen
            _184 sc = key_scancode;

            _15(!(sc & 0x80)) { /// Taste wurde GEDRÜCKT (Make Code)
                
                /// BARE METAL FIX: Die Taste als ASCII für unsere Apps kopieren!
                /// ==========================================
                /// HIER KOMMT DEIN DEBUGGER REIN!
                /// Er überschreibt bei JEDEM Tastendruck den Status unten rechts!
                /// ==========================================
                _30 dbg_str[30] = "KEY: ";
                _30 h_str[5]; 
                byte_to_hex(sc, h_str);
                str_cat(dbg_str, h_str);
                str_cpy(cmd_status, dbg_str);
                
                /// 1. Globale Hotkeys (F-Tasten für Apps)
                _15(sc EQ 0x3B) toggle_app(0); 
                _15(sc EQ 0x3C) toggle_app(1); 
                _15(sc EQ 0x3D) toggle_app(3); 
                _15(sc EQ 0x3E) toggle_app(4); 
                _15(sc EQ 0x3F) toggle_app(5);

                /// 2. Welches Fenster ist ganz oben (Fokus)?
                _43 fw_id = win_z[12]; 
                Window* fw = &windows[fw_id];
                
                _15(fw AND fw->open AND !fw->minimized) {
                    /// --- TASTATUR STEUERT DAS SYSTEM FENSTER (ID 3) ---
                    _15(fw->id EQ 3) {
                        _15(sc EQ 0x50) { // Pfeil Runter
                            if (sys_selected_item < 4) sys_selected_item++; 
                        } 
                        
                        _15(sc EQ 0x48) { // Pfeil Hoch
                            if (sys_selected_item > 0) sys_selected_item--; 
                        } 
                        
                        _15(sc EQ 0x1C) { // ENTER-TASTE
                            
                            /// INDEX 0: DER GROSSE ROTE ORAKEL-BUTTON OBEN
                            if (sys_selected_item == 0) {
                                mirror_count = 0;
                                for(uint32_t b=0; b<256; b++) {
                                    for(uint32_t d=0; d<32; d++) {
                                        for(uint32_t f=0; f<28; f++) {
                                            uint32_t id = pci_read(b, d, f, 0);
                                            if((id & 0xFFFF) != 0xFFFF && id != 0 && mirror_count < 30) {
                                                mirror_list[mirror_count].bus = b;
                                                mirror_list[mirror_count].dev = d;
                                                mirror_list[mirror_count].func = f;
                                                mirror_list[mirror_count].vendor = id & 0xFFFF;
                                                mirror_list[mirror_count].device = id >> 16;
                                                
                                                uint32_t bar0_l = pci_read(b, d, f, 0x10);
                                                uint32_t bar0_h = pci_read(b, d, f, 0x14);
                                                
                                                uint32_t class_rev = pci_read(b, d, f, 0x08);
                                                uint32_t cls = (class_rev >> 24) & 0xFF;
                                                uint32_t sub = (class_rev >> 16) & 0xFF;
                                                
                                                /// Ist es eine echte 64-Bit Adresse?
                                                if((bar0_l & 0x06) == 0x04) { 
                                                    mirror_list[mirror_count].bar0 = ((uint64_t)bar0_h << 32) | (bar0_l & 0xFFFFFFF0);
                                                } else {
                                                    mirror_list[mirror_count].bar0 = bar0_l & 0xFFFFFFF0;
                                                }
                                                
                                                if(cls == 0x0C && sub == 0x03) str_cpy(mirror_list[mirror_count].name, "USB 3.0 (xHCI)");
                                                else if(cls == 0x01 && sub == 0x06) str_cpy(mirror_list[mirror_count].name, "SATA (AHCI)");
                                                else if(cls == 0x02) str_cpy(mirror_list[mirror_count].name, "NETWORK");
                                                else if(cls == 0x03) str_cpy(mirror_list[mirror_count].name, "GRAPHICS");
                                                else str_cpy(mirror_list[mirror_count].name, "SYSTEM DEVICE");
                                                
                                                mirror_count++;
                                            }
                                        }
                                    }
                                }
                                /// WICHTIG: Das Orakel darf sich NUR hier beim Enter-Druck öffnen!
                                show_oracle = true; 
                            }
                            
                            /// INDEX 1, 2, 3: DIE ANDEREN HARDWARE-SCANS (Nur bei Enter!)
                            else if (sys_selected_item == 1) { scan_pci_class(0x01, hw_storage, "CTRL"); }
                            else if (sys_selected_item == 2) { scan_pci_class(0x02, hw_net, "NIC"); }
                            else if (sys_selected_item == 3) { scan_pci_class(0x03, hw_gpu, "GPU"); }
                                
                            /// INDEX 4: DER USB HOST START! (Nur bei Enter!)
                            else if (sys_selected_item == 4) {
                                system_init_usb(); 
                            }
                        }
                    }
                    /// --- NOTEPAD (ID 0) PURE OS1 LOGIC ---
                    _15(fw->id EQ 0) { 
                        _15(sc EQ 0x0E) { _15(fw->cursor_pos > 0) { fw->cursor_pos--; fw->content[fw->cursor_pos] = 0; } } 
                        _41 _15(sc EQ 0x1C) { _15(fw->cursor_pos < 2000) { fw->content[fw->cursor_pos++] = '\n'; fw->content[fw->cursor_pos] = 0; } } 
                        _41 { 
                            _30 c = get_ascii_qwertz(sc);
                            if(c >= 'a' && c <= 'z') c -= 32;
                            _15(c AND fw->cursor_pos < 2000) { fw->content[fw->cursor_pos++] = c; fw->content[fw->cursor_pos] = 0; } 
                        }
                    }
                    
                    /// --- CMD (ID 5) ---
                    _15(fw->id EQ 5) { 
                        _15(sc EQ 0x0E) { _15(cmd_input_idx > 0) { cmd_input_idx--; cmd_input_buf[cmd_input_idx] = 0; } } 
                        _41 _15(sc EQ 0x1C) { process_cmd(cmd_input_buf, fw); cmd_input_idx = 0; cmd_input_buf[0] = 0; } 
                        _41 { 
                            _30 c = get_ascii_qwertz(sc);
                            if(c >= 'a' && c <= 'z') c -= 32;
                            _15(c AND cmd_input_idx < 60) { cmd_input_buf[cmd_input_idx++] = c; cmd_input_buf[cmd_input_idx] = 0; } 
                        }
                    }
                }
				/// --- SAVE AS DIALOG (ID 2) ---
                _15(fw->id EQ 2) { 
                    _15(sc EQ 0x0E) { /// Backspace
                        _15(save_step EQ 1 AND save_name_idx > 0) save_filename[--save_name_idx] = 0;
                        _41 _15(save_step EQ 2 AND folder_name_idx > 0) new_folder_name[--folder_name_idx] = 0;
                    } 
                    _41 { 
                        _30 c = get_ascii_qwertz(sc);
                        if(c >= 'a' && c <= 'z') c -= 32; /// OS1 Style: Alles wird UPPERCASE
                        _15(c >= 32) { 
                            _15(save_step EQ 1 AND save_name_idx < 10) { 
                                save_filename[save_name_idx++] = c; 
                                save_filename[save_name_idx] = 0; 
                            }
                            _41 _15(save_step EQ 2 AND folder_name_idx < 10) { 
                                new_folder_name[folder_name_idx++] = c; 
                                new_folder_name[folder_name_idx] = 0; 
                            }
                        } 
                    }
                }
            }
        }
        _15(frame % 100 EQ 0) {
            read_rtc();
        }
        /// ==========================================
        /// FENSTER INTERAKTION (DRAG, RESIZE, BUTTONS)
        /// ==========================================
        _15(mouse_down) {
             _15(drag_win NEQ -1) { 
                 windows[drag_win].x = mouse_x - drag_off_x; 
                 windows[drag_win].y = mouse_y - drag_off_y; 
                 mouse_handled=_128; click_consumed=_128; 
             } 
             _41 _15(resize_win NEQ -1) { 
                 _43 nw = mouse_x - windows[resize_win].x; 
                 _43 nh = mouse_y - windows[resize_win].y; 
                 _15(nw > 100) windows[resize_win].w = nw; 
                 _15(nh > 100) windows[resize_win].h = nh; 
                 mouse_handled=_128; click_consumed=_128; 
             }
        } _41 { drag_win = -1; resize_win = -1; }
        
        _15(!mouse_handled) {
            _39(_43 i=12; i>=0; i--) { 
                _43 k = win_z[i]; Window* win=&windows[k];
                _15(win->open AND !win->minimized) {
                    _43 wx=(win->fullscreen?0:win->x); _43 wy=(win->fullscreen?0:win->y); _43 ww=(win->fullscreen?800:win->w); _43 wh=(win->fullscreen?600:win->h);
                    _15(mouse_x>=wx AND mouse_x<=wx+ww AND mouse_y>=wy AND mouse_y<=wy+wh) {
                        z_blocked = _128;
                        _15(mouse_just_pressed) {
                           click_consumed = _128; 
                           focus_window(k);
                           _43 bx = wx + ww/2; 
                           
                           /// DIE TITELLEISTE WIRD GEKLICKT
                           _15(mouse_y < wy+40) { 
                               
                               /// MINIMIEREN
                               _15(mouse_x > bx-70 AND mouse_x < bx-30) {
                                   _15((system_ticks - last_window_click) > 250) { 
                                       win->minimized=_128; last_window_click = system_ticks; 
                                   }
                               } 
                               /// FULLSCREEN
                               _41 _15(mouse_x > bx-20 AND mouse_x < bx+40) {
                                   _15((system_ticks - last_window_click) > 250) { 
                                       win->fullscreen = !win->fullscreen; last_window_click = system_ticks; 
                                   }
                               } 
                               /// SCHLIESSEN
                               _41 _15(mouse_x > bx+45 AND mouse_x < bx+70) {
                                   _15((system_ticks - last_window_click) > 250) { 
                                       win->open=_86; last_window_click = system_ticks; 
                                   }
                               }
                               /// DRAG & DROP (Greift sofort, wenn man keinen Button erwischt)
                               _41 { 
                                   drag_win = k; drag_off_x = mouse_x - wx; drag_off_y = mouse_y - wy; 
                               }
                           }
                           
                           /// FENSTERGRÖSSE ÄNDERN
                           _15(mouse_x > wx+ww-20 AND mouse_y > wy+wh-20) { resize_win = k; }
                        }
                        mouse_handled = _128; _37; 
                    }
                }
            }
        }
        /// ==========================================
        /// 1. DER ABSOLUT SCHWARZE WELTRAUM
        /// ==========================================
        _39(_43 i = 0; i < 800*600; i++) bb[i] = 0x000000;
        _39(_43 i=0; i<200; i++) {
            _43 t = stars[i].type;
            _15(t >= 995) stars[i].z -= 1; _41 _15(t >= 980) stars[i].z -= stars[i].speed; _41 _15(t >= 950) stars[i].z -= (stars[i].speed + 4); _41 stars[i].z -= stars[i].speed;
            _15(stars[i].z <= 0) { 
                stars[i].z = 1000; stars[i].x = (random() % 1599) - 799; stars[i].y = (random() % 1199) - 599;
                stars[i].type = random() % 1000; stars[i].speed = (random() % 10) + 8;
            }
            _43 sx = v_cx + (stars[i].x * 256) / stars[i].z; _43 sy = v_cy + (stars[i].y * 256) / stars[i].z;
            _15(sx >= 0 AND sx < 800 AND sy >= 0 AND sy < 600) {
                _15(t < 900) {
                    _43 intensity = 255 - (stars[i].z / 4); _15(intensity < 0) intensity = 0; _15(intensity > 255) intensity = 255;
                    _89 col = (intensity << 16) | (intensity << 8) | (intensity); Put(sx, sy, col);
                } _41 _15(t < 950) {
                    _43 pulse = (Sin((frame * 5) + i) + 256) / 2; _15(pulse > 255) pulse = 255;
                    _89 col = (pulse << 16) | (pulse << 8) | 255; Put(sx, sy, col);
                } _41 _15(t < 980) {
                    Put(sx, sy, 0xFFFFFF); 
                    _43 tail1_x = v_cx + (stars[i].x * 256) / (stars[i].z + 20); _43 tail1_y = v_cy + (stars[i].y * 256) / (stars[i].z + 20); Put(tail1_x, tail1_y, 0xFF8800); 
                    _43 tail2_x = v_cx + (stars[i].x * 256) / (stars[i].z + 40); _43 tail2_y = v_cy + (stars[i].y * 256) / (stars[i].z + 40); Put(tail2_x, tail2_y, 0xAA0000);
                } _41 _15(t < 995) {
                    _43 r = 3000 / stars[i].z; 
                    _15(r > 0 AND r < 40) {
                        _43 r2 = r*r; _43 focus_x = (sx - v_cx) * r / 400; _43 focus_y = (sy - v_cy) * r / 300;
                        _39(_43 cy_a=-r; cy_a<=r; cy_a++) _39(_43 cx_a=-r; cx_a<=r; cx_a++) _15(cx_a*cx_a+cy_a*cy_a <= r2) {
                            _43 pos_x = cx_a + r; _43 pos_y = cy_a + r; _43 noise = ((pos_x * 17 + pos_y * 31) % 40);
                            _43 lx = cx_a + focus_x; _43 ly = cy_a + focus_y; _43 l_dist = int_sqrt(lx*lx + ly*ly);
                            _43 diffuse = 30; _15(l_dist < r) diffuse += 225 - (l_dist * 225 / r);
                            _43 gray = ((50 + noise) * diffuse) / 256; _15(gray > 255) gray = 255;
                            Put(sx+cx_a, sy+cy_a, (gray<<16)|((gray*9)/10<<8)|((gray*8)/10)); 
                        }
                    }
                } _41 {
                    _43 r = 6000 / stars[i].z;
                    _15(r > 0 AND r < 60) {
                        _39(_43 j=0; j<30; j++) {
                            _43 ang1 = (j * 15 + (frame/3)) % 256; _43 dist = (j * r) / 30;
                            _43 gx1 = sx + (Cos(ang1)*dist)/84; _43 gy1 = sy + (Sin(ang1)*dist*3/4)/84; PutAlpha(gx1, gy1, 0xAA22AA); 
                            _43 ang2 = (ang1 + 128) % 256; _43 gx2 = sx + (Cos(ang2)*dist)/84; _43 gy2 = sy + (Sin(ang2)*dist*3/4)/84; PutAlpha(gx2, gy2, 0x2288AA); 
                        }
                        Put(sx, sy, 0xFFFFFF); 
                    }
                }
            }
        }
        /// Zentrifuge & Sonne rendern
        _15(!z_blocked AND mouse_just_pressed AND !click_consumed AND is_over(mouse_x, mouse_y, v_cx, v_cy, 50)) {
            galaxy_open = !galaxy_open; click_consumed = _128;
        }
        _15(galaxy_open AND galaxy_expansion < 320) galaxy_expansion += 24;
        _15(!galaxy_open AND galaxy_expansion > 0) galaxy_expansion -= 30;
        
        DrawDenseGalaxy(v_cx, v_cy, galaxy_expansion);
        
        // ==========================================
        // 3D ORBIT-PHYSIK & Z-SORTIERUNG (ECHTES SYSTEM)
        // ==========================================
        
        // 1. Die Geschwindigkeiten (Je weiter draußen, desto langsamer!)
        float t_mer = frame * 0.041f;
        float t_ven = frame * 0.016f;
        float t_ear = frame * 0.010f;
        float t_mar = frame * 0.005f;
        float t_jup = frame * 0.001f;
        float t_sat = frame * 0.0005f;
        float t_ura = frame * 0.0002f;
        float t_nep = frame * 0.0001f;

        // 2. Die Umlaufbahnen (Leichte Neigungen für einen organischen 3D-Look)
        Vec3 p1_mer = { bare_cos(t_mer) * 140.0f,  5.0f * bare_sin(t_mer), bare_sin(t_mer) * 140.0f };
        Vec3 p2_ven = { bare_cos(t_ven) * 180.0f, -3.0f * bare_sin(t_ven), bare_sin(t_ven) * 180.0f };
        Vec3 p3_ear = { bare_cos(t_ear) * 230.0f,  0.0f,                   bare_sin(t_ear) * 230.0f };
        Vec3 p4_mar = { bare_cos(t_mar) * 280.0f,  8.0f * bare_sin(t_mar), bare_sin(t_mar) * 280.0f };
        Vec3 p5_jup = { bare_cos(t_jup) * 400.0f, -10.0f* bare_sin(t_jup), bare_sin(t_jup) * 400.0f };
        Vec3 p6_sat = { bare_cos(t_sat) * 520.0f,  15.0f* bare_sin(t_sat), bare_sin(t_sat) * 520.0f };
        Vec3 p7_ura = { bare_cos(t_ura) * 640.0f, -5.0f * bare_sin(t_ura), bare_sin(t_ura) * 640.0f };
        Vec3 p8_nep = { bare_cos(t_nep) * 750.0f,  2.0f * bare_sin(t_nep), bare_sin(t_nep) * 750.0f };

        // 3. Das Universum auflisten (9 Objekte!)
        RenderObj system[9] = {
            { 0, {0, 0, 0}, 80,  0 },                  // Sonne (Kocht!)
            { 1, p1_mer,     3,  0xAAAAAA },           // Merkur (Winzig, Grau)
            { 1, p2_ven,     7,  0xFFDD88 },           // Venus (Hellgelb)
            { 1, p3_ear,     8,  0x0088FF },           // Erde (Blau)
            { 1, p4_mar,     5,  0xFF4400 },           // Mars (Rostrot)
            { 1, p5_jup,    24,  0xDDAA77 },           // Jupiter (Gigantisch, Braun/Orange)
            { 1, p6_sat,    20,  0xEEDD99 },           // Saturn (Blassgelb)
            { 1, p7_ura,    12,  0x66CCFF },           // Uranus (Hellblau)
            { 1, p8_nep,    11,  0x2244AA }            // Neptun (Dunkelblau)
        };

        // ==========================================
        // 4. DIE INTERAKTIVE KAMERA (Rechte Maustaste)
        // ==========================================
        
        // Wir fragen einfach unsere neue globale Variable ab!
        bool right_mouse_held = mouse_right_down; 

        // Nur wenn die rechte Maustaste gedrückt ist, drehen wir das Universum!
        if (right_mouse_held) {
            global_cam_rot_y += (mouse_x - last_mouse_x) * -0.005f; 
            global_cam_rot_x += (mouse_y - last_mouse_y) * -0.005f; 
        }
        
        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;
        
        // Sinus/Kosinus aus den globalen Winkeln ziehen
        float sy = bare_sin(global_cam_rot_y); float cy = bare_cos(global_cam_rot_y);
        float sx = bare_sin(global_cam_rot_x); float cx = bare_cos(global_cam_rot_x);

        // Die 9 Objekte rotieren
        for (int i = 0; i < 9; i++) {
            Vec3 p = system[i].pos;
            float nx = p.x * cy - p.z * sy;
            float nz = p.x * sy + p.z * cy;
            p.x = nx; p.z = nz;

            float ny = p.y * cx - p.z * sx;
            nz = p.y * sx + p.z * cx;
            p.y = ny; p.z = nz;
            
            system[i].pos = p; 
        }

        // 5. BUBBLE SORT FÜR 9 OBJEKTE (Z-Tiefe)
        // Die innere Schleife geht bis 8 (j+1 erreicht maximal 8)
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 8; j++) {
                if (system[j].pos.z < system[j+1].pos.z) {
                    RenderObj temp = system[j];
                    system[j] = system[j+1];
                    system[j+1] = temp;
                }
            }
        }

        // 6. ZEICHNEN der sortierten Liste
        for (int i = 0; i < 9; i++) {
            if (system[i].type == 0) {
                Vec2 s_2d = Project3D(system[i].pos, v_cx, v_cy);
                if (s_2d.x != -1) {
                    DrawActiveSun(s_2d.x, s_2d.y, system[i].radius);
                }
            } else {
                DrawPlanet3D(system[i].pos, system[i].radius, system[i].color, v_cx, v_cy);
            }
        }
		// ==========================================
        // 7. 3D POLYGON-TEST: IMPERIALER STERNENKREUZER / ZERSTÖRER
        // ==========================================
        //float station_rot = frame * 0.025f; // Etwas langsamer für die massive Wucht!
        //float sr = bare_sin(station_rot);
        //float cr = bare_cos(station_rot);
		//
        //// Der Bauplan: 10 strategische Eckpunkte im 3D-Raum
        //Vec3 ship[10] = {
        //    {   0.0f,   0.0f,  110.0f},  // 0: Die rasiermesserscharfe Bugspitze (Nose)
        //    { -50.0f,   5.0f,  -50.0f},  // 1: Heck oben links
        //    {  50.0f,   5.0f,  -50.0f},  // 2: Heck oben rechts
        //    { -50.0f,  -5.0f,  -50.0f},  // 3: Heck unten links
        //    {  50.0f,  -5.0f,  -50.0f},  // 4: Heck unten rechts
        //    {   0.0f,  28.0f,  -25.0f},  // 5: Kommandobrücke Dach (Spitze)
        //    { -15.0f,   5.0f,  -30.0f},  // 6: Brückenbasis hinten links
        //    {  15.0f,   5.0f,  -30.0f},  // 7: Brückenbasis hinten rechts
        //    {   0.0f,   5.0f,   -5.0f},  // 8: Brückenbasis vorne (Schildkeil)
        //    {   0.0f,   0.0f,  -50.0f}   // 9: Heckmitte (Triebwerksschott)
        //};
		//
        //// Die Matrix-Schleife: Zerstörer rotieren und im Raum parken
        //for (int i = 0; i < 10; i++) {
        //    // Um die Y-Achse rotieren (Drehung auf der Stelle)
        //    float nx = ship[i].x * cr - ship[i].z * sr;
        //    float nz = ship[i].x * sr + ship[i].z * cr;
        //    ship[i].x = nx; 
        //    ship[i].z = nz;
        //    
        //    // Positionierung: Schwebt massiv vor der Sonne bei Z = -190
        //    ship[i].x += 0.0f; 
        //    ship[i].y += -15.0f; // Leicht nach unten versetzt für perfekte Sicht
        //    ship[i].z -= 190.0f; 
        //}
		//
        //// ==========================================
        //// RASTER PIPELINE: Die Panzerplatten schweißen
        //// (Sortiert von hinten nach vorne für den Painter's Algorithm!)
        //// ==========================================
        //
        //// 1. Das Heck & Triebwerksschott (Dunkles Schatten-Graugrün)
        //DrawTriangle3D(ship[1], ship[9], ship[3], 0x0c1a0c, v_cx, v_cy);
        //DrawTriangle3D(ship[2], ship[4], ship[9], 0x0c1a0c, v_cx, v_cy);
        //
        //// 2. Die massive Unterseite (Stark im Schatten)
        //DrawTriangle3D(ship[0], ship[3], ship[4], 0x142b14, v_cx, v_cy);
        //
        //// 3. Die messerscharfen Seitenflanken
        //DrawTriangle3D(ship[0], ship[1], ship[3], 0x234a23, v_cx, v_cy); // Flanke Links
        //DrawTriangle3D(ship[0], ship[4], ship[2], 0x2d5e2d, v_cx, v_cy); // Flanke Rechts
        //
        //// 4. Das gewaltige Hauptdeck (Mittelgrün - fängt das Sonnenlicht)
        //DrawTriangle3D(ship[0], ship[2], ship[1], 0x3c7d3c, v_cx, v_cy);
        //
        //// 5. Die Brücken-Aufbauten (Hellgrüne Akzente für epische Tiefenwirkung!)
        //DrawTriangle3D(ship[8], ship[6], ship[7], 0x4b9c4b, v_cx, v_cy); // Turm-Deck
        //DrawTriangle3D(ship[5], ship[6], ship[8], 0x5cbd5c, v_cx, v_cy); // Brücke Wand Links
        //DrawTriangle3D(ship[5], ship[8], ship[7], 0x70de70, v_cx, v_cy); // Brücke Wand Rechts (Reflektion)
        //DrawTriangle3D(ship[5], ship[7], ship[6], 0x2d5e2d, v_cx, v_cy); // Brücke Rückwand

        /// ==========================================
        /// LIVE RTC (DATUM UND UHRZEIT) ALS HUD ÜBER ALLEM
        /// ==========================================
        TextC(v_cx, v_cy-15, "COSMOS", 0x000000, _128);
        TextC(v_cx, v_cy+5,  "SYSTEM", 0x000000, _128);
        _30 ts[]="00:00"; 
        ts[0]='0'+rtc_h/10; ts[1]='0'+rtc_h%10; 
        ts[3]='0'+rtc_m/10; ts[4]='0'+rtc_m%10; 
        TextC(v_cx, v_cy+20, ts, 0x000000, _128);
        _30 ds[]="00.00.2000"; 
        ds[0]='0'+rtc_day/10; ds[1]='0'+rtc_day%10; 
        ds[3]='0'+rtc_mon/10; ds[4]='0'+rtc_mon%10; 
        ds[8]='0'+(rtc_year%100)/10; ds[9]='0'+rtc_year%10; 
        TextC(v_cx, v_cy+35, ds, 0x000000, _128);
        /// ==========================================
        /// 2. PLANETEN (MIT FENSTER-VERKNÜPFUNG)
        /// ==========================================
        _39(_43 i=0; i<5; i++) {
            Window* win = &windows[map_ids[i]];
            _43 target_x, target_y; _44 draw_moons = _86;
            _15(win->minimized) { 
                target_x = 250 + (i * 70); target_y = 560; 
            } _41 _15(win->open) { 
                _43 orbit_dist = 60 + i*50; 
                target_x = v_cx + (Cos(planets[i].ang) * orbit_dist) / 84; 
                target_y = v_cy + (Sin(planets[i].ang) * orbit_dist * 3/4) / 84; 
                draw_moons = _128; 
            } _41 {
                _15(galaxy_expansion >= 100) { 
                    /// BARE METAL FIX: Schneller nach außen fliegen (6 statt 2)
                    _15(planets[i].dist < 60 + i*50) planets[i].dist += 6; 
                    
                    /// BARE METAL FIX: Bremse gelöst! Drehen sich jetzt in jedem Frame!
                    /// BARE METAL FIX: Planeten drehen sich weicher (nur jeden 3. Frame)
                    _15(planets[i].dist > 50) { 
                         _15(frame % 3 EQ 0) planets[i].ang = (planets[i].ang + 1) % 256;
                    } 
                } _41 { 
                    /// BARE METAL FIX: Schneller zurückfliegen (15 statt 8)
                    _15(planets[i].dist > 10) planets[i].dist -= 15; 
                }
                target_x = v_cx + (Cos(planets[i].ang) * planets[i].dist) / 84; 
                target_y = v_cy + (Sin(planets[i].ang) * planets[i].dist * 3/4) / 84;
            }
            
            /// BARE METAL FIX: Das Nachziehen (Easing) halbiert = doppelt so schnell am Ziel!
            planets[i].cur_x += (target_x - planets[i].cur_x) / 2; 
            planets[i].cur_y += (target_y - planets[i].cur_y) / 2;
            _15(galaxy_expansion > 10 OR win->minimized OR win->open) {
                _43 px = planets[i].cur_x; _43 py = planets[i].cur_y;
                _44 hov = is_over(mouse_x, mouse_y, px, py, 20);
                _89 p_col = 0x888888;         
                _15(i EQ 0) p_col = 0xA05566; _15(i EQ 1) p_col = 0x44AA88; 
                _15(i EQ 2) p_col = 0x6677CC; _15(i EQ 3) p_col = 0xCC9955; 
                _15(i EQ 4) p_col = 0x8899AA;
                _15(hov AND !z_blocked) {
                    _43 hr = ((p_col >> 16) & 0xFF) + 40; _15(hr>255) hr=255;
                    _43 hg = ((p_col >> 8) & 0xFF) + 40;  _15(hg>255) hg=255;
                    _43 hb = (p_col & 0xFF) + 40;         _15(hb>255) hb=255;
                    DrawOrganicPlanet(px, py, 22, (hr<<16)|(hg<<8)|hb);
                } _41 {
                    DrawOrganicPlanet(px, py, 20, p_col);
                }
                _15(draw_moons) { 
                    DrawOrganicPlanet(px-30, py, 5, 0x8899AA); 
                    DrawOrganicPlanet(px+30, py, 5, 0x8899AA); 
                }
                TextC(px, py-4, planets[i].name, 0xFFFFFF, _128);
                _15(!z_blocked AND mouse_just_pressed AND !click_consumed AND hov) { 
                    _15(win->minimized) win->minimized = _86; 
                    _41 { 
                        win->open = _128; 
                        focus_window(win->id); 
                    }
                    click_consumed = _128;
                }
            }
        }
        /// ==========================================
        /// 3. ACRYLIC GLASS WINDOW RENDERING
        /// ==========================================
		/// BARE METAL FIX: Modal-Status berechnen, BEVOR die Fenster gezeichnet werden!
        /// Wenn Fenster ID 2 (Save As) offen und sichtbar ist, ist der Modus aktiv.
        _44 is_modal_blocked = (windows[2].open AND !windows[2].minimized);
        _39(_43 i=0; i<13; i++) {
            _43 k = win_z[i]; 
            Window* win = &windows[k];
            _15(!win->open OR win->minimized) continue;
            _43 wx=(win->fullscreen?0:win->x); 
            _43 wy=(win->fullscreen?0:win->y); 
            _43 ww=(win->fullscreen?800:win->w); 
            _43 wh=(win->fullscreen?600:win->h);
			/// =========================================================
            /// BARE METAL FIX: BLOCKED-STATUS FÜR ALLE FENSTER DEFINIEREN!
            /// =========================================================
            _44 blocked = (is_modal_blocked AND k NEQ 2);
            /// Milchglas-Hintergrund für offene Fenster zeichnen
            DrawGlassRect(wx, wy, ww, wh, 12, win->color);
            /// Rahmen & Highlights
            DrawRoundedRect(wx+12, wy, ww-24, 1, 0, 0x999999);
            DrawRoundedRect(wx, wy+12, 1, wh-24, 0, 0x999999);
            _89 txt_color = (win->color > 0x888888) ? 0x000000 : 0xFFFFFF;
            Text(wx+15, wy+15, win->title, txt_color, _128);
            /// Fenster-Buttons
            _43 bx = wx + ww/2; 
            Text(bx-60, wy+15, "MIN", 0x555555, _128);
            Text(bx-10, wy+15, "FULL", 0x555555, _128); 
            Text(bx+50, wy+15, "X", 0x000000, _128);
			/// =========================================================
            /// APP: WEB BROWSER (ID 8)
            /// =========================================================
            _15(win->id EQ 8) {
                run_browser_engine(wx, wy, ww, wh, blocked);
            }
			/// =========================================================
            /// APP: PONG ARCADE (ID 11)
            /// =========================================================
            _15(win->id EQ 11) {
                run_pong_engine(wx, wy, ww, wh, blocked);
            }
            /// =========================================================
            /// APP: BLOBBY VOLLEY ARCADE (ID 12)
            /// =========================================================
            _15(win->id EQ 12) {
                run_blobby_engine(wx, wy, ww, wh, blocked);
            }
			/// --- SYSTEM FENSTER ZEICHNEN (ID 3) ---
            _15(win->id EQ 3) {
                _43 mid = wx + ww/2;
                _43 btn_y = wy + 45;
                
                /// FIX: Nur Klicks zulassen, wenn das Fenster GANZ OBEN liegt!
                _44 is_active = (win_z[12] EQ win->id);
                
                /// 1. THEME & LANG TOGGLES
                _30 lang_lbl[20], theme_lbl[30];
                _15(sys_lang EQ 0) str_cpy(lang_lbl, "[ LANG: EN ]"); _41 str_cpy(lang_lbl, "[ SPR: DE ]");
                _15(sys_lang EQ 0) { _15(sys_theme EQ 0) str_cpy(theme_lbl, "[ THEME: COMPUTER ]"); _41 str_cpy(theme_lbl, "[ THEME: GENESIS ]"); } 
                _41 { _15(sys_theme EQ 0) str_cpy(theme_lbl, "[ THEMA: COMPUTER ]"); _41 str_cpy(theme_lbl, "[ THEMA: GENESIS ]"); }
                
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+5, btn_y, 140, 20)) { sys_lang = !sys_lang; input_cooldown = 25; }
                Text(wx+10, btn_y+4, lang_lbl, 0x000000, _128);
                
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+5, btn_y+30, 200, 20)) { sys_theme = !sys_theme; input_cooldown = 25; }
                Text(wx+10, btn_y+34, theme_lbl, 0x000000, _128);
                
                /// Leerer Bereich
                DrawRoundedRect(wx+20, wy+120, ww-40, 2, 0, 0xAAAAAA);
                
                /// 2. CPU & USER INFO
                TextC(mid, wy+140, user_name, 0x222222, _128); 
                TextC(mid, wy+160, cpu_brand, 0x0000FF, _128);
                /// --- BARE METAL FIX: NETZWERK-DATEN ANZEIGEN ---
                _43 right_x = wx + ww - 160;
                /// Zeile 1: Deine Laptop IP (z.B. 192.168.14.100)
                Text(right_x, wy+140, ip_address, 0x222222, _128); 
                /// Zeile 2: Subnetzmaske
                Text(right_x, wy+155, net_mask, 0x555555, _86); 
                /// Zeile 3: Gateway (FritzBox)
                Text(right_x, wy+170, gateway_ip, 0x555555, _86);
                
                /// 3. HARDWARE STATUS
                TextC(mid, wy+200, "HARDWARE STATUS", 0x000000, _128);
                Text(wx+30, wy+230, "CORE:", 0x555555, _128); Text(wx+130, wy+230, "64-BIT PROTECTED MODE", 0x00AA00, _128);
                Text(wx+30, wy+250, "MEM:", 0x555555, _128); Text(wx+130, wy+250, "4 GB ADDRESS SPACE", 0x00AA00, _128);
                
                _30 l_disk[20] = "STORAGE:"; _30 l_net[20] = "NETWORK:"; _30 l_gpu[20] = "GRAPHIC:"; _30 l_usb[20] = "USB HOST:";
                _15(sys_lang NEQ 0) { str_cpy(l_disk, "FESTPLATTE:"); str_cpy(l_net, "NETZWERK:"); str_cpy(l_gpu, "GRAFIK:"); }
                
                /// --- DYNAMISCHE FARBEN FÜR TASTATUR-FOKUS ---
                uint32_t c_st_lbl = 0x555555, c_st_val = 0x0044CC;
                uint32_t c_nt_lbl = 0x555555, c_nt_val = 0x0044CC;
                uint32_t c_gp_lbl = 0x555555, c_gp_val = 0x0044CC;
                uint32_t c_us_lbl = 0x555555, c_us_val = 0x0044CC;
                uint32_t btn_color = 0x444444;

                /// BARE METAL FIX: Index verschoben, Button ist ganz oben!
                if (sys_selected_item == 0) { btn_color = 0xAA0000; }
                if (sys_selected_item == 1) { c_st_lbl = 0xFF0000; c_st_val = 0xFF0000; }
                if (sys_selected_item == 2) { c_nt_lbl = 0xFF0000; c_nt_val = 0xFF0000; }
                if (sys_selected_item == 3) { c_gp_lbl = 0xFF0000; c_gp_val = 0xFF0000; }
                if (sys_selected_item == 4) { c_us_lbl = 0xFF0000; c_us_val = 0xFF0000; }
				/// ==========================================
				/// DER GROSSE ORAKEL-BUTTON (GANZ OBEN)
				/// ==========================================
				_43 btn_scan_x = wx + 20;
				_43 btn_scan_y = wy + 150; /// Wieder nach oben geschoben!
				
				uint32_t btn_oracle_color = 0x444444; 
				/// Wenn er mit den Pfeiltasten angewählt ist (Index 0), leuchtet er Rot!
				if (sys_selected_item == 0) { btn_oracle_color = 0xAA0000; }
				
				DrawRoundedRect(btn_scan_x, btn_scan_y, 250, 30, 4, btn_oracle_color);
				Text(btn_scan_x + 10, btn_scan_y + 8, "OPEN 64-BIT ORACLE", 0xFFFFFF, _128);
				
				/// Klick-Abfrage für die MAUS
				_44 mouse_klick_oracle = (mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, btn_scan_x, btn_scan_y, 250, 30));
				
				_15(input_cooldown EQ 0 AND is_active AND mouse_klick_oracle) {
					/// Wenn die Maus klickt, simuliere einfach einen ENTER-Tastendruck auf Index 0
					sys_selected_item = 0; 
					key_scancode = 0x1C; 
					input_cooldown = 25;
				}

                /// KLICKBAR: STORAGE 
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+30, wy+270, 300, 20)) {
                    scan_pci_class(0x01, hw_storage, "CTRL");
                    input_cooldown = 25;
                }
                Text(wx+30, wy+275, l_disk, c_st_lbl, _128); Text(wx+130, wy+275, hw_storage, c_st_val, _128);
                
                /// KLICKBAR: NETWORK (Hardware Scan)
                /// Breite auf 180 reduziert, damit es nicht mit DHCP kollidiert!
                _44 net_hov = is_over_rect(mouse_x, mouse_y, wx+30, wy+295, 180, 20);
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND net_hov) {
                    scan_pci_class(0x02, hw_net, "NIC");
                    input_cooldown = 25;
                }
                Text(wx+30, wy+300, l_net, c_nt_lbl, _128); 
                Text(wx+130, wy+300, hw_net, net_hov ? 0xFFFFFF : c_nt_val, _128);
                
                /// BARE METAL FIX: DHCP START BUTTON
                /// Die Klickzone ist wx+ww-130, das ist sicher am rechten Rand!
               /// BARE METAL FIX: DHCP START BUTTON
                _44 dhcp_hov = is_over_rect(mouse_x, mouse_y, wx+ww-130, wy+295, 100, 20);
                DrawRoundedRect(wx+ww-130, wy+295, 100, 20, 3, dhcp_hov ? 0xFF8800 : 0xAA5500);
                TextC(wx+ww-80, wy+302, "DHCP REQ", 0xFFFFFF, _128);
                /// NEU: Hier zeichnen wir den Status-String direkt unter den Button!
                TextC(wx+ww-80, wy+325, cmd_status, 0xFF0000, _128);
                
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND dhcp_hov) {
                    str_cpy(cmd_status, "BROADCASTING DHCP DISCOVER...");
                    /// HIER KANNST DU SPÄTER DEN ECHTEN CALL EINFÜGEN!
					net_check_link();
                    send_dhcp_discover();
                    input_cooldown = 25;
                }
                Text(wx+30, wy+300, l_net, c_nt_lbl, _128); Text(wx+130, wy+300, hw_net, c_nt_val, _128);
                
                /// KLICKBAR: GRAPHIC 
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+30, wy+320, 300, 20)) {
                    scan_pci_class(0x03, hw_gpu, "GPU");
                    input_cooldown = 25;
                }
                Text(wx+30, wy+325, l_gpu, c_gp_lbl, _128); Text(wx+130, wy+325, hw_gpu, c_gp_val, _128);
                
                /// KLICKBAR: USB (DAS IST JETZT DIE ZÜNDUNG!)
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+30, wy+345, 300, 20)) {
                    system_init_usb(); 
                    input_cooldown = 25;
                }
                Text(wx+30, wy+350, l_usb, c_us_lbl, _128); Text(wx+130, wy+350, hw_usb, c_us_val, _128);
				/// ==========================================
                /// 5. BARE METAL TASK MANAGER (LIVE-ANZEIGE)
                /// ==========================================
                TextC(mid, wy+380, "LIVE TASK SCHEDULER", 0x000000, _128);
                
                _43 task_y = wy + 400;
                _39(_43 t = 0; t < 4; t++) {
                    _15(tasks[t].active) {
                        char s_id[5]; int_to_str(t, s_id);
                        char* t_name = (char*)((t == 0) ? "COSMOS KERNEL" : "BACKGROUND TASK");
                        
                        /// Ist dieser Task genau in diesem Frame aktiv auf der CPU? -> GRÜN!
                        _89 c_box = (current_task == t) ? 0x00AA00 : 0x555555; 
                        
                        DrawRoundedRect(wx+30, task_y, 290, 20, 3, c_box);
                        Text(wx+40, task_y+4, "TASK", 0xFFFFFF, _128);
                        Text(wx+80, task_y+4, s_id, 0xFFFFFF, _128);
                        Text(wx+110, task_y+4, t_name, 0xFFFFFF, _128);
                        
                        task_y += 25;
                    }
                }
                
                /// 4. POWER BUTTONS
                DrawRoundedRect(wx+30, wy+wh-50, 120, 30, 4, 0xAA0000); TextC(wx+90, wy+wh-40, "REBOOT", 0xFFFFFF, _128);
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+30, wy+wh-50, 120, 30)) { system_reboot(); }
                DrawRoundedRect(wx+ww-150, wy+wh-50, 120, 30, 4, 0x000000); TextC(wx+ww-90, wy+wh-40, "SHUT DOWN", 0xFFFFFF, _128);
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-150, wy+wh-50, 120, 30)) { system_shutdown(); }
            }
			/// =========================================================
            /// APP LAUNCHER (ID 1)
            /// =========================================================
            _15(win->id EQ 1) { 
                TextC(wx+ww/2, wy+40, "INSTALLED APPS", 0x222222, _128);
                
                /// --- ARCADE: PONG ---
                DrawRoundedRect(wx+20, wy+80, 140, 50, 5, 0x00FF00); 
                TextC(wx+90, wy+97, "PLAY PONG", 0x000000, _128); 
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, wy+80, 140, 50)) { 
                    windows[11].open = _128; windows[11].minimized = _86; focus_window(11);
                    pong_state = 0; input_cooldown = 25;
                }

                /// --- ARCADE: BLOBBY VOLLEY ---
                DrawRoundedRect(wx+190, wy+80, 140, 50, 5, 0x00AAFF); 
                TextC(wx+260, wy+97, "PLAY BLOBBY", 0xFFFFFF, _128); 
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+190, wy+80, 140, 50)) { 
                    windows[12].open = _128; windows[12].minimized = _86; focus_window(12);
                    bv_state = 0; input_cooldown = 25;
                }
                
                /// --- NEU: WEB EXPLORER (ID 8) ---
                DrawRoundedRect(wx+20, wy+150, 310, 50, 5, 0xCC5500); 
                TextC(wx+175, wy+167, "WEB EXPLORER", 0xFFFFFF, _128); 
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, wy+150, 310, 50)) { 
                    windows[8].open = _128; 
                    windows[8].minimized = _86; 
                    str_cpy(windows[8].title, "WEB BROWSER"); /// Namen überschreiben
                    focus_window(8);
                    input_cooldown = 25;
                }
            }
			
			/// ==========================================
            /// DISK MANAGER (FENSTER ID 4) - ULTIMATE HDD FIX + LOG VIEW
            /// ==========================================
            _15(win->id EQ 4) {
                _44 is_active = (win_z[12] EQ win->id);
                _89 txt_color = (win->color > 0x888888) ? 0x000000 : 0xFFFFFF;
                uint32_t buf_mbr = 0x00900000;
                uint32_t buf_dir = 0x00901000;
				static _44 is_ntfs_drive = _86;
                static _44 need_ui_refresh = _86;
                static int current_page_offset = 0;
				static uint32_t active_ntfs_folder_lba = 5; /// NEU: Speichert den Ordner sicher ab!
				/// BARE METAL FIX: DAS RAM CLIPBOARD (ZWISCHENABLAGE)
                static _44 clipboard_active = _86;
                static char clipboard_name[12] = {0};
				static _44 clipboard_is_folder = _86; /// NEU! Merkt sich, ob es ein Ordner ist
                
                /// ------------------------------------------
                /// VIEW 2: GEÖFFNETES LAUFWERK (DATEI-EXPLORER)
                /// ------------------------------------------
                _15(dsk_mgr_opened) {
                    DrawRoundedRect(wx+15, wy+45, 180, 55, 4, 0x222222); Text(wx+25, wy+50, "DRIVE CAPACITY:", 0xAAAAAA, _128);
                    char s_cap[10]; int_to_str(drive_total_gb, s_cap); char s_kb[10]; int_to_str(drive_used_kb, s_kb);
                    char* cap_lbl = (char*)((selected_drive_idx == 99) ? "MB TOTAL" : "GB TOTAL");
                    Text(wx+25, wy+65, s_cap, 0x00FF00, _128); Text(wx+55, wy+65, cap_lbl, 0x00FF00, _128);
                    Text(wx+25, wy+80, s_kb, 0xFF8800, _128); Text(wx+55, wy+80, "KB USED", 0xFF8800, _128);
                    
                    /// ==========================================
                    /// BARE METAL FIX: LOG AUSGABE IN VIEW 2!
                    /// ==========================================
                    Text(wx+250, wy+115, "SYSTEM LOG:", 0xAAAAAA, _128);
                    Text(wx+250, wy+130, win->content, txt_color, _86);

                    /// "+ FILE" Button
                    DrawRoundedRect(wx+280, wy+45, 80, 25, 4, 0xAA5500); TextC(wx+320, wy+53, "+ FILE", 0xFFFFFF, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+280, wy+45, 80, 25)) {
                        _39(int i=0; i<512; i++) ((char*)buf_dir)[i] = 0;
                        _15(selected_drive_idx != 99) {
                            active_sata_port = detected_ports[selected_drive_idx];
                            ahci_read_sectors(1002, (uint32_t)buf_dir);
                            _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                            CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)buf_dir;
                            _39(int i=0; i<28; i++) {
                                _15(dir[i].type == 0) {
                                    dir[i].type = 1; dir[i].file_size = 5120; dir[i].start_lba = 10000;
                                    _39(int n=0; n<11; n++) { dir[i].filename[n] = 0; cfs_files[i].name[n] = 0; }
                                    str_cpy(dir[i].filename, "APP.BIN"); 
                                    str_cpy(cfs_files[i].name, "APP.BIN");
                                    
                                    cfs_files[i].start_lba = 10000;
                                    cfs_files[i].size = 5120;
                                    cfs_files[i].is_folder = _86;
                                    
                                    ahci_write_sectors(1002, (uint64_t)buf_dir);
                                    _39(_192 _43 wait2 = 0; wait2 < 1000000; wait2++) __asm__ _192("pause");
                                    
                                    // ==========================================
                                    // BARE METAL FIX: FLUMMI AUF DIE FESTPLATTE BRENNEN!
                                    // ==========================================
                                    uint8_t* app_ram = (uint8_t*)0x09000000;
                                    for(int b=0; b<5120; b++) app_ram[b] = 0;
                                    for(int b=0; b<sizeof(app_bin); b++) app_ram[b] = app_bin[b];
                                    
                                    // 10 Sektoren (5120 Bytes) auf Sektor 10000 schreiben
                                    for(int s=0; s<10; s++) {
                                        ahci_write_sectors(10000 + s, (uint64_t)(app_ram + (s * 512)));
                                        _39(_192 _43 wait3 = 0; wait3 < 100000; wait3++) __asm__ _192("pause");
                                    }
                                    
                                    cfs_files[i].exists = 1;
                                    _37;
                                }
                            }
                        }
                        input_cooldown = 15;
                    }
                    
                    /// ==========================================
                    /// PASTE BUTTON (ZWISCHENABLAGE EINFÜGEN)
                    /// ==========================================
                    _15(clipboard_active) {
                        DrawRoundedRect(wx+370, wy+45, 80, 25, 4, 0x00AA55); 
                        TextC(wx+410, wy+53, "PASTE", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+370, wy+45, 80, 25)) {
                            _15(!is_ntfs_drive) {
                                uint32_t tmp_dir = 0x00901000;
                                /// ZWINGEND NULLEN, sonst gibt es Phantom-Dateien!
                                _39(int k = 0; k < 512; k++) ((char*)tmp_dir)[k] = 0;
                                
                                disk_read_auto(1002, tmp_dir);
                                _39(_192 _43 wait = 0; wait < 100000; wait++) __asm__ _192("pause");
                                
                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)tmp_dir;
                                int free_slot = -1;
                                _39(int s=0; s<28; s++) { if(entries[s].type == 0) { free_slot = s; break; } }
                                
                                _15(free_slot != -1) {
                                    uint32_t target_lba = 4000 + (free_slot * 10);
                                    
                                    _15(!clipboard_is_folder) {
                                        /// DATEI: Daten aus dem RAM auf die Festplatte brennen!
                                        _39(int s=0; s<10; s++) {
                                            disk_write_auto(target_lba + s, 0x09000000 + (s * 512));
                                            _39(_192 _43 wait = 0; wait < 50000; wait++) __asm__ _192("pause");
                                        }
                                        entries[free_slot].type = 1; 
                                        entries[free_slot].file_size = 5120; 
                                        entries[free_slot].start_lba = target_lba;
                                    } _41 {
                                        /// ORDNER: Einfach einen neuen Ordner-Eintrag erstellen!
                                        entries[free_slot].type = 2; 
                                        entries[free_slot].file_size = 0; 
                                        entries[free_slot].start_lba = 0;
                                    }
                                    
                                    entries[free_slot].parent_idx = current_folder_id; 
                                    
                                    _39(int n=0; n<11; n++) entries[free_slot].filename[n] = 0;
                                    str_cpy(entries[free_slot].filename, clipboard_name);
                                    
                                    disk_write_auto(1002, tmp_dir);
                                    _39(_192 _43 wait = 0; wait < 200000; wait++) __asm__ _192("pause");
                                    
                                    print_win(win, "\n[OK] PASTED FROM CLIPBOARD.\n");
                                    need_ui_refresh = _128;
                                    clipboard_active = _86; /// Clipboard leeren nach Einfügen
                                } _41 {
                                    print_win(win, "\n[ERR] FOLDER IS FULL.\n");
                                }
                            } _41 {
                                print_win(win, "\n[ERR] PASTE ONLY ON CFS NOW.\n");
                            }
                            input_cooldown = 25;
                        }
                    }
                    
                    /// ==========================================
                    /// BARE METAL FIX: ORDNER-NAVIGATION & KLICK-ROUTER
                    /// ==========================================
                    _15(current_folder_id NEQ 255) {
                        Text(wx+15, wy+120, "TARGET:", 0xAAAAAA, _128);
                        Text(wx+80, wy+120, cfs_files[current_folder_id].name, 0x00FF00, _128);
                        
                        /// [ BACK ] Button
                        DrawRoundedRect(wx+15, wy+140, 60, 20, 2, 0x444444); Text(wx+20, wy+145, "[ BACK ]", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+15, wy+140, 60, 20)) {
                            current_folder_id = 255; current_page_offset = 0; need_ui_refresh = _128; input_cooldown = 15; active_ntfs_folder_lba = 5;
                        }
                        
                        /// [ PREV ] Button
                        DrawRoundedRect(wx+85, wy+140, 45, 20, 2, 0x444444); Text(wx+92, wy+145, "PREV", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+85, wy+140, 45, 20)) {
                            if (current_page_offset >= 8) current_page_offset -= 8;
                            need_ui_refresh = _128; input_cooldown = 15;
                        }
                        
                        /// [ NEXT ] Button
                        DrawRoundedRect(wx+135, wy+140, 45, 20, 2, 0x444444); Text(wx+142, wy+145, "NEXT", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+135, wy+140, 45, 20)) {
                            current_page_offset += 8;
                            need_ui_refresh = _128; input_cooldown = 15;
                        }

                        /// [ REF ] REFRESH Button
                        DrawRoundedRect(wx+185, wy+140, 45, 20, 2, 0x0055AA); Text(wx+194, wy+145, "REF", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+185, wy+140, 45, 20)) {
                            need_ui_refresh = _128; input_cooldown = 15;
                        }
                    } _41 {
                        Text(wx+15, wy+120, "--- ROOT DIRECTORY ---", 0xFFFFFF, _128);
                        /// BACK-Button ins Hauptmenü
                        DrawRoundedRect(wx+210, wy+45, 60, 25, 4, 0x444444); TextC(wx+240, wy+53, "BACK", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+210, wy+45, 60, 25)) {
                            dsk_mgr_opened = _86; input_cooldown = 15; 
                        }
                    }
                    
                    /// ==========================================
                    /// DER ON-DEMAND RAM SCRAPER (LÖST DEN SPEED BUG!)
                    /// ==========================================
                    _15(is_ntfs_drive AND need_ui_refresh) {
                        /// BARE METAL FIX: Er nutzt nun die sichere Variable, nicht mehr das überschriebene Array!
                        _43 target_mft_id = (current_folder_id EQ 255) ? 5 : active_ntfs_folder_lba;
                        
                        _39(int f=0; f<28; f++) { cfs_files[f].exists = 0; cfs_files[f].name[0] = 0; } 
                        int file_idx = 0; int skipped = 0;
                        uint8_t* mft_cache = (uint8_t*)0x0A000000;
                        
                        _39(int rec = 16; rec < 50000; rec++) {
                            if (file_idx >= 8) break; 
                            
                            uint8_t* mft_rec = mft_cache + (rec * 1024); 
                            
                            if (mft_rec[0] == 'F' && mft_rec[1] == 'I' && mft_rec[2] == 'L' && mft_rec[3] == 'E' && (*(uint16_t*)&mft_rec[22] & 0x01)) {
                                int attr_pos = *(uint16_t*)&mft_rec[20];
                                _114(attr_pos > 0 && attr_pos < 1000) { 
                                    uint32_t attr_type = *(uint32_t*)&mft_rec[attr_pos];
                                    uint32_t attr_len = *(uint32_t*)&mft_rec[attr_pos + 4];
                                    if (attr_type == 0xFFFFFFFF || attr_len <= 0) break; 
                                    
                                    if (attr_type == 0x30 && mft_rec[attr_pos + 8] == 0) {
                                        int fn_base = attr_pos + *(uint16_t*)&mft_rec[attr_pos + 20];
                                        if (fn_base < 0 || fn_base + 80 >= 1024 || *(uint32_t*)&mft_rec[fn_base + 0] != target_mft_id || mft_rec[fn_base + 65] == 2) { attr_pos += attr_len; continue; } 

                                        /// BARE METAL FIX: Ein hartes 'break' überspringt den Sektor sauber!
                                        if (skipped < current_page_offset) { skipped++; break; }

                                        uint8_t name_len = mft_rec[fn_base + 64];
                                        cfs_files[file_idx].exists = 1; cfs_files[file_idx].parent_idx = current_folder_id; 
                                        cfs_files[file_idx].is_folder = (*(uint16_t*)&mft_rec[22] & 0x02) ? 1 : 0;
                                        cfs_files[file_idx].size = *(uint32_t*)&mft_rec[fn_base + 48]; cfs_files[file_idx].start_lba = rec; 

                                        int chars_to_copy = (name_len > 11) ? 11 : name_len;
                                        _39(int c=0; c<chars_to_copy; c++) {
                                            char ch = mft_rec[fn_base + 66 + (c * 2)];
                                            if (ch >= 'a' && ch <= 'z') ch -= 32;
                                            if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '_' || ch == '-' || ch == ' ') { cfs_files[file_idx].name[c] = ch; } 
                                            else { cfs_files[file_idx].name[c] = '_'; }
                                        }
                                        cfs_files[file_idx].name[chars_to_copy] = 0;
                                        file_idx++; 
                                        break; /// Springt direkt zur nächsten Datei
                                    }
                                    attr_pos += attr_len;
                                }
                            }
                        }
                        need_ui_refresh = _86; 
                    }

                    /// ==========================================
                    /// DATEI-LISTE RENDERN & KLICK-ROUTER
                    /// ==========================================
                    _43 y_off = wy + 170; 
                    _39(_43 i=0; i<28; i++) {
                        _15(cfs_files[i].exists AND cfs_files[i].parent_idx EQ current_folder_id) {
                            
                            /// BARE METAL FIX: Hover-Breite von 200 auf 265 erhöht! Jetzt leuchtet auch COPY!
                            _44 is_hov = is_over_rect(mouse_x, mouse_y, wx+15, y_off, 265, 20);
                            _89 icon_col = cfs_files[i].is_folder ? 0xFFAA00 : (is_hov ? 0x00AAFF : 0x0088FF);
                            DrawRoundedRect(wx+15, y_off, 16, 16, 2, icon_col);
                            Text(wx+40, y_off+4, cfs_files[i].name, is_hov ? 0x00FF00 : 0xFFFFFF, _86);
                            
                            /// Der OPEN Button
                            DrawRoundedRect(wx+190, y_off, 40, 16, 2, 0x0055AA);
                            Text(wx+198, y_off+4, "OPEN", 0xFFFFFF, _86);
                            
                            /// Der COPY Button
                            DrawRoundedRect(wx+235, y_off, 40, 16, 2, 0xAA5500);
                            Text(wx+241, y_off+4, "COPY", 0xFFFFFF, _86);
                            
                            /// ==========================================
                            /// KLICK-ROUTER: OPEN BUTTON
                            /// ==========================================
                            _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+190, y_off, 40, 16)) {
                                _15(cfs_files[i].is_folder) {
                                    print_win(win, "\n[SYS] ENTERING FOLDER...\n");
                                    active_ntfs_folder_lba = cfs_files[i].start_lba; 
                                    current_folder_id = i;
                                    current_page_offset = 0; 
                                    need_ui_refresh = _128;
                                } _41 {
                                    _44 is_bin = _86;
                                    _39(int c=0; c<11; c++) { if(cfs_files[i].name[c] == 'B' && cfs_files[i].name[c+1] == 'I' && cfs_files[i].name[c+2] == 'N') is_bin = _128; }
                                    _15(selected_drive_idx != 99) {
										active_sata_port = detected_ports[selected_drive_idx];
										ahci_read_sectors(1002, (uint32_t)buf_dir);
										_39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
									}
                                    _15(is_bin) {
                                        print_win(win, "\n[SYS] LOADING BINARY APP...\n");
                                        
                                        /// BARE METAL FIX: Nur EINMAL laden! 
                                        /// Kein Rebase-Müll, kein doppeltes Überschreiben!
                                        if (load_and_run_bin(cfs_files[i].start_lba, 10)) {
                                            print_win(win, "[SYS] TASK SPAWNED!\n"); 
                                        } else { 
                                            print_win(win, "[ERR] LOAD FAILED!\nGRUND: "); 
                                            print_win(win, cmd_status); 
                                            print_win(win, "\n");
                                        }
                                    } _41 {
                                        print_win(win, "\n[SYS] OPENING IN NOTEPAD...\n");
                                        windows[0].open = _128; windows[0].minimized = _86; focus_window(0);
                                        active_file_lba = cfs_files[i].start_lba; active_file_idx = i;
                                        str_cpy(windows[0].title, cfs_files[i].name);
                                        uint32_t text_ram_addr = 0x03000000; char* text_buffer = (char*)text_ram_addr;
                                        _39(int j=0; j<2000; j++) text_buffer[j] = 0;
                                        
                                        disk_read_auto(active_file_lba, text_ram_addr);
                                        _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                        
                                        int limit = cfs_files[i].size;
                                        if(limit > 2000) limit = 2000; if(limit == 0) limit = 512;
                                        int c_idx = 0;
                                        for(int c=0; c<limit; c++) {
                                            char ch = text_buffer[c]; if(ch == 0) break; 
                                            if((ch >= 32 && ch <= 126) || ch == '\n') { windows[0].content[c_idx++] = ch; }
                                        }
                                        windows[0].content[c_idx] = 0; windows[0].cursor_pos = c_idx;
                                    }
                                }
                                input_cooldown = 25;
                            }
                            
                            /// ==========================================
                            /// KLICK-ROUTER: COPY BUTTON (IN DEN RAM!)
                            /// ==========================================
                            _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+235, y_off, 40, 16)) {
                                str_cpy(win->title, "READING..."); 
                                print_win(win, "\n[SYS] COPY TO RAM CLIPBOARD...\n");
                                
                                /// BARE METAL FIX: Ist das Ziel ein Ordner?
                                clipboard_is_folder = cfs_files[i].is_folder;
                                
                                _15(!clipboard_is_folder) {
                                    uint32_t copy_ram_buffer = 0x09000000; 
                                    _39(int j=0; j<5120; j++) ((char*)copy_ram_buffer)[j] = 0; 
                                    
                                    /// 10 Sektoren von der HDD in den RAM lesen (nur wenn es eine Datei ist)
                                    _39(int s=0; s<10; s++) {
                                        disk_read_auto(cfs_files[i].start_lba + s, copy_ram_buffer + (s * 512));
                                        _39(_192 _43 wait = 0; wait < 50000; wait++) __asm__ _192("pause");
                                    }
                                }
                                
                                /// Namen für das Clipboard merken
                                str_cpy(clipboard_name, cfs_files[i].name);
                                clipboard_active = _128; /// ZACK! Jetzt taucht der Paste-Button auf!
                                
                                str_cpy(win->title, "IN CLIPBOARD");
                                print_win(win, "[OK] READY TO PASTE.\n");
                                input_cooldown = 25;
                            }
                            
                            y_off += 25;
                        }
                    }
                } _41 {
                    /// ------------------------------------------
                    /// VIEW 1: HAUPTMENÜ
                    /// ------------------------------------------
                    _43 list_y = wy + 60;
                    Text(wx+15, list_y - 15, "AVAILABLE DRIVES:", 0xAAAAAA, _128);
                    _39(_43 i=0; i < detected_port_count; i++) {
                        _43 port_num = detected_ports[i];
                        _44 is_sel = (selected_drive_idx == i);
                        DrawRoundedRect(wx+15, list_y, 120, 25, 4, is_sel ? 0x0088FF : 0x333333);
                        _30 d_n[] = "PORT 0"; d_n[5] = '0' + port_num; Text(wx+25, list_y+5, d_n, 0xFFFFFF, _128);
                        
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+15, list_y, 120, 25)) {
                            selected_drive_idx = i; active_sata_port = port_num; is_mounted = false; 
                            HBA_MEM* hba = (HBA_MEM*)active_ahci_bar5; ahci_init_port(&hba->ports[port_num], port_num);
                            input_cooldown = 15;
                        }
                        list_y += 30;
                    }
                    
                    _89 btn_col = (selected_drive_idx == -1) ? 0x444444 : 0x00AA00;
                    DrawRoundedRect(wx+150, wy+60, 80, 25, 4, btn_col); TextC(wx+190, wy+68, "OPEN", 0xFFFFFF, _128);
                    DrawRoundedRect(wx+240, wy+60, 80, 25, 4, (selected_drive_idx == -1) ? 0x444444 : 0xAA0000); TextC(wx+280, wy+68, "FORMAT", 0xFFFFFF, _128);
                    
                    /// FORMATIEREN
                    _15(selected_drive_idx != -1 AND input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+240, wy+60, 80, 25)) {
                        win->cursor_pos = 0; 
                        char* mbr = (char*)buf_mbr; _39(int i=0; i<512; i++) mbr[i] = 0;
                        mbr[3] = 'C'; mbr[4] = 'F'; mbr[5] = 'S'; mbr[510] = 0x55; mbr[511] = 0xAA;         
                        disk_write_auto(0, (uint32_t)buf_mbr); 
                        _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                        
                        char* dir = (char*)buf_dir; _39(int i=0; i<512; i++) dir[i] = 0;
                        disk_write_auto(1002, (uint32_t)buf_dir);
                        _39(_192 _43 wait2 = 0; wait2 < 1000000; wait2++) __asm__ _192("pause");
                        
                        _39(int i=0; i<28; i++) { cfs_files[i].exists = 0; cfs_files[i].is_folder = 0; cfs_files[i].parent_idx = 255; }
                        is_mounted = false; current_folder_id = 255; 
                        print_win(win, "\n[OK] OS2-CFS V2 FORMATTED.\n");
                        input_cooldown = 15;
                    }
                    
                    /// ==========================================
                    /// OPEN DRIVE (UNIVERSAL MOUNT)
                    /// ==========================================
                    _15(selected_drive_idx != -1 AND input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+150, wy+60, 80, 25)) {
                        win->cursor_pos = 0;
                        active_sata_port = detected_ports[selected_drive_idx];
                        
                        ahci_identify((uint32_t)buf_mbr);
                        uint32_t lba_low = *(uint32_t*)(buf_mbr + 200);
                        drive_total_gb = lba_low / 2097152; _15(drive_total_gb == 0) drive_total_gb = 1; 
                        
                        _39(int i=0; i<512; i++) ((char*)buf_mbr)[i] = 0;
                        ahci_read_sectors(0, (uint64_t)buf_mbr);
                        _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                        
                        uint8_t* boot = (uint8_t*)buf_mbr;
                        
                        _39(int i=0; i<28; i++) { cfs_files[i].exists = 0; cfs_files[i].parent_idx = 255; cfs_files[i].is_folder = 0; }
                        is_mounted = true; drive_used_kb = 0;

                        /// FALL 1: NATIVES CFS DATEISYSTEM
                        _15(boot[3] == 'C' && boot[4] == 'F' && boot[5] == 'S') {
                            
                            is_ntfs_drive = _86; /// BARE METAL FIX: Schaltet den NTFS Scraper ab!
                            
                            _39(int i=0; i<512; i++) ((char*)buf_dir)[i] = 0;
                            ahci_read_sectors(1002, (uint64_t)buf_dir);
                            _39(_192 _43 wait2 = 0; wait2 < 1000000; wait2++) __asm__ _192("pause");
                            
                            CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)buf_dir;
                            _39(int i=0; i<28; i++) {
                                _15(dir[i].type != 0) { 
                                    cfs_files[i].exists = 1; 
                                    cfs_files[i].is_folder = (dir[i].type == 2) ? 1 : 0; 
                                    cfs_files[i].parent_idx = dir[i].parent_idx;
                                    _39(int n=0; n<11; n++) { char c = dir[i].filename[n]; cfs_files[i].name[n] = (c >= 32 && c <= 126) ? c : 0; }
                                    cfs_files[i].name[11] = 0;
                                    cfs_files[i].size = dir[i].file_size; 
                                    cfs_files[i].start_lba = dir[i].start_lba;
                                    drive_used_kb += dir[i].file_size; 
                                }
                            }
                            print_win(win, "\n[OK] OS2 CFS V2 MOUNTED.\n");
                        } 
                        /// FALL 2: EASTER EGG
                        _41 _15(boot[0] == 0x50 && boot[1] == 0x4B && boot[2] == 0x03 && boot[3] == 0x04) {
                            print_win(win, "\n[OK] ANDROID APK DETECTED.\n");
                        }
                        /// FALL 3: NTFS CACHE EINLESEN
                        _41 {
                            uint8_t part_type = boot[446 + 4];
                            _39(int i=0; i<28; i++) cfs_files[i].exists = 0;
                            
                            _43 target_ntfs_lba = 0; 
                            
                            _15(boot[510] == 0x55 && boot[511] == 0xAA) {
                                _15(part_type == 0xEE) {
                                    print_win(win, "\n[OK] GPT DRIVE DETECTED.\n");
                                    ahci_read_sectors(1, (uint64_t)buf_dir);
                                    _39(_192 _43 w = 0; w < 500000; w++) __asm__ _192("pause"); 
                                    uint64_t table_lba = *(uint64_t*)(buf_dir + 72);
                                    ahci_read_sectors(table_lba, (uint64_t)buf_dir);
                                    _39(_192 _43 w2 = 0; w2 < 500000; w2++) __asm__ _192("pause"); 
                                    _39(int p=0; p<4; p++) {
                                        uint64_t slba = *(uint64_t*)(buf_dir + (p * 128) + 32);
                                        _15(slba > 0) {
                                            ahci_read_sectors(slba, (uint64_t)buf_mbr);
                                            _39(_192 _43 w3 = 0; w3 < 200000; w3++) __asm__ _192("pause"); 
                                            if (((uint8_t*)buf_mbr)[3]=='N' && ((uint8_t*)buf_mbr)[4]=='T') { target_ntfs_lba = slba; _37; }
                                        }
                                    }
                                } _41 _15(part_type == 0x07 || (boot[3]=='N' && boot[4]=='T')) {
                                    target_ntfs_lba = *(_43*)&boot[446 + 8];
                                }
                            }
                            
                            _15(target_ntfs_lba > 0) {
                                print_win(win, "\n[OK] NTFS VOLUME FOUND!\n");
                                is_ntfs_drive = _128;
                                
                                ahci_read_sectors(target_ntfs_lba, (uint64_t)buf_dir);
                                _39(_192 _43 w = 0; w < 500000; w++) __asm__ _192("pause"); 
                                uint8_t* vbr = (uint8_t*)buf_dir;
                                _43 sec_per_cluster = vbr[13];
                                uint64_t mft_cluster = *(uint64_t*)&vbr[48];
                                uint64_t mft_lba = target_ntfs_lba + (mft_cluster * sec_per_cluster);
                                
                                /// BARE METAL FIX: 0x0A000000 (160 MB) rettet den Heap vor Triple Fault!
                                uint8_t* mft_cache = (uint8_t*)0x0A000000; 
                                print_win(win, "[SYS] CACHING 50,000 MFT RECORDS...\n");
                                
                                _39(int rec = 0; rec < 50000; rec++) {
                                    uint64_t record_lba = mft_lba + (rec * 2);
                                    uint64_t ram_target = (uint64_t)(mft_cache + (rec * 1024));
                                    ahci_read_sectors(record_lba, ram_target);
                                    _39(_192 _43 w1 = 0; w1 < 2000; w1++) __asm__ _192("pause");
                                    ahci_read_sectors(record_lba + 1, ram_target + 512); 
                                    _39(_192 _43 w2 = 0; w2 < 2000; w2++) __asm__ _192("pause");
                                }
                                print_win(win, "[OK] CACHE READY! RAM SPEED UNLOCKED.\n");
                                need_ui_refresh = _128; 
                            } _41 { print_win(win, "\n[ERR] NO VALID NTFS MFT FOUND.\n"); }
                        }
                        
                        current_folder_id = 255; 
                        dsk_mgr_opened = _128;   
                        input_cooldown = 15;
                    }
                }
            }
			/// ==========================================
            /// NOTEPAD (ID 0) - BARE METAL FIX (SICHTBAR!)
            /// ==========================================
            _15(win->id EQ 0) {
                /// BARE METAL FIX: Fokus-Check für Notepad Buttons!
                _44 is_active = (win_z[12] EQ win->id);
                
                /// BARE METAL FIX: Harte Farbe (Weiß) für Text!
                _89 safe_txt_color = 0xFFFFFF; 
                Text(wx+15, wy+45, win->content, safe_txt_color, _86);
                
                /// BARE METAL FIX: Sichtbarer Block-Cursor!
                _15(win_z[12] EQ win->id AND (frame / 20) % 2 EQ 0) {
                    _43 cursor_off_x = 0; _43 cursor_off_y = 0;
                    _39(_43 c_idx = 0; c_idx < win->cursor_pos; c_idx++) { 
                        _15(win->content[c_idx] EQ '\n') { cursor_off_y += 15; cursor_off_x = 0; } 
                        _41 cursor_off_x += 6; 
                    }
                    DrawRoundedRect(wx + 15 + cursor_off_x, wy + 45 + cursor_off_y, 6, 10, 0, safe_txt_color);
                }
                
                /// SAVE BUTTON (Idiotensicher)
                DrawRoundedRect(wx+ww-80, wy+15, 60, 20, 3, 0x005500); TextC(wx+ww-50, wy+21, "SAVE", 0xFFFFFF, _128);
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-80, wy+15, 60, 20)) {
                    str_cpy(win->title, "SAVING..."); 
                    
                    uint32_t file_ram_addr = 0x09000000; char* file_data = (char*)file_ram_addr;
                    for(int i=0; i<512; i++) file_data[i] = 0; 
                    for(int i=0; i < win->cursor_pos; i++) file_data[i] = win->content[i];
                    
                    _43 test_lba = 500; 
                    if(active_file_lba > 0) test_lba = active_file_lba;
                    
                    if(disk_write_auto(test_lba, file_ram_addr)) {
                        str_cpy(win->title, "NOTEPAD - SAVED!");
                    } else {
                        str_cpy(win->title, "NOTEPAD - WRITE ERROR!");
                    }
                    input_cooldown = 25;
                }
                
                /// SAVE AS BUTTON
                DrawRoundedRect(wx+ww-160, wy+15, 75, 20, 3, 0x444444); 
                TextC(wx+ww-122, wy+21, "SAVE AS", 0xFFFFFF, _128);
                _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-160, wy+15, 75, 20)) {
                    windows[2].open = _128; 
                    windows[2].minimized = _86; 
                    focus_window(2); 
                    save_step = 0; 
                    input_cooldown = 25;
                }
            }
			/// =========================================================
            /// BARE METAL FIX: SAVE AS & CREATE FOLDER (ID 2)
            /// =========================================================
            _15(win->id EQ 2) {
                _44 is_active = (win_z[12] EQ win->id);
                /// Feste, sichere DMA RAM-Adressen (64-Bit OS2 Alignments)
                uint32_t buf_dir = 0x00901000;
                uint32_t text_ram_addr = 0x09000000; /// Gleicher Buffer wie beim normalen Save
                _15(save_step EQ 0) { 
                    Text(wx+20, wy+40, "DESTINATION: ACTIVE MOUNTED DRIVE", 0x000000, _128);
                    _15(!is_mounted) {
                        Text(wx+20, wy+70, "NO CFS DRIVE MOUNTED!", 0xFF0000, _128);
                        Text(wx+20, wy+90, "PLEASE OPEN DISK MGR AND MOUNT FIRST.", 0x555555, _128);
                    } _41 {
                        Text(wx+20, wy+70, "DRIVE IS READY. SELECT ACTION:", 0x00AA00, _128);
                        /// CREATE FOLDER BUTTON
                        DrawRoundedRect(wx+20, wy+wh-40, 110, 25, 5, 0xCCCCCC); 
                        TextC(wx+75, wy+wh-32, "NEW FOLDER", 0x000000, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+20, wy+wh-40, 110, 25)) { 
                            save_step = 2; input_cooldown = 25; 
                        }
                        /// NEXT (FILE SAVE) BUTTON
                        DrawRoundedRect(wx+ww-110, wy+wh-40, 90, 25, 5, 0x555555); 
                        TextC(wx+ww-65, wy+wh-32, "NEXT (FILE)", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-110, wy+wh-40, 90, 25)) { 
                            save_step = 1; input_cooldown = 25; 
                        }
                    }
                } _41 _15(save_step EQ 1) { 
                    Text(wx+20, wy+40, "ENTER FILENAME (MAX 11 CHARS):", 0x000000, _128); 
                    DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC); 
                    Text(wx+25, wy+65, save_filename, 0x000000, _128);
                    _15((frame/20)%2 EQ 0) DrawChar(wx+25+(save_name_idx*7), wy+65, '_', 0x000000, _128);
                    DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x00AA00); 
                    TextC(wx+ww-60, wy+wh-32, "SAVE", 0xFFFFFF, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) { 
                        /// 1. TEXT INS RAM KOPIEREN
                        char* file_data = (char*)text_ram_addr;
                        _39(int i=0; i<5120; i++) file_data[i] = 0; 
                        _39(int i=0; i < windows[0].cursor_pos; i++) file_data[i] = windows[0].content[i];
                        /// 2. INHALTSVERZEICHNIS LADEN (Nutzt deinen Universal-Adapter!)
                        disk_read_auto(1002, buf_dir);
                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                        /// 3. FREIEN SLOT SUCHEN
                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)buf_dir;
                        int slot = -1;
                        _39(int i=0; i<28; i++) { _15(entries[i].type EQ 0) { slot = i; _37; } }
                        _15(slot NEQ -1) {
                            /// 4. TEXT AUF FESTPLATTE ODER USB BRENNEN
                            uint32_t target_sec = 4000 + slot;
                            disk_write_auto(target_sec, text_ram_addr);
                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                            
                            /// 5. INHALTSVERZEICHNIS AKTUALISIEREN (V2!)
                            entries[slot].type = 1;
                            _39(int n=0; n<11; n++) entries[slot].filename[n] = 0;
                            str_cpy(entries[slot].filename, save_filename);
                            entries[slot].file_size = 5120; 
                            entries[slot].start_lba = target_sec;
                            
                            /// BARE METAL FIX: Auf die Platte brennen, wem die Datei gehört!
                            entries[slot].parent_idx = current_folder_id; 
                            
                            disk_write_auto(1002, buf_dir);
                            _39(_192 _43 wait2=0; wait2<500000; wait2++) __asm__ _192("pause");
                            
                            /// 6. LIVE UI UPDATE
                            cfs_files[slot].exists = 1;
                            cfs_files[slot].is_folder = 0;
                            cfs_files[slot].parent_idx = current_folder_id;
                            str_cpy(cfs_files[slot].name, save_filename);
                            cfs_files[slot].size = 5120;
                            cfs_files[slot].start_lba = target_sec;
                            
                            active_file_lba = target_sec;
                            active_file_idx = slot;
                            
                            str_cpy(windows[0].title, "SAVED SUCCESS!");
                        } _41 {
                            str_cpy(windows[0].title, "ERR: ROOT FULL!");
                        }
                        win->open = _86; 
                        input_cooldown = 25; 
                    }
                } _41 _15(save_step EQ 2) { 
                    Text(wx+20, wy+40, "FOLDER NAME (MAX 11 CHARS):", 0x000000, _128); 
                    DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC); 
                    Text(wx+25, wy+65, new_folder_name, 0x000000, _128);
                    _15((frame/20)%2 EQ 0) DrawChar(wx+25+(folder_name_idx*7), wy+65, '_', 0x000000, _128);
                    DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x0055AA); 
                    TextC(wx+ww-60, wy+wh-32, "CREATE", 0xFFFFFF, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_active AND is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) { 
                        /// 1. INHALTSVERZEICHNIS LADEN
                        disk_read_auto(1002, buf_dir);
                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)buf_dir;
                        int slot = -1;
                        _39(int i=0; i<28; i++) { _15(entries[i].type EQ 0) { slot = i; _37; } }
                        _15(slot NEQ -1) {
                            /// 2. ORDNER-EINTRAG SCHREIBEN (V2!)
                            entries[slot].type = 2; /// 2 = FOLDER
                            _39(int n=0; n<11; n++) entries[slot].filename[n] = 0;
                            str_cpy(entries[slot].filename, new_folder_name);
                            entries[slot].file_size = 0;
                            entries[slot].start_lba = 0; 
                            
                            /// BARE METAL FIX: Ordner-Hierarchie auf Platte sichern!
                            entries[slot].parent_idx = current_folder_id; 
                            
                            disk_write_auto(1002, buf_dir);
                            _39(_192 _43 wait2=0; wait2<500000; wait2++) __asm__ _192("pause");
                            
                            /// 3. UI UPDATE
                            cfs_files[slot].exists = 1;
                            cfs_files[slot].is_folder = 1;
                            cfs_files[slot].parent_idx = current_folder_id;
                            str_cpy(cfs_files[slot].name, new_folder_name);
                            cfs_files[slot].size = 0;
                            cfs_files[slot].start_lba = 0;
                            
                            str_cpy(windows[0].title, "FOLDER CREATED!");
                        } _41 {
                            str_cpy(windows[0].title, "ERR: ROOT FULL!");
                        }
                        win->open = _86; 
                        input_cooldown = 25; 
                    }
                }
            }
            /// ==========================================
            /// CMD (ID 5) - BARE METAL FIX (SICHTBAR!)
            /// ==========================================
            _15(win->id EQ 5) {
                _89 cmd_color = 0x00FF00; /// Hacker-Grün
                Text(wx+15, wy+45, win->content, cmd_color, _86);
                
                _43 lines = 0; _39(_43 i=0; i<win->cursor_pos; i++) { if(win->content[i] == '\n') lines++; }
                _43 prompt_y = wy + 45 + (lines * 15);
                
                Text(wx+15, prompt_y, "C:\\> ", cmd_color, _128);
                Text(wx+55, prompt_y, cmd_input_buf, cmd_color, _128);
                
                /// ==========================================
                /// TASTATUR-EINGABE & EXECUTION (Nur wenn aktiv)
                /// ==========================================
                _15(win_z[12] EQ win->id AND key_new) {
                    _30 c = last_app_key;
                    
                    /// 1. BACKSPACE (Löschen)
                    _15(c EQ '\b' OR c EQ 8) {
                        _15(cmd_input_idx > 0) {
                            cmd_input_idx--;
                            cmd_input_buf[cmd_input_idx] = 0;
                        }
                    }
                    /// 2. ENTER (Befehl abschicken!)
                    _41 _15(c EQ '\n' OR c EQ '\r') {
                        _15(cmd_input_idx > 0) {
                            /// Unseren eingebauten "Fake-Befehl" mit Newline versehen
                            _30 exec_cmd[80];
                            str_cpy(exec_cmd, cmd_input_buf);
                            str_cat(exec_cmd, "\n");
                            
                            /// ENGINE ZÜNDEN!
                            run_cosmos_script(exec_cmd, cmd_input_idx + 1);
                            
                            /// Eingabezeile für den nächsten Befehl leeren
                            cmd_input_idx = 0;
                            cmd_input_buf[0] = 0;
                        }
                    }
                    /// 3. NORMALE ZEICHEN TIPPEN
                    _41 _15(c >= 32 AND c <= 126) {
                        _15(cmd_input_idx < 60) {
                            cmd_input_buf[cmd_input_idx++] = c;
                            cmd_input_buf[cmd_input_idx] = 0;
                        }
                    }
                }
            }
        }
		// =========================================================================
		// 4. BARE METAL FIX: DER APP-PUFFER MUSS HIER HIN! (Nach den Fenstern!)
		// =========================================================================
		if (app_window_active) {
			uint32_t win_start_x = 100; // Position des Fensters auf dem Desktop
			uint32_t win_start_y = 100;
			
			for (int y = 0; y < 50; y++) {
				for (int x = 0; x < 50; x++) {
					// Den flüchtigen RAM auslesen (wird von app.bin befüllt)
					uint32_t pixel_color = app_window_buffer[y * 50 + x];
					
					// 0x000000 ist transparent! Wenn es eine andere Farbe ist, zeichnen wir!
					if (pixel_color != 0) { 
						Put(win_start_x + x, win_start_y + y, pixel_color);
					}
				}
			}
		}
        DrawAeroCursor(mouse_x, mouse_y);
        Swap(); 
        frame++;
    }
}