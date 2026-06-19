#pragma once
#include "schneider_lang.h"

/// ==========================================
/// DIE UNIVERSAL-HYPERVISOR ABSTRAKTION
/// ==========================================

struct VirtualMachine {
    const char* system_name;       
    const char* file_extension;    
    void (*reset)();                                         
    void (*load_rom)(unsigned long long ram_addr, int size); 
    void (*run_frame)();                                     
    void (*render)(_43 wx, _43 wy, _43 ww, _43 wh);
    void (*handle_input)(int scancode, bool is_pressed);     
};

// NUR ANKÜNDIGEN (extern), nicht hier mit Daten füllen!
extern VirtualMachine emu_chip8;
extern VirtualMachine emu_gameboy;
extern VirtualMachine* active_vm;

/// Die Brücke vom Kernel zur Arcade-Engine
_172 _50 run_pong_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked);
_172 _50 run_blobby_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked);
/// Der Mini Web-Browser
_172 _50 run_browser_engine(_43 wx, _43 wy, _43 ww, _43 wh, _44 blocked);

/// Status-Variablen für das APPS-Fenster freigeben (Reset-Trigger)
_172 _43 pong_state;
_172 _43 bv_state;