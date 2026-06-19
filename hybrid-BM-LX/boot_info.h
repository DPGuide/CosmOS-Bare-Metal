#pragma once
#include <stdint.h>

struct BootInfo {
    uint32_t framebuffer_addr;
    uint32_t screen_width;
    uint32_t screen_height;
    
    uint32_t framebuffer_pitch; 
    
    uint32_t ahci_drive_count;
    uint32_t drives_list_ptr;

    uint32_t uhci_base_port;
    uint32_t ac97_bus_port;
    uint32_t net_mac_addr_low;
    uint32_t net_mac_addr_high;

    /// === NEU: SCHRIFTART ===
    uint32_t font_ptr;  /// Zeiger auf den 8x16 (oder 8x8) Font-Array
    
    /// === NEU: RAMDISK/CARTRIDGE ===
    uint32_t cartridge_start;
    uint32_t cartridge_end;
} __attribute__((packed));