#include "cosmos_ahci.h"
#include "schneider_lang.h"
/// --- BARE METAL FIX: UI Hooks für Diagnostics ---
_172 _50 Text(_43 x, _43 y, _71 _30* s, _89 col, _44 bold);
_172 _50 int_to_str(_43 n, _30* s);
_172 _50 hex_to_str(_182 num, _30* out);
_172 _50 Swap();
/// --- GLOBALE VARIABLEN ---
_89 ahci_memory_base = 0x800000;
HBA_PORT* global_active_port = 0;
_89 global_ahci_abar = 0; 
_43 ahci_scan_state = 0; 
_43 ahci_scan_timer = 0;
_172 _43 drive_count;
_172 _30 hw_disk[48];
_172 _89 ahci_found_ports_bitmask; 
_172 _50 str_cpy(_30* d, _71 _30* s);
_172 _50 int_to_str(_43 val, _30* str);
/// ==========================================
/// BARE METAL PLUG & PLAY: Hardware Registry Import
/// ==========================================
_202 HardwareRegistry {
    _44 is_ahci_found;
    _89 ahci_bar5;
    _44 is_usb_found;
    _89 usb_bar0;
    _44 is_net_found;
    _89 net_bar0;
    _89 net_io_port;
};

/// Wir sagen der Datei, dass diese beiden Variablen extern in anderen .cpp Dateien liegen!
_172 HardwareRegistry sys_hw; 
_172 _30 cmd_status[256];
/// ==========================================
/// ISO9660 STRUKTUR FÜR DATEIEN
/// ==========================================
#pragma pack(push, 1)
_202 ISO_DirEntry {
    _184 length;
    _184 ext_attr_len;
    _89  lba_le;       /// LBA (Little Endian)
    _89  lba_be;       /// LBA (Big Endian)
    _89  size_le;      /// Dateigröße
    _89  size_be;
    _184 date[7];
    _184 flags;
    _184 unit_size;
    _184 gap_size;
    _182 vol_seq_le;
    _182 vol_seq_be;
    _184 name_len;
    _30  name[1];      /// Dynamische Namenslänge
} __attribute__((packed));
#pragma pack(pop)
/// Dem AHCI-Treiber den Disk Manager vorstellen
_202 DriveInfo { _43 type; _43 size_mb; _43 base_port; _30 model[41]; };
_172 DriveInfo drives[8];
_172 _43 drive_count;
#pragma pack(push, 1)
/// Die Struktur für einen Long File Name (LFN) Fake-Eintrag
_202 FAT32_LFN_Entry {
    _184 sequence_number; /// Welche Nummer hat dieser Teil des Namens? (Bit 6 = Letzter Teil)
    _182 name_part_1[5];  /// Erste 5 Zeichen (Unicode, je 2 Bytes)
    _184 attr;            /// IMMER 0x0F für LFN
    _184 reserved_1;      /// Immer 0x00
    _184 checksum;        /// Prüfsumme des 8.3 Kurznamens
    _182 name_part_2[6];  /// Nächste 6 Zeichen (Unicode)
    _182 reserved_2;      /// Immer 0x0000
    _182 name_part_3[2];  /// Letzte 2 Zeichen (Unicode)
} __attribute__((packed));
#pragma pack(pop)
/// ==========================================
/// FAT32 STRUKTUREN
/// ==========================================
#pragma pack(push, 1)
/// Der BIOS Parameter Block (Sektor 0 einer FAT32 Partition)
_202 FAT32_BPB {
    _184 jmp[3];
    _30  oem_name[8];
    _182 bytes_per_sector;
    _184 sectors_per_cluster;
    _182 reserved_sectors;
    _184 fat_count;
    _182 root_dir_entries;
    _182 total_sectors_16;
    _184 media_type;
    _182 sectors_per_fat_16;
    _182 sectors_per_track;
    _182 heads;
    _43  hidden_sectors;
    _43  total_sectors_32;
    _43  sectors_per_fat_32; /// WICHTIG für FAT32!
    _182 ext_flags;
    _182 fs_version;
    _43  root_cluster;       /// Wo fängt das Hauptverzeichnis an?
    _182 fs_info;
    _182 backup_boot_sector;
    _184 reserved[12];
    _184 drive_number;
    _184 reserved1;
    _184 boot_signature;     /// Sollte 0x29 sein
    _43  volume_id;
    _30  volume_label[11];
    _30  fs_type[8];         /// Hier steht "FAT32   "
} __attribute__((packed));
/// Ein Eintrag im Verzeichnis (Eine Datei oder ein Ordner)
_202 FAT32_DirectoryEntry {
    _30  name[11];           /// 8 Zeichen Name, 3 Zeichen Endung (z.B. "KERNEL  BIN")
    _184 attr;               /// Attribute (0x10 = Ordner, 0x20 = Archiv)
    _184 reserved;
    _184 create_time_tenths;
    _182 create_time;
    _182 create_date;
    _182 access_date;
    _182 cluster_high;       /// Wo liegen die Daten der Datei? (Obere 16 Bit)
    _182 modify_time;
    _182 modify_date;
    _182 cluster_low;        /// Wo liegen die Daten der Datei? (Untere 16 Bit)
    _43  size;               /// Dateigröße in Bytes
} __attribute__((packed));
#pragma pack(pop)
/// ==========================================
/// BARE METAL FIX: SCHNELLES REBASE
/// ==========================================
_50 ahci_port_rebase(HBA_PORT *port, _43 port_no) {
    /// 1. Motor stoppen (ST & FRE aus)
    port->cmd &= ~0x0001;
    port->cmd &= ~0x0010;
    /// Warten bis der Controller den Stop bestätigt (Bit 14 & 15)
    _43 timeout = 100000;
    _114 ((port->cmd & 0xC000) AND timeout > 0) { timeout--; }
    /// 2. Speicher-Layout (32 KB Abstand pro Port)
    /// Das verhindert die Spiegelung zu 100%
    _89 port_base = ahci_memory_base + (port_no * 32768);
    port->clb = port_base;               /// Command List (1 KB)
    port->clbu = 0;
    port->fb = port_base + 1024;         /// FIS Base (256 Bytes)
    port->fbu = 0;
    /// 3. Command Header & Tables
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
    _39 (_43 i = 0; i < 32; i++) {
        cmdheader[i].prdtl = 1; 
        /// Command Tables starten ab 2 KB Offset innerhalb des Port-Bereichs
        cmdheader[i].ctba = port_base + 2048 + (i * 256);
        cmdheader[i].ctbau = 0;
    }
    /// 4. Motor wieder starten
    port->serr = 0xFFFFFFFF; /// Alte Fehler löschen
    port->cmd |= 0x0010;     /// FRE an
    port->cmd |= 0x0001;     /// ST an
}
/// BARE METAL DRIVER: AHCI SATA Controller
/// BARE METAL DRIVER: AHCI SATA Controller
_50 ahci_init() {
    /// 1. Hat das Oracle einen Controller gefunden UND ist die Adresse gültig?
    _15(!sys_hw.is_ahci_found OR sys_hw.ahci_bar5 EQ 0) {
        str_cpy(cmd_status, "AHCI: NO CONTROLLER FOUND!");
        _96; 
    }

    /// 2. Adresse aus der Registry holen!
    _89 abar = sys_hw.ahci_bar5;
    volatile _89* hba_mem = (volatile _89*)abar;

    /// ==========================================
    /// BARE METAL FIX: WAKE UP THE CONTROLLER!
    /// Wenn das auf neuen PCs fehlt, friert die CPU sofort ein.
    /// Wir setzen GHC.AE (Bit 31 im Global Host Control Register).
    /// ==========================================
    hba_mem[1] = hba_mem[1] | 0x80000000; 

    /// 3. PI (Ports Implemented) auslesen
    _89 pi = hba_mem[3]; 
    _15(pi EQ 0) {
        str_cpy(cmd_status, "AHCI: NO PORTS IMPLEMENTED!");
        _96;
    }

    /// (Hier geht dein Code mit der FOR-Schleife für die Ports weiter...)
    _39(_43 i = 0; i < 32; i++) {
        _15(pi & (1 << i)) {
            /// Port-Register starten bei Offset 0x100. Jeder Port hat 0x80 Bytes an Registern.
            volatile _89* port = (volatile _89*)(abar + 0x100 + (i * 0x80));
            
            /// SATA Status Register (SSTS) lesen (Offset 0x28 innerhalb des Ports = Index 10)
            _89 ssts = port[10]; 
            
            _89 det = ssts & 0x0F;         /// Device Detection (Ist was eingesteckt?)
            _89 ipm = (ssts >> 8) & 0x0F;  /// Interface Power Management (Ist es wach?)

            /// DET = 3 (Device detected and Phy established)
            /// IPM = 1 (Active state)
            _15(det EQ 3 AND ipm EQ 1) {
                /// BARE METAL FIX: Laufwerk gefunden! Wir generieren eine schöne Systemmeldung.
                str_cpy(cmd_status, "AHCI: DRIVE READY ON PORT ");
                _30* p = cmd_status + 26;
                _15(i < 10) {
                    *p++ = '0' + i;
                } _41 {
                    *p++ = '0' + (i / 10);
                    *p++ = '0' + (i % 10);
                }
                *p = 0;
                
                /// Hier könnten wir uns die Port-Nummer für später merken
                /// z.B.: sys_hw.active_sata_port = i;
                _96; /// Fürs Erste stoppen wir beim ersten gefundenen Laufwerk
            }
        }
    }
    
    str_cpy(cmd_status, "AHCI: NO DRIVES CONNECTED!");
}
/// ==========================================
/// 2. DIE LESE-FUNKTION (ATAPI / CD-ROM)
/// ==========================================
_44 ahci_read_cdrom(_43 port_no, _89 lba, _43 count, _89 buffer_addr) {
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    _192 _89* port_regs = abar + 64 + (port_no * 32);
    _43 p_off = port_no * 0x10000; 
    _43 cmd_list_addr = 0x400000 + p_off;  
    _43 cmd_table_addr = 0x402000 + p_off;
    _192 _89* cmd_header = (_192 _89*)cmd_list_addr;
    cmd_header[0] = 5 | 0x20 | (1 << 16); 
    cmd_header[1] = 0; cmd_header[2] = cmd_table_addr; cmd_header[3] = 0;
    _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
    _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
    cmd_table[0] = 0x00A08027;
    _192 _184* acmd = (_192 _184*)(cmd_table_addr + 0x40);
    acmd[0] = 0xA8; 
    acmd[2] = (lba >> 24) & 0xFF; acmd[3] = (lba >> 16) & 0xFF;
    acmd[4] = (lba >> 8) & 0xFF;  acmd[5] = lba & 0xFF;
    acmd[6] = (count >> 24) & 0xFF; acmd[7] = (count >> 16) & 0xFF;
    acmd[8] = (count >> 8) & 0xFF;  acmd[9] = count & 0xFF;
    _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
    prdt[0] = buffer_addr; prdt[1] = 0; prdt[2] = 0; 
    prdt[3] = ((count * 2048) - 1) | 0x80000000;
    __asm__ _192("wbinvd" ::: "memory"); /// VORHER CACHE LEEREN!
    port_regs[12] = 0xFFFFFFFF; 
    port_regs[14] = 1;
    _43 timeout = 1000000;
    _114((port_regs[14] & 1) AND timeout > 0) timeout--;
    __asm__ _192("wbinvd" ::: "memory"); /// NACHHER CACHE LEEREN!
    _15(timeout > 0 AND !(port_regs[4] & 0x40000000)) _96 _128;
    _96 _86;
}
/// ==========================================
/// ISO9660 KERNEL LOADER
/// ==========================================
_50 iso9660_load_file(_43 port_no, _71 _30* target_filename) {
    _89 pvd_buffer = 0x600000; /// Puffer für Sektor 16
    _89 dir_buffer = 0x601000; /// Puffer für das Verzeichnis
    _89 file_load_addr = 0x1000000; /// 16 MB Grenze
    /// 1. Primary Volume Descriptor (Sektor 16) lesen
    _15(!ahci_read_cdrom(port_no, 16, 1, pvd_buffer)) {
        str_cpy(cmd_status, "ISO: PVD READ FAILED"); _96;
    }
    _192 _184* pvd = (_192 _184*)pvd_buffer;
    /// Check: Ist es wirklich eine CD? (Die Signatur lautet "CD001")
    _15(pvd[1] NEQ 'C' OR pvd[2] NEQ 'D' OR pvd[3] NEQ '0' OR pvd[4] NEQ '0' OR pvd[5] NEQ '1') {
        str_cpy(cmd_status, "ISO: NOT ISO9660"); _96;
    }
    /// Die Infos zum Hauptverzeichnis beginnen an Byte 156 im PVD
    ISO_DirEntry* root_record = (ISO_DirEntry*)&pvd[156];
    _89 root_lba = root_record->lba_le;
    _89 root_size = root_record->size_le;
    /// 2. Das Hauptverzeichnis laden
    /// Wie viele 2048-Byte Sektoren brauchen wir für das Verzeichnis?
    _43 dir_sectors = (root_size + 2047) / 2048;
    _15(!ahci_read_cdrom(port_no, root_lba, dir_sectors, dir_buffer)) {
        str_cpy(cmd_status, "ISO: DIR READ FAILED"); _96;
    }
    /// 3. Datei suchen
    _43 offset = 0;
    _89 file_lba = 0;
    _89 file_size = 0;
    _114(offset < root_size) {
        ISO_DirEntry* entry = (ISO_DirEntry*)(dir_buffer + offset);
        _15(entry->length EQ 0) _37; /// Ende des Verzeichnisses erreicht
        /// Wenn Bit 1 nicht gesetzt ist, ist es eine Datei (kein Ordner)
        _15(!(entry->flags & 0x02)) {
            _44 match = _128;
            _43 search_len = 0;
            _114(target_filename[search_len] NEQ 0) search_len++;
            /// BARE METAL FIX: ISO9660 hängt oft ein ";1" (Dateiversion) an den Dateinamen an.
            /// Wir prüfen nur die Buchstaben deines gesuchten Namens!
            _39(_43 c = 0; c < search_len; c++) {
                _15(entry->name[c] NEQ target_filename[c]) { match = _86; _37; }
            }
            _15(match) {
                file_lba = entry->lba_le;
                file_size = entry->size_le;
                _37;
            }
        }
        /// Zum nächsten Eintrag springen
        offset += entry->length;
    }
    _15(file_lba EQ 0) {
        str_cpy(cmd_status, "ISO: KERNEL.BIN NOT FOUND"); _96;
    }
    /// 4. DATEI DIREKT IN DEN RAM (0x1000000) KOPIEREN
    _43 file_sectors = (file_size + 2047) / 2048;
    _15(!ahci_read_cdrom(port_no, file_lba, file_sectors, file_load_addr)) {
        str_cpy(cmd_status, "ISO: FILE READ FAILED"); _96;
    }
    str_cpy(cmd_status, "ISO: KERNEL LOADED TO 0x1000000!");
}
/// ==========================================
/// 1. DIE LESE-FUNKTION (SATA)
/// ==========================================
_44 ahci_read_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr) {
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    _192 _89* port_regs = abar + 64 + (port_no * 32);
    _43 p_off = port_no * 0x10000; 
    _43 cmd_list_addr = 0x400000 + p_off;  
    _43 cmd_table_addr = 0x402000 + p_off;
    _192 _89* cmd_header = (_192 _89*)cmd_list_addr;
    cmd_header[0] = 5 | (1 << 16) | 0x400; 
    cmd_header[1] = 0; cmd_header[2] = cmd_table_addr; cmd_header[3] = 0;
    _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
    _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
    cmd_table[0] = 0x00258027; 
    cmd_table[1] = (lba & 0xFFFFFF) | 0x40000000; 
    cmd_table[2] = (lba >> 24) & 0xFF;            
    cmd_table[3] = count;
    _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
    prdt[0] = buffer_addr; prdt[1] = 0; prdt[2] = 0; 
    prdt[3] = ((count * 512) - 1) | 0x80000000;
    /// BARE METAL FIX 1: Cache zwingend leeren, BEVOR wir feuern!
    __asm__ _192("wbinvd" ::: "memory");
    port_regs[12] = 0xFFFFFFFF;
    port_regs[14] = 1;
    _43 timeout = 1000000;
    _114((port_regs[14] & 1) AND timeout > 0) timeout--;
    /// BARE METAL FIX 2: Cache leeren, NACHDEM die Hardware geschrieben hat!
    __asm__ _192("wbinvd" ::: "memory");
    _15(timeout > 0 AND !(port_regs[4] & 0x40000000)) _96 _128; 
    _96 _86; 
}
/// ==========================================
/// BARE METAL FIX: DER ECHTE WRITE-BEFEHL
/// ==========================================
_44 ahci_write_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr) {
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    _192 _89* port_regs = abar + 64 + (port_no * 32);
    _43 p_off = port_no * 0x10000; 
    _43 cmd_list_addr = 0x400000 + p_off;  
    _43 cmd_table_addr = 0x402000 + p_off;
    _192 _89* cmd_header = (_192 _89*)cmd_list_addr;
    /// BARE METAL FIX: Nur Length (5), W-Flag (0x40) und PRDTL (1)
    cmd_header[0] = 5 | 0x40 | (1 << 16); 
    cmd_header[1] = 0; cmd_header[2] = cmd_table_addr; cmd_header[3] = 0;
    _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
    _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
    /// 0x35 = WRITE DMA EXT
    cmd_table[0] = 0x00358027; 
    cmd_table[1] = (lba & 0xFFFFFF) | 0x40000000; 
    cmd_table[2] = (lba >> 24) & 0xFF;            
    cmd_table[3] = count;
    _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
    prdt[0] = buffer_addr; prdt[1] = 0; prdt[2] = 0; 
    prdt[3] = ((count * 512) - 1) | 0x80000000;
    __asm__ _192("wbinvd" ::: "memory");
    port_regs[12] = 0xFFFFFFFF;
    port_regs[14] = 1;
    _43 timeout = 1000000;
    _114((port_regs[14] & 1) AND timeout > 0) timeout--;
    __asm__ _192("wbinvd" ::: "memory");
    _15(timeout > 0 AND !(port_regs[4] & 0x40000000)) _96 _128; 
    _96 _86; 
}
/// ==========================================
/// FAT32: ROOT DIRECTORY MIT LONG FILE NAMES (LFN)
/// ==========================================
_50 fat32_read_root_dir(_43 port_no) {
    _89 bpb_buffer = 0x600000; 
    _89 dir_buffer = 0x601000;
    _15(!ahci_read_sectors(port_no, 0, 1, bpb_buffer)) {
        str_cpy(cmd_status, "FAT32: BPB READ FAILED"); _96;
    }
    FAT32_BPB* bpb = (FAT32_BPB*)bpb_buffer;
    _15(bpb->bytes_per_sector EQ 0) {
        str_cpy(cmd_status, "FAT32: INVALID BPB"); _96;
    }
    _43 fat_start = bpb->hidden_sectors + bpb->reserved_sectors;
    _43 data_start = fat_start + (bpb->fat_count * bpb->sectors_per_fat_32);
    _43 root_lba = data_start + ((bpb->root_cluster - 2) * bpb->sectors_per_cluster);
    _15(!ahci_read_sectors(port_no, root_lba, 1, dir_buffer)) {
        str_cpy(cmd_status, "FAT32: DIR READ FAILED"); _96;
    }
    FAT32_DirectoryEntry* dir = (FAT32_DirectoryEntry*)dir_buffer;
    _30 found_files[500]; /// Puffer deutlich größer für lange Namen!
    _39(_43 b=0; b<500; b++) found_files[b] = 0;
    _43 f_idx = 0;
    /// --- LFN PUFFER ---
    /// Ein langer Name kann theoretisch 255 Zeichen lang sein
    _30 current_lfn[256];
    _39(_43 l=0; l<256; l++) current_lfn[l] = 0;
    _44 has_lfn = _86;
    /// Wir parsen alle 16 Einträge dieses Sektors
    _39(_43 i = 0; i < 16; i++) {
        _15(dir[i].name[0] EQ 0x00) _37; /// Ende des Verzeichnisses
        _15(dir[i].name[0] EQ 0xE5) {    /// Gelöschte Datei
            has_lfn = _86; /// LFN Puffer resetten
            _101; 
        }
        /// ==========================================
        /// IST ES EIN LFN-EINTRAG (Attribut 0x0F)?
        /// ==========================================
        _15(dir[i].attr EQ 0x0F) {
            has_lfn = _128;
            FAT32_LFN_Entry* lfn = (FAT32_LFN_Entry*)&dir[i];
            /// Die Sequenznummer verrät uns, wo wir den Namensteil einfügen müssen
            /// (Wir maskieren Bit 6 weg, das nur anzeigt, ob es der letzte Teil ist)
            _43 seq = lfn->sequence_number & 0x1F;
            _15(seq EQ 0 OR seq > 20) _101; /// Sicherheitscheck
            /// Jeder LFN-Teil fasst maximal 13 Zeichen. 
            /// seq=1 ist Zeichen 0-12, seq=2 ist Zeichen 13-25, etc.
            _43 offset = (seq - 1) * 13;
            /// BARE METAL FIX: LFN speichert in UTF-16 (2 Bytes pro Zeichen).
            /// Da wir nacktes ASCII/ANSI nutzen, lesen wir nur das untere Byte!
            current_lfn[offset + 0] = (_30)lfn->name_part_1[0];
            current_lfn[offset + 1] = (_30)lfn->name_part_1[1];
            current_lfn[offset + 2] = (_30)lfn->name_part_1[2];
            current_lfn[offset + 3] = (_30)lfn->name_part_1[3];
            current_lfn[offset + 4] = (_30)lfn->name_part_1[4];
            current_lfn[offset + 5] = (_30)lfn->name_part_2[0];
            current_lfn[offset + 6] = (_30)lfn->name_part_2[1];
            current_lfn[offset + 7] = (_30)lfn->name_part_2[2];
            current_lfn[offset + 8] = (_30)lfn->name_part_2[3];
            current_lfn[offset + 9] = (_30)lfn->name_part_2[4];
            current_lfn[offset + 10] = (_30)lfn->name_part_2[5];
            current_lfn[offset + 11] = (_30)lfn->name_part_3[0];
            current_lfn[offset + 12] = (_30)lfn->name_part_3[1];
            _101; /// Zum nächsten Eintrag gehen!
        }
        /// ==========================================
        /// NORMALER EINTRAG (Der LFN gehört zu dieser Datei!)
        /// ==========================================
        _15(dir[i].attr & 0x08) { has_lfn=_86; _101; } /// Volume Label ignorieren
        /// WIR SCHREIBEN DEN NAMEN INS UI
        _15(has_lfn) {
            /// WIR HABEN EINEN LANGEN NAMEN!
            _43 l_idx = 0;
            /// Wir kopieren, bis eine 0 kommt oder das Limit (255) erreicht ist
            _114(current_lfn[l_idx] NEQ 0 AND current_lfn[l_idx] NEQ (_30)0xFF AND l_idx < 255 AND f_idx < 490) {
                found_files[f_idx++] = current_lfn[l_idx++];
            }
        } _41 {
            /// FALLBACK: Es gab keinen langen Namen, wir nutzen den 8.3 Kurznamen
            _39(_43 c = 0; c < 11; c++) {
                _15(dir[i].name[c] NEQ ' ') {
                    found_files[f_idx++] = dir[i].name[c];
                }
                _15(c EQ 7 AND dir[i].name[8] NEQ ' ') {
                    found_files[f_idx++] = '.';
                }
            }
        }
        /// Ist es ein Ordner?
        _15(dir[i].attr & 0x10) { found_files[f_idx++] = '/'; }
        found_files[f_idx++] = ' '; /// Leerzeichen für die nächste Datei
        /// GANZ WICHTIG: LFN-Status für die NÄCHSTE Datei zurücksetzen!
        has_lfn = _86; 
        _39(_43 l=0; l<256; l++) current_lfn[l] = 0;
        _15(f_idx > 480) _37; /// Puffer-Schutz
    }
    _15(f_idx > 0) {
        found_files[f_idx] = 0;
        str_cpy(cmd_status, found_files);
    } _41 {
        str_cpy(cmd_status, "FAT32: DIR IS EMPTY");
    }
}
/// ==========================================
/// FAT32: DATEI LADEN (UNTERSTÜTZT LANGE DATEINAMEN / LFN)
/// ==========================================
_50 fat32_load_file(_43 port_no, _71 _30* target_filename) {
    /// Puffer im sicheren Bereich
    _89 bpb_buffer = 0x600000; 
    _89 dir_buffer = 0x601000; 
    _89 fat_buffer = 0x602000; 
    _89 file_load_addr = 0x1000000; /// Ziel-Adresse für den Kernel (16 MB)
    _15(!ahci_read_sectors(port_no, 0, 1, bpb_buffer)) {
        str_cpy(cmd_status, "FAT32 LOAD: BPB READ FAILED"); _96;
    }
    FAT32_BPB* bpb = (FAT32_BPB*)bpb_buffer;
    _15(bpb->bytes_per_sector EQ 0) {
        str_cpy(cmd_status, "FAT32 LOAD: INVALID BPB"); _96;
    }
    /// Mathematik für die Sektoren
    _43 fat_start = bpb->hidden_sectors + bpb->reserved_sectors;
    _43 data_start = fat_start + (bpb->fat_count * bpb->sectors_per_fat_32);
    _43 root_lba = data_start + ((bpb->root_cluster - 2) * bpb->sectors_per_cluster);
    /// ==========================================
    /// SCHRITT 1: DATEI SUCHEN (MIT LFN-SUPPORT)
    /// ==========================================
    _15(!ahci_read_sectors(port_no, root_lba, 1, dir_buffer)) {
        str_cpy(cmd_status, "FAT32 LOAD: DIR READ FAILED"); _96;
    }
    FAT32_DirectoryEntry* dir = (FAT32_DirectoryEntry*)dir_buffer;
    _43 target_cluster = 0;
    _43 file_size = 0;
    _30 current_lfn[256];
    _39(_43 l=0; l<256; l++) current_lfn[l] = 0;
    _44 has_lfn = _86;
    _39(_43 i = 0; i < 16; i++) {
        _15(dir[i].name[0] EQ 0x00) _37; /// Ende des Ordners
        _15(dir[i].name[0] EQ 0xE5) { has_lfn = _86; _101; } /// Gelöschte Datei überspringen
        /// LFN-Eintrag sammeln
        _15(dir[i].attr EQ 0x0F) {
            has_lfn = _128;
            FAT32_LFN_Entry* lfn = (FAT32_LFN_Entry*)&dir[i];
            _43 seq = lfn->sequence_number & 0x1F;
            _15(seq EQ 0 OR seq > 20) _101;
            _43 offset = (seq - 1) * 13;
            current_lfn[offset + 0] = (_30)lfn->name_part_1[0]; current_lfn[offset + 1] = (_30)lfn->name_part_1[1];
            current_lfn[offset + 2] = (_30)lfn->name_part_1[2]; current_lfn[offset + 3] = (_30)lfn->name_part_1[3];
            current_lfn[offset + 4] = (_30)lfn->name_part_1[4]; current_lfn[offset + 5] = (_30)lfn->name_part_2[0];
            current_lfn[offset + 6] = (_30)lfn->name_part_2[1]; current_lfn[offset + 7] = (_30)lfn->name_part_2[2];
            current_lfn[offset + 8] = (_30)lfn->name_part_2[3]; current_lfn[offset + 9] = (_30)lfn->name_part_2[4];
            current_lfn[offset + 10] = (_30)lfn->name_part_2[5]; current_lfn[offset + 11] = (_30)lfn->name_part_3[0];
            current_lfn[offset + 12] = (_30)lfn->name_part_3[1];
            _101;
        }
        _15(dir[i].attr & 0x08) { has_lfn = _86; _101; }
        /// ==========================================
        /// VERGLEICH MIT DEINEM ZIELNAMEN
        /// ==========================================
        _44 match = _86;
        _15(has_lfn) {
            /// Check gegen den langen Namen
            match = _128;
            _39(_43 c = 0; c < 255; c++) {
                /// Beide Strings sind zu Ende? -> Treffer!
                _15(current_lfn[c] EQ 0 AND target_filename[c] EQ 0) _37; 
                /// Ein Buchstabe weicht ab? -> Kein Treffer!
                _15(current_lfn[c] NEQ target_filename[c]) { match = _86; _37; }
            }
        } _41 {
            /// Check gegen das alte 8.3 Format (z.B. "KERNEL  BIN")
            match = _128;
            _39(_43 c = 0; c < 11; c++) {
                _15(dir[i].name[c] NEQ target_filename[c]) { match = _86; _37; }
            }
        }
        _15(match) {
            target_cluster = (dir[i].cluster_high << 16) | dir[i].cluster_low;
            file_size = dir[i].size;
            _37;
        }
        /// Puffer für nächste Datei resetten
        has_lfn = _86;
        _39(_43 l=0; l<256; l++) current_lfn[l] = 0;
    }
    _15(target_cluster EQ 0) {
        str_cpy(cmd_status, "FAT32 LOAD: FILE NOT FOUND"); _96;
    }
    /// ==========================================
    /// SCHRITT 2: FAT-KETTE FOLGEN (DATEI LADEN)
    /// ==========================================
    _43 current_cluster = target_cluster;
    _89 current_load_ptr = file_load_addr;
    _43 clusters_read = 0;
    _114(current_cluster < 0x0FFFFFF8) { 
        _43 cluster_lba = data_start + ((current_cluster - 2) * bpb->sectors_per_cluster);
        _15(!ahci_read_sectors(port_no, cluster_lba, bpb->sectors_per_cluster, current_load_ptr)) {
            str_cpy(cmd_status, "FAT32 LOAD: DATA READ FAILED"); _96;
        }
        current_load_ptr += (bpb->sectors_per_cluster * 512); 
        clusters_read++;
        _43 fat_sector = fat_start + ((current_cluster * 4) / 512);
        _43 fat_offset = (current_cluster * 4) % 512;
        _15(!ahci_read_sectors(port_no, fat_sector, 1, fat_buffer)) {
            str_cpy(cmd_status, "FAT32 LOAD: FAT READ FAILED"); _96;
        }
        _43* fat_entries = (_43*)fat_buffer;
        current_cluster = fat_entries[fat_offset / 4] & 0x0FFFFFFF;
        _15(current_cluster EQ 0 OR clusters_read > 10000) {
            str_cpy(cmd_status, "FAT32 LOAD: FAT CHAIN BROKEN!"); _96;
        }
    }
    /// Meldung mit dem gesuchten Namen ausgeben
    str_cpy(cmd_status, "FILE LOADED TO 0x1000000!");
}
/// ==========================================
/// 3. DER FESTPLATTEN-SCANNER (INTEL Q67 EDITION)
/// ==========================================
extern "C" _50 ahci_mount_drive() {
    /// Wir löschen nicht mehr pauschal alles. Wir behalten alle Laufwerke,
    /// die KEINE SATA/AHCI-Platten sind (z.B. USB-Sticks mit Typ != 2).
    _43 retained_count = 0;
    _39(_43 d = 0; d < drive_count; d++) {
        _15(drives[d].type NEQ 2) { 
            drives[retained_count] = drives[d];
            retained_count++;
        }
    }
    drive_count = retained_count;
    _43 found = 0;
    _15(global_ahci_abar EQ 0) _96;
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    abar[1] = abar[1] | 0x80000000; 
    abar[1] = abar[1] | 2;
    _89 pi = abar[3];
    _39(_43 i = 0; i < 32; i++) {
        _15(pi & (1 << i)) {
            _192 _89* port_regs = abar + 64 + (i * 32);
            /// 1. Port GANZ sicher stoppen
            port_regs[6] &= ~0x0001; 
            _39(_192 _43 w=0; w<500000; w++) { _15(!(port_regs[6] & 0x8000)) _37; }
            port_regs[6] &= ~0x0010; 
            _39(_192 _43 w=0; w<500000; w++) { _15(!(port_regs[6] & 0x4000)) _37; }
            /// 2. COMRESET
            port_regs[11] = 1; 
            _39(_192 _43 w=0; w<500000; w++) {} 
            port_regs[11] = 0;
            _44 link_up = _86;
            _39(_192 _43 w=0; w<1000000; w++) {
                _15((port_regs[10] & 0x0F) EQ 3) { link_up = _128; _37; }
            }
            _15(!link_up) _101;
            /// 3. Fehler löschen & Adressen setzen
            port_regs[12] = 0xFFFFFFFF;
            port_regs[4]  = 0xFFFFFFFF;
            _43 p_off = i * 0x10000; 
            _43 cmd_list_addr = 0x400000 + p_off;  
            _43 fis_base_addr = 0x401000 + p_off; 
            _43 cmd_table_addr = 0x402000 + p_off; 
            _43 data_buffer_addr = 0x403000 + p_off;
            port_regs[0] = cmd_list_addr; port_regs[1] = 0;                 
            port_regs[2] = fis_base_addr; port_regs[3] = 0;
            /// 4. RICHTIGE STARTSEQUENZ FÜR INTEL!
            port_regs[6] |= 0x0010;
            /// WICHTIG: Intel verlangt zwingend, dass wir warten, bis BSY (0x80) und DRQ (0x08) aus sind, BEVOR wir ST anmachen!
            _43 wait_sig = 1000000;
            _114((port_regs[8] & 0x88) AND wait_sig > 0) wait_sig--;
            port_regs[6] |= 0x0001;
            _89 sig = port_regs[9];
            _192 _89* bptr = (_192 _89*)data_buffer_addr;
            _39(_43 b=0; b<512; b++) bptr[b] = 0;
            _192 _89* cmd_header = (_192 _89*)cmd_list_addr;
            cmd_header[0] = 5 | (1 << 16) | 0x400; 
            cmd_header[1] = 0; cmd_header[2] = cmd_table_addr; cmd_header[3] = 0;
            _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
            _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
            _89 identify_cmd = 0xEC; 
            _15(sig EQ 0xEB140101) identify_cmd = 0xA1;
            cmd_table[0] = (identify_cmd << 16) | 0x00008027;
            _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
            prdt[0] = data_buffer_addr; prdt[1] = 0; prdt[2] = 0; 
            prdt[3] = 511 | 0x80000000;
            /// CACHE FLUSH VOR DEM BEFEHL!
            __asm__ _192("wbinvd" ::: "memory");
            port_regs[14] = 1; 
            _43 timeout = 1000000;
            _114((port_regs[14] & 1) AND timeout > 0) timeout--;
            /// CACHE FLUSH NACH DEM BEFEHL!
            __asm__ _192("wbinvd" ::: "memory");
            _15(timeout > 0 AND !(port_regs[4] & 0x40000000)) {
                 _192 _184* byte_buf = (_192 _184*)data_buffer_addr;
                 _192 _182* word_buf = (_192 _182*)data_buffer_addr;
                 _43 real_size_mb = 0;
                 _15(sig NEQ 0xEB140101) {
                     _43 sectors = ((_43)word_buf[101] << 16) | (_43)word_buf[100]; 
                     _15(sectors EQ 0) { sectors = ((_43)word_buf[61] << 16) | (_43)word_buf[60]; }
                     real_size_mb = sectors >> 11;
                 } _41 {
                     real_size_mb = 1024; 
                 }
                 _30 model[80];
                 _43 m_idx = 0;
                 _39(_43 k = 0; k < 40; k += 2) {
                     model[m_idx++] = byte_buf[54 + k + 1]; 
                     model[m_idx++] = byte_buf[54 + k];     
                 }
                 model[m_idx] = 0;
                 _43 last_char = 39;
                 _114(last_char > 0 AND (model[last_char] EQ ' ' OR model[last_char] EQ 0)) {
                     model[last_char] = 0; last_char--;
                 }
                 _15(model[0] EQ 0) str_cpy(model, "UNKNOWN DRIVE");
                 _15(sig NEQ 0xEB140101) { 
                     _39(_43 b=0; b<512; b++) bptr[b] = 0; 
                     _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
                     cmd_table[0] = 0x00258027; cmd_table[1] = 0x40000000; cmd_table[2] = 0; cmd_table[3] = 1; 
                     prdt[0] = data_buffer_addr; prdt[1] = 0; prdt[2] = 0; prdt[3] = 511 | 0x80000000;
                     __asm__ _192("wbinvd" ::: "memory"); /// VORHER FLUSHEN
                     port_regs[12] = 0xFFFFFFFF; port_regs[14] = 1;
                     timeout = 1000000; 
                     _114((port_regs[14] & 1) AND timeout > 0) timeout--;
                     __asm__ _192("wbinvd" ::: "memory");
                     _15(timeout > 0 AND !(port_regs[4] & 0x40000000)) {
                         _15(byte_buf[510] EQ 0x55 AND byte_buf[511] EQ 0xAA) {
                             _30 fs_string[15]; str_cpy(fs_string, " [RAW]"); 
                             _15(byte_buf[82] EQ 'F' AND byte_buf[83] EQ 'A' AND byte_buf[84] EQ 'T' AND byte_buf[85] EQ '3') str_cpy(fs_string, " [FAT32]");
                             _41 _15(byte_buf[3] EQ 'C' AND byte_buf[4] EQ 'F' AND byte_buf[5] EQ 'S') {
                                 str_cpy(fs_string, " [CFS]");
                             }
                             _43 m_len = 0; _114(model[m_len] NEQ 0) m_len++;
                             _43 fs_len = 0; _114(fs_string[fs_len] NEQ 0) { model[m_len++] = fs_string[fs_len++]; }
                             model[m_len] = 0;
                         }
					 }
                 } _41 {
                     _43 m_len = 0; _114(model[m_len] NEQ 0) m_len++;
                     _30 cd_str[] = " [ISO9660]";
                     _43 c_idx = 0; _114(cd_str[c_idx] NEQ 0) { model[m_len++] = cd_str[c_idx++]; }
                     model[m_len] = 0;
                 }
                 _15(drive_count < 8) {
                     drives[drive_count].type = 2; 
                     drives[drive_count].size_mb = real_size_mb; 
                     drives[drive_count].base_port = i; 
                     str_cpy(drives[drive_count].model, model);
                     drive_count++; 
                     found++;
                 }
            }
        }
    }
    _15(found > 0) str_cpy(cmd_status, "AHCI: MULTI DRIVES + FS SCANNED!");
    _41 str_cpy(cmd_status, "AHCI: NO DRIVES FOUND");
}
/// ==========================================
/// BARE METAL FIX: SEKTOR 0 DATEISYSTEM ERKENNUNG
/// ==========================================
_50 ahci_read_mbr() {
    _15(drive_count EQ 0) {
        str_cpy(cmd_status, "ERROR: NO DRIVES MOUNTED");
        _96;
    }
    _15(global_ahci_abar EQ 0) {
        str_cpy(cmd_status, "ERROR: ABAR IS NULL");
        _96;
    }
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    /// Wir rastern jetzt alle Platten ab, die wir im Disk Manager (drives[]) haben!
    _39(_43 d = 0; d < drive_count; d++) {
        _43 port_no = drives[d].base_port;
        _192 _89* port_regs = abar + 64 + (port_no * 32);
        /// ==========================================
        /// 1. BRIEFKASTEN FÜR READ DMA EXT (0x25)
        /// Wir nutzen für jeden Port seinen EIGENEN 4MB Offset,
        /// damit sich die gelesenen Sektoren nicht spiegeln!
        /// ==========================================
        _43 cmd_list_addr = 0x400000 + (port_no * 1024);  
        _43 fis_base_addr = 0x408000 + (port_no * 256); 
        _43 cmd_table_addr = 0x410000 + (port_no * 8192); 
        _43 data_buffer_addr = 0x500000 + (port_no * 512); /// Puffer für Sektor 0 bei 5 MB!
        /// Puffer blitzblank putzen
        _89* bptr = (_89*)data_buffer_addr;
        _39(_43 i=0; i<128; i++) bptr[i] = 0;
        port_regs[0] = cmd_list_addr;     
        port_regs[1] = 0;                 
        port_regs[2] = fis_base_addr;     
        port_regs[3] = 0;
        _192 _89* cmd_header = (_192 _89*)cmd_list_addr;
        cmd_header[0] = 5 | (1 << 16) | 0x400; /// 5 Dwords, Read (Bit 16 aus), 1 PRDT Entry
        cmd_header[1] = 0; 
        cmd_header[2] = cmd_table_addr; 
        cmd_header[3] = 0;
        _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
        /// BARE METAL FIX: Type 0x27 (H2D FIS), Command 0x25 (READ DMA EXT)
        cmd_table[0] = 0x00258027; 
        cmd_table[1] = 0x40000000; /// LBA 0 (Sektor 0), LBA Mode an (Bit 6 in Dev-Reg)
        cmd_table[2] = 0x00000000; /// LBA High
        cmd_table[3] = 0x00000001; /// Sektor Count: Genau 1 Sektor!
        _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
        prdt[0] = data_buffer_addr; 
        prdt[1] = 0;
        prdt[2] = 0;
        prdt[3] = 511;
        _43 timeout = 1000000;
        _114((port_regs[8] & 0x88) AND timeout > 0) timeout--;
        port_regs[12] = 1;
        timeout = 1000000;
        _114((port_regs[12] & 1) AND timeout > 0) {
             _15(port_regs[13] & 0x40000000) _37; /// Read Error
             timeout--;
        }
        /// ==========================================
        /// 2. SEKTOR 0 AUSWERTEN (_192 Puffer!)
        /// ==========================================
        _15(timeout > 0 AND !(port_regs[13] & 0x40000000)) {
            _192 _184* byte_buf = (_192 _184*)data_buffer_addr;
            
            /// Ist es überhaupt ein gültiger Bootsektor? (Byte 510 und 511 müssen 55 AA sein)
            _15(byte_buf[510] EQ 0x55 AND byte_buf[511] EQ 0xAA) {
                /// BARE METAL FIX: Puffer auf 15 vergrößert für längere Labels!
                _30 fs_string[15];
                str_cpy(fs_string, "[RAW]");
                
                /// ----------------------------------------------------
                /// CHECK 1: SUPER-FLOPPY (Dateisystem direkt auf LBA 0)
                /// ----------------------------------------------------
                _15(byte_buf[82] EQ 'F' AND byte_buf[83] EQ 'A' AND byte_buf[84] EQ 'T' AND byte_buf[85] EQ '3' AND byte_buf[86] EQ '2') {
                    str_cpy(fs_string, "[FAT32]");
                }
                _41 _15(byte_buf[3] EQ 'E' AND byte_buf[4] EQ 'X' AND byte_buf[5] EQ 'F' AND byte_buf[6] EQ 'A' AND byte_buf[7] EQ 'T') {
                    str_cpy(fs_string, "[exFAT]");
                }
                _41 _15(byte_buf[3] EQ 'N' AND byte_buf[4] EQ 'T' AND byte_buf[5] EQ 'F' AND byte_buf[6] EQ 'S') {
                    str_cpy(fs_string, "[NTFS]");
                }
                _41 _15(byte_buf[3] EQ 'C' AND byte_buf[4] EQ 'F' AND byte_buf[5] EQ 'S') {
                    str_cpy(fs_string, "[CFS]");
                }
                /// ----------------------------------------------------
                /// CHECK 2: PARTITIONIERT (MBR oder GPT auf LBA 0)
                /// ----------------------------------------------------
                _41 {
                    /// Wir lesen den Typ der ersten Partition aus der MBR-Tabelle!
                    _184 part_type = byte_buf[446 + 4];
                    
                    _15(part_type EQ 0xEE) {
                        str_cpy(fs_string, "[GPT]");
                    } _41 _15(part_type EQ 0x07) {
                        str_cpy(fs_string, "[NTFS]");
                    } _41 _15(part_type EQ 0x0B OR part_type EQ 0x0C) {
                        str_cpy(fs_string, "[FAT32]");
                    } _41 _15(part_type EQ 0x83) {
                        str_cpy(fs_string, "[EXT]");
                    } _41 _15(part_type NEQ 0x00) {
                        str_cpy(fs_string, "[MBR]");
                    }
                }

                /// Den FS-String an das Modell anhängen
                _43 m_len = 0;
                _114(drives[d].model[m_len] NEQ 0 AND m_len < 30) m_len++;
                
                _43 fs_len = 0;
                _114(fs_string[fs_len] NEQ 0) {
                    drives[d].model[m_len++] = fs_string[fs_len++];
                }
                drives[d].model[m_len] = 0;
                str_cpy(cmd_status, "FS SCANNED!");
            }
        }
	}
}
_50 ahci_panzer_scan_tick() {
    _15(ahci_scan_state EQ 0 OR global_ahci_abar EQ 0) _96;
    HBA_MEM *hba = (HBA_MEM*)global_ahci_abar;
    hba->ghc |= (1 << 31); 
    _89 pi = hba->pi;
    _15(ahci_scan_state EQ 1) {
        _39(_43 i = 0; i < 32; i++) {
            _15(pi & (1 << i)) {
                HBA_PORT *port = &hba->ports[i];
                port->cmd &= ~1; port->cmd &= ~(1<<4);
                port->cmd |= (1<<1) | (1<<2); port->cmd = (port->cmd & 0x0FFFFFFF) | (1<<28);
                port->sctl = (port->sctl & 0xFFFFF0F0) | 0x301; 
            }
        }
        ahci_scan_state = 2; ahci_scan_timer = 60; 
    }
    _41 _15(ahci_scan_state EQ 2) {
        _15(ahci_scan_timer > 0) { ahci_scan_timer--; _96; }
        _39(_43 i = 0; i < 32; i++) {
            _15(pi & (1 << i)) {
                HBA_PORT *port = &hba->ports[i];
                port->sctl = (port->sctl & 0xFFFFFFF0); port->serr = 0xFFFFFFFF; 
            }
        }
        ahci_scan_state = 3; ahci_scan_timer = 300; 
    }
    _41 _15(ahci_scan_state EQ 3) {
        _15(ahci_scan_timer > 0) { ahci_scan_timer--; _96; }
        _39(_43 i = 0; i < 32; i++) {
            _15(pi & (1 << i)) {
                HBA_PORT *port = &hba->ports[i];
                _89 det = port->ssts & 0x0F;        
                _15(det EQ 3) {
                    _43 wait_rdy = 0; _114((port->tfd & 0x88) NEQ 0 AND wait_rdy < 1000000) wait_rdy++; 
                    _15(port->sig NEQ 0xEB140101) {
                        ahci_port_rebase(port, i);
                        _15(global_active_port EQ 0) global_active_port = port;
                        /// MULTI-DRIVE ANMELDUNG FÜR QEMU!
                        ahci_found_ports_bitmask |= (1 << i); 
                    }
                }
            }
        }
        ahci_scan_state = 0; 
    }
}
HBA_PORT* get_active_ahci_port() {
    _96 global_active_port; 
}
_44 ahci_read(HBA_PORT *port, _89 startlba, _50 *target_ram_address) {
    port->is = (_89)-1;
    port->serr = (_89)-1; /// BARE METAL FIX: SERR (Fehler-Sperre) löschen!
    
    _89 slot = 0;
    _89 slots = (port->sact | port->ci);
    _39 (_43 i=0; i<32; i++) {
        _15 ((slots & (1 << i)) == 0) { slot = i; _37; }
    }
    _15 (slot == 32) _96 0;
    
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
    cmdheader += slot;
    cmdheader->cfl = _64(FIS_REG_H2D)/_64(_89); 
    cmdheader->w = 0; /// BARE METAL FIX: Lesen ist 0!
    cmdheader->prdtl = 1;
    
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
    _39 (_43 i=0; i<_64(HBA_CMD_TBL) + (cmdheader->prdtl-1)*_64(HBA_PRDT_ENTRY); i++) {
        ((_184*)cmdtbl)[i] = 0;
    }
    
    /// BARE METAL FIX: Adresse sicher auf 64-Bit aufteilen!
    cmdtbl->prdt_entry[0].dba = (_89)(uint64_t)target_ram_address;
    cmdtbl->prdt_entry[0].dbau = (_89)(((uint64_t)target_ram_address) >> 32); 
    cmdtbl->prdt_entry[0].dbc = 511 | (1 << 31);
    
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
    cmdfis->fis_type = 0x27; 
    cmdfis->c = 1;           
    cmdfis->command = 0x25; /// BARE METAL FIX: 0x25 ist READ DMA EXT! (Stand auf 0x35 Write!)
    cmdfis->lba0 = (_184)startlba;
    cmdfis->lba1 = (_184)(startlba >> 8);
    cmdfis->lba2 = (_184)(startlba >> 16);
    cmdfis->device = 1<<6;   
    cmdfis->lba3 = (_184)(startlba >> 24);
    cmdfis->lba4 = 0;        
    cmdfis->lba5 = 0;
    cmdfis->countl = 1;      
    cmdfis->counth = 0;
    
    _43 spin = 0;
    _114 ((port->tfd & (0x80 | 0x08)) && spin < 1000000) spin++;
    _15 (spin >= 1000000) _96 0;
    
    /// BARE METAL FIX: Cache leeren, BEVOR der Controller liest!
    __asm__ _192("wbinvd" ::: "memory");
    
    port->ci = 1 << slot;
    
    _43 wait_spin = 0;
    _114 (wait_spin < 10000000) { /// Timeout massiv erhöht
        _15 ((port->ci & (1 << slot)) == 0) _37;
        _15 (port->is & (1<<30)) _96 0;
        wait_spin++;
    }
    
    /// BARE METAL FIX: Cache leeren, NACHDEM gelesen wurde!
    __asm__ _192("wbinvd" ::: "memory");
    
    _15 (wait_spin >= 10000000) _96 0;
    _96 1; 
}

_44 ahci_write(HBA_PORT *port, _89 startlba, _50 *source_ram_address) {
    port->is = (_89)-1;
    port->serr = (_89)-1; 
    
    _89 slot = 0;
    _89 slots = (port->sact | port->ci);
    _39 (_43 i=0; i<32; i++) {
        _15 ((slots & (1 << i)) == 0) { slot = i; _37; }
    }
    _15 (slot == 32) _96 0;
    
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
    cmdheader += slot;
    cmdheader->cfl = _64(FIS_REG_H2D)/_64(_89); 
    cmdheader->w = 1; /// BARE METAL FIX: Schreiben ist 1!
    cmdheader->prdtl = 1;
    
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
    _39 (_43 i=0; i<_64(HBA_CMD_TBL) + (cmdheader->prdtl-1)*_64(HBA_PRDT_ENTRY); i++) {
        ((_184*)cmdtbl)[i] = 0;
    }
    
    /// BARE METAL FIX: Adresse sicher auf 64-Bit aufteilen!
    cmdtbl->prdt_entry[0].dba = (_89)(uint64_t)source_ram_address;
    cmdtbl->prdt_entry[0].dbau = (_89)(((uint64_t)source_ram_address) >> 32); 
    cmdtbl->prdt_entry[0].dbc = 511 | (1 << 31);
    
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
    cmdfis->fis_type = 0x27; 
    cmdfis->c = 1;           
    cmdfis->command = 0x35;  /// 0x35 ist WRITE DMA EXT
    cmdfis->lba0 = (_184)startlba;
    cmdfis->lba1 = (_184)(startlba >> 8);
    cmdfis->lba2 = (_184)(startlba >> 16);
    cmdfis->device = 1<<6;   
    cmdfis->lba3 = (_184)(startlba >> 24);
    cmdfis->lba4 = 0;        
    cmdfis->lba5 = 0;
    cmdfis->countl = 1;      
    cmdfis->counth = 0;

    _43 spin = 0;
    _114 ((port->tfd & (0x80 | 0x08)) && spin < 1000000) spin++;
    _15 (spin >= 1000000) _96 0; 
    
    __asm__ _192("wbinvd" ::: "memory");
    
    port->ci = 1 << slot;
    
    _43 wait_spin = 0;
    _114 (wait_spin < 10000000) {
        _15 ((port->ci & (1 << slot)) == 0) _37; 
        _15 (port->is & (1<<30)) _96 0;      
        wait_spin++;
    }
    
    __asm__ _192("wbinvd" ::: "memory");
    
    _15 (wait_spin >= 10000000) _96 0;
    _96 1; 
}
/// ==========================================
/// BARE METAL FIX: RAW IDENTIFY READ (Für die CMD)
/// ==========================================
_50 ahci_get_raw_identify(_43 port_no, _182* out_buffer) {
    _192 _89* abar = (_192 _89*)global_ahci_abar;
    _192 _89* port_regs = abar + 64 + (port_no * 32);
    _43 p_off = port_no * 0x10000; 
    _43 cmd_list_addr = 0x400000 + p_off;  
    _43 cmd_table_addr = 0x402000 + p_off; 
    _43 data_buffer_addr = 0x403000 + p_off;
    _192 _89* cmd_table = (_192 _89*)cmd_table_addr;
    _39(_43 j=0; j<32; j++) cmd_table[j] = 0;
    _89 sig = port_regs[9]; 
    _89 identify_cmd = 0xEC; 
    _15(sig EQ 0xEB140101) identify_cmd = 0xA1;
    cmd_table[0] = (identify_cmd << 16) | 0x00008027;
    _192 _89* prdt = (_192 _89*)(cmd_table_addr + 128);
    prdt[0] = data_buffer_addr; prdt[1] = 0; prdt[2] = 0; 
    prdt[3] = 511 | 0x80000000;
    __asm__ _192("wbinvd" ::: "memory");
    port_regs[12] = 0xFFFFFFFF; 
    port_regs[14] = 1;
    _43 timeout = 1000000;
    _114((port_regs[14] & 1) AND timeout > 0) timeout--;
    __asm__ _192("wbinvd" ::: "memory");
    _192 _182* word_buf = (_192 _182*)data_buffer_addr;
    _39(_43 i=0; i<256; i++) {
        out_buffer[i] = word_buf[i];
    }
}
_44 ahci_read_multi(HBA_PORT *port, _89 startlba, _43 count, _50 *target_ram_address) {
    port->is = (_89)-1;
    port->serr = (_89)-1; 
    
    _89 slot = 0;
    _89 slots = (port->sact | port->ci);
    _39 (_43 i=0; i<32; i++) {
        _15 ((slots & (1 << i)) == 0) { slot = i; _37; }
    }
    _15 (slot == 32) _96 0;
    
    // ==========================================
    // BARE METAL FIX 1: 64-BIT POINTER FÜR CLB
    // Wir rekonstruieren die volle Hardware-Adresse!
    // ==========================================
    uint64_t clb_addr = port->clb;
    clb_addr |= ((uint64_t)port->clbu << 32);
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)clb_addr;
    
    cmdheader += slot;
    cmdheader->cfl = _64(FIS_REG_H2D)/_64(_89); 
    cmdheader->w = 0; 
    cmdheader->prdtl = 1;
    
    // ==========================================
    // BARE METAL FIX 2: 64-BIT POINTER FÜR CTBA
    // ==========================================
    uint64_t ctba_addr = cmdheader->ctba;
    ctba_addr |= ((uint64_t)cmdheader->ctbau << 32);
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)ctba_addr;
    
    _39 (_43 i=0; i<_64(HBA_CMD_TBL) + (cmdheader->prdtl-1)*_64(HBA_PRDT_ENTRY); i++) {
        ((_184*)cmdtbl)[i] = 0;
    }
    
    cmdtbl->prdt_entry[0].dba = (_89)(uint64_t)target_ram_address;
    cmdtbl->prdt_entry[0].dbau = (_89)(((uint64_t)target_ram_address) >> 32); 
    cmdtbl->prdt_entry[0].dbc = ((count * 512) - 1) | (1 << 31);
    
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
    cmdfis->fis_type = 0x27; 
    cmdfis->c = 1;           
    cmdfis->command = 0x25; 
    cmdfis->lba0 = (_184)startlba;
    cmdfis->lba1 = (_184)(startlba >> 8);
    cmdfis->lba2 = (_184)(startlba >> 16);
    cmdfis->device = 1<<6;   
    cmdfis->lba3 = (_184)(startlba >> 24);
    cmdfis->lba4 = 0;        
    cmdfis->lba5 = 0;
    cmdfis->countl = count & 0xFF;      
    cmdfis->counth = (count >> 8) & 0xFF;
    
    _43 spin = 0;
    // ==========================================
    // BARE METAL FIX 3: TIMEOUT 100x VERGRÖSSERN
    // Wir geben der Hardware die Zeit, die sie braucht!
    // (10 Millionen -> 1 Milliarde)
    // ==========================================
    _114 ((port->tfd & (0x80 | 0x08)) && spin < 1000000000) { 
        __asm__ _192("pause"); 
        spin++; 
    }
    _15 (spin >= 1000000000) _96 0;
    
    __asm__ _192("wbinvd" ::: "memory");
    
    port->ci = 1 << slot;
    
    _43 wait_spin = 0;
    // ==========================================
    // BARE METAL FIX 4: LESE-TIMEOUT VERGRÖSSERN
    // ==========================================
    _114 (wait_spin < 1000000000) {
        __asm__ _192("pause");
        _15 ((port->ci & (1 << slot)) == 0) _37;
        _15 (port->is & (1<<30)) _96 0;
        wait_spin++;
    }
    
   __asm__ _192("wbinvd" ::: "memory");
    
    _15 (wait_spin >= 1000000000) _96 0;
    
    // ==========================================
    // BARE METAL FIX: DEN CONTROLLER WIEDER FREIGEBEN!
    // Löscht alle Interrupt-Flags, damit der nächste 
    // Lesebefehl nicht blockiert wird!
    // ==========================================
    port->is = (_89)-1; 
    
    _96 1; 
}