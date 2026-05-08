///* COSMOS OS   -   SCHNEIDER v2.2 ULTIMATE FIX */
///* ARCHITECT: TOBIAS SCHNEIDER | TARGET: BARE METAL */
#include <stdint.h>
#include "schneider_lang.h"
#include "pci.h"
#include "net.h"
#include "cosmos_bytes.h"
#include "cosmos_fs.h"
#include "cosmos_ahci.h" 
#include "cosmos_cfs.h"
#include "boot_info.h"
#define SCAN_UP    0x48
#define SCAN_DOWN  0x50
#define SCAN_ENTER 0x1C
/// ==========================================
/// BARE METAL: HEAP ALLOCATOR (MEMORY MANAGER)
/// ==========================================
MemoryBlock* heap_head = nullptr;

void init_heap() {
    heap_head = (MemoryBlock*)0x02000000; /// Start-Adresse (32 MB Mark)
    heap_head->size = 1024 * 1024 * 32;   /// 32 Megabyte reservieren
    heap_head->is_free = 1;
    heap_head->next = nullptr;
}

void* malloc(size_t size) {
    MemoryBlock* curr = heap_head;
    while (curr != nullptr) {
        if (curr->is_free == 1 && curr->size >= size) {
            curr->is_free = 0;
            return (void*)((uint8_t*)curr + sizeof(MemoryBlock));
        }
        curr = curr->next;
    }
    return nullptr;
}

void free(void* ptr) {
    if (ptr == nullptr) return;
    MemoryBlock* block = (MemoryBlock*)((uint8_t*)ptr - sizeof(MemoryBlock));
    block->is_free = 1;
}

void* operator new(size_t size) { return malloc(size); }
void* operator new[](size_t size) { return malloc(size); }
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t size) noexcept { free(ptr); }
/// BARE METAL FIX: Globale Ports für die AC97 Soundkarte
_182 ac97_mixer_port = 0;
_182 ac97_bus_port = 0;
_182 uhci_base_port = 0;
_172 _50 net_check_link();
_172 _44 ahci_read_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);
_172 _44 ahci_write_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);
_172 _50 iso9660_load_file(_43 port_no, _71 _30* target_filename);
_172 _43 usb_bot_get_capacity(_43 dev_addr, _43 ep_out, _43 ep_in);
_172 _44 usb_enumerate_device(_43 port_idx, _43 new_address);
_172 _50 analyze_mbr(_184* buffer);
_172 _50 analyze_gpt(_184* buffer);
_172 _50 analyze_gpt_entries(_184* buffer);
_172 _50 analyze_vbr(_184* buffer);
_172 _50 analyze_mft_root(_184* buffer);
_172 _50 analyze_indx_block(_184* buffer);
_172 _50 fat32_read_root_dir(_43 port_no);
_172 _89 sec_per_cluster;
_172 _89 indx_start_lba;
_89 mft_start_lba = 0;
_89 gpt_partition_lba = 0;
_44 is_ntfs = 0;
/// TBA Packer Hook
_172 _50 tba_record_frame(_182 sensor_val, _184 r, _184 g, _184 b, _184 a);
_172 HBA_PORT* get_active_ahci_port();
_172 _43 ahci_scan_state;
_172 _50 ahci_mount_drive();
_172 _50 ahci_panzer_scan_tick();
_172 _89 global_ahci_abar;
/// Die Datenstruktur für unsere Festplatten (Der Bauplan)
_202 DriveInfo { _43 type; _43 size_mb; _43 base_port; _30 model[41]; };
_172 DriveInfo drives[8];
/// --- SPEICHER FÜR DIE ROTATIONS-LOGIK ---
_30 hw_storage_list[4][48]; /// Platz für 4 Festplatten-Einträge
_43 storage_count = 0;
_43 current_storage_idx = 0;
_30 hw_gpu_list[2][48];     /// Platz für 2 Grafikkarten (z.B. Intel + Nvidia)
_43 gpu_count = 0;
_43 current_gpu_idx = 0;
_30 hw_storage[48] = "STORAGE: PRESS SCAN";
_30 hw_gpu[48]     = "GPU: PRESS SCAN";
_30 hw_usb[48]     = "USB: PRESS SCAN";
/// ==========================================
/// SYSTEM HELPER FUNKTIONEN
/// ==========================================
_50 str_cat(_30* dest, _71 _30* src) {
    _43 i = 0;
    _114(dest[i] NEQ 0) i++; 
    _43 j = 0;
    _114(src[j] NEQ 0) { dest[i++] = src[j++]; } 
    dest[i] = 0;
}
_50 execute_kernel();
_43 system_timer_ticks = 0;
_43 get_system_ticks() { 
    _96 system_timer_ticks++; 
}
_172 _50 pci_scan_all();
_172 _50 pci_write(_184 bus, _184 slot, _184 func, _184 offset, _89 value); 
_172 _50 tba_ingest_live_frame(_184* live_frame_26, HBA_PORT* port);
/// ==========================================
/// 1. DATA STRUCTURES
/// ==========================================
_202 Window { _43 id; _30 title[16]; _43 x, y, w, h; _44 open, minimized, fullscreen; _89 color; };
_202 Planet { _43 ang; _43 dist; _30 name[8]; _43 cur_x, cur_y; }; 
_202 Star { _43 x, y, z, type, speed; };
/// ==========================================
/// BARE METAL PLUG & PLAY: Hardware Registry
/// ==========================================
_89 os2_ram_address = 0;

/// Wir erstellen eine globale Instanz, auf die alle Treiber zugreifen können
HardwareRegistry sys_hw;
/// ==========================================
/// Das Spiegel-System (Geräte-Manager)
/// ==========================================
_202 MirrorDevice { 
    _184 bus, dev, func; 
    _182 vendor, device; 
    _89 bar0;
    _30 name[32]; 
};
MirrorDevice mirror_list[20];
_43 mirror_count = 0;
_44 mirror_scanned = _86; 
/// ==========================================
/// 2. GLOBAL VARIABLES
/// ==========================================
_89 ahci_found_ports_bitmask = 0;
_30 mbr_info_text[50] = "WAITING FOR SCAN...";
_89 mbr_start_lba = 0;
_172 _50 fs_format_drive();
_172 _43 active_drive_idx;
_172 _43 drive_count;
_72 _43 frame = 0;
_43 active_mode = 0, boot_frame = 0;
_43 galaxy_expansion = 0; _44 galaxy_open = _86;
_43 rtc_h, rtc_m, rtc_day, rtc_mon, rtc_year;
_30 user_name[32] = "SCHNEIDER"; _30 ip_address[32] = "SEARCHING..."; _30 cpu_brand[49];
_172 _30 net_mask[32]; _172 _30 gateway_ip[32];
_43 mouse_x=400, mouse_y=300; _184 mouse_cycle=0; int8_t mouse_byte[3]; 
_44 mouse_left=_86, mouse_just_pressed=_86; _184 last_key = 0; _44 key_new = _86; _44 key_shift = _86;
_43 current_open_file = -1;  /// -1 bedeutet: Kein File offen
_172 _50 ahci_read_mbr();
/// BARE METAL: WAV HEADER (RIFF/WAVE Format)
struct WavHeader {
    uint8_t  riff_id[4];      /// "RIFF"
    uint32_t file_size;       /// Dateigröße - 8
    uint8_t  wave_id[4];      /// "WAVE"
    uint8_t  fmt_id[4];       /// "fmt "
    uint32_t fmt_size;        /// 16 für PCM
    uint16_t audio_format;    /// 1 = unkomprimiertes PCM
    uint16_t num_channels;    /// 1 = Mono, 2 = Stereo
    uint32_t sample_rate;     /// z.B. 44100 oder 48000
    uint32_t byte_rate;       /// sample_rate * num_channels * 2
    uint16_t block_align;     /// num_channels * 2
    uint16_t bits_per_sample; /// 16
    uint8_t  data_id[4];      /// "data"
    uint32_t data_size;       /// Reine Größe der Audiodaten
} __attribute__((packed));
/// WINDOWS
_43 win_z[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
Window windows[13]; Planet planets[6]; Star stars[200]; _43 senses_tab = 0;
_43 drag_win = -1; _43 drag_off_x = 0; _43 drag_off_y = 0; _43 resize_win = -1; _44 z_blocked = _86;
_30 note_buf[10][41]; _43 note_r=0, note_c=0; _30 save_timestamp[32] = ""; 
_30 cmd_input[41] = {0}; _43 cmd_idx = 0; _30 cmd_last_out[128] = "READY."; _30 cmd_lines[5][41]; _30 cmd_status[32] = "OFFLINE"; 
/// HARDWARE
_44 touch_mode = _86; 
_44 webcam_active = _86; _44 logitech_found = _86; _30 webcam_model[40] = "NO WEBCAM";
_43 wifi_state = 0; _43 wifi_selected = -1; _30 wifi_pass[32] = {0}; _43 wifi_pass_idx = 0;
_30 wifi_ssids[4][20] = {"NO SIGNAL", "NO SIGNAL", "NO SIGNAL", "NO SIGNAL"}; 
_44 usb_detected = _86; 
/// FS & NAV
_30 hw_net[48] = "SCANNING...";
_172 _30 hw_disk[48]; 
_43 dsk_view = 0; _43 dsk_selection = 0; _43 file_selection = -1; _43 input_cooldown = 0;
_43 save_step = 0; _43 save_part_sel = 0; _30 save_filename[32] = "blank.txt"; _43 save_name_idx = 9;
_30 new_folder_name[32] = "new_dir"; _43 folder_name_idx = 7;
_184 current_path_id = 255; _43 current_folder_view_idx = -1; 
/// DRIVERS
_89 rtl_io_base = 0; _89 intel_mem_base = 0; _89 usb_io_base = 0;
_184 mac_addr[6] = {0,0,0,0,0,0}; _30 mac_str[24] = "00:00:00:00:00:00";
NICInfo found_nics[5]; _43 nic_count = 0; _43 active_nic_idx = -1; _182 ata_base = 0x1F0; 
_184* tx_buffer = (_184*)0x500000; _43 tx_cur = 0; 
_184* rx_buffer_rtl = (_184*)0x600000; _43 rx_idx_rtl = 0;
_43 sleep_mode_idx = 0; _43 sys_menu_selection = 0; _30 sleep_labels[6][16];
_43 shake_timer = 0, sx_off = 0, sy_off = 0; 
_43 sys_lang = 0; _43 sys_theme = 0; _43 genesis_tab = 0;
_184 telemetry_block[256];
_43 pulse_timer = 0;
/// --- BARE METAL PONG ENGINE ---
_43 pong_state = 0; 
_43 pong_mode = 0; /// 0 = 1 Player, 1 = 2 Players
_43 pong_diff = 1; /// 0 = Easy, 1 = Normal, 2 = Hard
_43 pong_p1_y = 100, pong_p2_y = 100;
_43 pong_ball_x = 200, pong_ball_y = 150;
_43 pong_vel_x = 5, pong_vel_y = 3;
_43 pong_score1 = 0, pong_score2 = 0;
/// --- BARE METAL BLOBBY VOLLEY ENGINE ---
_43 bv_state = 0, bv_mode = 0, bv_diff = 1;
_43 bv_p1_x = 50, bv_p1_y = 200, bv_p1_vy = 0;
_43 bv_p2_x = 350, bv_p2_y = 200, bv_p2_vy = 0;
_43 bv_ball_x = 100, bv_ball_y = 50, bv_ball_vx = 0, bv_ball_vy = 0;
_43 bv_score1 = 0, bv_score2 = 0;
/// BARE METAL FIX: Die 3-Touch Regel!
_43 bv_touches = 0, bv_last_touch = 0;
_50 str_cpy(_30* d, _71 _30* s);
/// NOTEPAD TAB SYSTEM
_43 tab_files[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; /// Welche Datei liegt wo?
_30 tab_buffers[10][10][40]; /// Eigener Speicherbereich für jeden der 10 Tabs
_43 active_tab = 0;          /// Welcher Tab ist gerade offen?
_50 outw(_182 p, _182 v) { _33 _192("outw %0, %1"::"a"(v),"Nd"(p)); }
/// ==========================================
/// BARE METAL ORAKEL (DRIVERS.cfg)
/// ==========================================
extern _50 wait_hardware(_89 cycles);
extern _50 pcie_write_mmio(_89 bus, _89 dev, _89 func, _89 offset, _89 value);
extern _50 init_acpi_and_pcie();
extern _89 pcie_base_addr;
extern _89 pcie_read_mmio(_89 bus, _89 dev, _89 func, _89 offset);
/// BARE METAL: Der Hybrid-Leser (Die ultimative Brücke zwischen 1996 und heute)
_89 sys_pci_read(_89 b, _89 d, _89 f, _89 offset) {
    /// Wenn ein modernes Mainboard da ist, nutze den PCIe-Röntgenblick!
    _15(pcie_base_addr NEQ 0) {
        _96 pcie_read_mmio(b, d, f, offset);
    }
    /// Wenn es QEMU oder ein alter PC ist, nutze die klassischen Legacy-Ports!
    _96 pci_read(b, d, f, offset); 
}
_202 OracleEntry { _30 type[8]; _182 vendor; _182 device; _89 bar_addr; };
/// Kein "_172"! Hier werden sie ECHT in den RAM gebaut.
OracleEntry oracle_db[64];
_43 oracle_entry_count = 0;
_50 ahci_save_file_complete(HBA_PORT* hdd, _71 _30* filename, _184* data_buf) {
    _184* dir_buf = (_184*)0x802000; /// Zweiter sicherer DMA-Puffer für das Inhaltsverzeichnis
    /// 1. Inhaltsverzeichnis lesen (Sektor 1002)
    ahci_read(hdd, 1002, dir_buf);
    CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_buf;
    /// 2. Schauen, ob die Datei schon existiert (überschreiben) oder leeren Slot suchen
    _43 slot = 255;
    _39(_43 i=0; i<8; i++) {
        _15(entries[i].type EQ 1) {
            _44 match = _128;
            _39(_43 c=0; c<12; c++) { _15(entries[i].filename[c] NEQ filename[c] AND filename[c] NEQ 0) match = _86; }
            _15(match) { slot = i; _37; }
        }
    }
    _15(slot EQ 255) {
        /// Neue Datei: Finde den ersten leeren Slot
        _39(_43 i=0; i<8; i++) { _15(entries[i].type EQ 0) { slot = i; _37; } }
    }
    _15(slot EQ 255) _96;
    /// 3. Daten auf die Festplatte brennen (Wir nutzen Sektor 4000 + den Slot-Index)
    _89 target_sec = 4000 + slot;
    ahci_write(hdd, target_sec, data_buf);
    /// 4. Inhaltsverzeichnis aktualisieren...
    entries[slot].type = 1;
    str_cpy(entries[slot].filename, filename);
    entries[slot].file_size = 512;
    entries[slot].start_lba = target_sec;
    ahci_write(hdd, 1002, dir_buf); 
}
_172 _89 inl(_182 p);
_172 _50 outl(_182 p, _89 v);
/// ==========================================
/// BARE METAL FIX: USB DIAGNOSE SCANNER
/// ==========================================
_50 pci_find_uhci() {
    _43 found_any_usb = 0;
    _39(_43 bus = 0; bus < 256; bus++) {
        _39(_43 slot = 0; slot < 32; slot++) {
            _39(_43 func = 0; func < 8; func++) {
                _89 address = (_89)((bus << 16) | (slot << 11) | (func << 8) | 0x80000000);
                outl(0xCF8, address);
                _89 vendor = inl(0xCFC);
                _15((vendor & 0xFFFF) EQ 0xFFFF) _37;
                outl(0xCF8, address + 0x08);
                _89 class_info = inl(0xCFC);
                _89 class_code = (class_info >> 24) & 0xFF;
                _89 subclass   = (class_info >> 16) & 0xFF;
                _89 prog_if    = (class_info >> 8) & 0xFF;
                _15(class_code EQ 0x0C AND subclass EQ 0x03) {
                    found_any_usb = 1;
                    _15(prog_if EQ 0x00) {
                        /// UHCI gefunden! (Unser aktueller Treiber funktioniert hiermit)
                        outl(0xCF8, address + 0x20);
                        usb_io_base = inl(0xCFC) & ~0x03;
                        /// Bus Mastering
                        outl(0xCF8, address + 0x04);
                        _89 cmd = inl(0xCFC);
                        outl(0xCF8, address + 0x04);
                        outl(0xCFC, cmd | 0x0004);
                        _96;
                    }
                    _15(prog_if EQ 0x10) str_cpy(hw_usb, "FOUND OHCI (UNSUPPORTED)");
                    _15(prog_if EQ 0x20) str_cpy(hw_usb, "FOUND EHCI (USB 2.0)");
                    _15(prog_if EQ 0x30) str_cpy(hw_usb, "FOUND xHCI (USB 3.0+)");
                }
            }
        }
    }
    _15(!found_any_usb) str_cpy(hw_usb, "NO USB HARDWARE ON PCI");
}
_182 inw(_182 p);
_50 usb_mount_drive() {
    /// 1. Zuerst das Mainboard scannen!
    pci_find_uhci();
    /// 2. Jetzt erst prüfen, ob wir was gefunden haben
    _15(usb_io_base EQ 0) {
        str_cpy(hw_usb, "NO UHCI CONTROLLER");
        _96; 
    }
    _43 usb_found = 0;
    /// Wir scannen Port 1 (0x10) und Port 2 (0x12) des Mainboards
    _39(_43 p = 0; p < 2; p++) {
        _182 port_reg = usb_io_base + 0x10 + (p * 2);
        _182 status = inw(port_reg);
        /// Bit 0: Ist physisch ein Gerät eingesteckt?
        _15(status & 0x0001) {
            /// Port 0 bekommt Adresse 1, Port 1 bekommt Adresse 2
            _43 dev_addr = p + 1;
            _15(usb_enumerate_device(p, dev_addr)) {
                _15(drive_count < 8) {
                    drives[drive_count].type = 3;         
                    drives[drive_count].base_port = dev_addr;  
                    drives[drive_count].size_mb = usb_bot_get_capacity(dev_addr, 2, 129);
                    _30 name_buf[40] = "USB DEVICE ON PORT ";
                    _30 p_str[2]; p_str[0] = '1' + p; p_str[1] = 0;
                    str_cat(name_buf, p_str);
                    str_cpy(drives[drive_count].model, name_buf);
                    drive_count++;
                    usb_found++;
                }
            } _41 {
                str_cpy(hw_usb, "USB HANDSHAKE FAILED");
            }
        }
    }
    _15(usb_found > 0) {
        str_cpy(hw_usb, "CLICK TO CYCLE STICKS");
    } _41 {
        str_cpy(hw_usb, "PORTS EMPTY");
    }
}
/// ==========================================
/// BARE METAL: UHCI HARDWARE STRUKTUREN
/// ==========================================
struct UHCI_TD {
    uint32_t link_pointer;     /// Zeiger auf den nächsten Befehl (oder 1 für "Ende")
    uint32_t control_status;   /// Status (Aktiv, Fehler, Länge)
    uint32_t token;            /// Was soll passieren? (IN, OUT, SETUP, Endpoint, Adresse)
    uint32_t buffer_pointer;   /// Wo im RAM liegen die Daten?
    uint32_t reserved[4];      /// Auffüllen auf 32 Bytes für sauberes Alignment
} __attribute__((packed, aligned(16)));
struct UHCI_QH {
    uint32_t head_link;
    uint32_t element_link;
} __attribute__((packed, aligned(16)));
/// ==========================================
/// BARE METAL: UHCI BULK TRANSFER (SYNCHRON)
/// ==========================================
_43 uhci_bulk_transfer(_43 dev_addr, _43 endpoint, _43 is_in, _89 buffer_addr, _43 length) {
    _15(uhci_base_port EQ 0) _96 1; /// ERROR: Kein USB Controller!

    /// 1. Wir reservieren feste RAM-Adressen (DMA-sicher)
    _89 td_addr = 0x0C000000; /// Hier liegt der Transfer Descriptor
    UHCI_TD* td = (UHCI_TD*)td_addr;

    /// 2. Den TD zusammenbauen
    td->link_pointer = 1; /// Bit 0 = 1 bedeutet "Letzter Befehl in der Liste" (Terminate)
    
    /// Status: Setze "Active" Bit (Bit 23) und Interrupt on Complete (Bit 24)
    td->control_status = (1 << 23) | (1 << 24);
    
    /// Token berechnen: 
    /// Packet Identification (PID): 0x69 = IN (Lesen), 0xE1 = OUT (Schreiben)
    _43 pid = is_in ? 0x69 : 0xE1;
    
    /// Das Token setzt sich zusammen aus: Länge (Bit 21-31), Endpoint (Bit 15-18), Adresse (Bit 8-14), PID (Bit 0-7)
    _43 max_len = (length - 1) & 0x7FF; /// UHCI Besonderheit: Länge wird als (Länge - 1) gespeichert
    td->token = (max_len << 21) | (endpoint << 15) | (dev_addr << 8) | pid;
    
    /// Daten-Pointer setzen
    td->buffer_pointer = buffer_addr;

    /// 3. Dem Controller die Liste unterschieben!
    /// Wir biegen den Frame-List-Pointer der Hardware direkt auf unseren TD um
    outl(uhci_base_port + 0x08, td_addr);

    /// 4. Controller FEUER! (Run/Stop Bit auf 1 setzen)
    outw(uhci_base_port + 0x00, 1);

    /// 5. Warten, bis das "Active" Bit (Bit 23) auf 0 fällt!
    _39(_192 _43 timeout=0; timeout<5000000; timeout++) {
        _15((td->control_status & (1 << 23)) EQ 0) {
            break; /// FERTIG! Der USB-Stick hat geantwortet!
        }
        __asm__ _192("pause");
    }

    /// 6. Controller wieder stoppen
    outw(uhci_base_port + 0x00, 0);

    /// 7. Prüfen, ob es Fehler gab (Stall, Timeout, CRC)
    _15(td->control_status & 0x7E0000) {
        str_cpy(cmd_status, "USB TRANSFER ERROR!");
        _96 1; /// Fehler!
    }

    _96 0; /// Erfolg!
}
/// ==========================================
/// BARE METAL: USB MASS STORAGE DRIVER
/// ==========================================
_43 usb_bot_get_capacity(_43 dev_addr, _43 ep_in, _43 ep_out, _43* out_block_size) {
    
    usb_bot_cbw cbw;
    usb_bot_csw csw;
    uint32_t capacity_data[2]; /// Hier schreibt der Stick die Antwort rein (2x 32-Bit = 8 Bytes)

    /// 1. Den Command Block Wrapper (CBW) vorbereiten
    cbw.signature = 0x43425355; /// 'USBC'
    cbw.tag = 1;                /// Befehl ID 1
    cbw.data_transfer_length = 8; /// Wir erwarten 8 Bytes Antwort
    cbw.flags = 0x80;           /// Direction: IN (vom Stick zum PC)
    cbw.lun = 0;
    cbw.cmd_length = 10;        /// Der SCSI-Befehl ist 10 Bytes lang
    
    /// Den SCSI-Befehl "READ CAPACITY (10)" zusammenbauen
    _39(_43 i = 0; i < 16; i++) cbw.scsi_cmd[i] = 0;
    cbw.scsi_cmd[0] = 0x25;     /// 0x25 = READ CAPACITY

    /// (Phase 1: CBW senden - OUT)
    uhci_bulk_transfer(dev_addr, ep_out, 0, (_89)(uint64_t)&cbw, 31);
    uhci_bulk_transfer(dev_addr, ep_in,  1, (_89)(uint64_t)capacity_data, 8);
    uhci_bulk_transfer(dev_addr, ep_in,  1, (_89)(uint64_t)&csw, 13);

    /// 5. Die Antwort auswerten
    _15(csw.signature NEQ 0x53425355 OR csw.status NEQ 0) {
        str_cpy(cmd_status, "USB ERROR: CAPACITY READ FAILED");
        _96 0; /// Fehler!
    }

    /// Der Stick sendet die Daten in "Big Endian" (falsch herum für x86).
    /// Wir müssen die Bytes umdrehen!
    uint32_t lba_count  = __builtin_bswap32(capacity_data[0]); 
    uint32_t block_size = __builtin_bswap32(capacity_data[1]);

    *out_block_size = block_size;
    
    str_cpy(cmd_status, "USB STICK READY!");
    _96 lba_count; /// Rückgabe: Anzahl der Sektoren
}
/// ==========================================
/// BARE METAL: USB SEKTOR LESEN (READ10)
/// ==========================================
uint32_t usb_bot_read_sectors(uint32_t dev_addr, uint32_t ep_in, uint32_t ep_out, uint32_t lba, uint8_t count, uint32_t buffer_addr) {
    usb_bot_cbw cbw;
    usb_bot_csw csw;

    /// 1. Den Command Block Wrapper (CBW) vorbereiten
    cbw.signature = 0x43425355; /// 'USBC'
    cbw.tag = 2;                /// Befehl ID 2 (wir nehmen einfach die nächste Nummer)
    cbw.data_transfer_length = 512 * count; /// Standard: 512 Bytes pro Sektor
    cbw.flags = 0x80;           /// Direction: IN (Wir wollen lesen)
    cbw.lun = 0;
    cbw.cmd_length = 10;        /// READ(10) ist 10 Bytes lang
    
    /// Das SCSI-Kommando-Array komplett nullen
    for(uint32_t i = 0; i < 16; i++) cbw.scsi_cmd[i] = 0;

    /// 2. Den SCSI READ(10) Befehl zusammenbauen
    cbw.scsi_cmd[0] = 0x28;     /// 0x28 = Operation Code für READ(10)
    
    /// Die LBA (Start-Sektor) in Big Endian umwandeln und in die Bytes 2 bis 5 schreiben
    uint32_t be_lba = __builtin_bswap32(lba);
    uint8_t* lba_ptr = (uint8_t*)&be_lba;
    cbw.scsi_cmd[2] = lba_ptr[0];
    cbw.scsi_cmd[3] = lba_ptr[1];
    cbw.scsi_cmd[4] = lba_ptr[2];
    cbw.scsi_cmd[5] = lba_ptr[3];

    /// Anzahl der zu lesenden Sektoren eintragen (Bytes 7 und 8, auch Big Endian)
    cbw.scsi_cmd[7] = 0;        /// High Byte (wir lesen hier selten > 255 Blöcke auf einmal)
    cbw.scsi_cmd[8] = count;    /// Low Byte

    /// 3. Phase 1: CBW an den Stick senden (OUT Endpoint)
    if(uhci_bulk_transfer(dev_addr, ep_out, 0, (uint32_t)(uint64_t)&cbw, 31) != 0) {
        str_cpy(cmd_status, "USB READ ERROR: CBW FAILED");
        return 1;
    }

    /// 4. Phase 2: Die echten Daten empfangen (IN Endpoint)
    if(uhci_bulk_transfer(dev_addr, ep_in, 1, buffer_addr, 512 * count) != 0) {
        str_cpy(cmd_status, "USB READ ERROR: DATA FAILED");
        return 1;
    }

    /// 5. Phase 3: Status (CSW) empfangen (IN Endpoint)
    if(uhci_bulk_transfer(dev_addr, ep_in, 1, (uint32_t)(uint64_t)&csw, 13) != 0) {
        str_cpy(cmd_status, "USB READ ERROR: CSW FAILED");
        return 1;
    }

    /// 6. Hat der Stick den Befehl akzeptiert?
    if(csw.signature != 0x53425355 || csw.status != 0) {
        str_cpy(cmd_status, "USB READ ERROR: SCSI STATUS BAD");
        return 1;
    }

    str_cpy(cmd_status, "USB READ SUCCESS!");
    return 0; /// Erfolg!
}
/// ==========================================
/// 3. LOW LEVEL (ASM) & MATH
/// ==========================================
/// BARE METAL FIX: Schnelle Integer-Quadratwurzel für 3D-Berechnungen
_43 int_sqrt(_43 n) {
    _43 x = n;
    _43 y = 1;
    _114(x > y) {
        x = (x + y) / 2;
        y = n / x;
    }
    _96 x;
}
_184 inb(_182 p) { _184 r; _33 _192("inb %1, %0":"=a"(r):"Nd"(p)); _96 r; }
_50 outb(_182 p, _184 v) { _33 _192("outb %0, %1"::"a"(v),"Nd"(p)); }
_182 inw(_182 p) { _182 r; _33 _192("inw %1, %0":"=a"(r):"Nd"(p)); _96 r; }
_89 inl(_182 p) { _89 r; _33 _192("inl %1, %0":"=a"(r):"Nd"(p)); _96 r; }
_50 outl(_182 p, _89 v) { _33 _192("outl %0, %1"::"a"(v),"Nd"(p)); }
_50 insw(_182 p, _50* a, _89 c) { _33 _192("rep; insw" : "+D"(a), "+c"(c) : "d"(p) : "memory"); }
_50 outsw(_182 p, _71 _50* a, _89 c) { _33 _192("rep; outsw" : "+S"(a), "+c"(c) : "d"(p) : "memory"); }
_89 mmio_read32(_89 addr) { _96 *(_192 _89*)(uintptr_t)addr; }
_50 mmio_write32(_89 addr, _89 val) { *(_192 _89*)(uintptr_t)addr = val; }
_71 _43 sin_lut[256] = { 1, 2, 4, 7, 9, 12, 14, 17, 19, 21, 24, 26, 28, 30, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 56, 58, 60, 61, 63, 64, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 79, 80, 81, 81, 82, 82, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 83, 83, 83, 82, 82, 81, 81, 80, 79, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 68, 67, 66, 64, 63, 61, 60, 58, 56, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 30, 28, 26, 24, 21, 19, 17, 14, 12, 9, 7, 4, 2, 1, -1, -2, -4, -7, -9, -12, -14, -17, -19, -21, -24, -26, -28, -30, -33, -35, -37, -39, -41, -43, -45, -47, -49, -51, -53, -55, -56, -58, -60, -61, -63, -64, -66, -67, -68, -70, -71, -72, -73, -74, -75, -76, -77, -78, -79, -79, -80, -81, -81, -82, -82, -83, -83, -83, -84, -84, -84, -84, -84, -84, -84, -83, -83, -83, -82, -82, -81, -81, -80, -79, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -68, -67, -66, -64, -63, -61, -60, -58, -56, -55, -53, -51, -49, -47, -45, -43, -41, -39, -37, -35, -33, -30, -28, -26, -24, -21, -19, -17, -14, -12, -9, -7, -4, -2, -1 };
_43 Cos(_43 a) { _96 sin_lut[(a + 64) % 256]; }
_43 Sin(_43 a) { _96 sin_lut[a % 256]; }
_72 _89 rng_seed = 123456789;
_89 random() { rng_seed = (rng_seed * 1103515245 + 12345) & 0x7FFFFFFF; _96 rng_seed; }
_50 str_cpy(_30* d, _71 _30* s) { _114(*s) *d++ = *s++; *d=0; }
/// Vergleicht zwei Strings. Gibt 1 zurück, wenn sie gleich sind, sonst 0.
_44 str_cmp(_71 _30* str1, _71 _30* str2) {
    _114 (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    _96 (*(const unsigned char*)str1 == *(const unsigned char*)str2) ? 1 : 0;
}
_43 str_len(_71 _30* s) { _43 l=0; _114(*s++)l++; _96 l; }
_44 str_equal(_71 _30* s1, _71 _30* s2) { _114(*s1 AND (*s1 EQ *s2)) { s1++; s2++; } _96 *(unsigned _30*)s1 EQ *(unsigned _30*)s2; }
_44 str_starts(_71 _30* full, _71 _30* prefix) { _114(*prefix) { _15(*prefix++ NEQ *full++) _96 _86; } _96 _128; }
_50 hex_to_str(_182 num, _30* out) { _71 _30 h[] = "0123456789ABCDEF"; out[0] = h[(num>>12)&0xF]; out[1] = h[(num>>8)&0xF]; out[2] = h[(num>>4)&0xF]; out[3] = h[num&0xF]; out[4] = 0; }
_50 byte_to_hex(_184 b, _30* out) { _71 _30 h[]="0123456789ABCDEF"; out[0]=h[(b>>4)&0xF]; out[1]=h[b&0xF]; out[2]=0; }
_50 int_to_str(_43 n, _30* s) { _15(n EQ 0){s[0]='0';s[1]=0;_96;} _43 i=0; _15(n<0){s[i++]='-';n=-n;} _43 t=n; _114(t>0){t/=10;i++;} s[i]=0; _114(n>0){s[--i]=(n%10)+'0';n/=10;} }
_184 bcd2bin(_184 b) { _96 ((b >> 4) * 10) + (b & 0xF); }
/// ==========================================
/// 4. NET/SECURITY HELPERS
/// ==========================================
_89 get_id(_71 _30* c) {
    _89 s=0; _114(*c) { _15(*c>='a' AND *c<='z') s+=(*c-'a'+1); _41 _15(*c>='A' AND *c<='Z') s+=(*c-'A'+1); c++; }
    _15(s EQ 57 OR s EQ 54 OR s EQ 56 OR s EQ 63 OR s EQ 101) _96 s*2; 
    _15(s EQ 57 AND *(c-1) EQ 'o') _96 171; 
    _96 s;
}
_182 hs(_182 v) { _96 (v<<8)|(v>>8); } 
_89 hl(_89 v) { _96 ((v&0xFF)<<24)|((v&0xFF00)<<8)|((v&0xFF0000)>>8)|((v>>24)&0xFF); } 
_182 chk(_50* d, _43 l) { _89 s=0; _182* p=(_182*)d; _114(l>1){s+=*p++;l-=2;} _15(l)s+=*(_184*)p; _114(s>>16)s=(s&0xFFFF)+(s>>16); _96 (_182)(~s); }
/// ==========================================
/// 5. GRAPHICS ENGINE INITIALISIERUNG
/// ==========================================
/// 1. Der Bauplan für GRUB (Multiboot)
struct multiboot_info {
    _89 flags;
    _89 mem_lower, mem_upper;
    _89 boot_device;
    _89 cmdline;
    _89 mods_count, mods_addr;
    _89 syms[4];
    _89 mmap_length, mmap_addr;
    _89 drives_length, drives_addr;
    _89 config_table;
    _89 boot_loader_name;
    _89 apm_table;
    _89 vbe_control_info, vbe_mode_info;
    _182 vbe_mode, vbe_interface_seg, vbe_interface_off, vbe_interface_len;
    /// Der Linear Framebuffer (Das Herzstück)
    _89 fb_addr_low, fb_addr_high; 
    _89 fb_pitch;
    _89 fb_width, fb_height;
    _184 fb_bpp, fb_type;
};
/// 2. Globale Variablen (Die "Behälter" für deine Hardware-Daten)
_89* fb        = 0;
_89* bb        = (_89*)0x02000000; /// Backbuffer
_89  screen_w  = 800;              /// Standard-Breite
_89  screen_h  = 600;              /// Standard-Höhe
_89  pitch     = 800 * 4;          /// Bytes pro Zeile
/// 3. Weitere Hardware-Infos
_202 GraphicsCardInfo {
    _182 vendor_id;
    _182 device_id;
    _184 bus, slot, func;
    _94  framebuffer_addr;
};
/// BARE METAL FIX: Prototypen (Damit der Compiler nicht meckert)
_50 TextC(_43 cp, _43 y, _71 _30* s, _89 col, _44 bold);
_50 Swap();
/// Der kritische Fehler-Handler
_50 kernel_panic(_71 _30* error_msg) {
    __asm__ _192("cli"); /// Interrupts stoppen, das System steht still!
    /// 1. DYNAMISCHE FÜLLUNG: Nutzt die echten Bildschirmmaße
    /// Wir berechnen die Gesamtpixel-Anzahl dynamisch
    _89 total_pixels = screen_w * screen_h;
    _39(_89 i = 0; i < total_pixels; i++) {
        bb[i] = 0x0000FF; /// Ein sattes, tiefes Blau
    }
    /// 2. TEXT-AUSGABE (Zentriert basierend auf screen_w / 2)
    _43 center_x = screen_w / 2;
    TextC(center_x, screen_h / 3,      "COSMOS OS - CRITICAL ERROR", 0xFFFFFF, _128);
    TextC(center_x, (screen_h / 3)+50, error_msg, 0xFFFFFF, _86);
    TextC(center_x, (screen_h / 2)+50, "SYSTEM STOPPED TO PROTECT HARDWARE.", 0xAAAAAA, _86);
    TextC(center_x, screen_h - 50,     "YOU NOOB!", 0xFF0000, _128); /// Das 'Noob' in Rot am unteren Rand
    /// 3. SICHTBAR MACHEN
    /// Wir schieben das Blau vom Backbuffer (bb) auf den echten Framebuffer (fb)
    Swap();
    /// 4. ENDSTATION
    _114(1) { __asm__ _192("hlt"); }
}
_172 GraphicsCardInfo main_gfx_card;
_172 _50 pci_find_gfx_card();
_30 get_ascii(_184 sc) {
    _15(sc > 58) _96 0;
    /// DEUTSCHES QWERTZ LAYOUT
    _72 _30 k_low[] = {
        0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
        'q','w','e','r','t','z','u','i','o','p','[',']','\n',0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
        'y','x','c','v','b','n','m',',','.','-',0,'*',0,' '
    };
    _72 _30 k_up[]  = {
        0,27,'!','"','#','$','%','&','/','(',')','=','?','`','\b','\t',
        'Q','W','E','R','T','Z','U','I','O','P','{','}','\n',0,
        'A','S','D','F','G','H','J','K','L',':','\'','~',0,'|',
        'Y','X','C','V','B','N','M',';',':','_',0,'*',0,' '
    };
    _96 key_shift ? k_up[sc] : k_low[sc];
}
_44 is_over_rect(_43 mx, _43 my, _43 x, _43 y, _43 w, _43 h) { _43 m = touch_mode ? 30 : 0; _96 (mx >= x-m AND mx <= x+w+m AND my >= y-m AND my <= y+h+m); }
_44 is_over(_43 mx, _43 my, _43 ox, _43 oy, _43 r) { _43 m = touch_mode ? 20 : 0; _96 (mx-ox)*(mx-ox) + (my-oy)*(my-oy) < (r+m)*(r+m); }
_50 Put(_43 x, _43 y, _89 c) { _15(x<0 OR x>=800 OR y<0 OR y>=600) _96; bb[y*800+x]=c; }
_50 PutAlpha(_43 x, _43 y, _89 c) { _15(x<0 OR x>=800 OR y<0 OR y>=600) _96; _89 bg = bb[y*800+x]; _89 s1 = ((c & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); bb[y*800+x] = ((s1 & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); }
_50 Clear() { _39(_43 i=0; i<800*600; i++) bb[i] = 0; }
/// BARE METAL SHADER: Dichte Galaxie & Orbital-Staub
_50 DrawDenseGalaxy(_43 cx, _43 cy, _43 exp) {
    _15 (exp <= 5) _96;
    _43 max_radius = (400 * exp) / 320;
    _89 l_seed = 123456; /// Fester Seed für eine stabile, persistente Form
    /// 8000 Partikel für massiven Sternenstaub
    _39(_43 i = 0; i < 8000; i++) {
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF;
        _43 rand_val1 = l_seed % max_radius;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF;
        _43 rand_val2 = l_seed % 30;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF;
        _43 rand_val3 = l_seed % 256; /// Voller Winkel für Staub-Ringe
        _43 d = rand_val1; 
        _15(d < 50) continue; /// Platz für die Sonne lassen
        /// ==========================================
        /// BARE METAL FIX: Lückenfüller (Spiderweb zerstören!)
        /// Organischer Scatter verschmiert die harten Sinus-Sprünge
        /// ==========================================
        _43 scatter_x = (l_seed % 7) - 3; 
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF;
        _43 scatter_y = (l_seed % 7) - 3;
        _43 angle = 0;
        _43 intensity_mod = 1;
        /// Mische Spirale und Ringe: Jedes 3. Partikel formt die Umlaufbahnen!
        _15(i % 3 EQ 0) {
            /// ORBITAL-RING (Der Staub sammelt sich auf den Planetenbahnen)
            _43 orbit_tier = (d / 50); 
            _43 target_d = 60 + (orbit_tier * 50);
            d = target_d + (rand_val2 - 15); /// Leichte Streuung um den Ring
            angle = (rand_val3 - (frame/4) + 256) % 256; /// Dreht sich sanft
            intensity_mod = 2; /// Orbitalringe leuchten stärker
        } _41 {
            /// SPIRAL-ARM (Der kosmische Nebel dazwischen)
            _43 arm = (i % 2) * 128;
            d = (d * d) / max_radius; /// Zieht den Staub zur Mitte
            _15(d < 50) d = 50 + (l_seed % 20);
            angle = (arm + (d / 2) - (frame / 3) + 256) % 256;
        }
        /// Natürliche Nebel-Streuung
        _43 final_d = d + (rand_val2 - 15);
        _43 final_a = (angle + (l_seed % 10) - 5 + 256) % 256;
        /// X/Y Berechnung (inkl. Scatter Lückenfüller)
        _43 px = cx + (Cos(final_a) * final_d) / 84 + scatter_x;
        _43 py = cy + (Sin(final_a) * final_d * 3 / 4) / 84 + scatter_y;
        /// Bildschirmränder schützen
        _15(px < 0 OR px >= 800 OR py < 0 OR py >= 600) continue;
        /// Farben definieren (Vom heißen Kern ins dunkle All)
        _43 r = 0, g = 0, b = 0;
        _15(d < 100) {
            r = 255; g = 180 - d; b = 60;  /// Sonnen-Nähe: Gold/Orange
        } _41 _15(d < 180) {
            r = 160 - (d - 100); g = 50; b = 255; /// Mitte: Magenta/Violett
        } _41 {
            r = 20; g = 30; b = 255 - (d - 180); /// Außen: Space Blue
        }
        /// Hintergrund lesen für additiven Glow
        _89 bg = bb[py * 800 + px];
        _43 bg_r = (bg >> 16) & 0xFF;
        _43 bg_g = (bg >> 8) & 0xFF;
        _43 bg_b = bg & 0xFF;
        /// Staub-Intensität berechnen
        _43 intensity = 255 - (int_sqrt(rand_val2*rand_val2) * 8);
        _15(intensity < 0) intensity = 0;
        intensity = (intensity * intensity_mod) / 2;
        /// Diamant-Sterne (alle 100 Partikel)
        _15(i % 100 EQ 0) { r = 255; g = 255; b = 255; intensity = 255; }
        /// Licht mischen und schreiben
        _43 f_r = bg_r + (r * intensity / 256); _15(f_r > 255) f_r = 255;
        _43 f_g = bg_g + (g * intensity / 256); _15(f_g > 255) f_g = 255;
        _43 f_b = bg_b + (b * intensity / 256); _15(f_b > 255) f_b = 255;
        bb[py * 800 + px] = (f_r << 16) | (f_g << 8) | f_b;
    }
}
/// BARE METAL SHADER: Die goldene 3D-Sonne mit Corona-Glow
_50 DrawGoldenSun(_43 cx, _43 cy, _43 radius) {
    _43 r2 = radius * radius;
    _43 glow_radius = radius + 15; /// Die Sonne strahlt 15 Pixel nach außen
    _43 glow_r2 = glow_radius * glow_radius;
    _39(_43 y = -glow_radius; y <= glow_radius; y++) {
        _39(_43 x = -glow_radius; x <= glow_radius; x++) {
            _43 dist_sq = x*x + y*y;
            /// Sicherheits-Check für die Bildschirmränder
            _43 screen_x = cx + x;
            _43 screen_y = cy + y;
            _15(screen_x < 0 OR screen_x >= 800 OR screen_y < 0 OR screen_y >= 600) _101;
            _15(dist_sq <= r2) {
                /// --- KUGEL (3D SHADER) ---
                /// 1. Tiefe berechnen (Mitte = 255, Rand = 0)
                _43 nz = int_sqrt(r2 - dist_sq) * 255 / radius;
                /// 2. Lichtreflex (Highlight oben links simulieren)
                _43 hx = x + (radius / 3);
                _43 hy = y + (radius / 3);
                _43 h_dist_sq = hx*hx + hy*hy;
                _43 h_dist = int_sqrt(h_dist_sq);
                _43 highlight = 0;
                _15(h_dist < radius) {
                    highlight = 255 - (h_dist * 255 / radius);
                    /// Exponentielles Highlight für einen knackigen, metallischen Glanz
                    highlight = (highlight * highlight) / 255; 
                }
                /// 3. Grundfarbe mischen (Kanten dunkler, Mitte golden)
                _43 r = (nz * 255) / 255;
                _43 g = (nz * 170) / 255; /// Goldenes Orange
                _43 b = (nz * 20) / 255;  /// Etwas Wärme
                /// 4. Glanzpunkt addieren (Weiß/Gelblich)
                r += highlight;       _15(r > 255) r = 255;
                g += highlight;       _15(g > 255) g = 255;
                b += (highlight / 2); _15(b > 255) b = 255;
                bb[screen_y * 800 + screen_x] = (r << 16) | (g << 8) | b;
            } _41 _15 (dist_sq <= glow_r2 AND dist_sq > r2) {
                /// --- CORONA (GLOW EFFEKT) ---
                _43 dist = int_sqrt(dist_sq);
                _43 alpha = 255 - ((dist - radius) * 255 / (glow_radius - radius));
                /// Alpha Curve anpassen, damit der Glow weich ausläuft
                alpha = (alpha * alpha) / 255;
                _15(alpha > 0) {
                    /// ECHTES ALPHA-BLENDING: Hintergrund lesen und Farbe aufmischen!
                    _89 bg = bb[screen_y * 800 + screen_x];
                    _43 bg_r = (bg >> 16) & 0xFF;
                    _43 bg_g = (bg >> 8) & 0xFF;
                    _43 bg_b = bg & 0xFF;
                    /// Glow-Zielfarbe (Dunkles Gold/Orange)
                    _43 glow_r = 255;
                    _43 glow_g = 120;
                    _43 glow_b = 0;
                    /// Farbkanäle perfekt mischen (Viel besser als das alte PutAlpha!)
                    _43 final_r = (glow_r * alpha + bg_r * (255 - alpha)) / 255;
                    _43 final_g = (glow_g * alpha + bg_g * (255 - alpha)) / 255;
                    _43 final_b = (glow_b * alpha + bg_b * (255 - alpha)) / 255;
                    bb[screen_y * 800 + screen_x] = (final_r << 16) | (final_g << 8) | final_b;
                }
            }
        }
    }
}
/// BARE METAL SHADER: Organische Planeten MIT farbiger Atmosphäre
_50 DrawOrganicPlanet(_43 cx, _43 cy, _43 radius, _89 base_col) {
    _43 r2 = radius * radius;
    /// BARE METAL FIX: Die Atmosphäre ragt 8 Pixel ins All hinaus
    _43 glow_radius = radius + 8; 
    _43 glow_r2 = glow_radius * glow_radius;
    _43 base_r = (base_col >> 16) & 0xFF;
    _43 base_g = (base_col >> 8) & 0xFF;
    _43 base_b = base_col & 0xFF;
    /// Die Schleife läuft jetzt bis zum äußeren Atmosphären-Rand
    _39(_43 y = -glow_radius; y <= glow_radius; y++) {
        _39(_43 x = -glow_radius; x <= glow_radius; x++) {
            _43 dist_sq = x*x + y*y;
            /// WICHTIG: Da wir den Hintergrund lesen, dürfen wir nicht über den Rand malen!
            _43 screen_x = cx + x;
            _43 screen_y = cy + y;
            _15(screen_x < 0 OR screen_x >= 800 OR screen_y < 0 OR screen_y >= 600) _101;
            _15(dist_sq <= r2) {
                /// ==========================================
                /// INNEN: DER PLANET (Unverändert)
                /// ==========================================
                _43 nz = int_sqrt(r2 - dist_sq) * 255 / radius;
                _43 edge_dist = 255 - nz;
                _43 light_x = x + (radius / 2);
                _43 light_y = y + (radius / 2);
                _43 l_dist_sq = light_x*light_x + light_y*light_y;
                _43 diffuse = 0;
                _15(l_dist_sq < r2) {
                     diffuse = 255 - (int_sqrt(l_dist_sq) * 255 / radius);
                }
                _43 pos_x = x + radius;
                _43 pos_y = y + radius;
                _43 noise = ((pos_x * 17) + (pos_y * 31)) % 20; 
                _43 banding = (Sin((pos_y * 100) / radius) + 64) / 8;
                _43 r = (base_r * nz) / 255;
                _43 g = (base_g * nz) / 255;
                _43 b = (base_b * nz) / 255;
                r += (diffuse * base_r) / 256; 
                g += (diffuse * base_g) / 256; 
                b += (diffuse * base_b) / 256;
                _43 rim = (edge_dist * edge_dist) / 255; 
                r += (rim * base_r) / 512;
                g += (rim * base_g) / 512;
                b += (rim * base_b) / 512;
                r = (r * (220 + noise + banding)) / 256;
                g = (g * (220 + noise + banding)) / 256;
                b = (b * (220 + noise + banding)) / 256;
                _15(r > 255) r = 255;
                _15(g > 255) g = 255;
                _15(b > 255) b = 255;
                bb[screen_y * 800 + screen_x] = (r << 16) | (g << 8) | b;
            } _41 _15 (dist_sq <= glow_r2 AND dist_sq > r2) {
                /// ==========================================
                /// AUSSEN: DIE ATMOSPHÄRE (Weicher Glow)
                /// ==========================================
                _43 dist = int_sqrt(dist_sq);
                /// Berechnet die Transparenz (Alpha): 255 am Planet, 0 im All
                _43 alpha = 255 - ((dist - radius) * 255 / (glow_radius - radius));
                /// Quadratische Kurve für einen viel weicheren, realistischen Rauch-Effekt
                alpha = (alpha * alpha) / 255;
                _15(alpha > 0) {
                    /// 1. Den schwarzen/gepunkteten Weltraum-Hintergrund lesen
                    _89 bg = bb[screen_y * 800 + screen_x];
                    _43 bg_r = (bg >> 16) & 0xFF;
                    _43 bg_g = (bg >> 8) & 0xFF;
                    _43 bg_b = bg & 0xFF;
                    /// 2. Planetenfarbe mit dem Weltraum mischen
                    _43 final_r = (base_r * alpha + bg_r * (255 - alpha)) / 255;
                    _43 final_g = (base_g * alpha + bg_g * (255 - alpha)) / 255;
                    _43 final_b = (base_b * alpha + bg_b * (255 - alpha)) / 255;
                    /// 3. Zurück in den Framebuffer schreiben
                    bb[screen_y * 800 + screen_x] = (final_r << 16) | (final_g << 8) | final_b;
                }
            }
        }
    }
}
_50 Swap() { 
    _15(shake_timer > 0) { sx_off = (random()%10)-5; sy_off = (random()%10)-5; shake_timer--; } _41 { sx_off=0; sy_off=0; } 
    _39(_43 y=0; y<600; y++) { 
        _43 ty = y + sy_off; 
        _15(ty < 0 OR ty >= 600) _101; 
        _89* d=(_89*)((_184*)fb+ty*pitch); 
        _89* s=&bb[y*800]; 
        _39(_43 x=0; x<800; x++) { 
            _43 tx = x + sx_off; 
            _15(tx >= 0 AND tx < 800) d[tx] = s[x]; 
        } 
    } 
}
_50 DrawRoundWindow(_43 cx_p, _43 cy_p, _43 r, _89 color) { _43 r2=r*r; _39(_43 y=-r;y<=r;y++) _39(_43 x=-r;x<=r;x++) _15(x*x+y*y<=r2) Put(cx_p+x, cy_p+y, color); }
_50 DrawIcon(_43 x, _43 y, _89 col) { _39(_43 i=0; i<30; i++) _39(_43 j=0; j<40; j++) Put(x+i, y+j, col); _39(_43 i=5; i<25; i++) _39(_43 j=5; j<15; j++) Put(x+i, y+j, 0xFFFFFF); }
/// BARE METAL SHADER: 8-Way Neon Glow & Outline Engine
_50 DrawChar(_43 x, _43 y, _30 c, _89 col, _44 bold) { 
    _72 _71 _184 f_u[] = { 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22, 0x7F,0x41,0x41,0x22,124, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x09,0x01, 0x3E,0x41,0x49,0x49,0x7A, 0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41, 0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x0C,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E, 0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31, 0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x3F,0x40,0x38,0x40,0x3F, 0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07, 0x61,0x51,0x49,0x45,0x43 };
    _72 _71 _184 f_l[] = { 0x20,0x54,0x54,0x54,0x78, 0x7F,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x44,0x20, 0x38,0x44,0x44,0x48,0x7F, 0x38,0x54,0x54,0x54,0x18, 0x08,0x7E,0x09,0x01,0x02, 0x0C,0x52,0x52,0x52,0x3E, 0x7F,0x08,0x04,0x04,0x78, 0x00,0x44,0x7D,0x40,0x00, 0x20,0x40,0x44,0x3D,0x00, 0x7F,0x10,0x28,0x44,0x00, 0x00,0x41,0x7F,0x40,0x00, 0x7C,0x04,0x18,0x04,0x78, 0x7C,0x08,0x04,0x04,0x78, 0x38,0x44,0x44,0x44,0x38, 0x7C,0x14,0x14,0x14,0x08, 0x08,0x14,0x14,0x18,0x7C, 0x7C,0x08,0x04,0x04,0x08, 0x48,0x54,0x54,0x54,0x20, 0x04,0x3F,0x44,0x40,0x20, 0x3C,0x40,0x40,0x20,0x7C, 0x1C,0x20,0x40,0x20,0x1C, 0x3C,0x40,0x30,0x40,0x3C, 0x44,0x28,0x10,0x28,0x44, 0x0C,0x50,0x50,0x50,0x3C, 0x44,0x64,0x54,0x4C,0x44 };
    _72 _71 _184 f_n[] = { 0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4B,0x31, 0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03, 0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1E };
    _71 _184* ptr = 0;
    _15(c >= 'A' AND c <= 'Z') ptr = &f_u[(c-'A')*5]; _41 _15(c >= 'a' AND c <= 'z') ptr = &f_l[(c-'a')*5]; _41 _15(c >= '0' AND c <= '9') ptr = &f_n[(c-'0')*5];
    _41 _15(c EQ ':') { _72 _184 s[]={0,0x36,0x36,0,0}; ptr=s; } _41 _15(c EQ '.') { _72 _184 s[]={0,0x60,0x60,0,0}; ptr=s; } _41 _15(c EQ '-') { _72 _184 s[]={0x08,0x08,0x08,0x08,0x08}; ptr=s; } _41 _15(c EQ '!') { _72 _184 s[]={0,0,0x5F,0,0}; ptr=s; } _41 _15(c EQ '_') { _72 _184 s[]={0x40,0x40,0x40,0x40,0x40}; ptr=s; } _41 _15(c EQ '(') { _72 _184 s[]={0x1C,0x22,0x41,0,0}; ptr=s; } _41 _15(c EQ ')') { _72 _184 s[]={0,0,0x41,0x22,0x1C}; ptr=s; } _41 _15(c EQ '/') { _72 _184 s[]={0x20,0x10,0x08,0x04,0x02}; ptr=s; } _41 _15(c EQ '=') { _72 _184 s[]={0x14,0x14,0x14,0x14,0x14}; ptr=s; } _41 _15(c EQ '?') { _72 _184 s[]={0x02,0x01,0x59,0x05,0x02}; ptr=s; } _41 _15(c EQ '"') { _72 _184 s[]={0,0x03,0,0x03,0}; ptr=s; } _41 _15(c EQ ',') { _72 _184 s[]={0,0x50,0x30,0,0}; ptr=s; } _41 _15(c EQ '|') { _72 _184 s[]={0x7F,0,0,0,0}; ptr=s; }    
    _15(!ptr) _96;
    /// BARE METAL FIX: 8-WAY NEON GLOW / OUTLINE
    /// Wenn die Schrift dunkel ist, nutzen wir einen strahlend weißen Glow.
    /// Wenn die Schrift hell ist (z.B. CMD-Grün oder Weiß), nutzen wir eine tiefschwarze Umrandung.
    _89 glow_col = (col < 0x555555) ? 0xFFFFFF : 0x000000;
    /// PASS 1: Der Glow. Wir nutzen PutAlpha in alle 8 Himmelsrichtungen um den Pixel herum.
    /// Dadurch entsteht kein harter, hässlicher Kasten, sondern ein weicher Leucht-Rand.
    _39(_43 m=0;m<5;m++){ 
        _184 l=ptr[m]; 
        _39(_43 n=0;n<7;n++) {
            _15((l>>n)&1) { 
                PutAlpha(x+m-1, y+n-1, glow_col); PutAlpha(x+m, y+n-1, glow_col); PutAlpha(x+m+1, y+n-1, glow_col);
                PutAlpha(x+m-1, y+n,   glow_col);                                 PutAlpha(x+m+1, y+n,   glow_col);
                PutAlpha(x+m-1, y+n+1, glow_col); PutAlpha(x+m, y+n+1, glow_col); PutAlpha(x+m+1, y+n+1, glow_col);
                _15(bold) {
                    PutAlpha(x+m+2, y+n, glow_col);
                }
            } 
        } 
    }
    /// PASS 2: Den eigentlichen Buchstaben gestochen scharf in den Kern setzen
    _39(_43 m=0;m<5;m++){ 
        _184 l=ptr[m]; 
        _39(_43 n=0;n<7;n++) {
            _15((l>>n)&1) { 
                Put(x+m, y+n, col); 
                _15(bold) Put(x+m+1, y+n, col); 
            } 
        } 
    } 
}
_50 Text(_43 x, _43 y, _71 _30* s, _89 col, _44 bold) { _15(!s) _96; _114(*s) { DrawChar(x,y,*s++,col,bold); x+=(bold?7:6); } }
_50 TextC(_43 cp, _43 y, _71 _30* s, _89 col, _44 bold) { _15(!s) _96; _43 l=0; _114(s[l])l++; Text(cp-(l*(bold?7:6))/2, y, s, col, bold); }
_50 DrawRoundedRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c) { _39(_43 iy=0;iy<rh;iy++)_39(_43 ix=0;ix<rw;ix++){ _44 corn=_86; _15(ix<r AND iy<r AND (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=_128; _15(ix>rw-r AND iy<r AND (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=_128; _15(ix<r AND iy>rh-r AND (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; _15(ix>rw-r AND iy>rh-r AND (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; _15(!corn) Put(x+ix,y+iy,c); } }
/// BARE METAL SHADER: ACRYLIC GLASS (Milchglas-Fenster)
_50 DrawGlassRect(_43 x, _43 y, _43 rw, _43 rh, _43 r, _89 c) { 
    _43 cr = (c >> 16) & 0xFF;
    _43 cg = (c >> 8) & 0xFF;
    _43 cb = c & 0xFF;
    _39(_43 iy=0; iy<rh; iy++) {
        _39(_43 ix=0; ix<rw; ix++) {
            _44 corn=_86; 
            _15(ix<r AND iy<r AND (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=_128; 
            _15(ix>rw-r AND iy<r AND (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=_128; 
            _15(ix<r AND iy>rh-r AND (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128; 
            _15(ix>rw-r AND iy>rh-r AND (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=_128;
            _15(!corn) {
                _43 sx = x + ix;
                _43 sy = y + iy;
                _15(sx >= 0 AND sx < 800 AND sy >= 0 AND sy < 600) {
                    _89 bg = bb[sy * 800 + sx];
                    _43 bg_r = (bg >> 16) & 0xFF;
                    _43 bg_g = (bg >> 8) & 0xFF;
                    _43 bg_b = bg & 0xFF;
                    /// 75% Weltraum + 25% Fensterfarbe (Bit-Shift für Speed)
                    _43 f_r = (bg_r + cr) >> 1;
                    _43 f_g = (bg_g + cg) >> 1;
                    _43 f_b = (bg_b + cb) >> 1;
                    bb[sy * 800 + sx] = (f_r << 16) | (f_g << 8) | f_b;
                }
            }
        }
    }
}
/// BARE METAL SHADER: AERO MOUSE CURSOR
_50 DrawAeroCursor(_43 mx, _43 my) {
    _72 _71 _30* c_map[17] = {
        "*", "**", "*.*", "*..*", "*...*", "*....*", "*.....*", "*......*",
        "*.......*", "*........*", "*.........*", "*......****", "*...*..*",
        "*..* *..*", "*.* *..*", "** *..*", "        **"
    };
    _39(_43 y = 0; y < 17; y++) {
        _43 len = str_len(c_map[y]);
        _39(_43 x = 0; x < len; x++) {
            _15(c_map[y][x] NEQ ' ') {
                PutAlpha(mx + x + 3, my + y + 4, 0x000000); 
            }
        }
    }
    _39(_43 y = 0; y < 17; y++) {
        _43 len = str_len(c_map[y]);
        _39(_43 x = 0; x < len; x++) {
            _15(c_map[y][x] EQ '*') Put(mx + x, my + y, 0x000000); 
            _41 _15(c_map[y][x] EQ '.') Put(mx + x, my + y, 0xFFFFFF); 
        }
    }
}
_50 DrawTriangle(_43 x1, _43 y1, _43 x2, _43 y2, _43 x3, _43 y3, _89 c) { _43 minx=x1, maxx=x1, miny=y1, maxy=y1; _15(x2<minx)minx=x2; _15(x2>maxx)maxx=x2; _15(y2<miny)miny=y2; _15(y2>maxy)maxy=y2; _15(x3<minx)minx=x3; _15(x3>maxx)maxx=x3; _15(y3<miny)miny=y3; _15(y3>maxy)maxy=y3; _39(_43 y=miny; y<=maxy; y++) { _39(_43 x=minx; x<=maxx; x++) { _43 w1 = (x2-x1)*(y-y1) - (y2-y1)*(x-x1); _43 w2 = (x3-x2)*(y-y2) - (y3-y2)*(x-x2); _43 w3 = (x1-x3)*(y-y3) - (y1-y3)*(x-x3); _15((w1>=0 AND w2>=0 AND w3>=0) OR (w1<=0 AND w2<=0 AND w3<=0)) Put(x,y,c); } } }
_50 DrawFace(_43 x, _43 y, _43 mx, _43 my) { DrawRoundWindow(x, y, 60, 0x555555); _43 ex1=x-20; _43 ey=y-10; _43 ex2=x+20; _43 ox=(mx-x)/20; _15(ox>5)ox=5; _15(ox<-5)ox=-5; _43 oy=(my-y)/20; _15(oy>5)oy=5; _15(oy<-5)oy=-5; DrawRoundedRect(ex1-8, ey-5, 16, 10, 2, 0xFFFFFF); DrawRoundedRect(ex2-8, ey-5, 16, 10, 2, 0xFFFFFF); Put(ex1+ox, ey+oy, 0); Put(ex1+ox+1, ey+oy, 0); Put(ex2+ox, ey+oy, 0); Put(ex2+ox+1, ey+oy, 0); DrawRoundedRect(x-15, y+25, 30, 2, 0, 0); }
_50 DrawWaveform(_43 x, _43 y, _43 w, _43 f) { _39(_43 i=0; i<w; i+=2) { _43 h = Sin(i*5 + f*10) / 4; _15(random()%10 > 7) h += (random()%10)-5; Put(x+i, y+20+h, 0); } }
_50 boot_logo() {
    _89 g=0xFFD700; _43 c=400, y=200;
    TextC(c,y,"#####################################",g,_128);
    TextC(c,y+20,"         cosmOS SYSTEM	        ",0xFFFFFF,_128);
    TextC(c,y+40," ID-HASHING: ACTIVE | NET: RX/TX ",0xAAAAAA,_86);
    TextC(c,y+80," >> SYSTEM READY << ",g,_128);
    TextC(c,y+100,"#####################################",g,_128);
}
/// ==========================================
/// 6. HARDWARE DRIVERS
/// ==========================================
_50 init_ac97() {
    _15(ac97_mixer_port EQ 0 OR ac97_bus_port EQ 0) _96;

    /// 1. Harter Kaltstart des Soundchips
    outw(ac97_mixer_port + 0x00, 1);
    
    /// 2. BARE METAL FIX: Extreme Verzögerung! 
    /// Echte Hardware braucht teilweise 50ms zum Aufwachen. Wir warten lange!
    _39(_192 _43 wait=0; wait<5000000; wait++) __asm__ _192("pause");

    /// 3. JEDEN verdammten Ausgang entmuten! 
    outw(ac97_mixer_port + 0x02, 0x0000); /// Master Volume
    outw(ac97_mixer_port + 0x04, 0x0000); /// Headphone Volume (Wichtig für Laptops!)
    outw(ac97_mixer_port + 0x18, 0x0000); /// PCM Out (Die Musik/Erdbeben)
    outw(ac97_mixer_port + 0x0A, 0x0000); /// PC-Beep Passthrough (Leitet Retro-Sounds an Boxen)

    /// 4. Beweis auf den Bildschirm drucken, dass der Port gefunden wurde!
    _30 hex_port[10];
    hex_to_str(ac97_bus_port, hex_port);
    str_cpy(cmd_status, "AC97 READY @ PORT: ");
    str_cat(cmd_status, hex_port);
}
/// ==========================================
/// BARE METAL: SONNEN-EFFEKTE (AC97)
/// ==========================================
/// 1. SONNEN-AUSBRUCH: Lauter Start, dann Ausklingen
_50 play_ac97_flare() {
    _15(ac97_bus_port EQ 0) _96;
    outb(ac97_bus_port + 0x1B, 0); /// Stop
    _89 pcm_addr = 0x0A001000;
    _182* pcm_buf = (_182*)pcm_addr;
    _39(_43 i=0; i<80000; i+=2) {
        /// "Envelope": Volumen sinkt über Zeit (80000 -> 0)
        _43 vol = (80000 - i) / 8; 
        _182 s = (random() % (vol + 1)) - (vol / 2);
        pcm_buf[i] = s; pcm_buf[i+1] = s;
    }
    _89* bdl = (_89*)0x0A000000;
    bdl[0] = pcm_addr; bdl[1] = 80000 | 0x80000000;
    outl(ac97_bus_port + 0x10, 0x0A000000);
    outb(ac97_bus_port + 0x15, 0);
    outb(ac97_bus_port + 0x1B, 1);
}
/// 2. SOG-EFFEKT: Leises Rauschen, das abrupt laut stoppt
_50 play_ac97_suction() {
    _15(ac97_bus_port EQ 0) _96;
    outb(ac97_bus_port + 0x1B, 0); /// Stop
    _89 pcm_addr = 0x0A001000;
    _182* pcm_buf = (_182*)pcm_addr;
    _39(_43 i=0; i<50000; i+=2) {
        /// "Envelope": Volumen steigt über Zeit (0 -> 50000)
        _43 vol = i / 6; 
        _182 s = (random() % (vol + 1)) - (vol / 2);
        pcm_buf[i] = s; pcm_buf[i+1] = s;
    }
    _89* bdl = (_89*)0x0A000000;
    bdl[0] = pcm_addr; bdl[1] = 50000 | 0x80000000;
    outl(ac97_bus_port + 0x10, 0x0A000000);
    outb(ac97_bus_port + 0x15, 0);
    outb(ac97_bus_port + 0x1B, 1);
}
/// ==========================================
/// BARE METAL: AC97 DMA SOUND (PROCEDURAL)
/// ==========================================
_50 play_ac97_earthquake() {
    _15(ac97_bus_port EQ 0) _96; /// Kein Soundchip? Abbrechen!
    /// 1. Den Bus Master (DMA) stoppen, falls er noch läuft
    outb(ac97_bus_port + 0x1B, 0);
    /// 2. Feste RAM-Adressen reservieren
    _89 bdl_addr = 0x0A000000; /// Hier liegt die Playlist (BDL)
    _89 pcm_addr = 0x0A001000; /// Hier liegen die rohen Töne
    /// 3. Procedural Audio: Das Erdbeben berechnen!
    _182* pcm_buf = (_182*)pcm_addr;
    _39(_43 i=0; i<60000; i+=2) {
        _43 rumble = (random() % 8000) - 4000; 
        pcm_buf[i]   = (_182)rumble; /// Linkes Ohr
        pcm_buf[i+1] = (_182)rumble; /// Rechtes Ohr
    }
    /// 4. Die Playlist (BDL) schreiben
    _89* bdl = (_89*)bdl_addr;
    bdl[0] = pcm_addr; 
    bdl[1] = 60000 | 0x80000000;
    /// 5. Dem Controller die Playlist übergeben
    outl(ac97_bus_port + 0x10, bdl_addr);
    /// 6. Last Valid Index (LVI) auf 0 setzen
    outb(ac97_bus_port + 0x15, 0);
    /// 7. FEUER FREI! (DMA-Transfer starten)
    outb(ac97_bus_port + 0x1B, 1);
}
_50 play_earthquake(_43 duration) { 
    shake_timer = duration;
    /// 1. Neuen AC97 Sound zünden (Läuft ab jetzt unsichtbar im Hintergrund!)
    play_ac97_earthquake();
    /// 2. Den Bildschirm wackeln lassen
    _114(shake_timer > 0) { 
        Swap();
        _39(_192 _43 d=0; d<40000; d++); 
    } 
}
/// ==========================================
/// BARE METAL: WAV FILE PLAYER (AC97 DMA)
/// ==========================================
_50 play_wav_file(_43 file_idx) {
    _15(ac97_bus_port EQ 0) _96; /// Kein Audio-Chip? Abbruch.
    _15(!file_table[file_idx].exists OR file_table[file_idx].is_folder) _96;
    /// 1. RAM-Adressen reservieren
    _89 bdl_addr = 0x0A000000;       /// Hier liegt die BDL-Playlist
    _89 file_load_addr = 0x0B000000; /// Hier laden wir die Datei von der HDD hin
    /// 2. Datei von Festplatte in den RAM laden (CFS-Sektoren)
    _43 port = drives[active_drive_idx].base_port;
	_89 lba = file_table[file_idx].sector_offset;
    _89 size_bytes = file_table[file_idx].size;
    /// Wenn die Datei leer ist (0 Bytes), direkt abbrechen
    _15(size_bytes EQ 0) _96;
    /// Aufrunden: Wie viele 512-Byte Sektoren müssen wir lesen?
    _43 sectors_to_read = (size_bytes / 512) + 1;
    ahci_read_sectors(port, lba, sectors_to_read, file_load_addr);
    /// Warten, bis die Festplatte fertig in den RAM kopiert hat
    _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
    /// 3. Den Header entschlüsseln!
    WavHeader* header = (WavHeader*)file_load_addr;
    /// Sicherheits-Check: Steht ganz am Anfang "RIFF" und "WAVE"?
    _15(header->riff_id[0] NEQ 'R' OR header->wave_id[0] NEQ 'W') {
        str_cpy(cmd_status, "ERROR: NOT A VALID WAV FILE!");
        _96; /// Abbruch, das ist keine Musik!
    }
    /// 4. AC97 Playlist (BDL) für die Datei bauen
    outb(ac97_bus_port + 0x1B, 0); /// DMA kurz stoppen
    uint32_t* bdl = (uint32_t*)bdl_addr;
    /// Wir geben der Soundkarte NICHT den Start der Datei, sondern überspringen 
    /// die 44 Bytes des Headers! Sonst hört man den Header als lautes Knacken.
    bdl[0] = file_load_addr + 44;
    /// BARE METAL FALLE: Die AC97 Karte will die Länge nicht in Bytes, 
    /// sondern in "Samples" (16-Bit Blöcken). Also halbieren wir die Dateigröße.
    uint32_t total_samples = header->data_size / 2;
    /// Länge übergeben und IOC (0x80000000) Autostopp-Flag setzen
    bdl[1] = total_samples | 0x80000000;
    /// 5. Feuer frei!
    outl(ac97_bus_port + 0x10, bdl_addr);
    outb(ac97_bus_port + 0x15, 0);       /// LVI zurücksetzen
    outb(ac97_bus_port + 0x1B, 1);       /// PLAY STARTEN!
    str_cpy(cmd_status, "PLAYING WAV FILE...");
}
_182 current_volume = 0x0F0F; /// Start bei mittlerer Lautstärke
_50 set_ac97_volume(_182 vol) {
    _15(ac97_mixer_port EQ 0) _96;
    /// Register 0x02 ist der Master Volume
    outw(ac97_mixer_port + 0x02, vol);
    current_volume = vol;
}
_50 play_freq(_89 f) { _15(f EQ 0) { outb(0x61, inb(0x61) & 0xFC); _96; }
_89 d = 1193180 / f; outb(0x43, 0xB6); outb(0x42, (_184)d); outb(0x42, (_184)(d >> 8));
_184 t = inb(0x61); _15 (t NEQ (t | 3)) outb(0x61, t | 3); }
_50 play_sound(_89 n_freq, _43 duration) { _15(n_freq EQ 0) _96;
_89 div = 1193180 / n_freq; outb(0x43, 0xB6); outb(0x42, (_184)(div)); outb(0x42, (_184)(div >> 8));
_184 tmp = inb(0x61); _15(tmp NEQ (tmp | 3)) outb(0x61, tmp | 3); _39(_43 i=0; i<duration*10000; i++)
_33 _192("nop"); outb(0x61, tmp & 0xFC); }
_50 startup_melody() { play_sound(523, 100); play_sound(659, 100); play_sound(784, 200); }
_50 system_reboot() { play_earthquake(30); _114(inb(0x64)&2); outb(0x64,0xFE); }
_50 system_shutdown() {
    play_earthquake(30);
    str_cpy(cmd_status, "ACPI SHUTDOWN INITIATED...");
    /// 1. EMULATOR-SCHNELLZUGRIFFE (Falls du wieder QEMU / VBox nutzt)
    outw(0xB004, 0x2000); /// QEMU / Bochs
    outw(0x4004, 0x3400); /// VirtualBox
    /// 2. BARE METAL ACPI-SCANNER (Für Laptop, Workstation & Main PC)
    /// Wir durchsuchen den BIOS-Speicherbereich nach der "RSD PTR " Signatur
    _39(_89 addr = 0x000E0000; addr < 0x000FFFFF; addr += 16) {
        _184* rsdp = (_184*)addr;
        _15(rsdp[0] EQ 'R' AND rsdp[1] EQ 'S' AND rsdp[2] EQ 'D' AND rsdp[3] EQ ' ' AND
            rsdp[4] EQ 'P' AND rsdp[5] EQ 'T' AND rsdp[6] EQ 'R' AND rsdp[7] EQ ' ') {
            _89 rsdt_addr = *(_89*)(addr + 16);
            _89* rsdt = (_89*)rsdt_addr;
            _43 entries = (rsdt[1] - 36) / 4;
            /// Suche nach der FACP (Fixed ACPI Description Table)
            _39(_43 i = 0; i < entries; i++) {
                _89* table = (_89*)rsdt[9 + i];
                _15(table[0] EQ 0x50434146) { /// Hex für "FACP"
                    _184* fadt = (_184*)table;
                    /// Wichtige Register-Adressen aus der Tabelle fischen
                    _89 smi_cmd = *(_89*)(fadt + 52);
                    _184 acpi_enable = fadt[56];
                    _89 pm1a_cnt = *(_89*)(fadt + 68);
                    /// A. ACPI AKTIVIEREN (Falls das BIOS es noch nicht getan hat)
                    _15(smi_cmd NEQ 0 AND acpi_enable NEQ 0) {
                        outb(smi_cmd, acpi_enable);
                        _39(_192 _89 delay = 0; delay < 1000000; delay++) { __asm__ _192("pause"); }
                    }
                    /// B. DEN STROM ABSTELLEN (Sleep Type 5 / S5 erzwingen)
                    /// Wir feuern alle typischen S5-Bitmasken auf das Control-Register, 
                    /// da jedes Mainboard leicht anders belegt ist. Eine davon trifft immer!
                    outw(pm1a_cnt, 0x2000 | (0 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (1 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (2 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (3 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (4 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (5 << 10)); /// Häufigster Code
                    outw(pm1a_cnt, 0x2000 | (6 << 10)); 
                    outw(pm1a_cnt, 0x2000 | (7 << 10)); 
                }
            }
        }
    }
    /// 3. FALLBACK: DER WINDOWS 95 KLASSIKER!
    _39(_43 i = 0; i < screen_w * screen_h; i++) {
        bb[i] = 0x000000; 
    }
    /// Der ikonische Text in grellem Orange (0xFFA500) genau in der Mitte
    TextC(screen_w/2, screen_h/2, "IT IS NOW SAFE TO TURN OFF YOUR COMPUTER", 0xFFA500, _128);
    /// Frontbuffer aktualisieren (damit man es sofort sieht)
    _39(_43 i = 0; i < screen_w * screen_h; i++) {
        fb[i] = bb[i]; 
    }
    /// CPU anhalten, bis der User den Knopf drückt
    __asm__ _192("cli; hlt");
}
_50 read_rtc() { outb(0x70, 4); rtc_h = bcd2bin(inb(0x71)); outb(0x70, 2); rtc_m = bcd2bin(inb(0x71)); outb(0x70, 7); rtc_day = bcd2bin(inb(0x71)); outb(0x70, 8); rtc_mon = bcd2bin(inb(0x71)); outb(0x70, 9); rtc_year = bcd2bin(inb(0x71)); rtc_h = (rtc_h + 1) % 24; }
_50 get_cpu_brand() { _89 a,b,c,d; _33 _192("cpuid":"=a"(a):"a"(0x80000000)); _15(a<0x80000004){str_cpy(cpu_brand,"GENERIC");_96;} _30* s=cpu_brand; _39(_89 i=0x80000002;i<=0x80000004;i++){_33 _192("cpuid":"=a"(a),"=b"(b),"=c"(c),"=d"(d):"a"(i));*(_89*)s=a;s+=4;*(_89*)s=b;s+=4;*(_89*)s=c;s+=4;*(_89*)s=d;s+=4;} cpu_brand[48]=0; }
_50 mouse_wait(_184 type) { _89 t = 100000; _114(t--) { _15(type EQ 0 AND (inb(0x64)&1)) _96; _15(type EQ 1 AND !(inb(0x64)&2)) _96; } }
_50 mouse_write(_184 w) { mouse_wait(1); outb(0x64, 0xD4); mouse_wait(1); outb(0x60, w); }
_184 mouse_read() { mouse_wait(0); _96 inb(0x60); }
_50 init_mouse() { mouse_wait(1); outb(0x64,0xA8); mouse_wait(1); outb(0x64,0x20); mouse_wait(0); _184 s=inb(0x60)|2; mouse_wait(1); outb(0x64,0x60); mouse_wait(1); outb(0x60,s); mouse_write(0xF6); mouse_read(); mouse_write(0xF4); mouse_read(); }
_50 focus_window(_43 win_id); 
_44 mouse_down = _86;
_184 key_code = 0;
_72 _184 m_packet[3];
_72 _43 m_ptr = 0;
/// --- DER GLOBALE KEYBOARD-ROUTER ---
_50 handle_global_keys(_30 key) {
    _15(!key_new) _96; /// Abbrechen, wenn keine Taste gedrückt wurde
    _43 top_win = win_z[12]; /// Welches Fenster liegt ganz oben?
    /// BARE METAL FIX: Der Event-Eater
    /// Merkt sich, ob wir die Taste für das Menü "verbraucht" haben.
    _44 key_was_eaten = _86;
    /// --- ID 3: SYSTEM FENSTER ---
    _15(top_win EQ 3) {
        _15(last_key EQ SCAN_UP) { 
            sys_menu_selection--; 
            _15(sys_menu_selection < 0) sys_menu_selection = 8; 
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_DOWN) { 
            sys_menu_selection++; 
            _15(sys_menu_selection > 8) sys_menu_selection = 0; 
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_ENTER) {
            _15(sys_menu_selection EQ 0) system_reboot();
            _15(sys_menu_selection EQ 8) system_shutdown();
            key_was_eaten = _128;
        }
		/// Schnelle Lautstärkeregelung über Tastatur
		_15(key EQ '+') {
			_15(current_volume > 0x0000) set_ac97_volume(current_volume - 0x0101);
		}
		_15(key EQ '-') {
			_15(current_volume < 0x1F1F) set_ac97_volume(current_volume + 0x0101);
		}
    }
    /// --- ID 4: DISK MGR FENSTER ---
    _15(top_win EQ 4) { 
        /// Pfeiltasten funktionieren nur in der Laufwerks-Auswahl (view 0) oder Datei-Liste (view 1)
        _15(last_key EQ SCAN_UP) { 
            _15(dsk_view EQ 0 AND drive_count > 0) {
                active_drive_idx--; 
                _15(active_drive_idx < 0) active_drive_idx = drive_count - 1; 
            }
            _15(dsk_view EQ 1) {
                file_selection--;
                _15(file_selection < 0) file_selection = 7;
            }
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_DOWN) { 
            _15(dsk_view EQ 0 AND drive_count > 0) {
                active_drive_idx++; 
                _15(active_drive_idx >= drive_count) active_drive_idx = 0; 
            }
            _15(dsk_view EQ 1) {
                file_selection++;
                _15(file_selection > 7) file_selection = 0;
            }
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_ENTER) {
            _15(dsk_view EQ 0 AND active_drive_idx >= 0) {
                dsk_view = 1; /// Öffne Datei-Explorer
                mbr_scan();   /// Lade Daten
            }
            /// (Für dsk_view 1 könntest du hier das Öffnen von Dateien mit Enter einbauen)
            key_was_eaten = _128;
        }
    }
    /// --- ID 5: CMD FENSTER ---
    _15(top_win EQ 5) {
        _15(last_key EQ SCAN_UP) { 
            /// Hier kommt später dein: cmd_history_idx--;
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_DOWN) { 
            /// Hier kommt später dein: cmd_history_idx++;
            key_was_eaten = _128; 
        }
    }
    /// --- ID 2: SAVE AS FENSTER ---
    _15(top_win EQ 2) {
        _15(last_key EQ SCAN_UP) { 
            /// Datei in der Liste nach oben
            key_was_eaten = _128; 
        }
        _15(last_key EQ SCAN_DOWN) { 
            /// Datei in der Liste nach unten
            key_was_eaten = _128; 
        }
    }
    _15(key_was_eaten) {
        key_new = _86; 
    }
}
_50 handle_input() { 
    key_new = _86; mouse_just_pressed = _86;
    _184 st = inb(0x64);
    _114(st & 1) { 
        _184 d = inb(0x60);
        _15(st & 0x20) {
            /// === MOUSE DATEN (UNVERÄNDERT) ===
            m_packet[m_ptr++] = d;
            _15(m_ptr EQ 3) {
                m_ptr = 0;
                _184 t = m_packet[0];
                _184 x = m_packet[1];
                _184 y = m_packet[2];
                _15(!(t & 0x08)) { 
                    st = inb(0x64);
                    _37;
                }
                mouse_x += (_30) (signed char)x;
                mouse_y -= (_30) (signed char)y;
                _15(mouse_x < 0) mouse_x = 0; _15(mouse_x > 799) mouse_x = 799;
                _15(mouse_y < 0) mouse_y = 0; _15(mouse_y > 599) mouse_y = 599;
                _15(t & 1) { 
                    _15(!mouse_down) { mouse_just_pressed = _128; mouse_down = _128; } 
                } _41 { mouse_down = _86; mouse_just_pressed = _86; } 
            }
        } _41 { 
            /// === KEYBOARD DATEN ===
            _72 _44 is_ext = _86; /// Merkt sich den E0-Code über Schleifendurchläufe hinweg!
            _15(d EQ 0xE0) {
                is_ext = _128; /// Achtung, ein Fake-Shift könnte folgen!
            } _41 _15(is_ext AND (d EQ 0x2A OR d EQ 0xAA)) {
                is_ext = _86;  /// Fake-Shift (Make oder Break) eiskalt gefressen!
            } _41 {
                /// Normale Tastenverarbeitung
                _15(d < 0x80) {
                    key_new = _128; 
                    last_key = d;
                    /// Echte Shift-Taste gedrückt
                    _15(d EQ 0x2A OR d EQ 0x36) key_shift = _128;
                    _15(input_cooldown EQ 0) {
                        _43 target_win = -1;
                        _15(d EQ 0x3B) target_win = 0; /// F1
                        _15(d EQ 0x3C) target_win = 1; /// F2
                        _15(d EQ 0x3D) target_win = 3; /// F3
                        _15(d EQ 0x3E) target_win = 4; /// F4
                        _15(d EQ 0x3F) target_win = 5; /// F5
                        _15(d EQ 0x40) target_win = 11; /// F6						
                        _15(d EQ 0x41) target_win = 12; /// F7
                        _15(target_win NEQ -1) {
                            _15(windows[target_win].open) {
                                windows[target_win].open = _86;
                            } _41 {
                                windows[target_win].open = _128;
                                windows[target_win].minimized = _86;
                                focus_window(target_win);
                            }
                            input_cooldown = 10; 
                        }
                    }
                } _41 { /// TASTE LOSGELASSEN (d >= 0x80)
                    _15(d EQ 0xAA OR d EQ 0xB6) key_shift = _86;
                }
                is_ext = _86; /// Sicherheits-Reset für den nächsten normalen Tastendruck
            }
        }
        /// WICHTIG: Port neu lesen für die Schleifen-Bedingung
        st = inb(0x64); 
    } 
}
/// =========================================================
/// HARDWARE-INTERFACE: ATA PIO LBA28 WRITE
/// =========================================================
_50 ata_write_sector(_182 port, _184 slave, _89 lba, _184* buf) {
    _114 (inb(port + 7) & 0x80);
    outb(port + 6, 0xE0 | (slave << 4) | ((lba >> 24) & 0x0F));
    outb(port + 1, 0x00); 
    outb(port + 2, 0x01);
    outb(port + 3, (_184)lba);
    outb(port + 4, (_184)(lba >> 8));
    outb(port + 5, (_184)(lba >> 16));
    outb(port + 7, 0x30);
    _114 (!(inb(port + 7) & 0x08));
    _39 (_43 i = 0; i < 256; i++) {
        _182 data = buf[i * 2] | (buf[i * 2 + 1] << 8);
        outw(port, data);
    }
    outb(port + 7, 0xE7); 
    _114 (inb(port + 7) & 0x80); 
}
_50 uhci_init(_89 io_addr) { usb_io_base = io_addr & ~3; outw(usb_io_base, 4); _39(_43 i=0;i<10000;i++); outw(usb_io_base, 0); outw(usb_io_base, 1); _15(inb(usb_io_base + 2) & 0) str_cpy(cmd_status, "USB INIT FAIL"); _41 { str_cpy(cmd_status, "USB HOST RUNNING"); str_cpy(ip_address, "USB SCANNING..."); str_cpy(wifi_ssids[0], "FritzBox (ENC)"); str_cpy(wifi_ssids[1], "Telekom-AB12"); str_cpy(wifi_ssids[2], "Linksys-Home"); str_cpy(wifi_ssids[3], "Open-Wifi"); } }
_50 nic_select_next() { 
    _15(nic_count EQ 0) _96; 
    active_nic_idx++; 
    _15(active_nic_idx >= nic_count) active_nic_idx = 0;
    _89 addr = found_nics[active_nic_idx].address;
    _15(found_nics[active_nic_idx].type EQ 1) { 
        str_cpy(cmd_status, "BOOTING RTL...");
        rtl8139_init(addr); 
    } _41 _15(found_nics[active_nic_idx].type EQ 2) { 
        str_cpy(cmd_status, "BOOTING INTEL...");
        intel_e1000_init(addr); 
    } _41 _15(found_nics[active_nic_idx].type EQ 3) { 
        str_cpy(mac_str, "5C:F6:DC:32:C6:47"); 
        _15(usb_io_base NEQ 0) uhci_init(usb_io_base); 
        _41 str_cpy(cmd_status, "USB IO ERR"); 
        windows[7].open = _128; windows[7].minimized = _86; wifi_state = 0; 
    }
}
_50 focus_window(_43 id) { 
    _43 found_at = -1; 
    _39(_43 i=0; i<13; i++) _15(win_z[i] EQ id) found_at = i; 
    _15(found_at EQ -1) _96;
    /// BARE METAL FIX: Verschiebt alle bis zur 12!
    _39(_43 i=found_at; i<12; i++) win_z[i] = win_z[i+1]; 
    win_z[12] = id; /// Setzt das Fenster auf den Thron!
}
/// ==========================================
/// BARE METAL FIX: BUILT-IN DEVICE TREE (Vollständig)
/// ==========================================
/// BARE METAL FIX: Hex-String zu Integer (Wird vom Notepad INSTALL Button gebraucht!)
_89 hex_str_to_int(_30* str) {
    _89 val = 0;
    _114(*str) {
        _89 b = 0;
        _15(*str >= '0' AND *str <= '9') b = *str - '0';
        _41 _15(*str >= 'A' AND *str <= 'F') b = *str - 'A' + 10;
        _41 _15(*str >= 'a' AND *str <= 'f') b = *str - 'a' + 10;
        _41 _37;
        val = (val << 4) | b;
        str++;
    }
    _96 val;
}
_50 init_builtin_device_tree() {
    oracle_entry_count = 0;
    /// ==========================================
    /// 1. WORKSTATION (Intel 6-Series / GT 1030)
    /// ==========================================
    str_cpy(oracle_db[oracle_entry_count].type, "AHCI");
    oracle_db[oracle_entry_count].vendor = 0x8086; oracle_db[oracle_entry_count].device = 0x1C02;
    oracle_db[oracle_entry_count].bar_addr = 0xFB425000; oracle_entry_count++;
    str_cpy(oracle_db[oracle_entry_count].type, "GPU");
    oracle_db[oracle_entry_count].vendor = 0x10DE; oracle_db[oracle_entry_count].device = 0x1D01;
    oracle_db[oracle_entry_count].bar_addr = 0xC0000000; oracle_entry_count++;
    /// ==========================================
    /// 2. GAMING PC (Intel Z390 / GTX 1060)
    /// ==========================================
    str_cpy(oracle_db[oracle_entry_count].type, "AHCI");
    oracle_db[oracle_entry_count].vendor = 0x8086; oracle_db[oracle_entry_count].device = 0xA102;
    oracle_db[oracle_entry_count].bar_addr = 0xF7124000; oracle_entry_count++;
    str_cpy(oracle_db[oracle_entry_count].type, "GPU");
    oracle_db[oracle_entry_count].vendor = 0x10DE; oracle_db[oracle_entry_count].device = 0x1C03;
    oracle_db[oracle_entry_count].bar_addr = 0xE0000000; oracle_entry_count++;
    /// ==========================================
    /// 3. LAPTOP (Intel ICH9 / Quadro NVS 160M)
    /// ==========================================
    str_cpy(oracle_db[oracle_entry_count].type, "AHCI");
    oracle_db[oracle_entry_count].vendor = 0x8086; oracle_db[oracle_entry_count].device = 0x2929;
    oracle_db[oracle_entry_count].bar_addr = 0xFED1C800; oracle_entry_count++;
    str_cpy(oracle_db[oracle_entry_count].type, "GPU");
    oracle_db[oracle_entry_count].vendor = 0x10DE; oracle_db[oracle_entry_count].device = 0x06EB;
    oracle_db[oracle_entry_count].bar_addr = 0xE0000000; oracle_entry_count++;
    /// ==========================================
    /// 4. QEMU EMULATOR
    /// ==========================================
    str_cpy(oracle_db[oracle_entry_count].type, "AHCI");
    oracle_db[oracle_entry_count].vendor = 0x8086; oracle_db[oracle_entry_count].device = 0x2922;
    oracle_db[oracle_entry_count].bar_addr = 0xFEBB1000; oracle_entry_count++;
    str_cpy(oracle_db[oracle_entry_count].type, "NET");
    oracle_db[oracle_entry_count].vendor = 0x8086; oracle_db[oracle_entry_count].device = 0x100E;
    oracle_db[oracle_entry_count].bar_addr = 0x00000000; oracle_entry_count++;
    str_cpy(cmd_status, "ORACLE UPDATED WITH REAL ADDR");
}
/// ==========================================
/// BARE METAL FIX: DIE MEMORY-BRECHSTANGE
/// ==========================================
_89 find_dynamic_ahci_bar() {
    _39(_43 b=0; b<256; b++) {
        _39(_184 d=0; d<32; d++) {
            _39(_184 f=0; f<8; f++) {
                _89 id = pci_read(b, d, f, 0);
                _15((id & 0xFFFF) NEQ 0xFFFF) {
                    _89 class_rev = pci_read(b, d, f, 0x08);
                    _184 cls = (class_rev >> 24) & 0xFF;
                    _184 sub = (class_rev >> 16) & 0xFF;
                    /// Wir suchen Massenspeicher (Klasse 1)
                    _15(cls EQ 0x01) {
                        /// Wenn es AHCI ist (Subclass 6)
                        _15(sub EQ 0x06) {
                            /// ==========================================
                            /// DIE BRECHSTANGE: Command Register (0x04)
                            /// Bit 1: Memory Space Enable (RAM-Zugriff erlauben)
                            /// Bit 2: Bus Master Enable (Strom/DMA erlauben)
                            /// ==========================================
                            _89 cmd = pci_read(b, d, f, 0x04);
                            /// Wir erzwingen die Bits, löschen aber nichts anderes!
                            pci_write(b, d, f, 0x04, cmd | 0x06);
                            /// Adresse auslesen (BAR5)
                            _89 bar5 = pci_read(b, d, f, 0x24);
                            /// WICHTIG: Die unteren 4 Bits sind Status-Flags, keine Adresse!
                            /// Wir müssen sie wegschneiden, sonst greifen wir daneben.
                            _96 (bar5 & 0xFFFFFFF0); 
                        }
                    }
                }
            }
        }
    }
    _96 0; 
}
/// ==========================================
/// BARE METAL: DER UNZERSTÖRBARE PCI SCANNER
/// ==========================================
_89 read_pci_reg(_184 bus, _184 slot, _184 func, _184 offset) {
    _89 address = (_89)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    __asm__ volatile ("outl %0, %1" : : "a"(address), "Nd"((_182)0xCF8));
    _89 tmp;
    __asm__ volatile ("inl %1, %0" : "=a"(tmp) : "Nd"((_182)0xCFC));
    _96 tmp;
}
_50 write_pci_reg(_184 bus, _184 slot, _184 func, _184 offset, _89 value) {
    _89 address = (_89)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    __asm__ volatile ("outl %0, %1" : : "a"(address), "Nd"((_182)0xCF8));
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"((_182)0xCFC));
}
/// IN KERNEL.CPP
_89 force_find_ahci() {
    _39(_43 bus = 0; bus < 256; bus++) {
        _39(_43 slot = 0; slot < 32; slot++) {
            _39(_43 func = 0; func < 8; func++) {
                _89 vendor = read_pci_reg(bus, slot, func, 0) & 0xFFFF;
                _15(vendor EQ 0xFFFF) _101;
                _89 class_sub = read_pci_reg(bus, slot, func, 0x08) >> 16;
                /// Class 01 = Mass Storage, Subclass 06 = SATA
                _15(class_sub EQ 0x0106) {
                    _89 bar5 = read_pci_reg(bus, slot, func, 0x24) & 0xFFFFFFF0;
                    /// ==========================================
                    /// BARE METAL FIX: BIOS OVERRIDE
                    /// Wenn das BIOS die Karte ignoriert hat, 
                    /// brennen wir selbst eine Adresse in den PCI Chip!
                    /// ==========================================
                    _15(bar5 EQ 0) {
                        bar5 = 0xFEA00000; /// Eine sichere Adresse hoch oben im RAM
                        write_pci_reg(bus, slot, func, 0x24, bar5);
                    }
                    /// WICHTIG: Mainboard zwingen, Speicher & DMA freizugeben!
                    _89 cmd = read_pci_reg(bus, slot, func, 0x04);
                    write_pci_reg(bus, slot, func, 0x04, cmd | 0x0006);
                    
                    _96 bar5;
                }
            }
        }
    }
    _96 0;
}
_43 renaming_file_idx = 255;
/// ==========================================
/// BARE METAL: DEBUG CLASS FÜR HEAP TEST
/// ==========================================
class DebugNode {
public:
    uint32_t id;
    DebugNode* next;

    DebugNode(uint32_t node_id) {
        id = node_id;
        next = nullptr;
    }

    ~DebugNode() {}
};
/// ==========================================
///  KERNEL MAIN
/// ==========================================
_172 "C" _50 kernel_main(_89 magic, multiboot_info* mbi) {
    /// 1. Grafik-Daten von GRUB übernehmen
    _15 (magic == 0x2BADB002 && (mbi->flags & (1 << 12))) {
        fb       = (_89*)((_89)mbi->fb_addr_low);
        pitch    = mbi->fb_pitch;
        screen_w = mbi->fb_width;
        screen_h = mbi->fb_height;
    } 
    _41 {
        fb = (_89*)0xFD000000; screen_w = 800; screen_h = 600; pitch = 800*4;
    }

    /// ==========================================
    /// BARE METAL FIX: GRUB MODULE AUSLESEN
    /// ==========================================
    _15 (magic == 0x2BADB002 AND (mbi->flags & (1 << 3))) {
        _15 (mbi->mods_count > 0) {
            _89* mods = (_89*)mbi->mods_addr;
            os2_ram_address = mods[0]; /// Sichert die RAM-Adresse der KERNEL.BIN!
        }
    }

    /// 2. Den Backbuffer löschen (Hintergrund füllen)
    /// 2. Den Backbuffer löschen (Hintergrund füllen)
    /// Wir zeichnen erst auf 'bb' und kopieren es später auf 'fb'
    _39(_89 i = 0; i < (screen_w * screen_h); i++) {
        bb[i] = 0x000022; /// Ein schönes Dunkelblau
    }
    /// 3. Dein gewohnter OS-Startprozess
    init_acpi_and_pcie();
    get_cpu_brand();
    startup_melody();
    init_mouse();
    init_builtin_device_tree();
    fs_init();
    pci_scan_all();
	init_ac97();
    /// ==========================================
    /// 2. AUTO-WIRED HARDWARE MOUNTEN (via Oracle)
    /// ==========================================
    _15(sys_hw.is_ahci_found) {
        /// BARE METAL FIX: Wir geben die gefundene Adresse an deine globale Variable, 
        /// damit deine alten Lese-Funktionen (ahci_mount_drive) weiterhin perfekt funktionieren!
        global_ahci_abar = sys_hw.ahci_bar5; 
        
        ahci_init();        /// Aufruf jetzt OHNE Parameter!
        ahci_mount_drive(); /// Platten physisch erfassen
        ahci_read_mbr();    /// Dateisysteme scannen
        str_cpy(cmd_status, "AHCI HARDWARE FOUND & MOUNTED");
    } _41 {
        str_cpy(cmd_status, "CRITICAL: NO SATA CONTROLLER");
    }
	/// --- BARE METAL FIX: USB STICK ANMELDEN ---
    usb_mount_drive();
	/// ==========================================
    /// BARE METAL: USB SEKTOR-TEST (READ10)
    /// ==========================================
    
    /// Wir reservieren 512 Bytes im RAM für den Sektor (Muss DMA-sicher sein!)
    uint32_t usb_buffer_addr = 0x0C001000; 
    uint8_t* usb_buffer = (uint8_t*)usb_buffer_addr;

    /// Den Puffer zur Sicherheit vorher mit Nullen füllen, 
    /// damit wir keine alten "Geisterdaten" aus dem RAM lesen.
    for(int i = 0; i < 512; i++) usb_buffer[i] = 0;

    /// Wir rufen unseren neuen Befehl auf:
    /// dev_addr = 1, ep_in = 1, ep_out = 2 (Das sind Standardwerte für den ersten Stick)
    /// lba = 0 (Sektor 0), count = 1 (Genau ein Sektor)
    uint32_t read_status = usb_bot_read_sectors(1, 1, 2, 0, 1, usb_buffer_addr);

    if(read_status == 0) {
        /// ERFOLG! Wir prüfen die magische Boot-Signatur an Byte 510 und 511
        if(usb_buffer[510] == 0x55 && usb_buffer[511] == 0xAA) {
            str_cpy(cmd_status, "USB BOOT SECTOR (MBR) VERIFIED!");
        } else {
            str_cpy(cmd_status, "USB READ OK, BUT NO MBR FOUND.");
        }
    } else {
        str_cpy(cmd_status, "USB Sektor-Lesefehler!");
    }
    
    /// Optional: Wir drucken die ersten 16 Bytes des Sektors auf den Bildschirm,
    /// um zu sehen, was wirklich auf dem Stick steht.
    /// (Du kannst das in deiner UI-Schleife mit TextC anzeigen lassen).
    /// ==========================================
    /// 3. ERST JETZT DAS FILE SYSTEM STARTEN
    /// ==========================================
	
    str_cpy(sleep_labels[0], "SLEEP (LIGHT)"); str_cpy(sleep_labels[1], "SLEEP (DEEP)"); str_cpy(sleep_labels[2], "HIBERNATE");
    str_cpy(cmd_status, "SYSTEM BOOT SEQUENCE STARTED");
    str_cpy(sleep_labels[0], "SLEEP (LIGHT)"); str_cpy(sleep_labels[1], "SLEEP (DEEP)"); str_cpy(sleep_labels[2], "HIBERNATE");
    str_cpy(sleep_labels[3], "HYBRID SLEEP"); str_cpy(sleep_labels[4], "LOCK SCREEN"); str_cpy(sleep_labels[5], "LOG OFF");
    windows[0].id=0; str_cpy(windows[0].title, "NOTEPAD"); windows[0].x=100; windows[0].y=100; windows[0].w=400; windows[0].h=300; windows[0].color=0xEEEEEE;
    windows[1].id=1; str_cpy(windows[1].title, "APPS");    windows[1].x=150; windows[1].y=150; windows[1].w=350; windows[1].h=250; windows[1].color=0xDDDDDD;
    windows[2].id=2; str_cpy(windows[2].title, "SAVE AS..."); windows[2].x=200; windows[2].y=150; windows[2].w=300; windows[2].h=200; windows[2].color=0xDDDDDD;
    windows[3].id=3; str_cpy(windows[3].title, "SYSTEM");  windows[3].x=200; windows[3].y=50;  windows[3].w=350; windows[3].h=480; windows[3].color=0xFFD700;
    windows[4].id=4; str_cpy(windows[4].title, "DISK MGR"); windows[4].x=250; windows[4].y=200; windows[4].w=450; windows[4].h=350; windows[4].color=0x888888;
    windows[5].id=5; str_cpy(windows[5].title, "CMD");     windows[5].x=100; windows[5].y=300; windows[5].w=450; windows[5].h=250; windows[5].color=0x111111;
    windows[6].id=6; str_cpy(windows[6].title, "GENESIS DB"); windows[6].x=150; windows[6].y=100; windows[6].w=500; windows[6].h=400; windows[6].color=0x222222;
    windows[7].id=7; str_cpy(windows[7].title, "WIFI MGR"); windows[7].x=200; windows[7].y=150; windows[7].w=300; windows[7].h=300; windows[7].color=0x004488;
    windows[8].id=8; str_cpy(windows[8].title, "ACCESS");   windows[8].x=300; windows[8].y=100; windows[8].w=250; windows[8].h=300; windows[8].color=0x44AAAA;
    windows[9].id=9; str_cpy(windows[9].title, "SENSES");   windows[9].x=400; windows[9].y=100; windows[9].w=300; windows[9].h=350; windows[9].color=0x222222;
	windows[10].id=10; str_cpy(windows[10].title, "GERAETE MGR"); windows[10].x=100; windows[10].y=80; windows[10].w=600; windows[10].h=450; windows[10].color=0x111111;
	windows[11].id=11; str_cpy(windows[11].title, "ARCADE: PONG"); windows[11].x=200; windows[11].y=150; windows[11].w=450; windows[11].h=300; windows[11].color=0x112211; /// Schickes Arcade-Dunkelgrün
	windows[12].id=12; str_cpy(windows[12].title, "ARCADE: BLOBBY"); 
    windows[12].x=250; windows[12].y=100; windows[12].w=450; windows[12].h=300; 
    windows[12].color=0x4488FF; /// Himmelblau
    _43 clock_dirs[] = {213, 0, 42, 85, 128}; _39(_43 i=0;i<5;i++){ planets[i].ang = clock_dirs[i]; planets[i].dist = 10;
	planets[i].cur_x = 400; planets[i].cur_y = 300;
	str_cpy(planets[i].name, (i==0?"TXT":i==1?"APP":i==2?"SYS":i==3?"DSK":"CMD")); }
    _39(_43 i=0; i<200; i++) {
        stars[i].x = (random() % 1599) - 799;
        stars[i].y = (random() % 1199) - 599;
        stars[i].z = (random() % 1000) + 1;
        stars[i].type = random() % 1000; /// Raritäts-System (0-999)
        stars[i].speed = (random() % 4) + 2;
    }
    _43 cx = 400; _43 cy = 300; _44 is_modal_blocked = _86; _44 click_consumed = _86;
    _114(1) {
        handle_input(); Clear();
        _43 map_ids[]={0,1,3,4,5}; _15(input_cooldown > 0) input_cooldown--; 
        is_modal_blocked = (windows[2].open AND !windows[2].minimized);
        z_blocked = _86; click_consumed = _86; _44 mouse_handled = _86;
        _15(frame % 10 EQ 0) check_incoming();
        _15(mouse_down) {
             _15(drag_win NEQ -1) { windows[drag_win].x = mouse_x - drag_off_x; 
			 windows[drag_win].y = mouse_y - drag_off_y; mouse_handled=_128; click_consumed=_128; } 
             _41 _15(resize_win NEQ -1) { _43 nw = mouse_x - windows[resize_win].x; 
			 _43 nh = mouse_y - windows[resize_win].y; _15(nw > 100) windows[resize_win].w = nw; 
			 _15(nh > 100) windows[resize_win].h = nh; mouse_handled=_128; click_consumed=_128; }
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
                           _15(!is_modal_blocked OR k EQ 2) { 
                               focus_window(k);
                               _43 bx = wx + ww/2; 
                               _15(mouse_y < wy+40) { 
                                   _15(mouse_x > bx-70 AND mouse_x < bx-30) win->minimized=_128; 
                                   _41 _15(mouse_x > bx-20 AND mouse_x < bx+40) win->fullscreen = !win->fullscreen; 
                                   _41 _15(mouse_x > bx+45 AND mouse_x < bx+70) { win->open=_86; _15(win->id EQ 4) { dsk_view=0; current_path_id=255; } }
                                   _41 { drag_win = k; drag_off_x = mouse_x - wx; drag_off_y = mouse_y - wy; }
                               }
                               _15(mouse_x > wx+ww-20 AND mouse_y > wy+wh-20) { resize_win = k; }
                           }
                        }
                        mouse_handled = _128; _37; 
                    }
                }
            }
        }
		_15(active_mode EQ 0) { 
            _43 bf=++boot_frame; 
            _15(bf < 60) DrawGoldenSun(400, 300, bf * 3); 
            _41 _15(bf < 100) { Clear(); boot_logo(); _15(bf EQ 70) play_earthquake(15); }
            _41 active_mode = 1; 
        } _41 {
            _43 v_cx = 400; _43 v_cy = 300; 
            /// ==========================================
            /// DEEP SPACE BACKGROUND (Sterne, Kometen, Asteroiden, Galaxien)
            /// ==========================================
            _39(_43 i=0; i<200; i++) {
                _43 t = stars[i].type;
                /// Bewegung berechnen (Galaxien extrem langsam, Kometen extrem schnell)
                _15(t >= 995) stars[i].z -= 1; 
                _41 _15(t >= 980) stars[i].z -= stars[i].speed; 
                _41 _15(t >= 950) stars[i].z -= (stars[i].speed + 4); 
                _41 stars[i].z -= stars[i].speed;
                /// Objekt ist an der Kamera vorbeigeflogen -> Neu spawnen!
                _15(stars[i].z <= 0) { 
                    stars[i].z = 1000; 
                    stars[i].x = (random() % 1599) - 799; 
                    stars[i].y = (random() % 1199) - 599;
                    stars[i].type = random() % 1000; /// Neues Los ziehen!
                    stars[i].speed = (random() % 4) + 2;
                }
                _43 sx = v_cx + (stars[i].x * 256) / stars[i].z;
                _43 sy = v_cy + (stars[i].y * 256) / stars[i].z;
                /// Nur zeichnen, wenn es auf dem Monitor sichtbar ist
                _15(sx >= 0 AND sx < 800 AND sy >= 0 AND sy < 600) {
                    _15(t < 900) {
                        /// 1. NORMALER STERN (90%) - Fadet weich ein
                        _43 intensity = 255 - (stars[i].z / 4);
                        _15(intensity < 0) intensity = 0; _15(intensity > 255) intensity = 255;
                        _89 col = (intensity << 16) | (intensity << 8) | (intensity);
                        Put(sx, sy, col);
                    } _41 _15(t < 950) {
                        /// 2. FUNKELNDER STERN (5%) - Pulsiert durch Sinus-Welle
                        _43 pulse = (Sin((frame * 5) + i) + 256) / 2;
                        _15(pulse > 255) pulse = 255;
                        _89 col = (pulse << 16) | (pulse << 8) | 255; /// Eisblau
                        Put(sx, sy, col);
                    } _41 _15(t < 980) {
                        /// 3. KOMET (3%) - Mit leuchtendem Schweif ins Zentrum
                        Put(sx, sy, 0xFFFFFF); 
                        _43 tail1_x = v_cx + (stars[i].x * 256) / (stars[i].z + 20);
                        _43 tail1_y = v_cy + (stars[i].y * 256) / (stars[i].z + 20);
                        Put(tail1_x, tail1_y, 0xFF8800); 
                        _43 tail2_x = v_cx + (stars[i].x * 256) / (stars[i].z + 40);
                        _43 tail2_y = v_cy + (stars[i].y * 256) / (stars[i].z + 40);
                        Put(tail2_x, tail2_y, 0xAA0000);
                    } _41 _15(t < 995) {
                        /// 4. 3D ASTEROID (1.5%) - Wächst und bekommt Stein-Textur!
                        _43 r = 3000 / stars[i].z; 
                        _15(r > 0 AND r < 40) {
                            _43 r2 = r*r;
                            /// BARE METAL FIX: Sonnenlicht-Berechnung!
                            /// Wo steht der Asteroid im Verhältnis zur Sonne (Mitte)?
                            _43 focus_x = (sx - v_cx) * r / 400; 
                            _43 focus_y = (sy - v_cy) * r / 300;
                            _39(_43 cy=-r; cy<=r; cy++) {
                                _39(_43 cx=-r; cx<=r; cx++) {
                                    _15(cx*cx+cy*cy <= r2) {
                                        /// 1. Bug-Fix: Koordinaten immer positiv machen für sauberes Rauschen
                                        _43 pos_x = cx + r;
                                        _43 pos_y = cy + r;
                                        _43 noise = ((pos_x * 17 + pos_y * 31) % 40);
                                        /// 2. Lichtberechnung (Sonnenseite hell, Rückseite im Schatten)
                                        _43 lx = cx + focus_x;
                                        _43 ly = cy + focus_y;
                                        _43 l_dist = int_sqrt(lx*lx + ly*ly);
                                        _43 diffuse = 30; /// Grundhelligkeit (Schattenseite)
                                        _15(l_dist < r) {
                                            diffuse += 225 - (l_dist * 225 / r);
                                        }
                                        /// 3. Textur mit dem 3D-Licht mischen
                                        _43 gray = ((50 + noise) * diffuse) / 256;
                                        _15(gray > 255) gray = 255;
                                        /// Felsfarbe: Leicht braun-grau wie echtes Gestein
                                        Put(sx+cx, sy+cy, (gray<<16)|((gray*9)/10<<8)|((gray*8)/10)); 
                                    }
                                }
                            }
                        }
                    } _41 {
                        /// 5. MINI-GALAXIE (0.5%) - Dreht sich weit im Hintergrund
                        _43 r = 6000 / stars[i].z;
                        _15(r > 0 AND r < 60) {
                            _39(_43 j=0; j<30; j++) {
                                _43 ang1 = (j * 15 + (frame/3)) % 256;
                                _43 dist = (j * r) / 30;
                                _43 gx1 = sx + (Cos(ang1)*dist)/84;
                                _43 gy1 = sy + (Sin(ang1)*dist*3/4)/84;
                                PutAlpha(gx1, gy1, 0xAA22AA); /// Spiral-Arm 1 (Pink/Lila)
                                _43 ang2 = (ang1 + 128) % 256;
                                _43 gx2 = sx + (Cos(ang2)*dist)/84;
                                _43 gy2 = sy + (Sin(ang2)*dist*3/4)/84;
                                PutAlpha(gx2, gy2, 0x2288AA); /// Spiral-Arm 2 (Cyan/Blau)
                            }
                            Put(sx, sy, 0xFFFFFF); /// Heller Kern
                        }
                    }
                }
            }
            _15(galaxy_open AND galaxy_expansion < 320) galaxy_expansion += 8; 
			_15(!galaxy_open AND galaxy_expansion > 0) galaxy_expansion -= 10;
            DrawDenseGalaxy(v_cx, v_cy, galaxy_expansion); 
			DrawGoldenSun(v_cx, v_cy, 50); TextC(v_cx, v_cy-15, "COSMOS", 0x000000, _128);
            _15(frame % 200 EQ 0) read_rtc(); 
			_30 ts[]="00:00"; ts[0]='0'+rtc_h/10; 
			ts[1]='0'+rtc_h%10; ts[3]='0'+rtc_m/10; 
			ts[4]='0'+rtc_m%10; TextC(v_cx, v_cy+5, ts, 0x000000, _128); 
			_30 ds[]="00.00.2000"; ds[0]='0'+rtc_day/10; ds[1]='0'+rtc_day%10; 
			ds[3]='0'+rtc_mon/10; ds[4]='0'+rtc_mon%10; ds[8]='0'+(rtc_year%100)/10; 
			ds[9]='0'+rtc_year%10; TextC(v_cx, v_cy+20, ds, 0x000000, _128);
            /// 1. Alle 5 Fenster/Planeten durchgehen
            _39(_43 i=0; i<5; i++) {
                Window* win=&windows[map_ids[i]];
                _43 target_x, target_y; _44 draw_moons = _86;
                _15(win->minimized) { target_x = 250 + (i * 70); target_y = 560; } 
                _41 _15(win->open) { _43 orbit_dist = 60 + i*50; target_x = v_cx + (Cos(planets[i].ang) * orbit_dist) / 84; 
                target_y = v_cy + (Sin(planets[i].ang) * orbit_dist * 3/4) / 84; draw_moons = _128; }
                _41 {
                    _15(galaxy_expansion >= 100) { 
                        _15(planets[i].dist < 60 + i*50) planets[i].dist += 2; 
                        _15(planets[i].dist > 50) { 
                            _15(frame % (8+i) EQ 0) planets[i].ang = (planets[i].ang + 1) % 256; 
                        } 
                    } _41 { 
                        _15(planets[i].dist > 10) planets[i].dist -= 8; 
                    }
                    target_x = v_cx + (Cos(planets[i].ang) * planets[i].dist) / 84; 
                    target_y = v_cy + (Sin(planets[i].ang) * planets[i].dist * 3/4) / 84;
                }
                /// Position aktualisieren
                planets[i].cur_x += (target_x - planets[i].cur_x) / 4; 
                planets[i].cur_y += (target_y - planets[i].cur_y) / 4;
                /// ==========================================
                /// ARSHIVA LIVE STREAMING (Hier ist der saubere Hook!)
                /// ==========================================
                _182 dynamic_sensor = (_182)planets[i].cur_x + (_182)planets[i].cur_y;
                tba_record_frame(dynamic_sensor, 
                                 (_184)(planets[i].cur_x % 256), 
                                 (_184)(planets[i].cur_y % 256), 
                                 (_184)(planets[i].dist),        
                                 255);
                /// ==========================================
                /// UI RENDERN (ORGANISCHE PLANETEN)
                /// ==========================================
                _15(galaxy_expansion > 10 OR win->minimized OR win->open) {
                    _43 px = planets[i].cur_x; _43 py = planets[i].cur_y;
                    _44 hov = is_over(mouse_x, mouse_y, px, py, 20);
                    /// Neue, reifere Cosmic-Farben (weniger giftig)
                    _89 p_col = 0x888888;         
                    _15(i EQ 0) p_col = 0xA05566; /// TXT = Rostiges Mars-Rot
                    _15(i EQ 1) p_col = 0x44AA88; /// APP = Sanftes Meeres-Grün
                    _15(i EQ 2) p_col = 0x6677CC; /// SYS = Eis-Blau / Violett
                    _15(i EQ 3) p_col = 0xCC9955; /// DSK = Wüsten-Gold / Jupiter
                    _15(i EQ 4) p_col = 0x8899AA; /// CMD = Stahl-Blau
                    _15(hov AND !z_blocked) {
                        /// Leichtes Aufhellen beim Hovern
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
                    _15(!z_blocked AND mouse_just_pressed AND !is_modal_blocked AND !click_consumed AND hov) { 
                        _15(win->minimized) win->minimized = _86; 
                        _41 { win->open = _128; focus_window(win->id); _15(win->id EQ 4) { dsk_view=0; current_path_id=255; input_cooldown=10; ahci_mount_drive(); } }
                    }
                }
			}
            _15(key_shift) { DrawRoundedRect(740, 570, 50, 20, 2, 0xFFFFFF); Text(745, 575, "SHIFT", 0x000000, _128); }
            /// 1. Alle 12 Fenster durchgehen
            _39(_43 i=0; i<13; i++) {
                _43 k = win_z[i]; 
                Window* win = &windows[k];
				_15(!win->open OR win->minimized) _101;
				_44 blocked = (is_modal_blocked AND k NEQ 2);
				_43 wx=(win->fullscreen?0:win->x); 
				_43 wy=(win->fullscreen?0:win->y); 
				_43 ww=(win->fullscreen?800:win->w); 
				_43 wh=(win->fullscreen?600:win->h);
				_89 win_bg = win->color; 
				_15(is_modal_blocked AND k NEQ 2) win_bg = 0x333333; /// Blockierte Fenster werden dunkelgrau
				/// --- BARE METAL FIX: MILCHGLAS AKTIVIEREN ---
				DrawGlassRect(wx, wy, ww, wh, 12, win_bg);
				/// Hauchdünne Highlights (Rahmen) oben und links
				DrawRoundedRect(wx+12, wy, ww-24, 1, 0, 0x999999);
				DrawRoundedRect(wx, wy+12, 1, wh-24, 0, 0x999999);
				_89 txt_color = (win_bg > 0x888888) ? 0x000000 : 0xFFFFFF;
				_15(win->id EQ 10) txt_color = 0x00FF00; /// Geräte-Manager bleibt Hacker-Grün
				Text(wx+15, wy+15, win->title, txt_color, _128);
				/// --- HIER DIE SPEZIFISCHEN APPS ---
                _43 bx = wx + ww/2; Text(bx-60, wy+15, "MIN", 0x555555, _128);
				Text(bx-10, wy+15, "FULL", 0x555555, _128); Text(bx+50, wy+15, "X", 0x000000, _128);
                _15(win->id EQ 1) { 
                    TextC(wx+ww/2, wy+40, "INSTALLED APPS", 0x222222, _128);
                    /// --- ARCADE: PONG (Links) ---
                    DrawRoundedRect(wx+ww/2-155, wy+100, 140, 50, 5, 0x00FF00); 
                    TextC(wx+ww/2-85, wy+117, "PLAY PONG", 0x000000, _128); 
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww/2-155, wy+100, 140, 50)) { 
                        windows[11].open=_128; windows[11].minimized=_86; focus_window(11);
                        pong_state = 0; input_cooldown = 10;
                    }
                    /// --- ARCADE: BLOBBY VOLLEY (Rechts) ---
                    DrawRoundedRect(wx+ww/2+15, wy+100, 140, 50, 5, 0x00AAFF); 
                    TextC(wx+ww/2+85, wy+117, "PLAY BLOBBY", 0xFFFFFF, _128); 
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww/2+15, wy+100, 140, 50)) { 
                        windows[12].open=_128; windows[12].minimized=_86; focus_window(12);
                        bv_state = 0; input_cooldown = 10;
                    }
                }
                _15(win->id EQ 3) {
					_44 oracle_active = (windows[10].open AND !windows[10].minimized);
                    _43 mid=wx+ww/2; _43 btn_y = wy + 45; _30 lang_lbl[20], theme_lbl[30];
                    _15(sys_lang EQ 0) str_cpy(lang_lbl, "[ LANG: EN ]"); _41 str_cpy(lang_lbl, "[ SPR: DE ]");
                    _15(sys_lang EQ 0) { _15(sys_theme EQ 0) str_cpy(theme_lbl, "[ THEME: COMPUTER ]"); _41 str_cpy(theme_lbl, "[ THEME: GENESIS ]"); } 
					_41 { _15(sys_theme EQ 0) str_cpy(theme_lbl, "[ THEMA: COMPUTER ]"); 
					_41 str_cpy(theme_lbl, "[ THEMA: GENESIS ]"); }
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !oracle_active AND is_over_rect(mouse_x, mouse_y, wx+5, btn_y, 140, 20)) { 
					sys_lang = !sys_lang; input_cooldown = 10; }
                    Text(wx+10, btn_y+4, lang_lbl, 0x000000, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !oracle_active AND is_over_rect(mouse_x, mouse_y, wx+5, btn_y+30, 200, 20)) { 
					sys_theme = !sys_theme; input_cooldown = 10; }
                    Text(wx+10, btn_y+34, theme_lbl, 0x000000, _128);
                    TextC(mid, wy+45, "REBOOT", (sys_menu_selection EQ 0)?0x555555:0x000000, _128);
                    _39(_43 s=0; s<6; s++) { 
                        _43 ly = wy+85+s*15; _44 hov = is_over_rect(mouse_x, mouse_y, mid-50, ly-2, 100, 12);
                        _89 sc = (sleep_mode_idx EQ s OR sys_menu_selection EQ s+2) ? 0x888888 : 0x000000; _15(hov) sc = 0xFFFFFF;
                        _15(hov) DrawRoundedRect(mid-50, ly-2, 100, 14, 2, 0x000000); 
						TextC(mid, ly, sleep_labels[s], sc, _128); 
						_15(mouse_just_pressed AND  !blocked AND hov) sleep_mode_idx = s;
                    }
					/// --- BUTTON ZUM GERÄTE MANAGER (ORAKEL) ---
                    DrawRoundedRect(wx+20, wy+wh-80, ww-40, 25, 4, 0x00FF00); 
                    TextC(mid, wy+wh-73, "OPEN HARDWARE ORACLE", 0x000000, _128);
                    _15(mouse_just_pressed AND !oracle_active AND is_over_rect(mouse_x, mouse_y, wx+20, wy+wh-80, ww-40, 25)) {
                        windows[10].open = _128; windows[10].minimized = _86; focus_window(10);
                        input_cooldown = 10;
                    }
                    /// User und CPU bleiben mittig
                    TextC(mid, wy+190, user_name, 0x222222, _128); 
                    TextC(mid, wy+205, cpu_brand, 0x222222, _86);
                    /// Netzdaten am rechten Rand
                    _43 right_x = wx + ww - 160;
                    Text(right_x, wy+190, ip_address, 0x222222, _128); 
                    Text(right_x, wy+205, net_mask, 0x555555, _86); 
                    Text(right_x, wy+220, gateway_ip, 0x555555, _86);
                    DrawRoundedRect(wx+20, wy+240, ww-40, 2, 0, 0xAAAAAA);
                    _71 _30* l_hw = sys_lang ? "HARDWARE GEFUNDEN" : "DETECTED HARDWARE"; 
                    TextC(mid, wy+250, l_hw, 0x000000, _128);
                    _30 l_disk[30], l_net[30], l_gpu[30], l_usb[30];
                    _15(sys_theme EQ 0) { 
                        str_cpy(l_disk, sys_lang ? "FESTPLATTE:" : "STORAGE:"); 
                        str_cpy(l_net, sys_lang ? "NETZWERK:" : "NETWORK:"); 
                        str_cpy(l_gpu, sys_lang ? "GRAFIK:" : "GRAPHICS:"); 
                        str_cpy(l_usb, sys_lang ? "USB HOST:" : "USB HOST:"); 
                    } _41 { 
                        str_cpy(l_disk, sys_lang ? "ERD-ANKER:" : "TERRA ANCHOR:"); 
                        str_cpy(l_net, sys_lang ? "KOSMOS NETZ:" : "COSMIC WEB:"); 
                        str_cpy(l_gpu, sys_lang ? "REALITAETS RENDERER:" : "REALITY RENDERER:"); 
                        str_cpy(l_usb, sys_lang ? "DATEN INJEKTOR:" : "DATA INJECTOR:"); 
                    }
                    /// ==========================================
                    /// BARE METAL FIX: STORAGE KLICK & RENDER LOGIK
                    /// ==========================================
                    _72 _43 view_disk_idx = 0;
                    /// 1. KLICK ABFANGEN: Durch Festplatten schalten
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+275, 250, 20)) { 
                        _15(drive_count > 0) {
                            view_disk_idx++;
                            _15(view_disk_idx >= drive_count) view_disk_idx = 0;
                            str_cpy(hw_disk, drives[view_disk_idx].model);
                        }
                        input_cooldown = 10;
                    }
                    /// 2. RENDERN: Label und Festplatten-Name
                    Text(wx+30, wy+280, l_disk, 0xAAAAAA, _128); 
                    _15(drive_count > 0) {
                        /// Sicherheits-Check
                        _15(view_disk_idx >= drive_count) view_disk_idx = 0;
                        /// Pfeil bei mehreren Platten
                        _15(drive_count > 1) { Text(wx+150, wy+280, ">", 0xFFFFFF, _128); }
                        /// Plattenname in Ozeanblau
                        Text(wx+165, wy+280, drives[view_disk_idx].model, 0x0044CC, _128);
                        /// --- SCAN BUTTON FÜR FAT32 ---
                        DrawRoundedRect(wx+30, wy+300, 60, 20, 4, 0x444444);
                        Text(wx+42, wy+305, "SCAN", 0xFFFFFF, _128);
                        /// Klick auf den SCAN-Button
                        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+300, 60, 20)) { 
                            _30* mod = drives[view_disk_idx].model;
                            _44 is_fat32 = _86;
                            _44 is_cfs = _86;
                            /// Dateisystem am Namen erkennen
                            _39(_43 c=0; c<40; c++) {
                                _15(mod[c] EQ 'F' AND mod[c+1] EQ 'A' AND mod[c+2] EQ 'T') is_fat32 = _128;
                                _15(mod[c] EQ 'C' AND mod[c+1] EQ 'F' AND mod[c+2] EQ 'S') is_cfs = _128;
                            }
                            _15(is_fat32) {
                                /// FAT32 Scanner abfeuern
                                fat32_read_root_dir(drives[view_disk_idx].base_port);
                            } _41 _15(is_cfs) {
                                /// BARE METAL FIX: CFS Dateinamen auslesen!
                                _184 cfs_buf[512];
                                ahci_read_sectors(drives[view_disk_idx].base_port, 1002, 1, (uint64_t)cfs_buf);
                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)cfs_buf;
                                _44 found = _86;
                                /// Inhaltsverzeichnis nach der ersten echten Datei durchsuchen
                                _39(_43 i=0; i<8; i++) {
                                    _15(entries[i].type EQ 1) {
                                        str_cpy(cmd_status, entries[i].filename);
                                        found = _128;
                                        _37; /// Erste gefundene Datei anzeigen und Schleife abbrechen
                                    }
                                }
                                _15(!found) {
                                    str_cpy(cmd_status, "CFS: DIR IS EMPTY");
                                }
                            } _41 {
                                /// Weder FAT32 noch CFS gefunden
                                str_cpy(cmd_status, "ERR: RAW UNFORMATTED DRIVE");
                            }
                            click_consumed = _128;
                            input_cooldown = 10;
                        }
                        /// Gefundene Dateien anzeigen
                        Text(wx+100, wy+305, "FILES:", 0xAAAAAA, _86);
                        Text(wx+150, wy+305, cmd_status, 0x00FFFF, _86); 

                    } _41 {
                        Text(wx+165, wy+280, "NO DRIVES MOUNTED", 0xFF0000, _86);
                    }
					/// ==========================================
                    /// BOOT KERNEL V2 BUTTON (GRUB MODULE EDITION)
                    /// ==========================================
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+450, 250, 20)) {
                        _15(os2_ram_address NEQ 0) {
                            str_cpy(cmd_status, "GRUB MODULE FOUND! JUMPING...");
                            Swap();
                            /// Zündung OHNE Parameter!
                            execute_kernel(); 
                        } _41 {
                            str_cpy(cmd_status, "ERR: GRUB DID NOT LOAD KERNEL.BIN!");
                        }
                        input_cooldown = 30;
                    }
                    Text(wx+30, wy+450, "[ >>> KERNEL V2 STARTEN <<< ]", 0x00FF00, _128);
                    /// ==========================================
                    /// NETZWERK
                    /// ==========================================
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+335, 250, 20)) {
                        str_cpy(cmd_status, "NETZWERK: CHECKING LINK...");
                        input_cooldown = 10;
                    }
                    Text(wx+30, wy+340, l_net, 0xAAAAAA, _128);  
                    Text(wx+190, wy+340, hw_net, 0x0044CC, _128);
                    /// ==========================================
                    /// GPU
                    /// ==========================================
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+360, 250, 20)) {
                        _15(gpu_count > 0) {
                            _72 _43 view_gpu_idx = 0;
                            view_gpu_idx++;
                            _15(view_gpu_idx >= gpu_count) view_gpu_idx = 0;
                            str_cpy(hw_gpu, hw_gpu_list[view_gpu_idx]);
                            str_cpy(cmd_status, "GPU: CYCLE INFO");
                        }
                        input_cooldown = 10;
                    }
                    Text(wx+30, wy+365, l_gpu, 0xAAAAAA, _128);  
                    Text(wx+190, wy+365, hw_gpu, 0x0044CC, _128);

                    /// ==========================================
                    /// USB (HARDWARE DEBUGGER) - BRUTE FORCE SWEEPER
                    /// ==========================================
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+385, 250, 20)) {
                        uint32_t usb_buffer_addr = 0x0C001000; 
                        uint8_t* usb_buffer = (uint8_t*)usb_buffer_addr;
                        _43 stick_found = 0;
                        /// Wir scannen Gerät 1 bis 4 und Endpoints 1 bis 4
                        for(_43 dev = 1; dev <= 4; dev++) {
                            for(_43 ep_in = 1; ep_in <= 4; ep_in++) {
                                for(_43 ep_out = 1; ep_out <= 4; ep_out++) {
                                    /// IN und OUT sind nie derselbe Endpoint
                                    _15(ep_in == ep_out) continue;
                                    /// Puffer leeren
                                    for(int i = 0; i < 512; i++) usb_buffer[i] = 0;
                                    /// Feuer frei!
                                    if(usb_bot_read_sectors(dev, ep_in, ep_out, 0, 1, usb_buffer_addr) == 0) {
                                        /// Hat der Stick geantwortet?
                                        if(usb_buffer[510] == 0x55 && usb_buffer[511] == 0xAA) {
                                            str_cpy(cmd_status, "USB JACKPOT! MBR GEFUNDEN!");
                                            stick_found = 1;
                                            break;
                                        }
                                    }
                                }
                                if(stick_found) break;
                            }
                            if(stick_found) break;
                        }
                        if(!stick_found) {
                            str_cpy(cmd_status, "USB SWEEP: KEIN STICK ANTWORTET (EHCI/XHCI?)");
                        }
                        input_cooldown = 20;
                    }
                    Text(wx+30, wy+390, l_usb, 0xAAAAAA, _128);  
                    Text(wx+190, wy+390, hw_usb, 0x0044CC, _128);
                    /// ==========================================
                    /// AUDIO (AC97 TESTER)
                    /// ==========================================
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+30, wy+410, 250, 20)) {
                        _15(ac97_bus_port != 0) {
                            play_ac97_flare(); /// Zündet deinen Sonnen-Sound als Test!
                            
                            _30 port_str[15];
                            hex_to_str(ac97_bus_port, port_str);
                            str_cpy(cmd_status, "AUDIO: AC97 DMA FIRE @ PORT ");
                            str_cat(cmd_status, port_str);
                        } _41 {
                            str_cpy(cmd_status, "AUDIO: CHIP SLEEPING OR NOT FOUND");
                        }
                        input_cooldown = 15;
                    }
                    Text(wx+30, wy+415, "AUDIO", 0xAAAAAA, _128);  
                    
                    /// Zeigt blaues "ONLINE", wenn der Orakel-Scanner den Port gefunden hat
                    _15(ac97_bus_port != 0) Text(wx+190, wy+415, "AC97 ONLINE", 0x0044CC, _128);
                    _41 Text(wx+190, wy+415, "OFFLINE", 0x555555, _128);
                    /// ==========================================
                    /// SHUT DOWN / REBOOT LOGIK
                    /// ==========================================
                    TextC(mid, wy+wh-40, "SHUT DOWN", (sys_menu_selection EQ 8)?0x555555:0x000000, _128); 
                    _15(mouse_just_pressed AND !blocked) { 
                        _15(is_over(mouse_x,mouse_y,mid,wy+45,20)) system_reboot(); 
                        _15(is_over(mouse_x,mouse_y,mid,wy+wh-40,20)) system_shutdown(); 
                    }
                }
                _15(win->id EQ 8) {
                    TextC(wx+ww/2, wy+40, "ACCESSIBILITY", 0x000000, _128);
                    _44 t_hov = is_over_rect(mouse_x, mouse_y, wx+20, wy+80, 210, 40);
                    DrawRoundedRect(wx+20, wy+80, 210, 40, 5, touch_mode ? 0x000000 : (t_hov ? 0x999999 : 0xCCCCCC));
                    TextC(wx+ww/2, wy+92, touch_mode ? "TOUCH MODE: ON" : "TOUCH MODE: OFF", 0xFFFFFF, _128);
                    _15(mouse_just_pressed AND !blocked AND t_hov) touch_mode = !touch_mode;
                    Text(wx+30, wy+140, "LOGITECH HARDWARE:", 0x000000, _86);
                    Text(wx+30, wy+160, (logitech_found OR webcam_active) ? "DETECTED (C270)" : "NOT FOUND", 
					(logitech_found OR webcam_active) ? 0x000000 : 0x555555, _128);
                    DrawRoundedRect(wx+20, wy+240, 210, 30, 5, 0x333333); TextC(wx+ww/2, wy+247, "OPEN SENSES UI", 0xFFFFFF, _128);
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, wy+240, 210, 30)) { 
					windows[9].open=_128; windows[9].minimized=_86; focus_window(9); }
                }
                _15(win->id EQ 9) {
                    Text(wx+20, wy+40, "VISION", senses_tab EQ 0?0x000000:0x555555, _128); 
					Text(wx+100, wy+40, "VOICE", senses_tab EQ 1?0x000000:0x555555, _128);
                    _15(mouse_just_pressed AND !blocked) { 
					_15(is_over_rect(mouse_x, mouse_y, wx+20, wy+35, 60, 20)) senses_tab=0; 
					_15(is_over_rect(mouse_x, mouse_y, wx+100, wy+35, 60, 20)) senses_tab=1; }
                    _15(senses_tab EQ 0) { DrawRoundedRect(wx+50, wy+80, 200, 150, 5, 0x000000); 
					DrawFace(wx+150, wy+155, mouse_x, mouse_y); 
					DrawRoundedRect(wx+80, wy+250, 140, 30, 5, 0x444444); 
					TextC(wx+150, wy+257, "CALIBRATE", 0xFFFFFF, _128); } 
                    _41 { DrawRoundedRect(wx+50, wy+80, 200, 100, 5, 0xAAAAAA); 
					DrawWaveform(wx+50, wy+110, 200, frame); 
					DrawRoundedRect(wx+80, wy+200, 140, 30, 5, 0x444444); 
					TextC(wx+150, wy+207, "TEACH: 'OPEN'", 0xFFFFFF, _128); }
                }
				/// ==========================================
                /// BARE METAL FIX: ON-SCREEN KEYBOARD (RENAME)
                /// ==========================================
                _15(renaming_file_idx NEQ 255) {
                    /// Halbtransparenten Hintergrund simulieren
                    DrawRoundedRect(wx+50, wy+50, ww-100, 180, 5, 0x222222);
                    Text(wx+60, wy+60, "RENAME FILE / FOLDER:", 0x00AAFF, _128);
                    /// Aktuellen Namen anzeigen
                    DrawRoundedRect(wx+60, wy+80, 200, 20, 2, 0x000000);
                    Text(wx+65, wy+85, file_table[renaming_file_idx].name, 0xFFFFFF, _128);
                    /// Lösch-Taste (Backspace)
                    DrawRoundedRect(wx+270, wy+80, 40, 20, 2, 0xAA0000);
                    Text(wx+275, wy+85, "DEL", 0xFFFFFF, _86);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+270, wy+80, 40, 20)) {
                        _43 len = 0; _114(file_table[renaming_file_idx].name[len] NEQ 0 AND len < 12) len++;
                        _15(len > 0) file_table[renaming_file_idx].name[len-1] = 0; /// Letztes Zeichen löschen
                        input_cooldown = 10;
                    }
                    /// Die Tastatur rendern (A-Z, Unterstrich, Punkt, Zahlen)
                    _30 chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_.0123456789";
                    /// Feste Startkoordinaten für das Raster
                    _43 start_x = wx + 60; 
                    _43 cx = start_x; 
                    _43 cy = wy + 110;
                    _39(_43 i=0; i<38; i++) {
                        DrawRoundedRect(cx, cy, 20, 20, 2, 0x444444);
                        _30 s[2]; s[0] = chars[i]; s[1] = 0;
                        Text(cx+6, cy+5, s, 0xFFFFFF, _86);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, cx, cy, 20, 20)) {
                            _43 len = 0; _114(file_table[renaming_file_idx].name[len] NEQ 0 AND len < 11) len++;
                            _15(len < 11) {
                                file_table[renaming_file_idx].name[len] = chars[i];
                                file_table[renaming_file_idx].name[len+1] = 0; 
                            }
                            input_cooldown = 10;
                        }
                        cx += 25;
                        /// Harter Umbruch nach exakt 13 Tasten (unabhängig von der Fenstergröße!)
                        _15((i + 1) % 13 EQ 0) { 
                            cx = start_x; 
                            cy += 25; 
                        } 
                    }
                    /// SAVE BUTTON (Brennt den Namen per DMA!)
                    DrawRoundedRect(wx+60, cy+30, 80, 20, 3, 0x00AA00);
                    Text(wx+75, cy+35, "SAVE", 0xFFFFFF, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+60, cy+30, 80, 20)) {
                        _43 port = drives[active_drive_idx].base_port;
                        _89 dir_ram_addr = 0x07000000;
                        ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                        str_cpy(entries[renaming_file_idx].filename, file_table[renaming_file_idx].name);
                        ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                        renaming_file_idx = 255;
                        input_cooldown = 15;
                    }
                }
				/// ==========================================
                /// BARE METAL FIX: KAPAZITÄTSANZEIGE (IMMER SICHTBAR)
                /// ==========================================
                _15((dsk_view EQ 1 OR dsk_view EQ 2) AND active_drive_idx >= 0) {
                    _44 is_cfs = _86;
                    _39(_43 c=0; c<35; c++) {
                        _15(drives[active_drive_idx].model[c] EQ 'C' AND drives[active_drive_idx].model[c+1] EQ 'F' AND drives[active_drive_idx].model[c+2] EQ 'S') is_cfs = _128;
                    }
                    _15(is_cfs) {
                        _43 used_bytes = 4096;
                        /// Globale Tabelle scannen
                        _39(_43 i=0; i<8; i++) {
                            _15(file_table[i].exists AND !file_table[i].is_folder) {
                                _43 sectors = (file_table[i].size + 511) / 512;
                                used_bytes += (sectors * 512);
                            }
                        }
                        _43 total_bytes = drives[active_drive_idx].size_mb * 1024 * 1024;
                        _43 free_bytes = total_bytes - used_bytes;
                        _30 s_used[16], s_free[16], space_info[80];
                        int_to_str(used_bytes / 1024, s_used);
                        int_to_str(free_bytes / 1024, s_free);
                        str_cpy(space_info, "CFS CAPACITY: ");
                        str_cat(space_info, s_used); str_cat(space_info, " KB USED  |  ");
                        str_cat(space_info, s_free); str_cat(space_info, " KB FREE");
                        DrawRoundedRect(wx+20, wy+wh-30, ww-40, 20, 3, 0x222222);
                        TextC(wx+ww/2, wy-22+wh, space_info, 0x00AAFF, _86);
                    }
                }
				/// =========================================================
                /// APP: HARDWARE ORACLE / FORENSICS (ID 10)
                /// =========================================================
                _15(win->id EQ 10) {
                    static _43 oracle_view = 0;
                    /// --- DIE SEITENLEISTE (TOOLS) ---
                    DrawRoundedRect(wx, wy+20, 150, wh-20, 0, 0x222222);
                    Text(wx+10, wy+35, "ORACLE TOOLS", 0xAAAAAA, _128);
                    /// Tool 1: PCI Scanner
                    _89 col_t1 = (oracle_view EQ 0) ? 0x00FF00 : 0x555555;
                    DrawRoundedRect(wx+10, wy+60, 130, 25, 4, col_t1);
                    TextC(wx+75, wy+68, "PCI SCANNER", 0x000000, _128);
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+10, wy+60, 130, 25)) {
                        oracle_view = 0; input_cooldown = 10;
                    }
                    /// Tool 2: CFS Forensik (Geisterdaten-Suche)
                    _89 col_t2 = (oracle_view EQ 1) ? 0x00FF00 : 0x555555;
                    DrawRoundedRect(wx+10, wy+90, 130, 25, 4, col_t2);
                    TextC(wx+75, wy+98, "CFS FORENSICS", 0x000000, _128);
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+10, wy+90, 130, 25)) {
                        oracle_view = 1; input_cooldown = 10;
                    }
                    /// --- DER HAUPTBEREICH (RECHTS) ---
                    _43 hx = wx + 160; 
                    _43 hw = ww - 160;
                    /// ---------------------------------------------------------
                    /// VIEW 0: DEIN ORIGINALER SPIEGEL-SCAN: HYBRID ENGINE
                    /// ---------------------------------------------------------
                    _15(oracle_view EQ 0) {
                        TextC(hx+hw/2, wy+40, "SPIEGEL-SCAN: HYBRID ENGINE", 0x00FF00, _128);
                        /// BUTTON 1: STUFE 1 (Oberflächen-Scan + BAR0)
                        DrawRoundedRect(hx+20, wy+60, 120, 25, 3, 0x333333);
                        TextC(hx+80, wy+68, "1. INIT SCAN", 0xFFFFFF, _128);
                        /// BUTTON 2: STUFE 2 (Nur Nullen untersuchen!)
                        DrawRoundedRect(hx+150, wy+60, 120, 25, 3, 0x333333);
                        TextC(hx+210, wy+68, "2. DEEP READ", 0xFFFFFF, _128);
                        /// ----------------------------------------------------
                        /// KLICK 1: INIT SCAN (Sicher für alle Mainboards)
                        /// ----------------------------------------------------
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, hx+20, wy+60, 120, 25)) {
                            mirror_count = 0;
                            _39(_43 b=0; b<10; b++) {
                                _39(_184 d=0; d<32; d++) {
                                    _39(_184 f=0; f<8; f++) {
                                        _89 id = sys_pci_read(b, d, f, 0);
                                        _182 ven = id & 0xFFFF;
                                        _15(ven NEQ 0xFFFF AND ven NEQ 0x0000 AND mirror_count < 20) {
                                            mirror_list[mirror_count].bus = b;
                                            mirror_list[mirror_count].dev = d;
                                            mirror_list[mirror_count].func = f;
                                            mirror_list[mirror_count].vendor = ven;
                                            mirror_list[mirror_count].device = id >> 16;
                                            _89 bar0_raw = sys_pci_read(b, d, f, 0x10);
                                            _15(bar0_raw NEQ 0 AND bar0_raw NEQ 0xFFFFFFFF AND (bar0_raw & 1) EQ 0) {
                                                mirror_list[mirror_count].bar0 = bar0_raw & 0xFFFFFFF0;
                                            } _41 {
                                                mirror_list[mirror_count].bar0 = 0; 
                                            }
                                            _89 class_rev = sys_pci_read(b, d, f, 0x08);
                                            _184 cls = (class_rev >> 24) & 0xFF;
                                            _184 sub = (class_rev >> 16) & 0xFF;
                                            _15(cls EQ 0x01 AND sub EQ 0x06) str_cpy(mirror_list[mirror_count].name, "AHCI CONTROLLER");
                                            _41 _15(cls EQ 0x01 AND sub EQ 0x01) str_cpy(mirror_list[mirror_count].name, "IDE CONTROLLER");
                                            _41 _15(cls EQ 0x02) str_cpy(mirror_list[mirror_count].name, "NETWORK CHIP");
                                            _41 _15(cls EQ 0x03) str_cpy(mirror_list[mirror_count].name, "GRAPHICS GPU");
                                            _41 _15(cls EQ 0x0C AND sub EQ 0x03) str_cpy(mirror_list[mirror_count].name, "USB HUB (xHCI)");
                                            _41 _15(cls EQ 0x06) str_cpy(mirror_list[mirror_count].name, "PCI BRIDGE");
                                            _41 str_cpy(mirror_list[mirror_count].name, "SYSTEM DEVICE");
                                            mirror_count++;
                                        }
                                    }
                                }
                            }
                            mirror_scanned = _128;
                            input_cooldown = 10;
                        }
                        /// ----------------------------------------------------
                        /// KLICK 2: DEEP READ (Der Brücken-Inspektor)
                        /// ----------------------------------------------------
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, hx+150, wy+60, 120, 25) AND mirror_scanned) {
                            _39(_43 i=0; i<mirror_count; i++) {
                                _89 b = mirror_list[i].bus;
                                _89 d = mirror_list[i].dev;
                                _89 f = mirror_list[i].func;
                                _89 class_rev = sys_pci_read(b, d, f, 0x08);
                                _184 cls = (class_rev >> 24) & 0xFF;
                                _15(cls EQ 0x06) {
                                    _89 mem_base_limit = sys_pci_read(b, d, f, 0x20);
                                    _89 base_addr = (mem_base_limit & 0xFFFF) << 16;
                                    _15(base_addr NEQ 0) { mirror_list[i].bar0 = base_addr; }
                                }
                                _15(cls EQ 0x01 AND mirror_list[i].bar0 EQ 0) {
                                    _89 bar5 = sys_pci_read(b, d, f, 0x24);
                                    _15(bar5 NEQ 0 AND bar5 NEQ 0xFFFFFFFF) { mirror_list[i].bar0 = bar5 & 0xFFFFFFF0; }
                                }
                            }
                            input_cooldown = 10;
                        }
                        /// --- ERGEBNISSE ZEICHNEN ---
                        _15(mirror_scanned) {
                            Text(hx+20, wy+100, "B:D:F", 0xAAAAAA, _128);
                            Text(hx+80, wy+100, "VENDOR DEVICE", 0xAAAAAA, _128);
                            Text(hx+190, wy+100, "BASE ADDR", 0xAAAAAA, _128);
                            Text(hx+280, wy+100, "CLASS", 0xAAAAAA, _128);
                            _43 ry = wy+120;
                            _39(_43 i=0; i<mirror_count; i++) {
                                _30 s_b[10], s_v[10], s_d[10], s_bar[20];
                                int_to_str(mirror_list[i].bus, s_b);
                                hex_to_str(mirror_list[i].vendor, s_v);
                                hex_to_str(mirror_list[i].device, s_d);
                                _30 h1[10], h2[10];
                                hex_to_str((_182)(mirror_list[i].bar0 >> 16), h1); 
                                hex_to_str((_182)(mirror_list[i].bar0 & 0xFFFF), h2);
                                str_cpy(s_bar, h1); str_cpy(s_bar+4, h2);
                                Text(hx+20, ry, s_b, 0x888888, _86);
                                Text(hx+80, ry, s_v, 0x00FF00, _86); 
                                Text(hx+125, ry, s_d, 0x00FF00, _86);
                                _89 text_col_bar = (mirror_list[i].bar0 EQ 0) ? 0xFF0000 : 0xFFFFFF;
                                Text(hx+190, ry, s_bar, text_col_bar, _86);
                                Text(hx+280, ry, mirror_list[i].name, 0xCCCCCC, _128);
                                ry += 15;
                            }
                        }
                    }
                    /// ---------------------------------------------------------
                    /// VIEW 1: DAS NEUE FORENSIK TOOL (UNDELETE)
                    /// ---------------------------------------------------------
                    _41 _15(oracle_view EQ 1) {
                        Text(hx+20, wy+40, "CFS GHOST DATA RECOVERY", 0x00FF00, _128);
                        _15(active_drive_idx < 0 OR active_drive_idx >= drive_count) {
                            Text(hx+20, wy+80, "NO DRIVE SELECTED IN DISK MGR!", 0xFF0000, _128);
                        } _41 {
                            _30 title[60] = "SCANNING: ";
                            str_cat(title, drives[active_drive_idx].model);
                            Text(hx+20, wy+65, title, 0xAAAAAA, _128);
                            /// 1. Verzeichnis per DMA lesen
                            _43 port = drives[active_drive_idx].base_port;
                            _89 dir_ram_addr = 0x08000000; 
                            ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                            _43 y_off = wy + 100;
                            _44 found_ghost = _86;
                            /// 2. Geisterdaten suchen
                            _39(_43 i=0; i<8; i++) {
                                _15(entries[i].type EQ 0 AND entries[i].filename[0] NEQ 0) {
                                    found_ghost = _128;
                                    DrawRoundedRect(hx+20, y_off, hw-40, 25, 3, 0x222222);
                                    Text(hx+30, y_off+5, entries[i].filename, 0x888888, _128);
                                    Text(hx+150, y_off+5, "[DELETED]", 0xCC0000, _128);
                                    /// RECOVER BUTTON
                                    DrawRoundedRect(hx+hw-100, y_off+3, 70, 19, 3, 0x00AA00);
                                    Text(hx+hw-85, y_off+5, "RECOVER", 0xFFFFFF, _86);
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, hx+hw-100, y_off+3, 70, 19)) {
                                        entries[i].type = (entries[i].start_lba > 0) ? 1 : 2; 
                                        ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                        input_cooldown = 20;
                                        click_consumed = _128;
                                    }
                                    y_off += 30;
                                }
                            }
                            _15(!found_ghost) {
                                Text(hx+20, y_off, "NO RECOVERABLE GHOST DATA FOUND.", 0x555555, _128);
                            }
                        }
                    }
                }
				/// =========================================================
                /// APP: DISK MGR (ID 4)
                /// =========================================================
                _15(win->id EQ 4) {
                    /// --- OBERE NAVIGATION (REF / HOME) ---
                    /// 1. HOME BUTTON
                    DrawRoundedRect(wx+ww-160, wy+15, 50, 20, 5, 0x333333); 
                    Text(wx+ww-150, wy+18, "HOME", 0xFFFFFF, _128); 
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww-160, wy+15, 50, 20)) { 
                        dsk_view=0; current_folder_view_idx=-1; current_path_id=255; input_cooldown=10; 
                    }
                    /// 2. REF BUTTON
                    DrawRoundedRect(wx+ww-100, wy+15, 50, 20, 5, 0x555555); Text(wx+ww-90, wy+18, "REF", 0xFFFFFF, _128); 
                    _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+15, 50, 20)) { 
                        /// BARE METAL FIX: Hier feuern wir die Abfrage ab!
                        ahci_mount_drive(); 
                        input_cooldown = 10; 
                    }
                    /// --- DIE VERSCHIEDENEN ANSICHTEN ---
                    _15(dsk_view EQ 0) {
                        _43 mid = wx + ww / 2;
                        TextC(mid, wy + 70, "SELECT HARDWARE DRIVE", 0x333333, _128);
                        /// 1. LAUFWERKE ODER FEHLERMELDUNG
                        _15(drive_count EQ 0) {
                            TextC(mid, wy + 150, "NO DRIVES FOUND", 0x550000, _128);
                        } _41 {
                            _39(_43 i = 0; i < drive_count; i++) {
                                _43 y_pos = wy + 110 + (i * 25);
                                _89 text_col = (active_drive_idx EQ i) ? 0xFFFFFF : 0x000000;
                                _15(active_drive_idx EQ i) {
                                    DrawRoundedRect(wx + 20, y_pos - 4, ww - 40, 20, 2, 0x444444);
                                }
                                Text(wx + 30, y_pos, drives[i].model, text_col, _128);
								_30 size_str[16]; int_to_str(drives[i].size_mb, size_str);
								/// BARE METAL FIX: Platz machen für den 40-Zeichen Namen!
								Text(wx + 360, y_pos, size_str, 0x00FF00, _128); 
								Text(wx + 420, y_pos, "MB", 0x00FF00, _128);
                                _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx + 20, y_pos - 4, ww - 40, 20)) {
                                    active_drive_idx = i;
                                    input_cooldown = 10;
                                }
                            }
                        }
                        /// ==================================================
                        /// BUTTON A: OPEN DRIVE / BOOT KERNEL / FORMAT
                        /// ==================================================
                        _43 btn_y = wy + wh - 40; 
                        _89 col_open = (active_drive_idx NEQ -1 AND drive_count > 0) ? 0x005500 : 0x333333;
                        _44 is_cd = _86;
                        _15(active_drive_idx NEQ -1) {
                            /// 1. OPEN DRIVE BUTTON (Links)
                            DrawRoundedRect(wx+20, btn_y, 100, 25, 5, col_open); 
                            _30* boot_btn_text = is_cd ? (_30*)"BOOT KERNEL" : (_30*)"OPEN DRIVE";
                            TextC(wx+70, btn_y+8, boot_btn_text, 0xFFFFFF, _128);
							/// --- BARE METAL FIX: DAS MBR DISPLAY ---
                            Text(wx+255, btn_y+8, mbr_info_text, 0xCCFFEE, _86);
                            _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, btn_y, 100, 25)) { 
                                _15(is_cd) {
                                    execute_kernel();
                                } _41 {
                                    dsk_view = 1; input_cooldown = 10;
                                    _184* boot_sec = (_184*)0x04000000; 
                                    _39(_43 b=0; b<512; b++) boot_sec[b] = 0;
                                    /// Lese MBR (Sektor 0) - HIER IST DER FIX: (_89)
                                    ahci_read_sectors(drives[active_drive_idx].base_port, 0, 1, (uint64_t)boot_sec);
                                    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                    analyze_mbr(boot_sec);
                                    /// BARE METAL FIX: Wenn es eine GPT Platte ist, lesen wir den echten Header auf Sektor 1!
                                    _15(mbr_info_text[8] EQ 'G' AND mbr_info_text[9] EQ 'P' AND mbr_info_text[10] EQ 'T') {
                                        _39(_43 b=0; b<512; b++) boot_sec[b] = 0;
                                        ahci_read_sectors(drives[active_drive_idx].base_port, 1, 1, (uint64_t)boot_sec);
                                        _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                        analyze_gpt(boot_sec);
                                        /// 2. Sektor 2 lesen (Die GPT Einträge!)
                                        _15(mbr_info_text[0] EQ 'G' AND mbr_info_text[1] EQ 'P' AND mbr_info_text[2] EQ 'T') {
                                            _39(_43 b=0; b<512; b++) boot_sec[b] = 0;
                                            gpt_partition_lba = 0;
                                            ahci_read_sectors(drives[active_drive_idx].base_port, 2, 1, (uint64_t)boot_sec);
                                            _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                            analyze_gpt_entries(boot_sec);
                                            /// 3. Den VBR der gefundenen Partition lesen!
                                            _15(gpt_partition_lba > 0) {
                                                _39(_43 b=0; b<512; b++) boot_sec[b] = 0;
                                                ahci_read_sectors(drives[active_drive_idx].base_port, gpt_partition_lba, 1, (uint64_t)boot_sec);
                                                _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                                analyze_vbr(boot_sec);
                                                /// 4. MFT Record 5 lesen (Das Root-Verzeichnis!)
                                                _15(mft_start_lba > 0) {
                                                    _39(_43 b=0; b<1024; b++) boot_sec[b] = 0;
                                                    _89 record_5_lba = mft_start_lba + 10;
                                                    ahci_read_sectors(drives[active_drive_idx].base_port, record_5_lba, 2, (uint64_t)boot_sec);
                                                    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                                    analyze_mft_root(boot_sec);
                                                    is_ntfs = 1;
												}
											}
										}
									}
									/// ==================================================
									/// BARE METAL FIX: DIE DATEI-LISTE UNABHÄNGIG ZEICHNEN
									/// ==================================================
									_15(is_ntfs) {
										_43 list_start_y = wy + 80;                            
										_39(_43 i = 0; i < 8; i++) {
											_15(file_table[i].exists EQ 1) { 
												_43 item_y = list_start_y + (i * 25);
												DrawRoundedRect(wx + 20, item_y, 16, 16, 3, 0x0088CC);
												Text(wx + 50, item_y + 4, file_table[i].name, 0xFFFFFF, _86);
											}
										}
									}
                                    /// --- BARE METAL FIX: DER ULTIMATIVE MBR "BUCH-UMSCHLAG" SPRUNG ---
                                    _89 fat_boot_lba = 0;
                                    _44 is_fat32 = _86;
                                    _44 is_exfat = _86; 
                                    _44 is_ntfs = _86;
                                    _44 is_gpt = _86;
                                    _44 is_ext = _86;
                                    _44 is_apk = _86;
                                    _44 is_cfs = _86;
                                    /// Easter Egg Check 1: Ist es eine ZIP/APK Datei direkt am Start? (PK..)
                                    _15(boot_sec[0] EQ 0x50 AND boot_sec[1] EQ 0x4B AND boot_sec[2] EQ 0x03 AND boot_sec[3] EQ 0x04) {
                                        is_apk = _128;
                                    } _41 _15(boot_sec[510] EQ 0x55 AND boot_sec[511] EQ 0xAA) {
                                        /// ==========================================
                                        /// BARE METAL FIX: CFS (CUSTOM FILE SYSTEM) CHECK
                                        /// ==========================================
                                        _15(boot_sec[3] EQ 'C' AND boot_sec[4] EQ 'F' AND boot_sec[5] EQ 'S') {
                                            is_cfs = _128;
                                            _184 cfs_buf[512];
                                            /// Lese das Inhaltsverzeichnis (Sektor 1002) in den RAM
                                            ahci_read_sectors(drives[active_drive_idx].base_port, 1002, 1, (uint64_t)cfs_buf);
                                            _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)cfs_buf;
                                            _39(_43 i=0; i<8; i++) {
                                                _15(entries[i].type NEQ 0) {
                                                    file_table[i].exists = _128;
                                                    file_table[i].parent_idx = 255;
                                                    file_table[i].is_folder = (entries[i].type EQ 2) ? _128 : _86;
                                                    str_cpy(file_table[i].name, entries[i].filename);
                                                    file_table[i].size = entries[i].file_size;
                                                    str_cpy(file_table[i].date, "DMA");
                                                    file_table[i].sector_offset = entries[i].start_lba;
                                                } _41 {
                                                    file_table[i].exists = _86;
                                                }
                                            }
                                        } 
                                        /// Test 1: Kein Umschlag, direkt das Dateisystem (Super-Floppy)
                                        _41 _15(boot_sec[82] EQ 'F' AND boot_sec[83] EQ 'A' AND boot_sec[84] EQ 'T') {
                                            fat_boot_lba = 0; is_fat32 = _128;
                                        } _41 _15(boot_sec[3] EQ 'E' AND boot_sec[4] EQ 'X' AND boot_sec[5] EQ 'F') {
                                            fat_boot_lba = 0; is_exfat = _128;
                                        } _41 _15(boot_sec[3] EQ 'N' AND boot_sec[4] EQ 'T' AND boot_sec[5] EQ 'F' AND boot_sec[6] EQ 'S') {
                                            fat_boot_lba = 0; is_ntfs = _128;
											_89 mft_lba = fat_boot_lba + *(_89*)&boot_sec[48]; /// Offset 48: Cluster-Adresse der $MFT
											_43 clusters_per_mft_record = boot_sec[64];       /// Meistens negativ für Byte-Größe
                                        } _41 {
                                            /// Test 2: Es ist ein echter MBR/GPT Umschlag! Wir lesen die Tabelle.
                                            _184 part_type = boot_sec[446 + 4];
                                            _15(part_type EQ 0xEE) {
                                                /// --- BARE METAL FIX: GPT PROTECTIVE MBR GEFUNDEN!
                                                _39(_43 f=0; f<8; f++) file_table[f].exists = _86;
                                                file_table[0].exists = _128; file_table[0].parent_idx = 255; file_table[0].is_folder = _86;
                                                str_cpy(file_table[0].name, "[GPT DRIVE]"); file_table[0].size = 0; str_cpy(file_table[0].date, "SYSTEM");
                                                file_table[1].exists = _128; file_table[1].parent_idx = 255; file_table[1].is_folder = _86;
                                                str_cpy(file_table[1].name, "WINDOWS 11"); file_table[1].size = 0; str_cpy(file_table[1].date, "LOCKED");
                                                is_ntfs = _128; /// Trick, damit er nicht ins CFS-Fallback (RAW) rutscht
                                                is_gpt = _128;  /// Unser Reminder für die Zukunft!                                                
                                            } _41 _15(part_type EQ 0x83) {
                                                /// 0x83 = Linux Native Partition (ext2/3/4)
                                                is_ext = _128;
                                            } _41 _15(part_type EQ 0x0B OR part_type EQ 0x0C OR part_type EQ 0x07) {
                                                fat_boot_lba = *(_43*)&boot_sec[446 + 8]; 
                                                fs_read_sectors(active_drive_idx, fat_boot_lba, (_184*)boot_sec, 1);
                                                _15(boot_sec[82] EQ 'F' AND boot_sec[83] EQ 'A') {
                                                    is_fat32 = _128;
                                                } _41 _15(boot_sec[3] EQ 'E' AND boot_sec[4] EQ 'X' AND boot_sec[5] EQ 'F') {
                                                    is_exfat = _128; 
                                                } _41 _15(boot_sec[3] EQ 'N' AND boot_sec[4] EQ 'T' AND boot_sec[5] EQ 'F') {
                                                    is_ntfs = _128; /// Klassisches MBR Windows
                                                }
                                            }
                                        } /// <-- ENDE TEST 2
                                    } /// <-- ENDE 0x55AA CHECK
                                    _15(is_fat32) {
                                        /// 1. FAT32 Geometrie
                                        _43 bytes_per_sec = boot_sec[11] | (boot_sec[12] << 8);
                                        _43 sec_per_cluster = boot_sec[13];
                                        _43 rsvd_sec_cnt = boot_sec[14] | (boot_sec[15] << 8);
                                        _43 num_fats = boot_sec[16];
                                        _43 fat_size_32 = boot_sec[36] | (boot_sec[37] << 8) | (boot_sec[38] << 16) | (boot_sec[39] << 24);
                                        /// WICHTIG: Das Root-Directory ist verschoben um die Partition (fat_boot_lba)
                                        _43 root_dir_lba = fat_boot_lba + rsvd_sec_cnt + (num_fats * fat_size_32);
                                        /// 2. Den Cluster des Root-Directorys lesen
                                        _184 root_buf[512];
                                        fs_read_sectors(active_drive_idx, root_dir_lba, (_184*)root_buf, 1);
                                        /// 3. FAT32 Dateien ins 2. Fenster laden
                                        _43 file_idx = 0;
                                        _39(_43 offset = 0; offset < 512; offset += 32) {
                                            _15(file_idx >= 8) _37;
                                            _184 first_char = root_buf[offset];
                                            _15(first_char EQ 0x00) _37; 
                                            _15(first_char EQ 0xE5) continue;
                                            _184 attr = root_buf[offset + 11];
                                            _15(attr EQ 0x0F OR (attr & 0x06)) continue;
                                            file_table[file_idx].exists = _128; 
                                            file_table[file_idx].parent_idx = 255; 
                                            file_table[file_idx].is_folder = (attr & 0x10) ? _128 : _86;
                                            _39(_43 c = 0; c < 8; c++) {
                                                _30 ch = root_buf[offset + c];
                                                file_table[file_idx].name[c] = (ch EQ ' ') ? 0 : ch; 
                                            }
                                            file_table[file_idx].name[8] = 0;
                                            _15(!file_table[file_idx].is_folder AND root_buf[offset + 8] NEQ ' ') {
                                                _43 len = str_len(file_table[file_idx].name);
                                                file_table[file_idx].name[len] = '.';
                                                _39(_43 c = 0; c < 3; c++) {
                                                    _30 ch = root_buf[offset + 8 + c];
                                                    file_table[file_idx].name[len + 1 + c] = (ch EQ ' ') ? 0 : ch;
                                                }
                                                file_table[file_idx].name[len + 4] = 0;
                                            }
                                            file_table[file_idx].size = *(_43*)&root_buf[offset + 28];
                                            _43 start_cluster = (root_buf[offset + 20] | (root_buf[offset + 21] << 8)) << 16;
                                            start_cluster |= (root_buf[offset + 26] | (root_buf[offset + 27] << 8));
                                            _15(start_cluster >= 2) {
                                                /// WICHTIG: Die echte Sektor-Nummer der Datei (relativ zur Partition!)
                                                file_table[file_idx].sector_offset = root_dir_lba + ((start_cluster - 2) * sec_per_cluster);
                                            }
                                            str_cpy(file_table[file_idx].date, file_table[file_idx].is_folder ? "DIR" : "FILE");
                                            file_idx++;
                                        }
                                        _114(file_idx < 8) {
                                            file_table[file_idx].exists = _86;
                                            file_idx++;
                                        }
										/// --- BARE METAL FIX: EXFAT PARSER ---
                                    } _41 _15(is_exfat) {
                                        /// 1. exFAT Geometrie auslesen (Offsets sind bei exFAT anders!)
                                        _43 fat_offset = *(_43*)&boot_sec[80];       /// LBA der FAT
                                        _43 cluster_heap_offset = *(_43*)&boot_sec[88]; /// LBA der Daten
                                        _43 root_dir_cluster = *(_43*)&boot_sec[96];    /// Erster Cluster des Root-Dirs
                                        /// sec_per_cluster ist als 2^x gespeichert (z.B. 3 = 8 Sektoren)
                                        _184 sec_per_cluster_shift = boot_sec[109];
                                        _43 sec_per_cluster = 1 << sec_per_cluster_shift;
                                        /// 2. LBA des Root-Directorys berechnen
                                        _43 root_dir_lba = fat_boot_lba + cluster_heap_offset + ((root_dir_cluster - 2) * sec_per_cluster);
                                        /// 3. Einen Cluster lesen (z.B. 4096 Bytes, wir puffern 8 Sektoren für Sicherheit)
                                        static _184 exfat_buf[4096]; 
                                        fs_read_sectors(active_drive_idx, root_dir_lba, (_184*)exfat_buf, sec_per_cluster);
                                        /// 4. Einträge parsen (Jeder Eintrag ist 32 Byte)
                                        _43 file_idx = 0;
                                        _44 reading_file = _86;
                                        _43 current_size = 0;
                                        _43 current_cluster = 0;
                                        _44 is_folder = _86;
                                        _39(_43 offset = 0; offset < (sec_per_cluster * 512); offset += 32) {
                                            _15(file_idx >= 8) _37; /// UI voll
                                            _184 entry_type = exfat_buf[offset];
                                            _15(entry_type EQ 0x00) _37; /// Ende des Verzeichnisses
                                            /// 4a. Haupt-Eintrag der Datei (File Directory Entry)
                                            _15(entry_type EQ 0x85) {
                                                _184 attr = exfat_buf[offset + 4];
                                                is_folder = (attr & 0x10) ? _128 : _86;
                                                reading_file = _128;
                                                /// UI initialisieren
                                                file_table[file_idx].exists = _128;
                                                file_table[file_idx].parent_idx = 255;
                                                file_table[file_idx].is_folder = is_folder;
                                                /// Namen leeren, da er erst im 0xC1 Eintrag kommt
                                                _39(_43 i=0; i<15; i++) file_table[file_idx].name[i] = 0; 
                                            }
                                            /// 4b. Größen/Cluster-Eintrag (Stream Extension)
                                            _41 _15(entry_type EQ 0xC0 AND reading_file) {
                                                current_cluster = *(_43*)&exfat_buf[offset + 20];
                                                current_size = *(_43*)&exfat_buf[offset + 24]; /// Low 32-Bit der Größe
                                                file_table[file_idx].size = current_size;
                                                _15(current_cluster >= 2) {
                                                    file_table[file_idx].sector_offset = fat_boot_lba + cluster_heap_offset + ((current_cluster - 2) * sec_per_cluster);
                                                }
                                            }
                                            /// 4c. Namens-Eintrag (File Name Entry)
                                            _41 _15(entry_type EQ 0xC1 AND reading_file) {
                                                /// exFAT speichert Namen in UTF-16 (2 Bytes pro Zeichen). Wir lesen nur jedes zweite Byte.
                                                _43 name_len = 0;
                                                _39(_43 c = 0; c < 15 AND name_len < 12; c++) {
                                                    _30 ch = exfat_buf[offset + 2 + (c * 2)];
                                                    _15(ch EQ 0) _37; /// String Ende
                                                    file_table[file_idx].name[name_len] = ch;
                                                    name_len++;
                                                }
                                                file_table[file_idx].name[name_len] = 0;
                                                str_cpy(file_table[file_idx].date, is_folder ? "DIR" : "FILE");
                                                reading_file = _86; /// Datei abgeschlossen, weiter zur nächsten
                                                file_idx++;
                                            }
                                        }
                                        _114(file_idx < 8) {
                                            file_table[file_idx].exists = _86;
                                            file_idx++;
                                        }
										/// --- BARE METAL FIX: NTFS SICHTBAR MACHEN ---
                                    } _41 _15(is_ntfs) {
                                        _43 list_start_y = wy + 80;
                                        _39(_43 i = 0; i < 8; i++) {
                                            _15(file_table[i].exists) {
                                                _43 item_y = list_start_y + (i * 25);
                                                DrawRoundedRect(wx + 20, item_y, 16, 16, 3, 0x0088CC);
                                                /// Der Dateiname:
                                                Text(wx + 50, item_y + 4, file_table[i].name, 0xFFFFFF, _86);
                                                /// (Optional: Wenn du eine Funktion hast, um Zahlen als Text zu zeichnen, kannst du hier file_table[i].size ausgeben)
                                            }
                                        }
                                    } _41 _15(is_ext) {
                                        _39(_43 f=0; f<8; f++) file_table[f].exists = _86;
                                        file_table[0].exists = _128; file_table[0].parent_idx = 255; file_table[0].is_folder = _86;
                                        str_cpy(file_table[0].name, "[LINUX EXT]"); file_table[0].size = 0; str_cpy(file_table[0].date, "SYSTEM");
                                        file_table[1].exists = _128; file_table[1].parent_idx = 255; file_table[1].is_folder = _86;
                                        str_cpy(file_table[1].name, "ROOT FS"); file_table[1].size = 0; str_cpy(file_table[1].date, "LOCKED");
                                    /// --- BARE METAL FIX: APK EASTER EGG ---
                                    } _41 _15(is_apk) {
                                        _39(_43 f=0; f<8; f++) file_table[f].exists = _86;
                                        file_table[0].exists = _128; file_table[0].parent_idx = 255; file_table[0].is_folder = _86;
                                        str_cpy(file_table[0].name, "[ANDROID]"); file_table[0].size = 0; str_cpy(file_table[0].date, "PACKAGE");
                                        file_table[1].exists = _128; file_table[1].parent_idx = 255; file_table[1].is_folder = _86;
                                        str_cpy(file_table[1].name, "INSTALLER"); file_table[1].size = 0; str_cpy(file_table[1].date, "READY");
                                        /// --- FALLBACK: CFS INHALTSVERZEICHNIS LADEN ---
                                        _184 cfs_buf[512];
                                        fs_read_sectors(active_drive_idx, 1002, (_184*)cfs_buf, 1);
                                        _30* src = (_30*)cfs_buf;
                                        _30* dst = (_30*)file_table;
                                        _39(_43 i=0; i<sizeof(file_table); i++) dst[i] = src[i];
                                        _44 has_cfs = _86;
                                        _39(_43 i=0; i<8; i++) { _15(file_table[i].exists EQ _128) has_cfs = _128; }
                                        _15(!has_cfs) {
                                            _39(_43 i=0; i<8; i++) file_table[i].exists = _86; 
                                            file_table[0].exists = _128; file_table[0].parent_idx = 255; file_table[0].is_folder = _86;
                                            str_cpy(file_table[0].name, drives[active_drive_idx].model);
                                            file_table[0].size = 0;
                                            /// --- BARE METAL FIX: DER DMA X-RAY ---
                                            _30 hex_dbg[32];
                                            _30 h1[5], h2[5], h3[5], h4[5];
                                            byte_to_hex(boot_sec[0], h1); 
                                            byte_to_hex(boot_sec[1], h2); 
                                            byte_to_hex(boot_sec[510], h3); 
                                            byte_to_hex(boot_sec[511], h4);
                                            str_cpy(hex_dbg, "RAW: "); 
                                            str_cat(hex_dbg, h1); str_cat(hex_dbg, " "); str_cat(hex_dbg, h2); 
                                            str_cat(hex_dbg, " .. "); 
                                            str_cat(hex_dbg, h3); str_cat(hex_dbg, " "); str_cat(hex_dbg, h4);
                                            /// Anstelle von "UNFORMATTED" drucken wir den Hex-Code!
                                            str_cpy(file_table[0].date, hex_dbg);
                                        }
                                    }
                                }
                            }
                            /// 2. BARE METAL FIX: FORMAT BUTTON (Rechts, MIT SCHUTZ)
                            _44 is_safe = _128;
                            _30* mod = drives[active_drive_idx].model;
                            /// USB-STICKS SOFORT SPERREN (Hardware-Schutz!)
                            _15(drives[active_drive_idx].type EQ 3) {
                                is_safe = _86; 
                            }
                            /// Wir prüfen den String nach bekannten FS
                            _39(_43 c = 0; c < 50; c++) {
                                _15(mod[c] EQ 'N' AND mod[c+1] EQ 'T' AND mod[c+2] EQ 'F' AND mod[c+3] EQ 'S') is_safe = _86;
                                _15(mod[c] EQ 'e' AND mod[c+1] EQ 'x' AND mod[c+2] EQ 'F' AND mod[c+3] EQ 'A' AND mod[c+4] EQ 'T') is_safe = _86;
                                _15(mod[c] EQ 'F' AND mod[c+1] EQ 'A' AND mod[c+2] EQ 'T' AND mod[c+3] EQ '3' AND mod[c+4] EQ '2') is_safe = _86;
                                _15(mod[c] EQ 'I' AND mod[c+1] EQ 'S' AND mod[c+2] EQ 'O') is_safe = _86; /// CD-ROMs blockieren
                                _15(mod[c] EQ 'C' AND mod[c+1] EQ 'F' AND mod[c+2] EQ 'S') is_safe = _86; /// Bereits CFS!
                            }
                            _15(is_safe) {
                                DrawRoundedRect(wx+130, btn_y, 100, 25, 5, 0xAA0000); 
                                TextC(wx+180, btn_y+8, "FORMAT CFS", 0xFFFFFF, _128);
                                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+130, btn_y, 100, 25)) { 
                                    /// BARE METAL DMA FORMATIERUNG
                                    _43 port = drives[active_drive_idx].base_port;
                                    _89 format_ram_addr = 0x05000000; 
                                    _184* format_buf = (_184*)format_ram_addr;
                                    /// 1. MBR (Sektor 0) brennen
                                    _39(_43 i=0; i<512; i++) format_buf[i] = 0;
                                    format_buf[3] = 'C'; format_buf[4] = 'F'; format_buf[5] = 'S'; 
                                    format_buf[510] = 0x55; format_buf[511] = 0xAA; 
                                    ahci_write_sectors(port, 0, 1, format_ram_addr);
                                    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                    /// ==========================================
                                    /// BARE METAL FIX: DEEP CLEAN THE DIRECTORY
                                    /// ==========================================
                                    /// Wir nullen einen großen 4KB Puffer
                                    _39(_43 i=0; i<4096; i++) format_buf[i] = 0;
                                    /// Und brennen 8 leere Sektoren (Sektor 1002 bis 1009)
                                    ahci_write_sectors(port, 1002, 8, format_ram_addr);
                                    str_cpy(mod, "[CFS] BARE METAL DRIVE");
                                    input_cooldown = 20; 
                                }
                            } _41 {
                                /// Blockiert
                                DrawRoundedRect(wx+130, btn_y, 100, 25, 5, 0x555555); 
                                TextC(wx+180, btn_y+8, "LOCKED", 0xAAAAAA, _128);
                            }
                        }
                    } _41 _15(dsk_view EQ 1) {
                        _44 back_sel = (file_selection EQ -1); 
                        DrawRoundedRect(wx+20, wy+50, 60, 20, 5, back_sel ? 0x999999 : 0xCCCCCC); 
                        Text(wx+25, wy+55, "BACK", back_sel ? 0xFFFFFF : 0x000000, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, wy+50, 60, 20)) { 
                            dsk_view = 0; active_drive_idx = -1; input_cooldown=10;
                        }
						/// ==========================================
                        /// BARE METAL FIX: CREATION TOOLBAR (NUR CFS)
                        /// ==========================================
                        _44 is_cfs_drive = _86;
                        _39(_43 c=0; c<35; c++) {
                            _15(drives[active_drive_idx].model[c] EQ 'C' AND drives[active_drive_idx].model[c+1] EQ 'F' AND drives[active_drive_idx].model[c+2] EQ 'S') is_cfs_drive = _128;
                        }
                        _15(is_cfs_drive) {
                            /// BUTTON 1: [+ FILE]
                            DrawRoundedRect(wx+100, wy+40, 60, 20, 3, 0x222222);
                            TextC(wx+130, wy+46, "+ FILE", 0x00FF00, _86);
                            /// Wir ersetzen oracle_active durch den direkten Fenster-Check
                            _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+100, wy+40, 60, 20)) {
                                _43 port = drives[active_drive_idx].base_port;
                                _89 dir_buffer = 0x07000000;
                                ahci_read_sectors(port, 1002, 1, dir_buffer);
                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_buffer;
                                _43 free_slot = 255;
                                _39(_43 i=0; i<8; i++) { _15(entries[i].type EQ 0) { free_slot = i; _37; } }
                                _15(free_slot NEQ 255) {
                                    entries[free_slot].type = 1;
                                    str_cpy(entries[free_slot].filename, "new_file.txt");
                                    entries[free_slot].file_size = 0;
                                    entries[free_slot].start_lba = 4000 + free_slot;
                                    ahci_write_sectors(port, 1002, 1, dir_buffer);
                                    _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                    file_table[free_slot].exists = _128;
                                    file_table[free_slot].is_folder = _86;
                                    str_cpy(file_table[free_slot].name, "new_file.txt");
                                    file_table[free_slot].size = 0;
                                    file_table[free_slot].sector_offset = 4000 + free_slot;
                                    str_cpy(file_table[free_slot].date, "DMA");
                                }
                                input_cooldown = 15;
                            }
                            /// BUTTON 2: [+ DIR]
                            DrawRoundedRect(wx+170, wy+40, 60, 20, 3, 0x222222);
                            TextC(wx+200, wy+46, "+ DIR", 0xFFAA00, _86);
                            _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+170, wy+40, 60, 20)) {
                                _43 port = drives[active_drive_idx].base_port;
                                _89 dir_buffer = 0x07000000;
                                ahci_read_sectors(port, 1002, 1, dir_buffer);
                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_buffer;
                                _43 free_slot = 255;
                                _39(_43 i=0; i<8; i++) { _15(entries[i].type EQ 0) { free_slot = i; _37; } }
                                _15(free_slot NEQ 255) {
                                    entries[free_slot].type = 2;
                                    str_cpy(entries[free_slot].filename, "new_folder");
                                    entries[free_slot].file_size = 0; 
                                    entries[free_slot].start_lba = 0;
                                    ahci_write_sectors(port, 1002, 1, dir_buffer);
                                    _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                    file_table[free_slot].exists = _128;
                                    file_table[free_slot].is_folder = _128;
                                    str_cpy(file_table[free_slot].name, "new_folder");
                                    file_table[free_slot].size = 0;
                                    file_table[free_slot].sector_offset = 0;
                                    str_cpy(file_table[free_slot].date, "DMA");
                                }
                                input_cooldown = 15;
                            }
                        }
                        Text(wx+120, wy+55, "/ROOT/SYSTEM/", 0x000000, _128); 
                        Text(wx+45, wy+90, "NAME", 0x555555, _128); 
                        Text(wx+200, wy+90, "SIZE", 0x555555, _128); 
                        Text(wx+280, wy+90, "DATE", 0x555555, _128);
                        _43 y_off = wy+110;
                        /// HIER MUSS DIE SCHLEIFE STARTEN!
                        _39(_43 f=0; f<8; f++) {
                            /// 1. Prüfen, ob die Datei angezeigt werden soll
                            _44 show_file = _86;
                            /// BARE METAL FIX: Scope-Error gelöscht!
                            /// CFS-Dateien haben parent_idx = 255 und werden vom ELSE-Block ganz automatisch gezeichnet!
                            _15(is_ntfs) {
                                _15(file_table[f].exists EQ 1) show_file = _128;
                            } _41 {
                                _15(file_table[f].exists AND file_table[f].parent_idx EQ 255) show_file = _128;
                            }
                            _15(renaming_file_idx NEQ 255) show_file = _86;
                            /// 2. Nur zeichnen, wenn show_file auf _128 steht
                            _15(show_file) {
                                _44 sel = (f EQ file_selection);
                                _15(sel) DrawRoundedRect(wx+20, y_off, ww-40, 20, 0, 0xAAAAAA); 
                                DrawIcon(wx+25, y_off, file_table[f].is_folder ? 0x888888 : 0xCCCCCC); 
                                Text(wx+45, y_off+5, file_table[f].name, 0x000000, _128);
                                _15(!file_table[f].is_folder) { 
                                    _30 s_str[10]; int_to_str(file_table[f].size, s_str); 
                                    Text(wx+200, y_off+5, s_str, 0x333333, _128); 
                                } 
                                Text(wx+280, y_off+5, file_table[f].date, 0x333333, _128);
                                /// --- BARE METAL FIX: DER SAUBERE DELETE BUTTON ---
                                _44 is_cfs_drive = _86;
                                _39(_43 c=0; c<35; c++) {
                                    _15(drives[active_drive_idx].model[c] EQ 'C' AND drives[active_drive_idx].model[c+1] EQ 'F' AND drives[active_drive_idx].model[c+2] EQ 'S') is_cfs_drive = _128;
                                }
                                _15(sel AND is_cfs_drive) {
                                    /// 1. DER DEL BUTTON
                                    DrawRoundedRect(wx+350, y_off+2, 50, 16, 3, 0xCC0000); 
                                    Text(wx+355, y_off+4, "DEL", 0xFFFFFF, _86);
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+350, y_off+2, 50, 16)) {
                                        _43 port = drives[active_drive_idx].base_port;
                                        _89 dir_ram_addr = 0x07000000;
                                        ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                                        entries[f].type = 0;
                                        ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                        file_table[f].exists = _86; 
                                        file_selection = -1;
                                        input_cooldown = 15;
                                        click_consumed = _128;
                                    }
									/// BUTTON 4: [VOL-]
									DrawRoundedRect(wx+380, wy+40, 25, 20, 3, 0x444444); 
									TextC(wx+392, wy+46, "-", 0xFFFFFF, _86);
									_15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+380, wy+40, 25, 20)) {
										_15(current_volume < 0x1F1F) set_ac97_volume(current_volume + 0x0101);
										input_cooldown = 10;
									}
									/// BUTTON 5: [VOL+]
									DrawRoundedRect(wx+410, wy+40, 25, 20, 3, 0x444444); 
									TextC(wx+422, wy+46, "+", 0xFFFFFF, _86);
									_15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+410, wy+40, 25, 20)) {
										_15(current_volume > 0x0000) set_ac97_volume(current_volume - 0x0101);
										input_cooldown = 10;
									}
									/// ==========================================
									/// BUTTON 3: [STOP] (Audio anhalten)
									/// ==========================================
									DrawRoundedRect(wx+440, wy+40, 60, 20, 3, 0x880000); /// Dunkelrot
									TextC(wx+470, wy+46, "STOP", 0xFFFFFF, _86);
									_15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+440, wy+40, 60, 20)) {
										_15(ac97_bus_port NEQ 0) {
											outb(ac97_bus_port + 0x1B, 0); /// Zieht der Soundkarte den Stecker (DMA Stopp)
											str_cpy(cmd_status, "AUDIO STOPPED");
										}
										input_cooldown = 15;
									}
                                    /// 2. DER RENAME BUTTON (Jetzt sauber getrennt!)
                                    DrawRoundedRect(wx+410, y_off+2, 60, 16, 3, 0x0088FF); 
                                    Text(wx+415, y_off+4, "RENAME", 0xFFFFFF, _86);
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+410, y_off+2, 60, 16)) {
                                        renaming_file_idx = f; /// Overlay aktivieren!
                                        input_cooldown = 15;
                                        click_consumed = _128;
                                    }
									/// ==========================================
                                    /// 3. DER PLAY BUTTON (Musik abspielen!)
                                    /// ==========================================
                                    DrawRoundedRect(wx+480, y_off+2, 50, 16, 3, 0x00AA00); /// Ein sattes Play-Grün
                                    Text(wx+485, y_off+4, "PLAY", 0xFFFFFF, _86);
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+480, y_off+2, 50, 16)) {
                                        play_wav_file(f);
                                        input_cooldown = 15;
                                        click_consumed = _128;
                                    }
                                }
                                /// ==========================================
                                /// BARE METAL FIX: COPY TO CFS BUTTON (NUR AUF FAT32 SICHTBAR)
                                /// ==========================================
                                _44 is_fat_drive = _86;
                                _39(_43 c=0; c<35; c++) {
                                    _15(drives[active_drive_idx].model[c] EQ 'F' AND drives[active_drive_idx].model[c+1] EQ 'A' AND drives[active_drive_idx].model[c+2] EQ 'T') is_fat_drive = _128;
                                }
                                _15(sel AND is_fat_drive) {
                                    DrawRoundedRect(wx+350, y_off+2, 80, 16, 3, 0x0055AA); 
                                    Text(wx+355, y_off+4, "COPY TO CFS", 0xFFFFFF, _86);
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+350, y_off+2, 80, 16)) {
                                        /// 1. Quelle: Daten von FAT32 lesen
                                        _43 src_port = drives[active_drive_idx].base_port;
                                        _43 src_lba = file_table[f].sector_offset;
                                        _89 transit_buffer = 0x09000000;
                                        ahci_read_sectors(src_port, src_lba, 1, transit_buffer);
                                        _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                        /// 2. Ziel: CFS Platte suchen
                                        _43 dest_port = 0;
                                        _39(_43 p=0; p<4; p++) {
                                            _39(_43 c=0; c<35; c++) {
                                                _15(drives[p].model[c] EQ 'C' AND drives[p].model[c+1] EQ 'F' AND drives[p].model[c+2] EQ 'S') dest_port = drives[p].base_port;
                                            }
                                        }
                                        _15(dest_port NEQ 0) {
                                            /// 3. Freien Slot im CFS Verzeichnis suchen
                                            _89 dir_buffer = 0x07000000;
                                            ahci_read_sectors(dest_port, 1002, 1, dir_buffer);
                                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                            
                                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_buffer;
                                            _43 free_slot = 255;
                                            _39(_43 i=0; i<8; i++) { _15(entries[i].type EQ 0) { free_slot = i; _37; } }
                                            _15(free_slot NEQ 255) {
                                                /// 4. Daten auf CFS brennen
                                                _43 dest_lba = 4000 + free_slot;
                                                ahci_write_sectors(dest_port, dest_lba, 1, transit_buffer);
                                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                                /// 5. Verzeichnis-Eintrag schreiben
                                                entries[free_slot].type = 1;
                                                str_cpy(entries[free_slot].filename, file_table[f].name);
                                                entries[free_slot].file_size = file_table[f].size;
                                                entries[free_slot].start_lba = dest_lba;
                                                ahci_write_sectors(dest_port, 1002, 1, dir_buffer);
                                            }
                                        }
                                        input_cooldown = 20;
                                        click_consumed = _128;
                                    }
                                }
                                /// --- KLICK AUF DIE DATEI SELBST (Ordner betreten / Notepad laden) ---
                                _41 _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, y_off, 320, 20)) { 
                                    file_selection = f;
                                    _15(file_table[f].is_folder) { 
                                        dsk_view = 2; current_folder_view_idx = f; current_path_id = f; input_cooldown=10; 
                                    } _41 { 
                                        /// Datei ins Notepad laden!
                                        windows[0].open = _128; windows[0].minimized = _86; focus_window(0); 
                                        current_open_file = f;
                                        tab_files[active_tab] = f;
                                        _39(_43 r=0;r<10;r++) _39(_43 c=0;c<40;c++) note_buf[r][c]=0; 
                                        note_r=0; note_c=0;
                                        _43 start_lba = file_table[f].sector_offset;
                                        _43 num_sectors = (file_table[f].size + 511) / 512;
                                        _15(num_sectors EQ 0) num_sectors = 1; 
                                        _15(num_sectors > 128) num_sectors = 128;
                                        _15(start_lba NEQ 0) {
                                            _89 text_ram_addr = 0x04000000;
                                            _30* text_buffer = (_30*)text_ram_addr;
                                            _39(_43 i=0; i < (num_sectors * 512); i++) text_buffer[i] = 0;
                                            /// BARE METAL FIX: Universeller DMA Reader für CFS & FAT32!
                                            /// Die Hardware liest einfach stumpf ab dem errechneten Sektor
                                            ahci_read_sectors(drives[active_drive_idx].base_port, start_lba, num_sectors, text_ram_addr);
                                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                            _43 idx = 0; 
                                            _39(_43 r=0; r<10; r++) { 
                                                _43 c = 0;
                                                _114(c < 39) { 
                                                    _30 ch = text_buffer[idx++]; 
                                                    _15(ch EQ 0 OR ch EQ '\n') _37; 
                                                    /// FAT32 von Windows nutzt oft \r\n (13 10) statt nur \n. 
                                                    /// Unser Filter (>= 32) sortiert das \r automatisch aus!
                                                    _15(ch >= 32 AND ch <= 126) note_buf[r][c++] = ch; 
                                                } 
                                                note_buf[r][c] = 0; 
                                            } 
                                        }
                                        _39(_43 r=0; r<10; r++) _39(_43 c=0; c<40; c++) tab_buffers[active_tab][r][c] = note_buf[r][c];
                                    } 
                                    input_cooldown = 10;
                                }
							}
						}
						/// ==========================================
                        /// BARE METAL FIX: REAL-TIME STORAGE CALCULATOR
                        /// ==========================================
                        _15(drives[active_drive_idx].model[1] EQ 'C' AND drives[active_drive_idx].model[2] EQ 'F' AND drives[active_drive_idx].model[3] EQ 'S') {
                            _43 used_bytes = 4096; /// 4KB CFS System Overhead (MBR + Directory Sektoren)
                            /// Wir iterieren durch die RAM-Tabelle und zählen die belegten Sektoren
                            _39(_43 i=0; i<8; i++) {
                                _15(file_table[i].exists AND !file_table[i].is_folder) {
                                    /// Physische Aufrundung: Jede angefangene Datei belegt volle 512-Byte Blöcke!
                                    _43 sectors = (file_table[i].size + 511) / 512;
                                    used_bytes += (sectors * 512);
                                }
                            }
                            /// Gesamtspeicher der Festplatte in Bytes umrechnen
                            _43 total_bytes = drives[active_drive_idx].size_mb * 1024 * 1024;
                            _43 free_bytes = total_bytes - used_bytes;
                            /// Strings für das UI bauen
                            _30 s_used[16], s_free[16], space_info[80];
                            int_to_str(used_bytes / 1024, s_used);
                            int_to_str(free_bytes / 1024, s_free);
                            str_cpy(space_info, "CFS CHIP CAPACITY:  ");
                            str_cat(space_info, s_used); str_cat(space_info, " KB USED   |   ");
                            str_cat(space_info, s_free); str_cat(space_info, " KB FREE");
                            /// Unten links im Fenster in leuchtendem Cyan zeichnen
                            Text(wx+20, wy+wh-30, space_info, 0x00AAFF, _128);
                        }
                    } _41 _15(dsk_view EQ 2) { 
                        DrawRoundedRect(wx+20, wy+50, 60, 20, 5, 0xAAAAAA); Text(wx+25, wy+55, "BACK", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, wy+50, 60, 20)) { dsk_view = 1; current_path_id = 255; input_cooldown=10; } 
                        _15(current_folder_view_idx >= 0) {
                            _30 title[50]; str_cpy(title, "INSIDE FOLDER: "); _30* p = title; _114(*p) p++; _30* n = file_table[current_folder_view_idx].name; _114(*n) *p++ = *n++; *p=0; 
                            Text(wx+120, wy+55, title, 0x000000, _128);
                            /// ==========================================
                            /// BARE METAL FIX: CREATION TOOLBAR (UNTERORDNER)
                            /// HIER EINSETZEN!
                            /// ==========================================
                            _44 is_cfs_drive = _86;
                            _39(_43 c=0; c<35; c++) {
                                _15(drives[active_drive_idx].model[c] EQ 'C' AND drives[active_drive_idx].model[c+1] EQ 'F' AND drives[active_drive_idx].model[c+2] EQ 'S') is_cfs_drive = _128;
                            }
                            _15(is_cfs_drive) {
                                /// BUTTON 1: [+ FILE]
                                DrawRoundedRect(wx+300, wy+50, 60, 20, 3, 0x222222);
                                TextC(wx+330, wy+56, "+ FILE", 0x00FF00, _86);
                                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+300, wy+50, 60, 20)) {
                                    _43 free_slot = 255;
                                    _39(_43 i=0; i<8; i++) { _15(file_table[i].exists EQ _86) { free_slot = i; _37; } }
                                    _15(free_slot NEQ 255) {
                                        file_table[free_slot].exists = _128;
                                        file_table[free_slot].is_folder = _86;
                                        str_cpy(file_table[free_slot].name, "new_file.txt");
                                        file_table[free_slot].size = 0;
                                        file_table[free_slot].sector_offset = 4000 + free_slot;
                                        str_cpy(file_table[free_slot].date, "DMA");
                                        /// Wir weisen die Datei dem aktuellen Ordner zu!
                                        file_table[free_slot].parent_idx = current_folder_view_idx; 
                                    }
                                    input_cooldown = 15;
                                }
                                /// BUTTON 2: [+ DIR]
                                DrawRoundedRect(wx+370, wy+50, 60, 20, 3, 0x222222);
                                TextC(wx+400, wy+56, "+ DIR", 0xFFAA00, _86);
                                _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+370, wy+50, 60, 20)) {
                                    _43 free_slot = 255;
                                    _39(_43 i=0; i<8; i++) { _15(file_table[i].exists EQ _86) { free_slot = i; _37; } }
                                    _15(free_slot NEQ 255) {
                                        file_table[free_slot].exists = _128;
                                        file_table[free_slot].is_folder = _128;
                                        str_cpy(file_table[free_slot].name, "new_folder");
                                        file_table[free_slot].size = 0;
                                        file_table[free_slot].sector_offset = 0;
                                        str_cpy(file_table[free_slot].date, "DMA");
                                        /// Wir weisen den Ordner dem aktuellen Ordner zu!
                                        file_table[free_slot].parent_idx = current_folder_view_idx;
                                    }
                                    input_cooldown = 15;
                                }
                            }
                            /// ==========================================
                            /// ENDE CREATION TOOLBAR
                            /// ==========================================
                            Text(wx+45, wy+100, "NAME", 0x555555, _128); 
                            Text(wx+200, wy+100, "SIZE", 0x555555, _128); 
                            Text(wx+280, wy+100, "DATE", 0x555555, _128);
                            _43 y_off = wy+120;
                            _39(_43 f=0; f<8; f++) {
                                _15(file_table[f].exists AND file_table[f].parent_idx EQ current_folder_view_idx) {
                                    /// --- ZEILE FÜR ZEILE ZEICHNEN (Inklusive Größe und Datum) ---
                                    DrawIcon(wx+25, y_off, file_table[f].is_folder ? 0x888888 : 0xCCCCCC); 
                                    Text(wx+45, y_off+5, file_table[f].name, 0x000000, _128);
                                    /// Größe nur bei Dateien rendern
                                    _15(!file_table[f].is_folder) { 
                                        _30 s_str[10]; int_to_str(file_table[f].size, s_str); 
                                        Text(wx+200, y_off+5, s_str, 0x333333, _128); 
                                    }
                                    /// Datum IMMER rendern (auch für Ordner)
                                    Text(wx+280, y_off+5, file_table[f].date, 0x333333, _128);
									/// ==========================================
                                    /// BARE METAL FIX: DER DELETE BUTTON (SUBFOLDER)
                                    /// ==========================================
                                    _44 sel_sub = (f EQ file_selection);
                                    _15(sel_sub) {
                                        DrawRoundedRect(wx+350, y_off+2, 50, 16, 3, 0xCC0000); 
                                        Text(wx+355, y_off+4, "DEL", 0xFFFFFF, _86);
                                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+350, y_off+2, 50, 16)) {
                                            _15(drives[active_drive_idx].model[1] EQ 'C' AND drives[active_drive_idx].model[2] EQ 'F' AND drives[active_drive_idx].model[3] EQ 'S') {
                                                _43 port = drives[active_drive_idx].base_port;
                                                _89 dir_ram_addr = 0x07000000;
                                                ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                                                entries[f].type = 0;
                                                ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                                file_table[f].exists = _86;
                                                file_selection = -1;
                                            }
											/// BUTTON 4: [VOL-]
											DrawRoundedRect(wx+380, wy+40, 25, 20, 3, 0x444444); 
											TextC(wx+392, wy+46, "-", 0xFFFFFF, _86);
											_15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+380, wy+40, 25, 20)) {
												_15(current_volume < 0x1F1F) set_ac97_volume(current_volume + 0x0101);
												input_cooldown = 10;
											}
											/// BUTTON 5: [VOL+]
											DrawRoundedRect(wx+410, wy+40, 25, 20, 3, 0x444444); 
											TextC(wx+422, wy+46, "+", 0xFFFFFF, _86);
											_15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+410, wy+40, 25, 20)) {
												_15(current_volume > 0x0000) set_ac97_volume(current_volume - 0x0101);
												input_cooldown = 10;
											}
											/// ==========================================
											/// BUTTON 3: [STOP] (Audio anhalten)
											/// ==========================================
											DrawRoundedRect(wx+440, wy+50, 60, 20, 3, 0x880000); 
											TextC(wx+470, wy+56, "STOP", 0xFFFFFF, _86);
											_15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+440, wy+50, 60, 20)) {
												_15(ac97_bus_port NEQ 0) {
													outb(ac97_bus_port + 0x1B, 0); /// Zieht der Soundkarte den Stecker (DMA Stopp)
													str_cpy(cmd_status, "AUDIO STOPPED");
												}
												input_cooldown = 15;
											}
											/// ==========================================
											/// 3. DER PLAY BUTTON (Musik abspielen!)
											/// ==========================================
											DrawRoundedRect(wx+480, y_off+2, 50, 16, 3, 0x00AA00); /// Ein sattes Play-Grün
											Text(wx+485, y_off+4, "PLAY", 0xFFFFFF, _86);
											_15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+480, y_off+2, 50, 16)) {
												play_wav_file(f);
												input_cooldown = 15;
												click_consumed = _128;
											}
										}
									}
                                    /// HIER KOMMT RENAME HIN:
                                    DrawRoundedRect(wx+410, y_off+2, 60, 16, 3, 0x0088FF); 
                                    Text(wx+415, y_off+4, "RENAME", 0xFFFFFF, _86);
                                    
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND !(windows[10].open AND !windows[10].minimized) AND is_over_rect(mouse_x, mouse_y, wx+410, y_off+2, 60, 16)) {
                                        renaming_file_idx = f; /// Das Tastatur-Overlay vom Root-Ordner übernimmt den Rest!
                                        input_cooldown = 15;
                                        click_consumed = _128;
                                    }									
                                    /// --- BARE METAL Klick-Logik DSK_VIEW 2 ---
                                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+20, y_off, ww-40, 20)) { 
                                        file_selection = f;
                                        /// Ist es ein Ordner oder eine Datei?
                                        _15(file_table[f].is_folder) { 
                                            /// Ordner betreten
                                            current_folder_view_idx = f; 
                                            current_path_id = f; 
                                        } _41 {
                                            /// Datei ins Notepad laden
                                            windows[0].open = _128; windows[0].minimized = _86; focus_window(0); 
                                            current_open_file = f;
                                            tab_files[active_tab] = f;
                                            /// Notepad Puffer leeren
                                            _39(_43 r=0;r<10;r++) _39(_43 c=0;c<40;c++) note_buf[r][c]=0; 
                                            note_r=0; note_c=0;
                                            /// --- BARE METAL FIX: DER NEUE UNIVERSAL-LOADER ---
                                            _43 start_lba = file_table[f].sector_offset;
                                            _43 num_sectors = (file_table[f].size + 511) / 512;
                                            _15(num_sectors EQ 0) num_sectors = 1; 
                                            _15(num_sectors > 128) num_sectors = 128; /// Sicherheits-Limit: Max 64 KB laden
                                            _89 text_ram_addr = 0x03000000;
                                            _30* text_buffer = (_30*)text_ram_addr;
                                            /// Puffer nullen, damit kein Müll von alten Dateien bleibt
                                            _39(_43 i=0; i < (num_sectors * 512); i++) text_buffer[i] = 0;
                                            /// ROUTER ABFEUERN! (Die SSD / HDD schreibt jetzt per DMA in den RAM)
                                            fs_read_sectors(active_drive_idx, start_lba, (_184*)text_ram_addr, num_sectors);
                                            /// --- BARE METAL FIX: TEXT FILTER & RENDERING ---
                                            _43 idx = 0; 
                                            _39(_43 r=0; r<10; r++) { 
                                                _39(_43 c=0; c<40; c++) { 
                                                    _30 ch = text_buffer[idx++]; 
                                                    _15(ch < 32 OR ch > 126) ch = 0; /// Wir lassen nur echten Text durch!
                                                    note_buf[r][c] = ch; 
                                                } 
                                                note_buf[r][40] = 0; /// Zeilenende sicherstellen
                                            }
                                            /// Geladenen Text sofort in den Hintergrund-Tab kopieren
                                            _39(_43 r=0; r<10; r++) _39(_43 c=0; c<40; c++) tab_buffers[active_tab][r][c] = note_buf[r][c];
                                        }
                                        input_cooldown = 10;
                                    }
                                    y_off += 25;
                                }
                            }
                        }
					}
				}
				/// ==========================================
                /// BARE METAL FIX: REAL-TIME STORAGE CALCULATOR
                /// ==========================================
                _15(drives[active_drive_idx].model[1] EQ 'C' AND drives[active_drive_idx].model[2] EQ 'F' AND drives[active_drive_idx].model[3] EQ 'S') {
                    _43 used_bytes = 4096; /// 4KB CFS System Overhead (MBR + Directory Sektoren)
                    /// Wir iterieren durch die RAM-Tabelle und zählen die belegten Sektoren
                    _39(_43 i=0; i<8; i++) {
                        _15(file_table[i].exists AND !file_table[i].is_folder) {
                            /// Physische Aufrundung: Jede angefangene Datei belegt volle 512-Byte Blöcke!
                            _43 sectors = (file_table[i].size + 511) / 512;
                            used_bytes += (sectors * 512);
                        }
                    }
                    /// Gesamtspeicher der Festplatte in Bytes umrechnen
                    _43 total_bytes = drives[active_drive_idx].size_mb * 1024 * 1024;
                    _43 free_bytes = total_bytes - used_bytes;
                    /// Strings für das UI bauen
                    _30 s_used[16], s_free[16], space_info[80];
                    int_to_str(used_bytes / 1024, s_used);
                    int_to_str(free_bytes / 1024, s_free);
                    str_cpy(space_info, "CFS CHIP CAPACITY:  ");
                    str_cat(space_info, s_used); str_cat(space_info, " KB USED   |   ");
                    str_cat(space_info, s_free); str_cat(space_info, " KB FREE");
                    /// Unten links im Fenster in leuchtendem Cyan zeichnen
                    Text(wx+20, wy+wh-30, space_info, 0x00AAFF, _128);
                }
				/// =========================================================
                /// APP: BLOBBY VOLLEY ARCADE (ID 12)
                /// =========================================================
                _15(win->id EQ 12) {
                    _43 inner_w = ww; _43 inner_h = wh - 40;
                    _43 py = wy + 40;
                    _43 floor_y = inner_h - 20; /// Der Sand/Rasen
                    _15(bv_state EQ 0) {
                        TextC(wx + ww/2, py + 20, "BLOBBY VOLLEY BARE METAL", 0x00FF00, _128);
                        TextC(wx + ww/2, py + 60, "SELECT MODE", 0xFFFFFF, _128);
                        DrawRoundedRect(wx + ww/2 - 110, py + 80, 100, 25, 3, bv_mode EQ 0 ? 0x00AA00 : 0x555555);
                        TextC(wx + ww/2 - 60, py + 87, "1 PLAYER", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 110, py + 80, 100, 25)) bv_mode = 0;
                        DrawRoundedRect(wx + ww/2 + 10, py + 80, 100, 25, 3, bv_mode EQ 1 ? 0x00AA00 : 0x555555);
                        TextC(wx + ww/2 + 60, py + 87, "2 PLAYERS", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 10, py + 80, 100, 25)) bv_mode = 1;
                        _15(bv_mode EQ 0) {
                            TextC(wx + ww/2, py + 130, "CPU DIFFICULTY", 0xFFFFFF, _128);
                            DrawRoundedRect(wx + ww/2 - 120, py + 150, 70, 25, 3, bv_diff EQ 0 ? 0x00AA00 : 0x555555); TextC(wx + ww/2 - 85, py + 157, "EASY", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 120, py + 150, 70, 25)) bv_diff = 0;
                            DrawRoundedRect(wx + ww/2 - 35, py + 150, 70, 25, 3, bv_diff EQ 1 ? 0x00AA00 : 0x555555); TextC(wx + ww/2, py + 157, "NORMAL", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 35, py + 150, 70, 25)) bv_diff = 1;
                            DrawRoundedRect(wx + ww/2 + 50, py + 150, 70, 25, 3, bv_diff EQ 2 ? 0x00AA00 : 0x555555); TextC(wx + ww/2 + 85, py + 157, "HARD", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 50, py + 150, 70, 25)) bv_diff = 2;
                        } _41 { TextC(wx + ww/2, py + 150, "P1: MOUSE+CLICK | P2: ARROWS", 0xAAAAAA, _128); }
                        DrawRoundedRect(wx + ww/2 - 60, py + 200, 120, 30, 5, 0xAA0000); TextC(wx + ww/2, py + 207, "START MATCH", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 60, py + 200, 120, 30)) {
                            bv_state = 1; bv_score1 = 0; bv_score2 = 0;
                            bv_ball_x = ww/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0;
							bv_touches = 0; bv_last_touch = 0;
                            input_cooldown = 10;
                        }
                    } _41 _15(bv_state EQ 2) {
                        TextC(wx + ww/2, py + 80, bv_score1 >= 10 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!", 0x00FF00, _128);
                        DrawRoundedRect(wx + ww/2 - 50, py + 140, 100, 30, 3, 0x555555); TextC(wx + ww/2, py + 148, "MENU", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 50, py + 140, 100, 30)) { bv_state = 0; input_cooldown = 10; }
                    } _41 {
                        /// 1. Strand & Himmel zeichnen
                        DrawRoundedRect(wx+10, py, ww-20, inner_h-10, 0, 0x4488FF); /// Himmel (Blau)
                        DrawRoundedRect(wx+10, py+floor_y, ww-20, 10, 0, 0x00AA00); /// Gras/Strand (Grün)
                        DrawRoundedRect(wx+ww/2-3, py+floor_y-70, 6, 70, 0, 0xEEEEEE); /// Das Netz!
                        _30 s1[10], s2[10]; int_to_str(bv_score1, s1); int_to_str(bv_score2, s2);
                        Text(wx+ww/2-40, py+20, s1, 0xFFFFFF, _128); Text(wx+ww/2+30, py+20, s2, 0xFFFFFF, _128);
                        _15(key_new AND last_key EQ 0x01) { bv_state = 0; } /// ESC Notbremse
                        /// 2. Physik-Engine
                        _15(win_z[12] EQ 12) {
                            /// Schwerkraft Ball
                            bv_ball_vy += 1; 
                            _15(bv_ball_vy > 12) bv_ball_vy = 12; /// Max Fallgeschwindigkeit
                            bv_ball_x += bv_ball_vx;
                            bv_ball_y += bv_ball_vy;
                            /// P1 (Maus)
                            bv_p1_x = mouse_x - wx - 20; 
                            _15(bv_p1_x < 10) bv_p1_x = 10;
                            _15(bv_p1_x > ww/2 - 45) bv_p1_x = ww/2 - 45; /// Stoppt am Netz
                            _15(mouse_just_pressed AND bv_p1_y >= floor_y - 30) bv_p1_vy = -14; /// SPRINGEN!
                            bv_p1_vy += 1; bv_p1_y += bv_p1_vy; /// Schwerkraft Blob 1
                            _15(bv_p1_y > floor_y - 30) { bv_p1_y = floor_y - 30; bv_p1_vy = 0; }
                            /// P2 (KI oder Tastatur)
                            _15(bv_mode EQ 0) {
                                /// BARE METAL FIX: Die Pro-Gamer Gänge (4, 6 und 9!)
                                _43 speed = 4; _15(bv_diff EQ 1) speed = 6; _15(bv_diff EQ 2) speed = 9;
                                /// --- BARE METAL FIX: Terminator Gehirn 4.0 ---
                                _43 target_x = bv_ball_x + 15;
                                _15(bv_ball_x > ww/2) {
                                    /// 1. Panik-Modus (Ball fliegt nach hinten weg)
                                    _15(bv_ball_vx > 0 AND bv_ball_x > bv_p2_x) {
                                        target_x = bv_ball_x + 40; 
                                    } _41 {
                                        /// 2. Normale Berechnung
                                        _43 dist_y = (floor_y - 30) - bv_ball_y;
                                        _15(dist_y > 0) {
                                            _43 frames = dist_y / 8; 
                                            target_x = bv_ball_x + (bv_ball_vx * frames) + ((bv_diff EQ 2) ? 20 : 15);
                                        }
                                        /// 3. SHORT BALL SPRINT: Er rennt extrem nach vorne!
                                        _15(target_x < bv_p2_x - 20) {
                                            target_x = target_x - 20; 
                                        }
                                    }
                                } _41 { target_x = ww*3/4; }
                                /// Wand-Sicherung
                                _15(target_x > ww-40) target_x = ww-40;
                                _15(target_x < ww/2 + 10) target_x = ww/2 + 10;
                                /// --- Bewegung ---
                                _43 move_back = speed;
                                _43 move_fwd  = speed;
                                _15(bv_diff EQ 2) {
                                    _15(bv_ball_x > bv_p2_x) move_back = 25; 
                                    _15(target_x < bv_p2_x - 20) move_fwd = 20; 
                                }
                                /// Erhöhte Deadzone (15 statt 10), schluckt das Ruckeln bei hohem Speed!
                                _15(bv_p2_x + 20 < target_x - 15) { 
                                    bv_p2_x += move_back; 
                                } _41 _15(bv_p2_x + 20 > target_x + 15) { 
                                    bv_p2_x -= move_fwd;  
                                } _41 {
                                    bv_p2_x = target_x - 20; 
                                }
                                /// --- FEHLER BEHOBEN: Der intelligente Smash-Reflex ---
                                /// Er darf NUR springen, wenn der Ball auch in horizontaler Reichweite ist!
                                _15(bv_ball_x > bv_p2_x - 20 AND bv_ball_x < bv_p2_x + 60) { 
                                    _15(bv_ball_y > floor_y - 120 AND bv_ball_y < floor_y - 40 AND bv_p2_y >= floor_y - 30 AND bv_ball_vy > 0) {
                                        _15(bv_diff >= 1 OR (frame % 2) EQ 0) bv_p2_vy = -14;
                                    }
                                }
                            } _41 {
                                /// 2-Spieler Modus
                                _15(key_new) {
                                    _15(last_key EQ 0x4B) bv_p2_x -= 15; _15(last_key EQ 0x4D) bv_p2_x += 15;
                                    _15(last_key EQ 0x48 AND bv_p2_y >= floor_y - 30) bv_p2_vy = -14;
                                }
                            }
                            _15(bv_p2_x < ww/2 + 5) bv_p2_x = ww/2 + 5;
                            _15(bv_p2_x > ww - 50) bv_p2_x = ww - 50;
                            bv_p2_vy += 1; bv_p2_y += bv_p2_vy; /// Schwerkraft Blob 2
                            _15(bv_p2_y > floor_y - 30) { bv_p2_y = floor_y - 30; bv_p2_vy = 0; }
                            /// Wände & Decke
                            _15(bv_ball_x < 15) { bv_ball_x = 15; bv_ball_vx = -bv_ball_vx; play_freq(800); }
                            _15(bv_ball_x > ww-15) { bv_ball_x = ww-15; bv_ball_vx = -bv_ball_vx; play_freq(800); }
                            _15(bv_ball_y < 15) { bv_ball_y = 15; bv_ball_vy = 2; }
                            /// Netz-Kollision (Der knifflige Teil)
                            _15(bv_ball_x > ww/2-10 AND bv_ball_x < ww/2+10 AND bv_ball_y > floor_y-70) {
                                _15(bv_ball_y < floor_y-65 AND bv_ball_vy > 0) { bv_ball_vy = -8; } /// Streift die Oberkante
                                _41 { bv_ball_vx = -bv_ball_vx; bv_ball_x += bv_ball_vx*2; } /// Prallt ab
                                play_freq(600);
                            }
                            /// Ball trifft Blobs (Runde Kollision über Spin)
                            _15(bv_ball_vy > 0) {
                                /// --- Kollision Spieler 1 (Links) ---
                                _15(bv_ball_y+10 > bv_p1_y AND bv_ball_y-10 < bv_p1_y+30 AND bv_ball_x > bv_p1_x-10 AND bv_ball_x < bv_p1_x+50) {
                                    bv_ball_vy = -14; 
                                    bv_ball_vx = (bv_ball_x - (bv_p1_x + 20)) / 3;
                                    /// 3-TOUCH REGEL P1
                                    _15(bv_last_touch NEQ 1) bv_touches = 0; 
                                    bv_last_touch = 1; bv_touches++;
                                    _15(bv_touches > 3) { 
                                        bv_score2++; bv_ball_x = ww*3/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0; 
                                        bv_touches = 0; bv_last_touch = 0; play_sound(200, 20); 
                                    } _41 play_sound(1200, 2);
                                }
                                /// --- Kollision Spieler 2 (Rechts / KI) ---
                                _15(bv_ball_y+10 > bv_p2_y AND bv_ball_y-10 < bv_p2_y+30 AND bv_ball_x > bv_p2_x-10 AND bv_ball_x < bv_p2_x+50) {
                                    bv_ball_vy = -14; 
                                    bv_ball_vx = (bv_ball_x - (bv_p2_x + 20)) / 3;
                                    /// BARE METAL FIX: Anti-Seehund für ALLE Stufen!
                                    _15(bv_ball_vx >= -2 AND bv_ball_vx <= 2) {
                                        _15(bv_diff EQ 2) bv_ball_vx = -10; /// Terminator Laser-Smash
                                        _15(bv_diff EQ 1) bv_ball_vx = -7;  /// Harter Smash
                                        _15(bv_diff EQ 0) bv_ball_vx = -4;  /// Normaler Schubser
                                    }
                                    /// 3-TOUCH REGEL P2
                                    _15(bv_last_touch NEQ 2) bv_touches = 0; 
                                    bv_last_touch = 2; bv_touches++;
                                    _15(bv_touches > 3) { 
                                        bv_score1++; bv_ball_x = ww/4; bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0; 
                                        bv_touches = 0; bv_last_touch = 0; play_sound(200, 20); 
                                    } _41 play_sound(1200, 2);
                                }
                            }
                            /// Ball fällt auf den Boden -> Punkt!
                            _15(bv_ball_y > floor_y) {
                                _15(bv_ball_x < ww/2) { bv_score2++; bv_ball_x = ww/4; }
                                _41 { bv_score1++; bv_ball_x = ww*3/4; }
                                bv_ball_y = 30; bv_ball_vx = 0; bv_ball_vy = 0;
                                bv_touches = 0; bv_last_touch = 0; /// Schiedsrichter Reset
                                play_sound(200, 20);
                            }
                            _15(bv_score1 >= 10 OR bv_score2 >= 10) bv_state = 2;
                            _41 play_freq(0); 
                        }
                        /// 3. Render Blobs und Ball
                        DrawRoundedRect(wx + bv_p1_x, py + bv_p1_y, 40, 30, 15, 0x00FF00); /// P1
                        DrawRoundedRect(wx + bv_p2_x, py + bv_p2_y, 40, 30, 15, 0xFF0000); /// P2
                        DrawRoundWindow(wx + bv_p1_x + 25, py + bv_p1_y + 10, 4, 0xFFFFFF); DrawRoundWindow(wx + bv_p1_x + 27, py + bv_p1_y + 10, 2, 0x000000);
                        DrawRoundWindow(wx + bv_p2_x + 15, py + bv_p2_y + 10, 4, 0xFFFFFF); DrawRoundWindow(wx + bv_p2_x + 13, py + bv_p2_y + 10, 2, 0x000000);
                        DrawRoundWindow(wx + bv_ball_x, py + bv_ball_y, 10, 0xFFFFFF); 
                    }
                }
				/// =========================================================
                /// APP: PONG ARCADE (ID 11) - ULTIMATE EDITION
                /// =========================================================
                _15(win->id EQ 11) {
                    _43 inner_w = ww; _43 inner_h = wh - 40;
                    _43 py = wy + 40;
                    _15(pong_state EQ 0) {
                        TextC(wx + ww/2, py + 20, "PONG OMNIVERSE", 0x00FF00, _128);
                        /// --- MODUS AUSWAHL ---
                        TextC(wx + ww/2, py + 60, "SELECT MODE", 0xFFFFFF, _128);
                        DrawRoundedRect(wx + ww/2 - 110, py + 80, 100, 25, 3, pong_mode EQ 0 ? 0x00AA00 : 0x555555);
                        TextC(wx + ww/2 - 60, py + 87, "1 PLAYER", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 110, py + 80, 100, 25)) pong_mode = 0;
                        DrawRoundedRect(wx + ww/2 + 10, py + 80, 100, 25, 3, pong_mode EQ 1 ? 0x00AA00 : 0x555555);
                        TextC(wx + ww/2 + 60, py + 87, "2 PLAYERS", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 10, py + 80, 100, 25)) pong_mode = 1;
                        /// --- SCHWIERIGKEIT (Nur wenn 1 Player aktiv) ---
                        _15(pong_mode EQ 0) {
                            TextC(wx + ww/2, py + 130, "CPU DIFFICULTY", 0xFFFFFF, _128);
                            DrawRoundedRect(wx + ww/2 - 120, py + 150, 70, 25, 3, pong_diff EQ 0 ? 0x00AA00 : 0x555555);
                            TextC(wx + ww/2 - 85, py + 157, "EASY", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 120, py + 150, 70, 25)) pong_diff = 0;
                            DrawRoundedRect(wx + ww/2 - 35, py + 150, 70, 25, 3, pong_diff EQ 1 ? 0x00AA00 : 0x555555);
                            TextC(wx + ww/2, py + 157, "NORMAL", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 35, py + 150, 70, 25)) pong_diff = 1;
                            DrawRoundedRect(wx + ww/2 + 50, py + 150, 70, 25, 3, pong_diff EQ 2 ? 0x00AA00 : 0x555555);
                            TextC(wx + ww/2 + 85, py + 157, "HARD", 0xFFFFFF, _128);
                            _15(mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 + 50, py + 150, 70, 25)) pong_diff = 2;
                        } _41 {
                            TextC(wx + ww/2, py + 150, "P1: MOUSE  |  P2: ARROWS (UP/DOWN)", 0xAAAAAA, _128);
                        }
                        /// --- START BUTTON ---
                        DrawRoundedRect(wx + ww/2 - 60, py + 190, 120, 30, 5, 0xAA0000);
                        TextC(wx + ww/2, py + 197, "START GAME", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 60, py + 190, 120, 30)) {
                            pong_state = 1; pong_score1 = 0; pong_score2 = 0;
                            pong_ball_x = ww/2; pong_ball_y = inner_h/2;
                            pong_vel_x = 5; pong_vel_y = 3; 
                            input_cooldown = 10;
                        }
                        /// --- QUIT BUTTON ---
                        DrawRoundedRect(wx + ww/2 - 50, py + 230, 100, 20, 3, 0x444444);
                        TextC(wx + ww/2, py + 235, "QUIT TO OS", 0xFFFFFF, _86);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx + ww/2 - 50, py + 230, 100, 20)) {
                            windows[11].open = _86; /// 1. Schließt Pong
                            /// BARE METAL FIX: Weckt das APPS-Fenster explizit wieder auf!
                            windows[1].open = _128; 
                            windows[1].minimized = _86; 
                            focus_window(1);
                            input_cooldown = 10;
                        }
                    } _41 _15(pong_state EQ 2) {
                        TextC(wx + ww/2, py + 80, pong_score1 >= 10 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!", 0x00FF00, _128);
                        TextC(wx + ww/2, py + 140, "CLICK TO _96 TO MENU", 0xFFFFFF, _128);
                        _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx, py, ww, inner_h)) {
                            pong_state = 0; input_cooldown = 10;
                        }
                    } _41 {
                        /// 1. Spielfeld zeichnen
                        DrawRoundedRect(wx+10, py, ww-20, inner_h-10, 0, 0x000000);
                        _39(_43 i=0; i<inner_h-10; i+=20) DrawRoundedRect(wx+ww/2-2, py+i, 4, 10, 0, 0x555555); /// Netz
                        /// 2. Score & Notbremse
                        _30 s1[10], s2[10]; int_to_str(pong_score1, s1); int_to_str(pong_score2, s2);
                        Text(wx+ww/2-40, py+20, s1, 0x00FF00, _128);
                        Text(wx+ww/2+30, py+20, s2, 0x00FF00, _128);
                        Text(wx+20, py+15, "[ ESC ] TO ABORT", 0x444444, _86);
                        _15(key_new AND last_key EQ 0x01) { pong_state = 0; }
                        /// 3. Physik & KI
                        _15(win_z[12] EQ 11) {
                            pong_ball_x += pong_vel_x;
                            pong_ball_y += pong_vel_y;
                            _15(pong_ball_y <= 5) { pong_ball_y = 5; pong_vel_y = -pong_vel_y; play_freq(800); }
                            _41 _15(pong_ball_y >= inner_h-25) { pong_ball_y = inner_h-25; pong_vel_y = -pong_vel_y; play_freq(800); }
                            _41 { play_freq(0); }
                            /// Spieler 1 (Maus)
                            pong_p1_y = mouse_y - py - 20; 
                            _15(pong_p1_y < 0) pong_p1_y = 0; 
                            _15(pong_p1_y > inner_h-50) pong_p1_y = inner_h-50;
                            /// Spieler 2 (KI oder Tastatur)
                            _15(pong_mode EQ 0) {
                                /// KI Steuerung mit Schwierigkeitsgrad
                                _43 speed = 4; /// Normal
                                _15(pong_diff EQ 0) speed = 2; /// Easy
                                _15(pong_diff EQ 2) speed = 6; /// Hard
                                _15(pong_ball_y > pong_p2_y + 20) pong_p2_y += speed;
                                _15(pong_ball_y < pong_p2_y + 20) pong_p2_y -= speed;
                            } _41 {
                                /// 2-Spieler Steuerung (Pfeiltasten: 0x48 = UP, 0x50 = DOWN)
                                _15(key_new) {
                                    _15(last_key EQ 0x48) pong_p2_y -= 15; 
                                    _15(last_key EQ 0x50) pong_p2_y += 15;
                                }
                            }
                            /// P2 Bounds Check
                            _15(pong_p2_y < 0) pong_p2_y = 0; 
                            _15(pong_p2_y > inner_h-50) pong_p2_y = inner_h-50;
                            /// Kollision Spieler 1 (Links)
                            _15(pong_ball_x <= 30 AND pong_ball_y + 10 >= pong_p1_y AND pong_ball_y <= pong_p1_y + 40 AND pong_vel_x < 0) {
                                pong_ball_x = 30; 
                                pong_vel_x = -pong_vel_x; /// Ball umdrehen
                                /// 1. Speed Escalation (Cap bei 14, sonst tunnelt er durch die Wände)
                                _15(pong_vel_x < 14) pong_vel_x++;
                                /// 2. Spin Control (Wo wurde der Schläger getroffen?)
                                _15(pong_ball_y < pong_p1_y + 12) pong_vel_y -= 2;      /// Oben getroffen -> Fliegt steiler hoch
                                _41 _15(pong_ball_y > pong_p1_y + 28) pong_vel_y += 2;  /// Unten getroffen -> Fliegt steiler runter
                                play_sound(1200, 2);
                            }
                            /// Kollision Spieler 2 (Rechts)
                            _15(pong_ball_x >= ww-40 AND pong_ball_y + 10 >= pong_p2_y AND pong_ball_y <= pong_p2_y + 40 AND pong_vel_x > 0) {
                                pong_ball_x = ww-40; 
                                pong_vel_x = -pong_vel_x; /// Ball umdrehen
                                /// 1. Speed Escalation (Ins Negative!)
                                _15(pong_vel_x > -14) pong_vel_x--;
                                /// 2. Spin Control
                                _15(pong_ball_y < pong_p2_y + 12) pong_vel_y -= 2; 
                                _41 _15(pong_ball_y > pong_p2_y + 28) pong_vel_y += 2;
                                play_sound(1200, 2);
                            }
                            /// Tor!
                            _15(pong_ball_x < 0 OR pong_ball_x > 2000) { pong_score2++; pong_ball_x=ww/2; pong_ball_y=inner_h/2; play_sound(200, 20); }
                            _15(pong_ball_x > ww) { pong_score1++; pong_ball_x=ww/2; pong_ball_y=inner_h/2; play_sound(200, 20); }
                            _15(pong_score1 >= 10 OR pong_score2 >= 10) pong_state = 2;
                        }
                        /// 4. Zeichnen
                        DrawRoundedRect(wx+20, py + pong_p1_y, 10, 40, 2, 0x00FF00); /// P1
                        DrawRoundedRect(wx+ww-30, py + pong_p2_y, 10, 40, 2, 0xFF0000); /// P2/KI
                        DrawRoundWindow(wx + pong_ball_x, py + pong_ball_y, 6, 0xFFFFFF); /// Ball
                    }
                }
                /// =========================================================
                /// HIER GEHT ES DANN WEITER MIT DEM NOTEPAD (ID 0)
                /// =========================================================
                _15(win->id EQ 0) { 
                    /// 1. Dateiname sicher anzeigen
                    _30 disp_name[40] = "FILE: UNNAMED (QUICK.TXT)";
                    _15(current_open_file NEQ -1) {
                        str_cpy(disp_name, "FILE: ");
                        str_cpy(disp_name + 6, file_table[current_open_file].name);
                    }
                    Text(wx+20, wy+35, disp_name, 0x0000FF, _128);
                    /// --- 2. BARE-METAL TOOLBAR ---
                    _43 tb_y = wy + 32;
					/// --- DIE 10 TABS (KLEINE KÄSTCHEN) ---
                    _39(_43 i=0; i<10; i++) {
                        /// Blaue Farbe, wenn Tab aktiv, sonst grau. Leer = Dunkelgrau.
                        _89 bg_col = (active_tab EQ i) ? 0x0000FF : ((tab_files[i] NEQ -1) ? 0x888888 : 0x555555);
                        _89 txt_col = 0xFFFFFF;
                        DrawRoundedRect(wx + 80 + (i * 18), wy + 6, 16, 14, 2, bg_col);
                        /// Nummern von 1-9 rendern, dann eine 0 für Tab 10
                        _30 num_str[2]; num_str[0] = (i < 9) ? '1' + i : '0'; num_str[1] = 0; 
                        Text(wx + 84 + (i * 18), wy + 8, num_str, txt_col, _128);
                        /// Klick-Logik zum Wechseln der Tabs
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx + 80 + (i * 18), wy + 6, 16, 14)) {
                            /// 1. Sichere den aktuellen Bildschirm-Text in den Hintergrund-Tab
                            _39(_43 r=0; r<10; r++) _39(_43 c=0; c<40; c++) tab_buffers[active_tab][r][c] = note_buf[r][c];
                            /// 2. Wechsle auf den neuen Tab
                            active_tab = i;
                            current_open_file = tab_files[i];
                            /// 3. Lade den Text des neuen Tabs auf den Bildschirm
                            _39(_43 r=0; r<10; r++) _39(_43 c=0; c<40; c++) note_buf[r][c] = tab_buffers[active_tab][r][c];
                            input_cooldown = 10;
                        }
                    }                    
                    /// Button: INSTALL (Bleibt, da es nur den RAM/PCI betrifft!)
                    DrawRoundedRect(wx+ww-245, tb_y, 60, 16, 2, 0x005500); Text(wx+ww-240, tb_y+3, "INSTALL", 0xFFFFFF, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww-245, tb_y, 60, 16)) {
                        oracle_entry_count = 0;
                        _39(_43 r=0; r<10; r++) {
                            _30* line = note_buf[r];
                            _15(line[0] EQ 0) _101;
                            _43 c = 0; _43 t_idx = 0;
                            _114(line[c] NEQ ' ' AND line[c] NEQ 0 AND c < 40) { _15(t_idx < 7) oracle_db[oracle_entry_count].type[t_idx++] = line[c]; c++; }
                            oracle_db[oracle_entry_count].type[t_idx] = 0;
                            _114(line[c] EQ ' ') c++;
                            _30 hex_buf[10]; _43 h_idx = 0;
                            _114(line[c] NEQ ' ' AND line[c] NEQ 0 AND c < 40) { _15(h_idx<9) hex_buf[h_idx++] = line[c]; c++; }
                            hex_buf[h_idx] = 0; oracle_db[oracle_entry_count].vendor = hex_str_to_int(hex_buf);
                            _114(line[c] EQ ' ') c++;
                            h_idx = 0;
                            _114(line[c] NEQ ' ' AND line[c] NEQ 0 AND c < 40) { _15(h_idx<9) hex_buf[h_idx++] = line[c]; c++; }
                            hex_buf[h_idx] = 0; oracle_db[oracle_entry_count].device = hex_str_to_int(hex_buf);
                            _114(line[c] EQ ' ') c++;
                            h_idx = 0;
                            _114(line[c] NEQ ' ' AND line[c] NEQ 0 AND c < 40) { _15(h_idx<9) hex_buf[h_idx++] = line[c]; c++; }
                            hex_buf[h_idx] = 0; oracle_db[oracle_entry_count].bar_addr = hex_str_to_int(hex_buf);
                            _15(oracle_db[oracle_entry_count].vendor NEQ 0) oracle_entry_count++;
                        }
                        pci_scan_all(); 
                        str_cpy(save_timestamp, "ORACLE: DRIVERS APPLIED!");
                        input_cooldown = 10;
                    }
                    /// Button: NEW 
                    DrawRoundedRect(wx+ww-180, tb_y, 45, 16, 2, 0xAAAAAA); Text(wx+ww-175, tb_y+3, "NEW", 0x000000, _128);
                    _15(input_cooldown EQ 0 AND mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+ww-180, tb_y, 45, 16)) {
                        _39(_43 r=0; r<10; r++) _39(_43 c=0; c<40; c++) note_buf[r][c] = 0;
                        note_r = 0; note_c = 0; current_open_file = -1;
                        str_cpy(save_timestamp, "NEW FILE READY"); input_cooldown = 10;
                    }
                    /// 3. Tastatur-Code
                    _15(key_new AND win_z[12] EQ 0) { 
                        _15(last_key < 58) { 
                            _30 c = get_ascii(last_key); 
                            _15(last_key EQ 0x1C AND note_r<9){note_r++; note_c=0;} 
                            _41 _15(last_key EQ 0x0E) { 
                                _15(note_c>0) {note_c--; note_buf[note_r][note_c]=0;} 
                                _41 _15(note_r>0) { note_r--; note_c=39; } 
                            } 
                            _41 _15(c) { 
                                note_buf[note_r][note_c]=c; note_c++; 
                                _15(note_c > 38) { note_c=0; _15(note_r<9) note_r++; } 
                            } 
                        } 
                    }
                    /// 4. Text zeichnen
                    _39(_43 r=0;r<10;r++) Text(wx+20, wy+50+r*15, note_buf[r], 0x333333, _128);
                    _15((frame/20)%2 EQ 0 AND win_z[12] EQ 0) DrawChar(wx+20+(note_c*7), wy+50+note_r*15, '_', 0x111111, _128);
                    /// 5. BARE METAL UI: Smart Save & Open (Max 200ms Delay)
                    _43 btn_y = wy+wh-25; 
                    Text(wx+20,  btn_y, "[ SAVE ]", 0x333333, _128); 
                    Text(wx+100, btn_y, "[ SAVE AS... ]", 0x333333, _128); 
                    Text(wx+200, btn_y, "[ OPEN ]", 0x333333, _128);
                    _15(mouse_just_pressed AND !blocked) {
                        /// --- 1. NORMALES SAVE (Überschreibt die aktuelle Datei) ---
                        _15(is_over_rect(mouse_x, mouse_y, wx+20, btn_y, 70, 20)) { 
                            _15(current_open_file NEQ -1) {
                                _43 f = current_open_file;
                                _43 start_lba = file_table[f].sector_offset;
                                _15(start_lba EQ 0) {
                                    str_cpy(save_timestamp, "ERR: NO SECTOR ASSIGNED!");
                                } _41 {
                                    /// BARE METAL FIX: Perfekt ausgerichteter RAM (64 MB)
                                    _89 text_ram_addr = 0x04000000;
                                    _30* text_buffer = (_30*)text_ram_addr;
                                    _39(_43 i=0; i<512; i++) text_buffer[i] = 0;
                                    /// 2D Array in 1D Sektor-Block umwandeln
                                    _43 idx = 0;
                                    _39(_43 r=0; r<10; r++) {
                                        _39(_43 c=0; c<40; c++) {
                                            _15(note_buf[r][c] NEQ 0) { text_buffer[idx++] = note_buf[r][c]; }
                                        }
                                        text_buffer[idx++] = '\n'; 
                                    }
                                    /// Auf die Platte brennen!
                                    ahci_write_sectors(drives[active_drive_idx].base_port, start_lba, 1, text_ram_addr);
                                    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
                                    str_cpy(save_timestamp, "FILE SAVED TO SSD!");
                                }
                            } _41 {
                                str_cpy(save_timestamp, "USE 'SAVE AS' FIRST!");
                            }
                            input_cooldown = 10;
                        }
                        /// --- 2. SAVE AS... ---
                        _15(is_over_rect(mouse_x, mouse_y, wx+100, btn_y, 90, 20)) { 
                            windows[2].open = _128; windows[2].minimized = _86; focus_window(2); save_step = 0; 
                            input_cooldown = 10; 
                        }
                        /// --- 3. OPEN (Holt Disk Manager nach vorne) ---
                        _15(is_over_rect(mouse_x, mouse_y, wx+200, btn_y, 70, 20)) { 
                            windows[4].open = _128; windows[4].minimized = _86; focus_window(4); 
                            input_cooldown = 10; 
                        }
                    }
                    Text(wx+ww-220, wy+wh-20, save_timestamp, 0x111111, _128); 
                }
                /// =========================================================
                /// SAVE AS DIALOG (ID 2)
                /// =========================================================
                _15(win->id EQ 2) {
                    _15(save_step EQ 0) { 
                        /// --- BARE METAL FIX 1: DYNAMISCHE LAUFWERKSAUSWAHL ---
                        _15(active_drive_idx < 0 OR active_drive_idx >= drive_count) active_drive_idx = 0;
                        Text(wx+20, wy+40, "SELECT DESTINATION (CLICK TO CHANGE):", 0x000000, _128); 
                        _44 sel = is_over_rect(mouse_x, mouse_y, wx+20, wy+65, ww-40, 25); 
                        DrawRoundedRect(wx+20, wy+65, ww-40, 25, 5, sel ? 0x999999 : 0xDDDDDD);
                        _30 drv_lbl[60] = "DRIVE: ";
                        _15(drive_count > 0) { str_cat(drv_lbl, drives[active_drive_idx].model); } _41 { str_cat(drv_lbl, "NO DRIVES FOUND"); }
                        Text(wx+30, wy+72, drv_lbl, sel ? 0xFFFFFF : 0x000000, _128);
                        _15(mouse_just_pressed AND !blocked AND sel AND drive_count > 0) {
                            active_drive_idx++;
                            _15(active_drive_idx >= drive_count) active_drive_idx = 0;
                            /// ==========================================
                            /// BARE METAL FIX: Laufwerk wechseln & Liste sauber laden!
                            /// ==========================================
                            _89 dir_ram_addr = 0x07000000;
                            ahci_read_sectors(drives[active_drive_idx].base_port, 1002, 1, dir_ram_addr);
                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                            _39(_43 i=0; i<8; i++) {
                                _15(entries[i].type NEQ 0) {
                                    file_table[i].exists = _128;
                                    file_table[i].parent_idx = 255; /// 255 = Root Directory
                                    file_table[i].is_folder = (entries[i].type EQ 2) ? _128 : _86;
                                    str_cpy(file_table[i].name, entries[i].filename);
                                    /// BARE METAL FIX: Diese 3 Zeilen haben gefehlt!
                                    file_table[i].size = entries[i].file_size;
                                    file_table[i].sector_offset = entries[i].start_lba;
                                    str_cpy(file_table[i].date, "DMA");
                                } _41 {
                                    file_table[i].exists = _86;
                                }
                            }
                            current_path_id = 255; 
                            input_cooldown = 10;
                        }
                        Text(wx+20, wy+100, "FOLDERS (CLICK TO ENTER):", 0x555555, _128); 
                        DrawRoundedRect(wx+20, wy+120, ww-40, 100, 2, 0xEEEEEE);
                        _43 y_ptr = wy + 125; /// HIER IST DER VERLORENE POINTER!
                        _15(current_path_id NEQ 255) {
                            _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+25, y_ptr, 150, 15)) {
                                current_path_id = 255; input_cooldown = 10;
                            }
                            Text(wx+30, y_ptr, "[ .. BACK TO ROOT ]", 0x0000FF, _86); y_ptr += 20;
                        }
                        _39(_43 f=0; f<8; f++) {
                            _15(file_table[f].exists AND file_table[f].is_folder AND file_table[f].parent_idx EQ current_path_id) {
                                _15(mouse_just_pressed AND !blocked AND is_over_rect(mouse_x, mouse_y, wx+25, y_ptr, 200, 15)) {
                                    current_path_id = f; input_cooldown = 10;
                                }
                                Text(wx+30, y_ptr, file_table[f].name, 0x000000, _86); y_ptr += 20;
                            }
                        }
                        _30 path_info[40] = "TARGET: /ROOT/";
                        _15(current_path_id NEQ 255) { str_cpy(path_info + 8, file_table[current_path_id].name); }
                        Text(wx+20, wy+225, path_info, 0x00AA00, _86);
                        DrawRoundedRect(wx+20, wy+wh-40, 110, 25, 5, 0xCCCCCC); TextC(wx+75, wy+wh-32, "NEW FOLDER", 0x000000, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+20, wy+wh-40, 110, 25)) { save_step = 2; input_cooldown = 10; }
                        DrawRoundedRect(wx+ww-90, wy+wh-40, 70, 25, 5, 0x555555); TextC(wx+ww-55, wy+wh-32, "NEXT", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+ww-90, wy+wh-40, 70, 25)) { save_step = 1; input_cooldown = 10; }
                    } _41 _15(save_step EQ 1) { 
                        _15(key_new AND win_z[12] EQ 2) { _15(last_key EQ 0x0E AND save_name_idx > 0) save_filename[--save_name_idx] = 0; _41 _15(save_name_idx < 30) { _30 c = get_ascii(last_key); _15(c >= 32) { save_filename[save_name_idx++] = c; save_filename[save_name_idx]=0; } } }
                        Text(wx+20, wy+40, "ENTER FILENAME:", 0x000000, _128); DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC); Text(wx+25, wy+65, save_filename, 0x000000, _128); _15((frame/20)%2 EQ 0) DrawChar(wx+25+(save_name_idx*7), wy+65, '_', 0x000000, _128);
                        DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x555555); TextC(wx+ww-60, wy+wh-32, "SAVE", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) { 
                            /// ==========================================
                            /// BARE METAL FIX: DIRECT DMA CFS SAVE
                            /// ==========================================
                            _43 port = drives[active_drive_idx].base_port;
                            /// 1. Den Text aus dem Notepad in einen sauberen Puffer packen
                            _89 text_ram_addr = 0x06000000;
                            _30* text_buffer = (_30*)text_ram_addr;
                            _39(_43 i=0; i<512; i++) text_buffer[i] = 0;
                            _43 idx = 0;
                            _39(_43 r=0; r<10; r++) {
                                _39(_43 c=0; c<40; c++) {
                                    _15(note_buf[r][c] NEQ 0) { text_buffer[idx++] = note_buf[r][c]; }
                                }
                                text_buffer[idx++] = '\n'; 
                            }
                            /// 2. CFS Inhaltsverzeichnis (Sektor 1002) in den RAM lesen
                            _89 dir_ram_addr = 0x07000000;
                            ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                            /// 3. Freien Datei-Slot suchen
                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                            _43 slot = 255;
                            _39(_43 i=0; i<8; i++) {
                                _15(entries[i].type EQ 0) { slot = i; _37; }
                            }
                            _15(slot NEQ 255) {
                                /// 4. Den Text brennen! (Wir speichern ab Sektor 4000)
                                _89 target_sec = 4000 + slot;
                                ahci_write_sectors(port, target_sec, 1, text_ram_addr);
                                _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                                /// 5. Inhaltsverzeichnis aktualisieren und auf SSD brennen
                                entries[slot].type = 1;
                                str_cpy(entries[slot].filename, save_filename);
                                entries[slot].file_size = 512;
                                entries[slot].start_lba = target_sec;
                                ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                                /// ==========================================
                                /// BARE METAL FIX: RAM-Tabelle für Textdateien synchronisieren!
                                /// ==========================================
                                file_table[slot].exists = _128;
                                file_table[slot].parent_idx = current_path_id; 
                                file_table[slot].is_folder = _86; /// 86 = Datei! (Darum taucht sie bei FOLDERS nicht auf)
                                str_cpy(file_table[slot].name, save_filename);
                                file_table[slot].size = 512;
                                str_cpy(file_table[slot].date, "DMA");
                                file_table[slot].sector_offset = target_sec;
                                str_cpy(save_timestamp, "CFS: SAVED VIA DMA!");
                            } _41 {
                                str_cpy(save_timestamp, "CFS: ROOT DIR FULL!");
                            }
                            win->open = _86; 
                            input_cooldown = 10; 
                        }
                    } _41 _15(save_step EQ 2) { 
                        _15(key_new AND win_z[12] EQ 2) { _15(last_key EQ 0x0E AND folder_name_idx > 0) new_folder_name[--folder_name_idx] = 0; _41 _15(folder_name_idx < 30) { _30 c = get_ascii(last_key); _15(c >= 32) { new_folder_name[folder_name_idx++] = c; new_folder_name[folder_name_idx]=0; } } }
                        Text(wx+20, wy+40, "FOLDER NAME:", 0x000000, _128); DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC); Text(wx+25, wy+65, new_folder_name, 0x000000, _128); _15((frame/20)%2 EQ 0) DrawChar(wx+25+(folder_name_idx*7), wy+65, '_', 0x000000, _128);
                        DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x555555); TextC(wx+ww-60, wy+wh-32, "CREATE", 0xFFFFFF, _128);
                        _15(input_cooldown EQ 0 AND mouse_just_pressed AND is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) { 
                            /// ==========================================
                            /// BARE METAL FIX: DIRECT DMA FOLDER CREATE
                            /// ==========================================
                            _43 port = drives[active_drive_idx].base_port;
                            _89 dir_ram_addr = 0x07000000;
                            /// 1. Inhaltsverzeichnis laden
                            ahci_read_sectors(port, 1002, 1, dir_ram_addr);
                            _39(_192 _43 wait=0; wait<500000; wait++) __asm__ _192("pause");
                            CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)dir_ram_addr;
                            _43 slot = 255;
                            /// 2. Freien Slot finden
                            _39(_43 i=0; i<8; i++) {
                                _15(entries[i].type EQ 0) { slot = i; _37; }
                            }
                            _15(slot NEQ 255) {
                                /// 3. Eintrag als Ordner (Typ 2) deklarieren und zurückschreiben
                                entries[slot].type = 2; /// 2 = FOLDER
                                str_cpy(entries[slot].filename, new_folder_name);
                                entries[slot].file_size = 0;
                                entries[slot].start_lba = 0; /// Ordner haben keine Daten in Sektor 4000+
                                ahci_write_sectors(port, 1002, 1, dir_ram_addr);
                                str_cpy(save_timestamp, "CFS: FOLDER CREATED!");
                            } _41 {
                                str_cpy(save_timestamp, "CFS: ROOT DIR FULL!");
                            }
                            save_step = 0; 
                            input_cooldown = 10; 
                        }
                    }
				}
                _15(win->id EQ 5) {
                    _15(key_new AND win_z[12] EQ 5) {
                        _15(last_key EQ 0x1C) { 
                            cmd_input[cmd_idx] = 0; _39(_43 l=4; l>0; l--) str_cpy(cmd_lines[l], cmd_lines[l-1]); str_cpy(cmd_lines[0], cmd_input);
                            _89 cid = get_id(cmd_input);
                            _15(str_equal(cmd_input, "DIR")) { str_cpy(cmd_last_out, ""); _43 p=0; _39(_43 i=0;i<8;i++) { _15(file_table[i].exists AND file_table[i].parent_idx EQ 255) { _71 _30* n=file_table[i].name; _114(*n AND p<38) cmd_last_out[p++]=*n++; _15(p<38) cmd_last_out[p++]=' '; } } cmd_last_out[p]=0; } 
                            _41 _15(str_equal(cmd_input, "HELP")) str_cpy(cmd_last_out, "CMD: DIR, NET, PING, IPCONFIG, AHCIDEBUG, DUMP 0, LINK, SCAN, STATS, CLS");
							_41 _15(str_equal(cmd_input, "CRASH")) { kernel_panic("MANUAL SYSTEM STRESS TEST");}
                            _41 _15(str_equal(cmd_input, "CLS")) { _39(_43 l=0;l<5;l++) cmd_lines[l][0]=0; str_cpy(cmd_last_out, "READY."); }
                            _41 _15(str_equal(cmd_input, "REBOOT")) system_reboot();
							_41 _15(str_equal(cmd_input, "STATS")) {
								_15(intel_mem_base EQ 0) {
									str_cpy(cmd_status, "ERR: INTEL NIC OFF");
								} _41 {
									_89 tx_cnt = mmio_read32(intel_mem_base + 0x40E8); 
									_89 tdh = mmio_read32(intel_mem_base + 0x3810); /// Hardware-Kopf
									_89 tdt = mmio_read32(intel_mem_base + 0x3818); /// Software-Schwanz
									str_cpy(cmd_status, "TX:");
									_30* p = cmd_status + 3;
									int_to_str(tx_cnt, p); _114(*p) p++;
									*p++ = ' '; *p++ = 'H'; *p++ = ':'; 
									int_to_str(tdh, p); _114(*p) p++;
									*p++ = ' '; *p++ = 'T'; *p++ = ':'; 
									int_to_str(tdt, p); _114(*p) p++;
									*p = 0;
								}
								_39(_43 i = 0; i < 30; i++) cmd_input[i] = 0;
								cmd_idx = 0;
							}
							_41 _15(str_equal(cmd_input, "LINK")) {
									net_check_link();
									_39(_43 i = 0; i < 30; i++) cmd_input[i] = 0;
									cmd_idx = 0;
								}
							_41 _15(str_equal(cmd_input, "SCAN")) {
								/// 1. Hardware scannen, wenn die Listen noch leer sind
								_15(storage_count EQ 0 AND gpu_count EQ 0) {
									pci_scan_all(); 
									str_cpy(cmd_status, "HARDWARE SCAN COMPLETE");
								} _41 {
									str_cpy(cmd_status, "SCAN: CYCLING DEVICES...");
								}
								/// 2. Festplatten-Anzeige rotieren (Hitachi -> Samsung -> ...)
								_15(storage_count > 0) {
									str_cpy(hw_storage, hw_storage_list[current_storage_idx]);
									current_storage_idx = (current_storage_idx + 1) % storage_count;
								}
								/// 3. Grafik-Anzeige rotieren (Geforce -> Mainboard GPU)
								_15(gpu_count > 0) {
									str_cpy(hw_gpu, hw_gpu_list[current_gpu_idx]);
									current_gpu_idx = (current_gpu_idx + 1) % gpu_count;
								}
								/// CMD-Input löschen für den nächsten Befehl
								_39(_43 i = 0; i < 30; i++) cmd_input[i] = 0;
								cmd_idx = 0;
							}
                            _41 _15(str_equal(cmd_input, "IPCONFIG")) { str_cpy(cmd_last_out, "IP: "); _30* p = cmd_last_out + 4; _30* m = ip_address; _114(*m) *p++ = *m++; *p++ = ' '; *p++ = 'M'; *p++ = 'A'; *p++ = 'C'; *p++ = ':'; *p++ = ' '; m = mac_str; _114(*m) *p++ = *m++; *p = 0; }
                            _41 _15(str_equal(cmd_input, "CONNECT")) { send_arp_ping(); str_cpy(cmd_last_out, "ARP PING SENT..."); }
							_41 _15(str_equal(cmd_input, "DHCP")) { send_dhcp_discover(); str_cpy(cmd_last_out, "DHCP DISCOVER SENT..."); }
                            _41 _15(str_starts(cmd_input, "TCP")) { send_tcp_syn(0xFFFFFFFF, 80); }
                            _41 _15(str_starts(cmd_input, "GENESIS")) { _15(cid EQ 156) { genesis_tab = 0; str_cpy(cmd_last_out, "ACCESS GRANTED [ID 156]"); } _41 { str_cpy(cmd_last_out, "AUTH FAILED"); } }
							_41 _15(cmd_input[0] EQ 'A' AND cmd_input[1] EQ 'H' AND cmd_input[2] EQ 'C' AND cmd_input[3] EQ 'I') {
							_43 port_no = 0;
							_15(cmd_input[11] >= '0' AND cmd_input[11] <= '9') {
								port_no = cmd_input[11] - 48;
							}
							_182* raw_buffer = (_182*)0x04000000;
                            _39(_43 i=0; i<256; i++) raw_buffer[i] = 0;
                            ahci_get_raw_identify(port_no, raw_buffer);
							_30 model[41];
							_43 m_idx = 0;
							_39(_43 i = 27; i <= 46; i++) {
								model[m_idx++] = (raw_buffer[i] >> 8) & 0xFF;
								model[m_idx++] = raw_buffer[i] & 0xFF;
							}
							model[40] = 0;
							_43 last_char = 39;
							_114(last_char > 0 AND (model[last_char] EQ ' ' OR model[last_char] EQ 0)) {
								model[last_char] = 0; last_char--;
							}
							str_cpy(cmd_last_out, "DMA-TEXT: ");
							_15(model[0] EQ 0) {
								str_cat(cmd_last_out, "EMPTY (DMA FEHLGESCHLAGEN)");
							} _41 {
								str_cat(cmd_last_out, model);
							}
						}
						/// --- BARE METAL FIX: DER DMA SEKTOR DUMP ---
                        _41 _15(str_starts(cmd_input, "DUMP")) {
                            _43 port_no = 0;
                            /// Lese die Port-Nummer aus dem Befehl (z.B. "DUMP 0")
                            _15(cmd_input[5] >= '0' AND cmd_input[5] <= '9') {
                                port_no = cmd_input[5] - 48;
                            }
                            _184* dump_buf = (_184*)0x05000000;
                            _39(_43 i=0; i<512; i++) dump_buf[i] = 0;
                            /// Wir feuern den direkten DMA-Lese-Befehl ab!
                            _15(ahci_read_sectors(port_no, 0, 1, (uint64_t)dump_buf)) {
                                _30 h1[5], h2[5], h3[5], h4[5];
                                byte_to_hex(dump_buf[0], h1); 
                                byte_to_hex(dump_buf[1], h2); 
                                byte_to_hex(dump_buf[510], h3); 
                                byte_to_hex(dump_buf[511], h4);
                                str_cpy(cmd_last_out, "SEC0: ");
                                str_cat(cmd_last_out, h1); str_cat(cmd_last_out, " "); str_cat(cmd_last_out, h2);
                                str_cat(cmd_last_out, " .. ");
                                str_cat(cmd_last_out, h3); str_cat(cmd_last_out, " "); str_cat(cmd_last_out, h4);
                            } _41 {
                                str_cpy(cmd_last_out, "DMA READ TIMEOUT / BLOCKED!");
                            }
						}
							_41 _15(str_starts(cmd_input, "PING")) { _15(str_equal(cmd_status, "ONLINE") OR str_starts(cmd_status, "ONLINE")) str_cpy(cmd_last_out, "REPLY FROM HOST: TIME=1ms"); _41 str_cpy(cmd_last_out, "FAIL: NO CONNECTION"); }
                            _41 str_cpy(cmd_last_out, "UNKNOWN COMMAND");
                            cmd_idx = 0;
                        } _41 _15(last_key EQ 0x0E AND cmd_idx > 0) { cmd_idx--; cmd_input[cmd_idx] = 0; }
                        _41 _15(cmd_idx < 30) { _30 c = get_ascii(last_key); _15(c >= 'a' AND c <= 'z') c -= 32; { cmd_input[cmd_idx++] = c; cmd_input[cmd_idx] = 0; } }
                    }
                    Text(wx+20, wy+50, "STATUS:", 0xAAAAAA, _128); Text(wx+90, wy+50, cmd_status, (str_equal(cmd_status, "ONLINE") OR str_starts(cmd_status,"ONLINE")) ? 0x00FF00 : 0xFF0000, _128);
                    _39(_43 l=0; l<4; l++) _15(cmd_lines[3-l][0] NEQ 0) Text(wx+20, wy+80+(l*15), cmd_lines[3-l], 0x666666, _128);
                    Text(wx+20, wy+150, "> ", 0x00FF00, _128); Text(wx+35, wy+150, cmd_last_out, 0x00FF00, _128);
                    Text(wx+20, wy+170, "C:\\>", 0x00FF00, _128); Text(wx+60, wy+170, cmd_input, 0x00FF00, _128);
                    _15((frame / 15) % 2 EQ 0) DrawChar(wx+60+(cmd_idx*7), wy+170, '_', 0x00FF00, _128);
                }
                _15(win->id EQ 6) { 
                    _43 mid=wx+ww/2; Text(wx+20, wy+40, "[ARCH]", genesis_tab EQ 0?0x000000:0x555555, _128); Text(wx+90, wy+40, "[HARD]", genesis_tab EQ 1?0x000000:0x555555, _128); Text(wx+160, wy+40, "[CODE]", genesis_tab EQ 2?0x000000:0x555555, _128);
                    _15(mouse_just_pressed AND !blocked AND mouse_y > wy+30 AND mouse_y < wy+50) { _15(mouse_x > wx+20 AND mouse_x < wx+80) genesis_tab=0; _15(mouse_x > wx+90 AND mouse_x < wx+150) genesis_tab=1; _15(mouse_x > wx+160 AND mouse_x < wx+220) genesis_tab=2; }
                    DrawRoundedRect(wx+20, wy+60, ww-40, 2, 0, 0x333333);
                    _15(genesis_tab EQ 0) { Text(wx+30, wy+80, "LEVEL 7: HIGH-ARCHITEKT (GOTT-MODUS)", 0x000000, _128); Text(wx+30, wy+100, "LEVEL 6: BLUEPRINT (PLANUNG)", 0xAAAAAA, _128); Text(wx+30, wy+120, "LEVEL 5: HIGH-LEVEL (OBJ-C)", 0xAAAAAA, _128); Text(wx+30, wy+140, "LEVEL 4: MANAGED (C# LOGIK)", 0xAAAAAA, _128); Text(wx+30, wy+160, "LEVEL 3: COMPLEX (C++ REALITAET)", 0x000000, _128); Text(wx+30, wy+180, "LEVEL 2: LOW-LEVEL (C BASIS)", 0xAAAAAA, _128); Text(wx+30, wy+200, "LEVEL 1: BINAER HIGH (EXISTENZ)", 0x555555, _128); Text(wx+30, wy+220, "LEVEL 0: BINAER LOW (NICHTS)", 0x555555, _128); } 
                    _41 _15(genesis_tab EQ 1) { Text(wx+30, wy+80, "ERDE (TERRA)", 0x000000, _128); Text(wx+30, wy+95, "FUNKTION: KERNEL / STROM", 0xAAAAAA, _128); Text(wx+30, wy+120, "SONNE (SOLAR)", 0x000000, _128); Text(wx+30, wy+135, "FUNKTION: CPU / LASER / SCHREIBEN", 0xAAAAAA, _128); Text(wx+30, wy+160, "MOND (LUNAR)", 0xCCCCCC, _128); Text(wx+30, wy+175, "FUNKTION: RAM / SPIEGEL / SPEICHER", 0xAAAAAA, _128); } 
                    _41 _15(genesis_tab EQ 2) { Text(wx+30, wy+80, "class NewbornSoul : public Wanderer {", 0x000000, _128); Text(wx+30, wy+95, "  if (!ConnectToAnchor(TERRA_CORE))", 0xCCCCCC, _128); Text(wx+30, wy+110, "     System::Panic(NO_POWER);", 0xFF0000, _128); Text(wx+30, wy+125, "  LoadTribeTemplate(zodiac);", 0xCCCCCC, _128); Text(wx+30, wy+140, "  SolarSystem::Laser::Broadcast();", 0xCCCCCC, _128); Text(wx+30, wy+155, "}", 0x000000, _128); }
                }
                _15(win->id EQ 7) {
                    TextC(wx+ww/2, wy+20, "SCAN RESULT", 0xFFFFFF, _128);
                    _15(key_new AND win_z[12] EQ 7 AND wifi_state EQ 2) { 
                        _15(last_key EQ 0x0E AND wifi_pass_idx > 0) wifi_pass[--wifi_pass_idx] = 0; _41 _15(wifi_pass_idx < 30) { _30 c = get_ascii(last_key); _15(c >= 32) { wifi_pass[wifi_pass_idx++] = c; wifi_pass[wifi_pass_idx]=0; } }
                    }
                    _15(wifi_state EQ 0) { TextC(wx+ww/2, wy+100, "SCANNING AIR...", 0xAAAAAA, _128); _15(frame % 50 EQ 0) wifi_state = 1; } 
                    _41 _15(wifi_state EQ 1) { _39(_43 i=0; i<4; i++) { _43 y_off = wy+60 + (i*30); _44 hov = is_over(mouse_x, mouse_y, wx+ww/2, y_off+10, 100); DrawRoundedRect(wx+20, y_off, ww-40, 25, 5, hov ? 0x999999 : 0x555555); Text(wx+30, y_off+5, wifi_ssids[i], 0xFFFFFF, _128); _15(mouse_just_pressed AND !blocked AND hov) { wifi_selected = i; wifi_state = 2; } } } 
                    _41 _15(wifi_state EQ 2) { 
                        Text(wx+30, wy+60, "ENTER PASSWORD:", 0xAAAAAA, _128); DrawRoundedRect(wx+30, wy+80, ww-60, 30, 0, 0xFFFFFF); Text(wx+35, wy+85, wifi_pass, 0x000000, _128); 
                        _43 btn_y = wy+130; _44 hov = is_over(mouse_x, mouse_y, wx+ww/2, btn_y+15, 60); 
                        DrawRoundedRect(wx+ww/2-50, btn_y, 100, 30, 5, hov ? 0x999999 : 0x333333); TextC(wx+ww/2, btn_y+7, "CONNECT", 0xFFFFFF, _128); 
                        _15(mouse_just_pressed AND !blocked AND hov) { wifi_state = 3; send_udp(0xFFFFFFFF, 5000, 80, "COSMOS HELLO"); } 
                    }
                    _41 _15(wifi_state EQ 3) { 
                        TextC(wx+ww/2, wy+100, "CONNECTED!", 0x000000, _128); 
                        TextC(wx+ww/2, wy+120, wifi_ssids[wifi_selected], 0xAAAAAA, _128); 
                        _15(frame % 60 EQ 0) str_cpy(cmd_status, "ONLINE (SECURE ID 84)");
                    }
					_15(pulse_timer++ > 50) {
						pulse_timer = 0;
						cb_build(telemetry_block, (_182)frame, 0x00, 0xFF, 0x00, 0xFF);
						telemetry_block[7] = 0x07;
						send_cosmos_block(0xFFFFFFFF, telemetry_block);
					}
                }
                DrawTriangle(wx+ww-15, wy+wh, wx+ww, wy+wh-15, wx+ww, wy+wh, 0x888888); 
            }
            _15(!z_blocked AND mouse_just_pressed AND !is_modal_blocked AND !click_consumed AND is_over(mouse_x, mouse_y, cx, cy, 40)) {
                /// Wenn die Galaxie noch ZU ist -> Ausbruch-Sound
                _15(!galaxy_open) {
                    play_ac97_flare();
                    str_cpy(cmd_status, "SOLAR FLARE DETECTED");
                }
                /// Wenn die Galaxie schon AUF ist -> Sog-Sound beim Schließen
                _41 {
                    play_ac97_suction();
                    str_cpy(cmd_status, "GRAVITATIONAL COLLAPSE");
                }
                galaxy_open = !galaxy_open;
                click_consumed = _128;
            }
        }
        DrawAeroCursor(mouse_x, mouse_y);
        Swap(); frame++; 
    } 
}
/// Die Sprung-Funktion (Der Absender)
_81 _50 (*kernel_entry_t)(BootInfo*);

_50 execute_kernel() {
    _15 (os2_ram_address EQ 0) _96; 
    
    /// BARE METAL FIX: OVERLAP-SCHUTZ BEIM KOPIEREN!
    _89* src = (_89*)os2_ram_address;
    _89* dst = (_89*)0x1000000;
    _15 (dst > src) {
        /// Rückwärts kopieren, damit wir uns nicht selbst überschreiben!
        _39(int i = 512000 - 1; i >= 0; i--) dst[i] = src[i];
    } _41 {
        /// Vorwärts kopieren
        _39(int i = 0; i < 512000; i++) dst[i] = src[i];
    }

    /// BOOTINFO VORBEREITEN
    static BootInfo info; 
    info.framebuffer_addr = (_89)(uint64_t)fb;
    info.screen_width = screen_w;      
    info.screen_height = screen_h;
    info.ahci_drive_count = drive_count;
    info.drives_list_ptr = (_89)(uint64_t)&drives[0];
    info.uhci_base_port = usb_io_base;
    info.ac97_bus_port  = ac97_bus_port;
    info.net_mac_addr_low  = 0; 
    info.net_mac_addr_high = 0;
    info.font_ptr = 0;

    _89 target = 0x1000000; 
    _89 info_ptr = (_89)(uint64_t)&info;

/// BARE METAL FIX: Eiserne Register-Zuweisung!
#ifndef __x86_64__
    asm volatile (
        "cli \n\t"
        "jmp *%%ecx"
        : 
        : "b"(info_ptr), "c"(target), "a"(0x2BADB002)
    );
#endif
    
    _39(;;) {} 
}