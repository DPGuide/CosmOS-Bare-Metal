#include "cosmos_cfs.h"
#include "cosmos_ahci.h"
#include "schneider_lang.h"
/// Externe Hilfsfunktion (String Compare), die dein OS sicher schon hat
_172 _44 str_cmp(_71 _30* str1, _71 _30* str2);
_172 _50 str_cpy(_30* dest, _71 _30* src);
/// =========================================================
/// 1. DATEI VERSIEGELN (Speichern ins Inhaltsverzeichnis)
/// =========================================================
_50 cfs_finalize_file(HBA_PORT* port, _71 _30* filename, _89 start_lba, _89 total_bytes) {
    /// BARE METAL FIX: Feste RAM Adresse statt Stack-Variable!
    _184* root_dir_sector = (_184*)0x805000;
    _89 root_lba = 1002;
    _15(!ahci_read(port, root_lba, root_dir_sector)) _96;
    CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)root_dir_sector;
    _43 free_slot = -1;
    _39(_43 i = 0; i <28; i++) {
        _15(entries[i].type EQ 0) { 
            free_slot = i;
            _37; 
        }
    }
    _15(free_slot EQ -1) _96;
    entries[free_slot].type = 1;      
    entries[free_slot].flags = 0;     
    str_cpy(entries[free_slot].filename, filename);
    entries[free_slot].start_lba = start_lba;
    entries[free_slot].file_size = total_bytes;
    ahci_write(port, root_lba, root_dir_sector);
}
/// =========================================================
/// 2. DATEI LADEN (Suchen und in den RAM holen)
/// =========================================================
_89 cfs_read_file(HBA_PORT* port, _71 _30* target_filename, _50* ram_destination) {
    /// BARE METAL FIX: Feste RAM Adresse statt Stack-Variable!
    _184* root_dir_sector = (_184*)0x805000;
    _89 root_lba = 1002;
    /// 1. Inhaltsverzeichnis laden
    _15(!ahci_read(port, root_lba, root_dir_sector)) _96 0;
    CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)root_dir_sector;
    /// 2. Datei suchen
    _43 found_slot = -1;
    _39(_43 i = 0; i < 28; i++) {
        _15(entries[i].type EQ 1) {
            _15(str_cmp(entries[i].filename, target_filename) EQ 1) {
                found_slot = i;
                _37;
            }
        }
    }
    _15(found_slot EQ -1) _96 0;
    /// 3. Daten auslesen
    _89 target_lba = entries[found_slot].start_lba;
    _89 file_size = entries[found_slot].file_size;
    /// Berechnen, wie viele 512-Byte Sektoren wir lesen müssen
    _89 sectors_to_read = (file_size + 511) / 512;
    /// 4. Die Sektoren von der Platte direkt in die Ziel-Speicheradresse kopieren!
    _89 current_ram_offset = 0;
    _39(_89 i = 0; i < sectors_to_read; i++) {
        /// Wir nutzen Pointer-Arithmetik, um den RAM-Zeiger pro Sektor um 512 Bytes zu verschieben
        _184* current_dest = (_184*)ram_destination + current_ram_offset;
        ahci_read(port, target_lba + i, current_dest);
        current_ram_offset += 512;
    }
    _96 file_size;
}
/// =========================================================
/// 3. LAUFWERK FORMATIEREN (CFS SUPERBLOCK AUF SEKTOR 0)
/// =========================================================
_172 _30 cmd_status[32]; /// UI Status Text aus kernel.cpp
_50 cfs_format_drive(HBA_PORT* port, _94 total_sectors) {
    _15(port EQ 0) {
        str_cpy(cmd_status, "CFS ERR: PORT IS NULL");
        _96;
    }
    /// BARE METAL FIX: Fester, sicherer DMA-Puffer für den AHCI Controller (8 MB Grenze)
    _89 dma_buffer = 0x800000;
    _184* raw_bytes = (_184*)dma_buffer;
    
    /// 1. Puffer komplett nullen (Geisterdaten aus dem RAM vernichten)
    _39(_43 i = 0; i < 512; i++) {
        raw_bytes[i] = 0;
    }
    /// 2. Den Superblock in den sauberen Puffer gießen
    CFS_SUPERBLOCK* sb = (CFS_SUPERBLOCK*)dma_buffer;
    /// BARE METAL FIX: 32-Bit Signatur ("COSM" in Little Endian)
    /// Passt jetzt perfekt in den _89 (uint32_t) Datentyp ohne Overflow!
    sb->magic_signature = 0x4D534F43; 
    sb->version = 1;
    sb->sector_size = 512;
    sb->sectors_per_cluster = 8; /// 4 KB Cluster (8 * 512 Bytes)
    sb->total_sectors = total_sectors;
    /// Das Layout der Partition festlegen
    sb->bitmap_lba = 1;          /// Sektor 1: Free Space Bitmap
    sb->root_dir_lba = 1002;     /// Sektor 1002: Inhaltsverzeichnis (Match zu cfs_finalize_file)
    sb->data_start_lba = 2000;   /// Sektor 2000: Echte Dateidaten
    /// Harter Cast auf _30* (char*), da volume_name ein _184 (uint8_t) Array ist
    str_cpy((_30*)sb->volume_name, "COSMOS_SYS");
    /// 3. Superblock auf Sektor 0 brennen!
    _15(!ahci_write(port, 0, sb)) {
        str_cpy(cmd_status, "CFS ERR: WRITE MBR FAILED");
        _96;
    }
    /// 4. Das Inhaltsverzeichnis (Root Dir) bei LBA 1002 zwingend mit-nullen!
    /// Sonst liest cfs_read_file später alten Müll als angebliche Dateien.
    _39(_43 i = 0; i < 512; i++) {
        raw_bytes[i] = 0;
    }
    _15(!ahci_write(port, sb->root_dir_lba, raw_bytes)) {
        str_cpy(cmd_status, "CFS ERR: WRITE ROOT FAILED");
        _96;
    }
    str_cpy(cmd_status, "CFS: DRIVE FORMATTED!");
}