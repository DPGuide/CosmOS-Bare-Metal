#include <stdint.h>

extern "C" __attribute__((section(".text.entry"))) void app_main() {
    // 1. DIE MAGISCHE SIGNATUR
    __asm__ volatile("nop; nop; nop; nop;");
    
    // 2. RAM-FENSTER VOM KERNEL ANFORDERN (Syscall 5 = Fullscreen Overlay)
    // Die Adresse eures 800x600 Puffers landet in "window_buffer"
    volatile uint32_t* window_buffer;
    __asm__ volatile("mov $5, %%rax \n int $0x80" : "=a"(window_buffer) : : "memory");
    
    // 3. FLUMMI VARIABLEN (Laser Punkt)
    long long x = 400;
    long long y = 300;
    long long dx = 2;
    long long dy = 2;
    uint32_t color = 0xFF0000; /// Laser-Rot!
    
    // Beethoven's 5th Symphony Notes (Frequencies)
    // G4, G4, G4, Eb4, F4, F4, F4, D4
    uint64_t notes[] = { 392, 392, 392, 311, 0, 349, 349, 349, 293, 0 };
    // Durations in "ticks" (1 tick = ~2ms because of yield)
    uint64_t durations[] = { 100, 100, 100, 400, 200, 100, 100, 100, 400, 200 };
    
    int current_note = 0;
    int note_timer = 0;
    
    // Start first note
    if (notes[0] != 0) {
        __asm__ volatile("mov $7, %%rax \n mov %0, %%rdi \n int $0x80" : : "r"(notes[0]) : "rax", "rdi", "memory");
    }
    
    while(1) {
        
        x += dx;
        y += dy;
        
        // Wand-Kollision (Der Puffer ist 800x600 Pixel groß!)
        if (x <= 5) dx = 2;
        if (x >= 795) dx = -2;
        if (y <= 5) dy = 2;
        if (y >= 595) dy = -2;
        
        // Neuen Flummi (Laserpunkt) in den Puffer zeichnen
        // Ein kleiner 4x4 Punkt
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                window_buffer[(y + py) * 800 + (x + px)] = color;
            }
        }
        
        // Musik-Logik:
        note_timer++;
        if (note_timer > durations[current_note]) { 
            note_timer = 0;
            current_note++;
            if (current_note >= 10) {
                current_note = 0; // Endlosschleife
            }
            
            // Note spielen
            if (notes[current_note] == 0) {
                // Pause
                __asm__ volatile("mov $7, %%rax \n mov $0, %%rdi \n int $0x80" : : : "rax", "rdi", "memory");
            } else {
                // Ton
                __asm__ volatile("mov $7, %%rax \n mov %0, %%rdi \n int $0x80" : : "r"(notes[current_note]) : "rax", "rdi", "memory");
            }
        }
        
        // BARE METAL FIX: Keine volatile Bremse mehr!
        // Wir nutzen ausschließlich OS-Ticks durch Yield, das ist zu 100% konstant!
        
        // Yield (Syscall 0)
        __asm__ volatile("mov $0, %%rax \n int $0x80" : : : "rax", "memory");
    }
}
