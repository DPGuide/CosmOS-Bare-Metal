#ifndef COSMOS_CFS_H
#define COSMOS_CFS_H

#include <stdint.h>
#include "cosmos_ahci.h" 
#include "schneider_lang.h"

#pragma pack(push, 1) 

_81 _202 {
    _89 magic_signature; 
    _89 version;         
    _89 sector_size;     
    _89 sectors_per_cluster; 
    
    _94 total_sectors;   
    _94 bitmap_lba;      
    _94 root_dir_lba;    
    _94 data_start_lba;  
    
    _184  volume_name[32]; 
    _184  reserved[432];   
} CFS_SUPERBLOCK;

_81 _202 {
    _184  type;            
    _184  flags;           
    _30     filename[46];    
    
    _94 start_lba;       
    _94 file_size;       
} CFS_DIR_ENTRY;

#pragma pack(pop)

// Funktions-Deklarationen für den Rest des Kernels
_172 _50 cfs_finalize_file(HBA_PORT* port, _71 _30* filename, _89 start_lba, _89 total_bytes);
_172 _89 cfs_read_file(HBA_PORT* port, _71 _30* target_filename, void* ram_destination);
_172 _50 cfs_format_drive(HBA_PORT* port, _94 total_sectors);

#endif