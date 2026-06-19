#ifndef COSMOS_FAT32_H
#define COSMOS_FAT32_H

#include <stdint.h>
#include "schneider_lang.h"

_202 FAT32_ParsedFile {
    uint8_t exists;
    char name[64];
    uint32_t size;
    uint32_t start_lba;
    uint8_t is_folder;
    uint8_t parent_idx;
};

_172 _43 fat32_data_start;
_172 _43 fat32_sectors_per_cluster;
_172 _43 fat32_root_lba;

_172 _44 fat32_init(_43 partition_lba, uint8_t* bpb_buffer);
_172 _44 fat32_list_dir(_43 folder_lba, uint8_t* dir_buffer, FAT32_ParsedFile* output_files, _43 max_files, uint8_t current_folder_id, int current_page_offset);

#endif
