#ifndef COSMOS_FS_H
#define COSMOS_FS_H

#include <stdint.h>
#include "schneider_lang.h"
#include "cosmos_bytes.h"

_202 PhysicalDrive { _44 present; _30 model[41]; _89 size_mb; _182 base_port; _44 is_slave; _43 type; } __attribute__((packed));
_202 Partition { _184 status; _184 type; _89 start_lba; _89 size; } __attribute__((packed));
_202 FileEntry { _30 name[32]; _89 sector_offset; _44 is_folder; _89 size; _30 date[16]; _44 exists; _184 parent_idx; _184 padding[5]; } __attribute__((packed));

_172 _43 drive_count;
_172 FileEntry file_table[28];
_172 _43 active_drive_idx;
_172 _30 hw_disk[48];
_172 _43 drive_status;
_172 _184 hdd_buf[512];
_172 _184 sector0[512];
_172 Partition partitions[4];

_172 _50 fs_init();
_172 _50 fs_save();
_172 _50 fs_save_file(_71 _30* name, _89 size);
_172 _50 fs_create_folder(_71 _30* name);
_172 _50 fs_flush_table();
_172 _50 mbr_scan();
_172 _50 ata_scan_drives();
_172 _50 ata_read_sector(_182 base, _44 slave, _89 lba, _184* buffer);
_172 _50 ata_write_sector(_182 base, _44 slave, _89 lba, _184* buffer);
_172 _50 fs_write_cosmos_blocks(_43 drive_idx, _89 start_lba, _184* block_array, _43 num_blocks);
_172 _50 fs_read_sectors(_43 drive_idx, _89 start_lba, _184* block_array, _43 num_blocks);
_172 _43 fs_read_cosmos_blocks(_43 drive_idx, _89 start_lba, _184* block_array, _43 num_blocks);


#endif