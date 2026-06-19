#ifndef PE_LOADER_H
#define PE_LOADER_H

#include <stdint.h>

extern "C" void* malloc(size_t);
// External function that MeinOS must provide to resolve Windows APIs
extern uint64_t resolve_windows_api(const char* dll_name, const char* func_name);

// PE Structures
struct PE_IMAGE_SECTION_HEADER {
    uint8_t  Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

struct PE_IMAGE_IMPORT_DESCRIPTOR {
    uint32_t OriginalFirstThunk;
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
};

static void memcpy_pe(void* dest, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (uint64_t i = 0; i < n; i++) d[i] = s[i];
}

static void memset_pe(void* s, uint8_t c, uint64_t n) {
    uint8_t* p = (uint8_t*)s;
    for (uint64_t i = 0; i < n; i++) p[i] = c;
}

// A barebones PE32/PE32+ Header parser (Inspection only)
static bool parse_and_inspect_pe(uint8_t* pe_data, uint32_t size, uint64_t* out_entry_point, uint16_t* out_machine, uint16_t* out_sections) {
    if (size < 0x40) return false;
    
    if (pe_data[0] != 'M' || pe_data[1] != 'Z') return false;
    
    uint32_t pe_offset = *(uint32_t*)(&pe_data[0x3C]);
    if (pe_offset + 24 >= size) return false;
    
    if (pe_data[pe_offset] != 'P' || pe_data[pe_offset+1] != 'E' || 
        pe_data[pe_offset+2] != 0 || pe_data[pe_offset+3] != 0) {
        return false;
    }
    
    uint16_t machine = *(uint16_t*)(&pe_data[pe_offset + 4]);
    uint16_t num_sections = *(uint16_t*)(&pe_data[pe_offset + 6]);
    uint16_t size_of_opt_header = *(uint16_t*)(&pe_data[pe_offset + 20]);
    
    if (out_machine) *out_machine = machine;
    if (out_sections) *out_sections = num_sections;
    
    if (pe_offset + 24 + size_of_opt_header >= size) return false;
    
    uint16_t magic = *(uint16_t*)(&pe_data[pe_offset + 24]);
    uint32_t entry_point_rva = *(uint32_t*)(&pe_data[pe_offset + 40]);
    uint64_t image_base = 0;
    
    if (magic == 0x010B) { 
        image_base = *(uint32_t*)(&pe_data[pe_offset + 52]);
    } else if (magic == 0x020B) { 
        image_base = *(uint64_t*)(&pe_data[pe_offset + 48]);
    } else {
        return false;
    }
    
    if (out_entry_point) *out_entry_point = image_base + entry_point_rva;
    return true;
}

// FULL PE LOADER: Maps memory and resolves IAT (WINE Core)
static uint64_t load_and_resolve_pe(uint8_t* pe_data, uint32_t size) {
    if (size < 0x40) return 0;
    if (pe_data[0] != 'M' || pe_data[1] != 'Z') return 0;
    
    uint32_t pe_offset = *(uint32_t*)(&pe_data[0x3C]);
    if (pe_offset + 24 >= size) return 0; 
    
    if (pe_data[pe_offset] != 'P' || pe_data[pe_offset+1] != 'E' || 
        pe_data[pe_offset+2] != 0 || pe_data[pe_offset+3] != 0) return 0;
    
    uint16_t num_sections = *(uint16_t*)(&pe_data[pe_offset + 6]);
    uint16_t size_of_opt_header = *(uint16_t*)(&pe_data[pe_offset + 20]);
    
    uint16_t magic = *(uint16_t*)(&pe_data[pe_offset + 24]);
    uint32_t entry_point_rva = *(uint32_t*)(&pe_data[pe_offset + 40]);
    uint32_t size_of_image = *(uint32_t*)(&pe_data[pe_offset + 80]);
    
    uint32_t import_dir_rva = 0;
    uint32_t import_dir_size = 0;
    
    if (magic == 0x010B) { // 32-bit
        import_dir_rva = *(uint32_t*)(&pe_data[pe_offset + 128]);
        import_dir_size = *(uint32_t*)(&pe_data[pe_offset + 132]);
    } else if (magic == 0x020B) { // 64-bit
        import_dir_rva = *(uint32_t*)(&pe_data[pe_offset + 144]);
        import_dir_size = *(uint32_t*)(&pe_data[pe_offset + 148]);
    } else {
        return 0;
    }
    
    // Allocate memory for the virtual image
    uint8_t* image_base_ptr = (uint8_t*)malloc(size_of_image);
    if (!image_base_ptr) return 0;
    memset_pe(image_base_ptr, 0, size_of_image);
    
    // Copy Headers
    uint32_t size_of_headers = *(uint32_t*)(&pe_data[pe_offset + 84]);
    memcpy_pe(image_base_ptr, pe_data, size_of_headers);
    
    // Copy Sections to their VirtualAddress
    PE_IMAGE_SECTION_HEADER* section_headers = (PE_IMAGE_SECTION_HEADER*)(&pe_data[pe_offset + 24 + size_of_opt_header]);
    for (int i = 0; i < num_sections; i++) {
        if (section_headers[i].SizeOfRawData > 0) {
            uint8_t* dest = image_base_ptr + section_headers[i].VirtualAddress;
            uint8_t* src = pe_data + section_headers[i].PointerToRawData;
            memcpy_pe(dest, src, section_headers[i].SizeOfRawData);
        }
    }
    
    // Resolve Imports (IAT)
    if (import_dir_rva > 0 && import_dir_size > 0) {
        PE_IMAGE_IMPORT_DESCRIPTOR* import_desc = (PE_IMAGE_IMPORT_DESCRIPTOR*)(image_base_ptr + import_dir_rva);
        
        while (import_desc->Name != 0) {
            char* dll_name = (char*)(image_base_ptr + import_desc->Name);
            
            uint32_t thunk_rva = import_desc->OriginalFirstThunk ? import_desc->OriginalFirstThunk : import_desc->FirstThunk;
            
            if (magic == 0x010B) { // 32-bit IAT
                uint32_t* thunk32 = (uint32_t*)(image_base_ptr + thunk_rva);
                uint32_t* iat32 = (uint32_t*)(image_base_ptr + import_desc->FirstThunk);
                int i = 0;
                while (thunk32[i] != 0) {
                    char* func_name = nullptr; // nullptr means Ordinal import
                    if ((thunk32[i] & 0x80000000) == 0) {
                        func_name = (char*)(image_base_ptr + thunk32[i] + 2);
                    }
                    uint64_t resolved_addr = resolve_windows_api(dll_name, func_name);
                    iat32[i] = (uint32_t)resolved_addr;
                    i++;
                }
            } else { // 64-bit IAT
                uint64_t* thunk64 = (uint64_t*)(image_base_ptr + thunk_rva);
                uint64_t* iat64 = (uint64_t*)(image_base_ptr + import_desc->FirstThunk);
                int i = 0;
                while (thunk64[i] != 0) {
                    char* func_name = nullptr;
                    if ((thunk64[i] & 0x8000000000000000ULL) == 0) {
                        func_name = (char*)(image_base_ptr + (thunk64[i] & 0x7FFFFFFF) + 2);
                    }
                    uint64_t resolved_addr = resolve_windows_api(dll_name, func_name);
                    iat64[i] = resolved_addr;
                    i++;
                }
            }
            import_desc++;
        }
    }
    
    return (uint64_t)(image_base_ptr + entry_point_rva);
}

#endif
