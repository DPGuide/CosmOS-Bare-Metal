#ifndef COSMOS_PARTITION_H
#define COSMOS_PARTITION_H

#include <stdint.h>
#include "schneider_lang.h"

#pragma pack(push, 1)

/// Der Bauplan für Sektor 1 (GPT Header)
struct GPTHeader {
    uint64_t signature;         /// Muss "EFI PART" sein
    uint32_t revision;
    uint32_t headerSize;
    uint32_t headerCRC32;
    uint32_t reserved;
    uint64_t myLBA;
    uint64_t alternateLBA;
    uint64_t firstUsableLBA;
    uint64_t lastUsableLBA;
    uint8_t  diskGUID[16];
    uint64_t partitionEntryLBA; /// Hier fangen die Partitionen an
    uint32_t numPartitionEntries;
    uint32_t partitionEntrySize;
    uint32_t partitionEntryArrayCRC32;
};

/// Der Bauplan für Sektor 2+ (Die echten Partitionen)
struct GPTPartitionEntry {
    uint8_t  partitionTypeGUID[16];
    uint8_t  uniquePartitionGUID[16];
    uint64_t startingLBA;       /// DER JACKPOT!
    uint64_t endingLBA;
    uint64_t attributes;
    uint16_t partitionName[36]; /// UTF-16 String
};

#pragma pack(pop)

/// Die Brücke zu OS1
extern "C" void scan_partitions(uint8_t port_no);

/// Die Speicherplätze für unsere gefundenen Partitionen (Die Brücke zu OS2)
extern uint64_t gpt_partition_starts[16];
extern int gpt_partition_count;

#endif