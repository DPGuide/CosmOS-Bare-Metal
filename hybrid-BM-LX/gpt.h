// gpt.h - Die Brücke zur modernen Welt
struct GPTHeader {
    uint64_t signature;       // Muss "EFI PART" sein
    uint32_t revision;
    uint32_t headerSize;
    uint32_t headerCRC32;
    uint32_t reserved;
    uint64_t myLBA;
    uint64_t alternateLBA;
    uint64_t firstUsableLBA;
    uint64_t lastUsableLBA;
    uint8_t  diskGUID[16];
    uint64_t partitionEntryLBA; // Hier fängt die Liste an (meist LBA 2)
    uint32_t numPartitionEntries;
    uint32_t partitionEntrySize; // Meist 128 Bytes
};

struct GPTPartitionEntry {
    uint8_t  partitionTypeGUID[16];
    uint8_t  uniquePartitionGUID[16];
    uint64_t startingLBA;       // DER wichtigste Wert!
    uint64_t endingLBA;
    uint64_t attributes;
    uint16_t partitionName[36]; // UTF-16
};