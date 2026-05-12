#include "cosmos_fs.h"
#include "schneider_lang.h"
_202 HBA_PORT;
_172 HBA_PORT* get_active_ahci_port();
/// Wir nutzen hier 'void*' statt '_184*', damit es exakt zur cosmos_ahci.cpp passt!
_172 _44 ahci_read(HBA_PORT* port, _89 startlba, void* target_ram_address);
_172 _44 ahci_write(HBA_PORT* port, _89 startlba, void* source_ram_address);
_172 _50 ahci_read_sectors(_43 port_no, _43 startlba, _43 count, _89 dma_addr);
_172 _44 usb_bot_read_sectors(_43 dev_addr, _43 ep_out, _43 ep_in, _89 lba, _43 num_sectors, _184* buffer);
_172 _44 ahci_write_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);
/// Globale Variablen des Dateisystems
PhysicalDrive drives[20]; 
_43 drive_count = 0; 
Partition partitions[4]; 
_184 sector0[512]; 
FileEntry file_table[28]; 
_43 drive_status = 0; 
_184 hdd_buf[512];
_43 active_drive_idx = -1;
_30 hw_disk[48] = "SCANNING...";
/// EXTERNE Abhängigkeiten aus kernel.cpp
_172 _44 usb_detected;
_172 _182 ata_base;
_172 _50 outb(_182 p, _184 v);
_172 _184 inb(_182 p);
_172 _50 insw(_182 p, _50* a, _89 c);
_172 _50 outsw(_182 p, _71 _50* a, _89 c);
_172 _50 str_cpy(_30* d, _71 _30* s);
_172 _44 str_equal(_71 _30* s1, _71 _30* s2);
_172 _30 cmd_status[32];
_172 _43 current_open_file;
/// UI & System Variablen, die das Dateisystem braucht
_172 _43 rtc_day;
_172 _43 rtc_mon;
_172 _43 rtc_year;
_172 _50 read_rtc();
_172 _184 current_path_id;
_172 _30 note_buf[10][41];
_172 _50 pci_scan_all();
_172 _30 wifi_ssids[4][20];
_172 _30 mbr_info_text[50];
_172 _89 mbr_start_lba;
_172 _89 gpt_partition_lba;
_172 _89 mft_start_lba;
_172 _89 sec_per_cluster;
_172 _89 indx_start_lba;
/// Globale Variablen für das Dateisystem (HIER OHNE _172!)
_89 sec_per_cluster = 0;
_89 indx_start_lba = 0;
/// ==========================================
/// PHASE 1: MBR PARTITION SCANNER
/// ==========================================
_50 analyze_mbr(_184* buffer) {
    /// 1. Sicherheits-Check
    _15(buffer[510] NEQ 0x55 OR buffer[511] NEQ 0xAA) {
        str_cpy(mbr_info_text, "STATUS: RAW (NO SIGNATURE)");
        _96; 
    }
    /// 2. Die Partitionstabelle scannen
    _39(_43 i = 0; i < 4; i++) {
        _43 offset = 446 + (i * 16);
        _184 part_type = buffer[offset + 4];
        /// Leere Slots ignorieren
        _15(part_type EQ 0x00) _37;
        /// Den Start-Sektor speichern!
        mbr_start_lba = buffer[offset + 8] | (buffer[offset + 9] << 8) | 
                        (buffer[offset + 10] << 16) | (buffer[offset + 11] << 24);
        /// Die Wahrheit ins Display schreiben und abbrechen (_96 = return)
        _15(part_type EQ 0x07) { str_cpy(mbr_info_text, "FORMAT: NTFS / exFAT"); _96; }
        _15(part_type EQ 0x0B OR part_type EQ 0x0C) { str_cpy(mbr_info_text, "FORMAT: FAT32"); _96; }
        _15(part_type EQ 0xEE) { str_cpy(mbr_info_text, "FORMAT: GPT (MODERN EFI)"); _96; }
        str_cpy(mbr_info_text, "FORMAT: UNKNOWN TYPE");
        _96; 
    }
}
/// ==========================================
/// PHASE 1.5: GPT PARTITION SCANNER
/// ==========================================
_50 analyze_gpt(_184* buffer) {
    /// 1. Die Magische Signatur prüfen: "EFI PART"
    /// In ASCII: E=0x45, F=0x46, I=0x49, (Space)=0x20, P=0x50, A=0x41, R=0x52, T=0x54
    _15(buffer[0] NEQ 0x45 OR buffer[1] NEQ 0x46 OR buffer[2] NEQ 0x49 OR buffer[3] NEQ 0x20 OR
        buffer[4] NEQ 0x50 OR buffer[5] NEQ 0x41 OR buffer[6] NEQ 0x52 OR buffer[7] NEQ 0x54) {
        str_cpy(mbr_info_text, "GPT ERROR: INVALID SIGNATURE");
        _96;
    }
    /// 2. Wo liegt das Inhaltsverzeichnis (Partition Entry Array)?
    /// Steht auf Byte 72 (8 Bytes lang, Little Endian)
    _89 entry_lba = buffer[72] | (buffer[73] << 8) | (buffer[74] << 16) | (buffer[75] << 24);
    /// Da wir vorerst nur 32-Bit (4 Bytes) LBA nutzen, reicht uns das hier.
    /// Bei winzigen Platten ist entry_lba fast immer 2.
    /// Fürs Erste signalisieren wir dem UI, dass wir den Header gefunden haben!
    str_cpy(mbr_info_text, "GPT HEADER FOUND! READ LBA 2 NEXT.");
    gpt_partition_lba = entry_lba;
}
/// ==========================================
/// PHASE 2: GPT PARTITION ENTRY SCANNER
/// ==========================================
_50 analyze_gpt_entries(_184* buffer) {
    _43 found_any = 0;
    /// Wir prüfen die 4 möglichen Einträge in diesem Sektor (4 * 128 = 512 Bytes)
    _39(_43 i = 0; i < 4; i++) {
        _43 offset = i * 128;
        /// Ist der Eintrag überhaupt belegt? (Wenn die ersten 16 Bytes Null sind, ist er leer)
        _44 is_empty = _128;
        _39(_43 j = 0; j < 16; j++) { _15(buffer[offset + j] NEQ 0) { is_empty = _86; _37; } }
        _15(is_empty) _37;
        /// 1. Start-LBA auslesen (liegt bei Offset +32)
        _89 first_lba = buffer[offset + 32] | (buffer[offset + 33] << 8) | 
                        (buffer[offset + 34] << 16) | (buffer[offset + 35] << 24);
        /// 2. Die ersten 4 Bytes der Type-GUID lesen, um das Format zu erkennen
        _89 guid_part_1 = buffer[offset + 0] | (buffer[offset + 1] << 8) | 
                          (buffer[offset + 2] << 16) | (buffer[offset + 3] << 24);
        /// Auswertung der magischen Windows/Linux GUIDs
        _15(guid_part_1 EQ 0xEBD0A0A2) {
            str_cpy(mbr_info_text, "FOUND: MS DATA (exFAT/NTFS)");
            gpt_partition_lba = first_lba;
            _96;
        }
        _15(guid_part_1 EQ 0xC12A7328) {
            str_cpy(mbr_info_text, "FOUND: EFI SYSTEM (FAT32)");
            gpt_partition_lba = first_lba;
            found_any = 1;
            /// Wir brechen hier noch NICHT ab, falls danach noch die echte Datenpartition kommt!
        }
    }
    _15(!found_any) {
        str_cpy(mbr_info_text, "GPT: NO KNOWN PARTITION FOUND");
    }
}
/// ==========================================
/// PHASE 3: VBR / DATEISYSTEM SCANNER
/// ==========================================
_50 analyze_vbr(_184* buffer) {
    /// Prüfen auf NTFS
    _15(buffer[3] EQ 'N' AND buffer[4] EQ 'T' AND buffer[5] EQ 'F' AND buffer[6] EQ 'S') {
        /// 1. Sektoren pro Cluster auslesen (meistens 8)
        _89 sec_per_cluster = buffer[13];
        /// 2. Den MFT-Start-Cluster auslesen (Byte 48-51, reicht als 32-Bit für uns)
        _89 mft_cluster = buffer[48] | (buffer[49] << 8) | 
                          (buffer[50] << 16) | (buffer[51] << 24);
        /// 3. Die magische Formel: Wo liegt die MFT physisch auf der SSD?
        /// Echte LBA = Partitions-Start + (MFT_Cluster * Sektoren_pro_Cluster)
        mft_start_lba = gpt_partition_lba + (mft_cluster * sec_per_cluster);
        str_cpy(mbr_info_text, "MFT FOUND! DRIVE CAN BE ROOTED NOW.");
        _96;
    }
    /// Prüfen auf exFAT (der Vollständigkeit halber)
    _15(buffer[3] EQ 'E' AND buffer[4] EQ 'X' AND buffer[5] EQ 'F' AND buffer[6] EQ 'A') {
        str_cpy(mbr_info_text, "FILESYSTEM: exFAT");
        _96;
    }
    str_cpy(mbr_info_text, "FILESYSTEM: UNKNOWN");
}
/// ==========================================
/// PHASE 4: THE TRUE ROOT PARSER (0x90)
/// ==========================================
_50 analyze_mft_root(_184* buffer) {
    _15(buffer[0] NEQ 'F' OR buffer[1] NEQ 'I') { str_cpy(mbr_info_text, "MFT ERROR"); _96; }
    _182 attr_offset = buffer[0x14] | (buffer[0x15] << 8);
    _43 file_idx = 0;
    /// Tabelle NUR HIER einmalig leeren
    _39(_43 f = 0; f < 8; f++) { file_table[f].exists = _86; }
    _114(attr_offset < 1000) {
        _89 attr_type = buffer[attr_offset] | (buffer[attr_offset+1]<<8);
        _89 attr_len  = buffer[attr_offset+4] | (buffer[attr_offset+5]<<8);
        _15(attr_type EQ 0xFFFFFFFF OR attr_len EQ 0) _37;
        _15(attr_type EQ 0x90) { 
            _182 content_offset = attr_offset + (buffer[attr_offset + 0x14] | (buffer[attr_offset + 0x15] << 8));
            _182 node_header = content_offset + 0x10;
            _182 first_entry = buffer[node_header] | (buffer[node_header+1]<<8);
            _182 entry_offset = node_header + first_entry;
            _114(entry_offset < attr_offset + attr_len AND file_idx < 8) {
                _182 entry_size = buffer[entry_offset + 8] | (buffer[entry_offset + 9]<<8);
                _182 flags = buffer[entry_offset + 12];
                _15(entry_size EQ 0 OR (flags & 0x02)) _37;
                _43 name_len = buffer[entry_offset + 0x50];
                _15(buffer[entry_offset + 0x52] NEQ '$' AND name_len > 0) {
                    file_table[file_idx].exists = 1;
                    /// --- NEU: Echte Dateigröße auslesen (Offset 0x40) ---
                    _89 real_size = buffer[entry_offset + 0x40] | (buffer[entry_offset + 0x41]<<8) | (buffer[entry_offset + 0x42]<<16) | (buffer[entry_offset + 0x43]<<24);
                    file_table[file_idx].size = real_size;
                    _43 max_chars = name_len < 12 ? name_len : 11;
                    _39(_43 c = 0; c < max_chars; c++) {
                        file_table[file_idx].name[c] = buffer[entry_offset + 0x52 + (c * 2)];
                    }
                    file_table[file_idx].name[max_chars] = 0;
                    file_idx++;
                }
                entry_offset += entry_size; 
            }
            str_cpy(mbr_info_text, "FILES LOADED!");
            _96;
        }
        attr_offset += attr_len; 
    }
}
//// ==========================================
//// PHASE 5: READING THE EXTERNAL BRAIN (INDX)
//// ==========================================
//_50 analyze_indx_block(_184* buffer) {
//    // 1. Magische Signatur prüfen ("INDX")
//    _15(buffer[0] NEQ 'I' OR buffer[1] NEQ 'N' OR buffer[2] NEQ 'D' OR buffer[3] NEQ 'X') {
//        
//        // --- BARE METAL RÖNTGEN-BLICK ---
//        // Wenn es nicht INDX ist, was hat er dann gelesen?
//        _30 hex[] = "0123456789ABCDEF";
//        str_cpy(mbr_info_text, "ERR: XX XX XX XX");
//        
//        mbr_info_text[5] = hex[buffer[0] >> 4]; mbr_info_text[6] = hex[buffer[0] & 0x0F];
//        mbr_info_text[8] = hex[buffer[1] >> 4]; mbr_info_text[9] = hex[buffer[1] & 0x0F];
//        mbr_info_text[11] = hex[buffer[2] >> 4]; mbr_info_text[12] = hex[buffer[2] & 0x0F];
//        mbr_info_text[14] = hex[buffer[3] >> 4]; mbr_info_text[15] = hex[buffer[3] & 0x0F];
//        _96;
//    }
//
//    // Wo beginnen die Einträge? (Steht im INDX Header bei Offset 0x18)
//    _182 entry_offset = 0x18 + (buffer[0x18] | (buffer[0x19] << 8));
//    _43 file_idx = 0;
//    
//    _39(_43 f = 0; f < 8; f++) { file_table[f].exists = _86; }
//
//    _114(entry_offset < 4000 AND file_idx < 8) {
//        _182 entry_size = buffer[entry_offset + 8] | (buffer[entry_offset + 9]<<8);
//        _182 flags = buffer[entry_offset + 12];
//
//        _15(entry_size EQ 0 OR (flags & 0x02)) _37; // Ende der B-Tree Liste
//
//        _43 name_len = buffer[entry_offset + 0x50];
//        
//        _15(buffer[entry_offset + 0x52] NEQ '$' AND name_len > 0) {
//            file_table[file_idx].exists = _128;
//            file_table[file_idx].size = 0; 
//            
//            _43 max_chars = name_len < 12 ? name_len : 11;
//            _39(_43 c = 0; c < max_chars; c++) {
//                file_table[file_idx].name[c] = buffer[entry_offset + 0x52 + (c * 2)];
//            }
//            file_table[file_idx].name[max_chars] = 0;
//            file_idx++;
//        }
//        entry_offset += entry_size; 
//    }
//    
//    str_cpy(mbr_info_text, "(FILES LOADED) - KungFu is be Water");
//
/// ==========================================
/// 1. ATA LOW-LEVEL TREIBER
/// ==========================================
/// --- BARE METAL FIX: Der DMA-sichere Lese-Router ---
_50 fs_read_sectors(_43 drive_idx, _89 lba, _184* buf, _43 count) {
    _15(drive_idx < 0 OR drive_idx >= drive_count) _96;
    _43 d_type = drives[drive_idx].type;
    _43 d_port = drives[drive_idx].base_port;
    /// --- SATA / AHCI PLATTEN ---
    _15(d_type EQ 0 OR d_type EQ 1) {
        ahci_read_sectors(d_port, lba, count, (_89)(uint64_t)buf);
    }
    /// --- BARE METAL FIX: DER USB-WEG ---
    _41 _15(d_type EQ 3) {
        /// Bei USB nutzen wir den 'base_port' im Array als USB Device Address.
        /// Standard-Endpoints für USB-Sticks sind typischerweise:
        /// EP_OUT = 2  (Host zum Stick)
        /// EP_IN  = 129 (Stick zum Host, 0x81)
        usb_bot_read_sectors(d_port, 2, 129, lba, count, buf);
        /// USB braucht genauso wie AHCI eine winzige Atempause für den DMA-Transfer
        _39(_192 _43 wait = 0; wait < 1000000; wait++) { __asm__ _192("pause"); }
    }
}
_43 ata_wait_busy(_182 base) { _43 t=100000; _114(t--) { _15(!(inb(base+7)&0x80)) _96 0; } _96 1; }
_43 ata_wait_drq(_182 base) { _43 t=100000; _114(t--) { _15(inb(base+7)&0x08) _96 0; } _96 1; }
_50 ata_read_sector(_182 base, _44 slave, _89 lba, _184* buffer) { 
    _15(base EQ 0) _96; _15(ata_wait_busy(base)) _96; 
    outb(base+6, 0xE0 | (slave << 4) | ((lba >> 24) & 0x0F)); outb(base+1, 0x00); outb(base+2, 1); 
    outb(base+3, (_184)lba); outb(base+4, (_184)(lba >> 8)); outb(base+5, (_184)(lba >> 16)); 
    outb(base+7, 0x20); _15(ata_wait_drq(base)) _96; insw(base, buffer, 256); 
}
_50 ata_write_sector(_182 base, _44 slave, _89 lba, _184* buffer) { 
    _15(base EQ 0) _96; _15(ata_wait_busy(base)) _96; 
    outb(base+6, 0xE0 | (slave << 4) | ((lba >> 24) & 0x0F)); outb(base+1, 0x00); outb(base+2, 1); 
    outb(base+3, (_184)lba); outb(base+4, (_184)(lba >> 8)); outb(base+5, (_184)(lba >> 16)); 
    outb(base+7, 0x30); _15(ata_wait_drq(base)) _96; outsw(base, buffer, 256); 
}
_43 ata_probe(_182 port) { ata_base = port; outb(ata_base + 6, 0xA0); outb(ata_base + 7, 0xEC); _15(inb(ata_base + 7) EQ 0) _96 0; _15(!ata_wait_busy(ata_base)) _96 0; _15(inb(ata_base + 7) & 1) _96 0; insw(ata_base, hdd_buf, 256); _96 1; }
_50 ata_swap_string(_184* src, _30* dst, _43 len) {
    _39(_43 i = 0; i < len; i += 2) {
        dst[i]   = src[i + 1];
        dst[i+1] = src[i];
    }
    dst[len] = 0;
    _39(_43 i = len - 1; i > 0; i--) {
        _15(dst[i] EQ ' ') dst[i] = 0;
        _41 _37;
    }
}
_50 ata_identify(_182 base, _44 slave, _43 slot) { 
    _15(inb(base+7) EQ 0xFF) _96;
    outb(base+6, slave ? 0xB0 : 0xA0); 
    _39(_43 i=0; i<40; i++) inb(base+7);
    outb(base+2, 0); outb(base+3, 0); outb(base+4, 0); outb(base+5, 0); 
    outb(base+7, 0xEC);
    _184 status = 0;
    _39(_43 i=0; i<1000; i++) {
        status = inb(base+7);
        _15(status NEQ 0) _37;
    }
    _15(status EQ 0 OR status EQ 0xFF) _96;
    _89 timeout1 = 10000;
    _114((inb(base+7) & 0x80) NEQ 0 AND timeout1 > 0) { timeout1--; } 
    _15(timeout1 EQ 0) _96;
    _184 cl = inb(base+4);
    _184 ch = inb(base+5);
    _15(cl EQ 0x14 AND ch EQ 0xEB) _96; 
    _15(cl EQ 0x69 AND ch EQ 0x96) _96;
    _89 timeout2 = 1000000;
    _114(timeout2 > 0) {
        status = inb(base+7);
        _15(status & 0x01) _96; 
        _15(status & 0x08) _37; 
        timeout2--;
    }
    _15(timeout2 EQ 0) _96;
    insw(base, hdd_buf, 256);
    _15(slot < 8) { 
        drives[slot].present = _128; 
        drives[slot].base_port = base; 
        drives[slot].is_slave = slave; 
        drives[slot].type = 0;
        /// Der Namens-Reparierer (damit nicht QEMU-Müll da steht)
        ata_swap_string((_184*)&hdd_buf[27 * 2], drives[slot].model, 40);
        /// DEIN EXAKTER ORIGINAL-CODE FÜR DIE GRÖSSE:
        _89 lba28 = ((_182*)hdd_buf)[60] | (((_182*)hdd_buf)[61] << 16); 
        drives[slot].size_mb = (lba28 / 2048);
        drive_count++; 
    } 
}
_50 ata_scan_drives() { 
    drive_count = 0; 
    _39(_43 i=0; i<20; i++) drives[i].present = _86;
    ata_identify(0x1F0, _86, 0); 
    ata_identify(0x1F0, _128, 1); 
    ata_identify(0x170, _86, 2); 
    ata_identify(0x170, _128, 3);
    _15(ata_base > 0 AND ata_base NEQ 0x1F0 AND ata_base NEQ 0x170) {
        ata_identify(ata_base, _86, 4);
        ata_identify(ata_base, _128, 5);
    }
    _15(usb_detected AND drive_count < 20) { 
        _43 d = drive_count; drives[d].present = _128; drives[d].type = 3; 
        str_cpy(drives[d].model, "USB MASS STORAGE"); drives[d].size_mb = 16000; 
        drive_count++; 
    }
    _15(drive_count > 0) { 
        str_cpy(hw_disk, drives[0].model); 
    } _41 { 
        str_cpy(hw_disk, "NO DRIVES"); 
    }
}
_50 mbr_scan() { _15(drives[0].present AND drives[0].type EQ 0) { ata_read_sector(drives[0].base_port, drives[0].is_slave, 0, sector0); _15(sector0[510] EQ 0x55 AND sector0[511] EQ 0xAA) { str_cpy(hw_disk, "DRIVE 0: MBR FOUND"); _39(_43 i=0; i<4; i++) { _43 off = 446 + (i * 16); partitions[i].status = sector0[off]; partitions[i].type = sector0[off+4]; partitions[i].start_lba = *(_89*)&sector0[off+8]; partitions[i].size = *(_89*)&sector0[off+12]; } } _41 str_cpy(hw_disk, "DRIVE 0: UNKNOWN"); } }
/// ==========================================
/// 2. COSMOS BYTE-BLOCK SYSTEM
/// ==========================================
_50 fs_write_cosmos_blocks(_43 drive_idx, _89 start_lba, _184* block_array, _43 num_blocks) {
    _15(drive_idx < 0 OR drive_idx >= drive_count) _96;
    _43 array_pos = 0;
    _89 current_lba = start_lba;
    _114(num_blocks > 0) {
        _39(_43 i = 0; i < 512; i++) hdd_buf[i] = 0;
        _43 blocks_this_sector = (num_blocks > 2) ? 2 : num_blocks;
        _39(_43 b = 0; b < blocks_this_sector; b++) {
            _39(_43 i = 0; i < 256; i++) {
                hdd_buf[(b * 256) + i] = block_array[array_pos++];
            }
        }
        ata_write_sector(drives[drive_idx].base_port, drives[drive_idx].is_slave, current_lba, hdd_buf);
        num_blocks -= blocks_this_sector;
        current_lba++;
    }
}
/// LIEST: Array von 26-Byte Blöcken
_43 fs_read_cosmos_blocks(_43 drive_idx, _89 start_lba, _184* block_array, _43 num_blocks) {
    _15(drive_idx < 0 OR drive_idx >= drive_count) _96 0;
    _43 array_pos = 0;
    _89 current_lba = start_lba;
    _43 valid_blocks_found = 0;
    _43 blocks_left = num_blocks;
    _114(blocks_left > 0) {
        ata_read_sector(drives[drive_idx].base_port, drives[drive_idx].is_slave, current_lba, hdd_buf);
        _43 blocks_this_sector = (blocks_left > 2) ? 2 : blocks_left;
        _39(_43 b = 0; b < blocks_this_sector; b++) {
            _184 temp_block[256];
            _39(_43 i = 0; i < 256; i++) temp_block[i] = hdd_buf[(b * 256) + i];
            _15(cb_validate(temp_block)) {
                _39(_43 i = 0; i < 256; i++) block_array[array_pos++] = temp_block[i];
                valid_blocks_found++;
            } _41 {
                array_pos += 256; 
            }
        }
        blocks_left -= blocks_this_sector;
        current_lba++;
    }
    _96 valid_blocks_found;
}
/// ==========================================
/// 3. HIGH-LEVEL DATEISYSTEM
/// ==========================================
_50 fs_flush_table() {
    HBA_PORT* hdd = get_active_ahci_port();
    _15(hdd != 0) {
        _184* safe_buf = (_184*)0x800000;
        _39(_43 i=0; i<512; i++) safe_buf[i] = 0;
        _30* src = (_30*)file_table;
        _30* dst = (_30*)safe_buf;
        _39(_43 i=0; i<sizeof(file_table); i++) dst[i] = src[i];
        ahci_write(hdd, 1002, safe_buf);
    } _41 {
        _15(active_drive_idx NEQ -1) {
            _39(_43 i=0; i<512; i++) hdd_buf[i] = 0;
            _30* src = (_30*)file_table;
            _30* dst = (_30*)hdd_buf;
            _39(_43 i=0; i<sizeof(file_table); i++) dst[i] = src[i];
            ata_write_sector(drives[active_drive_idx].base_port, drives[active_drive_idx].is_slave, 1002, hdd_buf);
        }
    }
}
_50 fs_create_folder(_71 _30* foldername) {
    _43 free_slot = -1;
    _39(_43 i=0; i<28; i++) {
        _15(file_table[i].exists EQ 0) { free_slot = i; _37; }
    }
    _15(free_slot EQ -1) _96;
    file_table[free_slot].exists = 1;
    file_table[free_slot].is_folder = 1;
    file_table[free_slot].parent_idx = current_path_id;
    file_table[free_slot].size = 0;
    file_table[free_slot].sector_offset = 0;
    str_cpy(file_table[free_slot].name, (_30*)foldername);
    str_cpy(file_table[free_slot].date, "16.04.2026");
    ahci_write_sectors(drives[active_drive_idx].base_port, 1002, 1, (_89)(uint64_t)file_table);
    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
}
_50 fs_save_file(_71 _30* filename, _89 size) {
    /// 1. Freien Platz in der Tabelle suchen
    _43 free_slot = -1;
    _39(_43 i=0; i<28; i++) {
        _15(file_table[i].exists EQ 0) { free_slot = i; _37; }
    }
    _15(free_slot EQ -1) _96; /// Abbruch, wenn Tabelle voll
    /// 2. Neue Datei in der RAM-Tabelle anlegen
    file_table[free_slot].exists = 1;
    file_table[free_slot].is_folder = 0;
    file_table[free_slot].parent_idx = current_path_id;
    file_table[free_slot].size = size;
    file_table[free_slot].sector_offset = 1100 + free_slot;
    /// Cast auf _30* erzwingen, um const-Warnungen bei str_cpy zu ignorieren
    str_cpy(file_table[free_slot].name, (_30*)filename);
    str_cpy(file_table[free_slot].date, "16.04.2026");
    /// 3. Den Notepad-Text auf den neuen Sektor brennen (64 MB Alignment!)
    _89 text_ram_addr = 0x04000000;
    _30* text_buffer = (_30*)text_ram_addr;
    _39(_43 i=0; i<512; i++) text_buffer[i] = 0;
    _43 idx = 0;
    _39(_43 r=0; r<10; r++) {
        _39(_43 c=0; c<40; c++) {
            _15(note_buf[r][c] NEQ 0) { text_buffer[idx++] = note_buf[r][c]; }
        }
        text_buffer[idx++] = '\n'; 
    }
    ahci_write_sectors(drives[active_drive_idx].base_port, file_table[free_slot].sector_offset, 1, text_ram_addr);
    _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
    /// 4. Das Inhaltsverzeichnis (Sektor 1002) auf der Festplatte aktualisieren
    ahci_write_sectors(drives[active_drive_idx].base_port, 1002, 1, (_89)(uint64_t)file_table);
    _39(_192 _43 wait2 = 0; wait2 < 1000000; wait2++) __asm__ _192("pause");
    current_open_file = free_slot; 
}
_50 fs_save() { fs_save_file("QUICK.TXT", 512); } 
_50 fs_init() { 
    /// 1. RAM-MÜLL LÖSCHEN
    _39(_43 i = 0; i < 8; i++) { 
        file_table[i].exists = _86; 
        file_table[i].name[0] = 0; 
        file_table[i].size = 0; 
        file_table[i].is_folder = _86; 
        file_table[i].parent_idx = 255;
    }
    _44 found_table = _86;
    /// 2. FESTPLATTE LESEN (Die AHCI / IDE Weiche)
    HBA_PORT* hdd = get_active_ahci_port();
    _15(hdd != 0) {
        _184* safe_buf = (_184*)0x800000;
        ahci_read(hdd, 1002, safe_buf);
        _30* src = (_30*)safe_buf;
        _30* dst = (_30*)file_table;
        _39(_43 i=0; i<sizeof(file_table); i++) dst[i] = src[i];
        _39(_43 i=0; i<28; i++) _15(file_table[i].exists) found_table = _128;
    } _41 {
        _15(active_drive_idx NEQ -1) {
            ata_read_sector(drives[active_drive_idx].base_port, drives[active_drive_idx].is_slave, 1002, hdd_buf);
            _30* src = (_30*)hdd_buf;
            _30* dst = (_30*)file_table;
            _39(_43 i=0; i<sizeof(file_table); i++) dst[i] = src[i];
            _39(_43 i=0; i<28; i++) _15(file_table[i].exists) found_table = _128;
        }
    }
    /// 3. DEIN ORIGINALER FALLBACK (Wenn die Platte leer ist)
    _15(!found_table) { 
        _39(_43 i=0; i<28; i++) file_table[i].exists = _86;
        file_table[0].exists=_128; 
        str_cpy(file_table[0].name, "boot.sys"); 
        file_table[0].is_folder=_86; 
        file_table[0].size=512; 
        str_cpy(file_table[0].date, "22.12.2025"); 
        file_table[0].parent_idx = 255;
        file_table[1].exists=_128; 
        str_cpy(file_table[1].name, "kernel.bin"); 
        file_table[1].is_folder=_86; 
        file_table[1].size=10240; 
        str_cpy(file_table[1].date, "22.12.2025"); 
        file_table[1].parent_idx = 255;
        /// Neu: Sofort auf die Platte brennen, damit sie beim nächsten Mal da sind!
        fs_flush_table(); 
    }
    /// 4. DEIN WIFI CODE (Unbedingt behalten!)
    str_cpy(wifi_ssids[0], "Home-WiFi (90%)"); 
    str_cpy(wifi_ssids[1], "Telekom-AB12"); 
    str_cpy(wifi_ssids[2], "Office-Net"); 
    str_cpy(wifi_ssids[3], "Guest-Access"); 
}
/// ==========================================
/// BARE METAL FIX: GANZ UNTEN IN DER DATEI, UNABHÄNGIG!
/// ==========================================
_172 _50 cfs_format_drive(HBA_PORT* port, _94 total_sectors); /// Schnittstelle zu cfs
_50 fs_format_drive() {
    _15(active_drive_idx < 0 OR active_drive_idx >= drive_count) {
        str_cpy(cmd_status, "ERR: NO DRIVE SELECTED");
        _96;
    }
    HBA_PORT* hdd = get_active_ahci_port();
    /// ==========================================
    /// 1. CFS SUPERBLOCK AUF SEKTOR 0 BRENNEN
    /// ==========================================
    _15(hdd != 0) {
        /// Die Plattengröße in Sektoren berechnen (1 MB = 2048 Sektoren)
        _94 total_sec = (_94)drives[active_drive_idx].size_mb * 2048; 
        cfs_format_drive(hdd, total_sec);
    }
    /// ==========================================
    /// 2. RAM PUTZEN & ROOT-ORDNER ANLEGEN
    /// ==========================================
    _39(_43 i = 0; i < 8; i++) {
        file_table[i].exists = _86;
        file_table[i].is_folder = _86;
        file_table[i].size = 0;
    }
    file_table[0].exists = _128;
    str_cpy(file_table[0].name, "COSMOS_ROOT");
    file_table[0].is_folder = _128;
    file_table[0].parent_idx = 255; 
    str_cpy(file_table[0].date, "01.04.2026");
    /// ==========================================
    /// 3. BEGRÜSSUNGSTEXT IN DIE TABELLE EINTRAGEN
    /// ==========================================
    file_table[1].exists = _128;
    str_cpy(file_table[1].name, "readme.txt");
    file_table[1].is_folder = _86;
    file_table[1].size = 512;
    file_table[1].sector_offset = 2000; /// CFS Standard: Daten starten bei LBA 2000!
    file_table[1].parent_idx = 0; 
    str_cpy(file_table[1].date, "01.04.2026");
    /// RAM auf die Platte flushen (Schreibt unser Inhaltsverzeichnis auf Sektor 1002)
    drive_status = 1; 
    fs_flush_table();
    /// ==========================================
    /// 4. DEN INHALT DER README.TXT BRENNEN
    /// ==========================================
    _30* msg = (_30*)"WELCOME - BARE METAL COSMOS OS!\nDas Dateisystem CFS ist aktiv.";
    _15(hdd != 0) {
        /// BARE METAL FIX: 64 MB Alignment für den DMA Chip!
        _89 text_ram_addr = 0x04000000;
        _30* safe_buf = (_30*)text_ram_addr;
        _39(_43 i = 0; i < 512; i++) safe_buf[i] = 0;
       _30* p = msg; _43 idx = 0; _114(*p) safe_buf[idx++] = *p++;
        /// Unsere kugelsichere Schreib-Funktion nutzen
        ahci_write_sectors(drives[active_drive_idx].base_port, 2000, 1, text_ram_addr);
        _39(_192 _43 wait = 0; wait < 1000000; wait++) __asm__ _192("pause");
    } _41 {
        /// Legacy ATA Fallback
        _39(_43 i = 0; i < 512; i++) hdd_buf[i] = 0;
        _30* p = msg; _43 idx = 0; _114(*p) hdd_buf[idx++] = *p++;
        ata_write_sector(drives[active_drive_idx].base_port, drives[active_drive_idx].is_slave, 2000, hdd_buf);
    }
    str_cpy(cmd_status, "DRIVE FORMATTED TO CFS!");
}