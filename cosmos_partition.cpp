#include "cosmos_partition.h"
#include "cosmos_ahci.h"

/// BARE METAL FIX: Die exakte Brücke in Schneider-Lang!
extern _44 ahci_read_sectors(_43 port_no, _43 lba, _43 count, _89 buffer_addr);

/// Die Speicherplätze für unsere Partitionen
uint64_t gpt_partition_starts[16];
int gpt_partition_count = 0;

extern "C" void scan_partitions(uint8_t port_no) {
    gpt_partition_count = 0;
    uint8_t* buffer = (uint8_t*)0x06000000; 
    
    /// ====================================================
    /// VERSUCH 1: GPT (LBA 1)
    /// ====================================================
    if (ahci_read_sectors(port_no, 1, 1, (uint64_t)buffer)) {
        GPTHeader* gpt = (GPTHeader*)buffer;
        
        if (gpt->signature == 0x5452415020494645) { /// "EFI PART"
            uint64_t table_lba = gpt->partitionEntryLBA;
            if (ahci_read_sectors(port_no, table_lba, 4, (uint64_t)buffer)) {
                GPTPartitionEntry* entries = (GPTPartitionEntry*)buffer;
                for (uint32_t i = 0; i < 16; i++) {
                    if (entries[i].startingLBA != 0) {
                        gpt_partition_starts[gpt_partition_count] = entries[i].startingLBA;
                        gpt_partition_count++;
                    }
                }
            }
            return; /// Wenn GPT gefunden, sind wir fertig!
        }
    }

    /// ====================================================
    /// VERSUCH 2: FALLBACK ZU MBR (LBA 0)
    /// ====================================================
    if (ahci_read_sectors(port_no, 0, 1, (uint64_t)buffer)) {
        /// Ein MBR endet IMMER mit der Signatur 0x55AA an Byte 510 und 511
        if (buffer[510] == 0x55 && buffer[511] == 0xAA) {
            
            /// Die MBR-Partitionstabelle hat 4 Einträge à 16 Bytes und startet bei Offset 446
            uint8_t* pTbl = buffer + 446;
            
            for (int i = 0; i < 4; i++) {
                uint8_t type = pTbl[(i * 16) + 4];
                uint32_t start_lba = *(uint32_t*)(pTbl + (i * 16) + 8);
                
                /// Typ 0x07 ist NTFS / exFAT in der MBR Welt! Typ 0x00 ist leer.
                if (type != 0x00 && start_lba != 0) {
                    gpt_partition_starts[gpt_partition_count] = start_lba;
                    gpt_partition_count++;
                }
            }
        }
    }
}