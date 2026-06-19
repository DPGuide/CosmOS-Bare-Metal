#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "schneider_lang.h"
#include "cosmos_fs.h"
#include "cosmos_ahci.h"
#include "pci.h"
#include "net.h"
#include "cosmos_fat32.h"
#include <limits.h>
// GANZ OBEN in die linux_stubs.cpp (unter die anderen #includes)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL2/SDL.h>

void play_hda_wav(uint64_t ram_addr, uint32_t size, uint32_t sample_rate, uint16_t channels, uint16_t bits) {
    SDL_AudioSpec wav_spec;
    SDL_zero(wav_spec);
    wav_spec.freq = sample_rate;
    // Format basierend auf Bittiefe (meistens 16-Bit oder 8-Bit PCM)
    wav_spec.format = (bits == 16) ? AUDIO_S16LSB : AUDIO_U8; 
    wav_spec.channels = channels;
    wav_spec.samples = 4096;
    wav_spec.callback = NULL; // Wir nutzen Push-Audio (Queue), kein Callback

    // Audio-Gerät öffnen
    SDL_AudioDeviceID audio_dev = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
    if (audio_dev > 0) {
        // Die rohen WAV-Daten aus deinem RAM-Buffer in die Soundkarte pushen
        SDL_QueueAudio(audio_dev, (void*)ram_addr, size);
        // Play drücken! (0 = Unpause)
        SDL_PauseAudioDevice(audio_dev, 0);
    }
}

// Define DriveInfo exactly as kernel.cpp does so we can allocate drives[]
_202 DriveInfo { _43 type; _43 size_mb; _43 base_port; _30 model[41]; };

// Global variables missing
_44 mouse_right_down = _86;
_43 frame = 0;
_43 active_drive_idx = 0;
_184 key_scancode = 0;
_43 drive_count = 0;
_30 hw_disk[48];
_30 linux_dev_paths[8][48] = {{0}};
DriveInfo drives[8];
FileEntry file_table[28];
_89 pcie_base_addr = 0;
_89 global_ahci_abar = 0;
_89 hda_base_addr = 0;
char net_mask[32] = "255.255.255.0";
char gateway_ip[32] = "10.0.2.2";

// Variables for linux_main
volatile bool app_window_active = false;
uint64_t system_ticks = 0;
uint8_t linux_scancode = 0;
int linux_mx = 0;
int linux_my = 0;
int linux_mdown = 0;

extern "C" void kernel_main(uint32_t magic, void* mbi);
extern "C" void kernel_main64(void* boot_info);
extern uint32_t* bb;
extern uint32_t* fb;
extern uint32_t* os2_fb;

extern "C" void cosmos_main(void* boot_info) {
    bb = new uint32_t[1920 * 1080];
    os2_fb = fb;
    kernel_main64(boot_info);
}

void pci_write(_184 bus, _184 slot, _184 func, _184 offset, _89 value) {}
_89 pci_read(_184 bus, _184 slot, _184 func, _184 offset) { return 0xFFFFFFFF; }
_89 pcie_read_mmio(_89 bus, _89 slot, _89 func, _89 offset) { return 0xFFFFFFFF; }

void init_acpi_and_pcie() {}
void pci_scan_all() {}
void fs_init() {}
void mbr_scan() {}

_50 fs_read_sectors(_43 drive, _89 lba, _184* buffer, _43 count) {}
_50 ahci_read_mbr() {}
_50 ahci_init() {}

/// ==========================================
/// ECHTE HARDWARE: BLOCK-DEVICE SCANNER
/// ==========================================
static int linux_get_block_size_mb(const char* dev_name) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/size", dev_name);
    FILE* f = fopen(path, "r");
    if (!f) return 0;
    uint64_t sectors = 0;
    fscanf(f, "%lu", &sectors);
    fclose(f);
    return (int)(sectors / 2048);
}

// =========================================================
// FAT32 STUBS (Damit Linux kompiliert, bis der Bare-Metal FAT32 Code portiert ist)
// =========================================================
_44 fat32_init(_43 partition_lba, uint8_t* bpb_buffer) {
    return 0; // Gibt "false" zurück -> Engine weiß, dass FAT32 hier gerade nicht geht
}

static bool linux_is_usb_device(const char* dev_name) {
    char path[512];
    snprintf(path, sizeof(path), "/sys/block/%s/device", dev_name);
    char resolved[1024];
    if (realpath(path, resolved)) {
        if (strstr(resolved, "usb") != NULL) return true;
    }
    snprintf(path, sizeof(path), "/sys/block/%s/removable", dev_name);
    FILE* f = fopen(path, "r");
    if (f) {
        int removable = 0;
        fscanf(f, "%d", &removable);
        fclose(f);
        if (removable) return true;
    }
    return false;
}

static void linux_get_disk_model(const char* dev_name, char* model_out, int max_len) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/device/model", dev_name);
    FILE* f = fopen(path, "r");
    if (f) {
        if (fgets(model_out, max_len, f)) {
            int len = strlen(model_out);
            while (len > 0 && (model_out[len-1] == '\n' || model_out[len-1] == ' ')) {
                model_out[--len] = 0;
            }
        }
        fclose(f);
    } else {
        snprintf(model_out, max_len, "%s", dev_name);
    }
}

extern "C" void ahci_mount_drive() {
    drive_count = 0;
    DIR* d = opendir("/sys/block");
    if (!d) {
        drives[0].type = 2; drives[0].size_mb = 1024; drives[0].base_port = 0;
        const char* m = "LIVE ISO DISK";
        for (int i=0; i<14; i++) drives[0].model[i] = m[i];
        drive_count = 1;
        return;
    }
    
    struct dirent* ent;
    while ((ent = readdir(d)) != NULL && drive_count < 8) {
        const char* name = ent->d_name;
        bool valid = false;
        if (strncmp(name, "sd", 2) == 0 && strlen(name) == 3) valid = true;     
        if (strncmp(name, "nvme", 4) == 0 && strstr(name, "p") == NULL) valid = true; 
        if (strncmp(name, "vd", 2) == 0 && strlen(name) == 3) valid = true;     
        if (strncmp(name, "mmcblk", 6) == 0 && strstr(name, "p") == NULL) valid = true; 
        
        if (!valid) continue;
        int size_mb = linux_get_block_size_mb(name);
        if (size_mb <= 0) continue; 
        bool is_usb = linux_is_usb_device(name);
        
        drives[drive_count].type = is_usb ? 3 : 2; 
        drives[drive_count].size_mb = size_mb;
        drives[drive_count].base_port = drive_count;
        
        char model[41] = {0};
        linux_get_disk_model(name, model, 40);
        char full_model[41];
        snprintf(full_model, 40, "/dev/%s %s", name, model);
        full_model[40] = 0;
        for(int i=0; i<41; i++) drives[drive_count].model[i] = full_model[i];
        
        snprintf(linux_dev_paths[drive_count], 47, "/dev/%s", name);
        linux_dev_paths[drive_count][47] = 0;
        snprintf(hw_disk, sizeof(hw_disk), "%s", linux_dev_paths[drive_count]);
        
        drive_count++;
    }
    closedir(d);
    
    if (drive_count == 0) {
        drives[0].type = 2; drives[0].size_mb = 1024; drives[0].base_port = 0;
        const char* m = "LIVE ISO DISK";
        for (int i=0; i<14; i++) drives[0].model[i] = m[i];
        drive_count = 1;
    }
}

extern "C" void usb_scan_and_mount() {
    ahci_mount_drive();
}

_44 ahci_read(HBA_PORT *port, _89 startlba, _50 *target_ram_address) { return 0; }
_44 ahci_write(HBA_PORT *port, _89 startlba, _50 *source_ram_address) { return 0; }

/// ==========================================
/// HYBRID-INTERCEPTOR FÜR DATEISYSTEME
/// ==========================================

// ACHTUNG: Das ist exakt die Struktur, die der Disk Manager erwartet!
struct CFS_DIR_ENTRY_LOCAL {
    uint8_t  type;           
    char     filename[11];
    uint32_t file_size;
    uint32_t start_lba;
    uint8_t  parent_idx;
    uint8_t  reserved;
} __attribute__((packed));

// Globale Variablen für den Hybrid-Ordner
char hybrid_file_paths[28][256];
uint32_t hybrid_file_lbas[28];
uint32_t hybrid_file_sizes[28];
int hybrid_file_count = 0;

extern "C" bool disk_read_auto(uint32_t lba, uint64_t target_ram_addr) {
    // --- SCHRITT 0: LBA 0 (BOOT SEKTOR) SIMULIEREN! ---
    if (lba == 0) {
        memset((void*)target_ram_addr, 0, 512);
        uint8_t* boot = (uint8_t*)target_ram_addr;
        boot[3] = 'C'; boot[4] = 'F'; boot[5] = 'S'; // Die magische Signatur!
        boot[510] = 0x55; boot[511] = 0xAA;
        return true;
    }

    // --- SCHRITT 1: DIRECTORY INTERCEPTOR (Sektor 1002) ---
    if (lba == 1002) {
        CFS_DIR_ENTRY_LOCAL* dir = (CFS_DIR_ENTRY_LOCAL*)target_ram_addr;
        memset((void*)target_ram_addr, 0, 2048); 

        const char* host_path = "./roms"; 
        DIR* d = opendir(host_path);
        
        if (!d) {
            mkdir(host_path, 0777);
            d = opendir(host_path);
            if (!d) return false;
        }

        hybrid_file_count = 0;
        uint32_t current_fake_lba = 10000; 
        struct dirent* ent;
        
        while ((ent = readdir(d)) != NULL && hybrid_file_count < 28) {
            if (ent->d_name[0] == '.') continue;
            
            if (ent->d_type == DT_REG || ent->d_type == DT_UNKNOWN) { 
                
                dir[hybrid_file_count].type = 1;         // 1 = EINE ECHTE DATEI
                dir[hybrid_file_count].parent_idx = 255; // 255 = LIEGT IM ROOT ORDNER

                // Dateinamen formatieren
                memset(dir[hybrid_file_count].filename, 0, 11);
                strncpy(dir[hybrid_file_count].filename, ent->d_name, 11);
                for(int i=0; i<11; i++) {
                    if (dir[hybrid_file_count].filename[i] >= 'a' && dir[hybrid_file_count].filename[i] <= 'z') {
                        dir[hybrid_file_count].filename[i] -= 32; 
                    }
                }

                snprintf(hybrid_file_paths[hybrid_file_count], 256, "%s/%s", host_path, ent->d_name);
                
                FILE* f = fopen(hybrid_file_paths[hybrid_file_count], "rb");
                if (f) {
                    fseek(f, 0, SEEK_END);
                    dir[hybrid_file_count].file_size = ftell(f);
                    fclose(f);
                } else {
                    dir[hybrid_file_count].file_size = 0;
                }

                dir[hybrid_file_count].start_lba = current_fake_lba;
                
                hybrid_file_lbas[hybrid_file_count] = current_fake_lba;
                hybrid_file_sizes[hybrid_file_count] = dir[hybrid_file_count].file_size;

                current_fake_lba += (dir[hybrid_file_count].file_size / 512) + 1; 
                hybrid_file_count++;
            }
        }
        closedir(d);
        return true;
    }

    // --- SCHRITT 2: DATEI LESE INTERCEPTOR (LBA ab 10000) ---
    if (lba >= 10000) {
        for (int i = 0; i < hybrid_file_count; i++) {
            uint32_t file_start = hybrid_file_lbas[i];
            uint32_t file_end_lba = file_start + (hybrid_file_sizes[i] / 512);

            if (lba >= file_start && lba <= file_end_lba) {
                uint32_t sector_offset = lba - file_start; 
                FILE* f = fopen(hybrid_file_paths[i], "rb");
                if (f) {
                    fseek(f, sector_offset * 512, SEEK_SET);
                    fread((void*)target_ram_addr, 1, 512, f); 
                    fclose(f);
                    return true;
                }
            }
        }
    }
    return false; 
}

extern "C" bool disk_write_auto(uint32_t lba, uint64_t source_ram_addr) {
    return false; // Dummy-Interceptor für Writes
}
unsigned char* decode_image(unsigned char* buffer, int len, int* w, int* h, int* comp, int req_comp) {
    // Wandelt das rohe JPG/PNG aus dem RAM in rohe RGB-Pixel um!
    return stbi_load_from_memory(buffer, len, w, h, comp, req_comp);
}

void free_image(unsigned char* data) {
    if (data) stbi_image_free(data);
}

_43 ahci_read_sectors(_43 port, _43 start_lba, _43 count, _94 target_ram) { 
    if (port == 0) {
        // PORT 0: Unser virtuelles Systemlaufwerk (./roms Interceptor)
        for(int i = 0; i < count; i++) {
            disk_read_auto(start_lba + i, target_ram + (i * 512));
        }
        return count; 
    } else {
        // PORT > 0: Die ECHTEN Linux USB-Sticks und Festplatten lesen!
        if (linux_dev_paths[port][0] != 0) {
            int fd = open(linux_dev_paths[port], O_RDONLY);
            if (fd >= 0) {
                pread(fd, (void*)target_ram, count * 512, (off_t)start_lba * 512ULL);
                close(fd);
            } else {
                printf("[ERR] LINUX LAUFWERK NICHT LESBAR: %s\n", linux_dev_paths[port]);
            }
        }
        return count;
    }
}

_43 ahci_write_sectors(_43 port, _43 start_lba, _43 count, _94 source_ram) { 
    if (port == 0) {
        // PORT 0: Dummy Interceptor
        for(int i = 0; i < count; i++) {
            disk_write_auto(start_lba + i, source_ram + (i * 512));
        }
        return count; 
    } else {
        // PORT > 0: ECHTE LINUX HARDWARE BESCHREIBEN!
        if (linux_dev_paths[port][0] != 0) {
            int fd = open(linux_dev_paths[port], O_WRONLY);
            if (fd >= 0) {
                pwrite(fd, (void*)source_ram, count * 512, (off_t)start_lba * 512ULL);
                close(fd);
            } else {
                printf("[ERR] SCHREIBEN AUF %s BLOCKIERT! (Fehlen sudo Rechte?)\n", linux_dev_paths[port]);
            }
        }
        return count;
    }
}

_50 ahci_get_raw_identify(_43 port, _182* buffer) {}

// USB Stubs
_50 usb_enumerate_device(_43 dev_addr, _43 type) {}
_43 usb_bot_get_capacity(_43 dev_addr, _43 ep_in, _43 ep_out) { return 0; }

// File system analyzers
_50 analyze_mbr(_184* boot_sec) {}
_50 analyze_gpt(_184* boot_sec) {}
_50 analyze_gpt_entries(_184* entry_sector) {}
_50 analyze_mft_root(_184* mft_record) {}
_50 analyze_vbr(_184* boot_sec) {}
_50 fat32_read_root_dir(_43 drive) {}

/// ==========================================
/// ECHTE HARDWARE: NETZWERK
/// ==========================================
extern char ip_address[32];
extern char net_mask[32];
extern char gateway_ip[32];

_50 check_incoming() {}

_50 send_dhcp_discover() {
    struct ifaddrs *ifap, *ifa;
    if (getifaddrs(&ifap) == 0) {
        for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == NULL) continue;
            if (ifa->ifa_addr->sa_family != AF_INET) continue;
            if (strcmp(ifa->ifa_name, "lo") == 0) continue;
            
            struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
            char* ip = inet_ntoa(sa->sin_addr);
            
            strncpy(ip_address, ip, 31);
            ip_address[31] = 0;
            
            if (ifa->ifa_netmask) {
                struct sockaddr_in* mask = (struct sockaddr_in*)ifa->ifa_netmask;
                uint32_t m = ntohl(mask->sin_addr.s_addr);
                snprintf(net_mask, 32, "%d.%d.%d.%d", (m >> 24) & 0xFF, (m >> 16) & 0xFF, (m >> 8) & 0xFF, m & 0xFF);
                FILE* f = fopen("/proc/net/route", "r");
                if (f) {
                    char line[256];
                    while(fgets(line, sizeof(line), f)) {
                        char iface[16];
                        unsigned long dest, gw;
                        if (sscanf(line, "%15s %lx %lx", iface, &dest, &gw) == 3) {
                            if (dest == 0 && gw != 0) {
                                snprintf(gateway_ip, 32, "%d.%d.%d.%d", 
                                    (int)(gw & 0xFF), (int)((gw >> 8) & 0xFF), 
                                    (int)((gw >> 16) & 0xFF), (int)((gw >> 24) & 0xFF));
                                break;
                            }
                        }
                    }
                    fclose(f);
                } else {
                    snprintf(gateway_ip, 32, "10.0.2.2");
                }
            }            
            freeifaddrs(ifap);
            return;
        }
        freeifaddrs(ifap);
    }
    strncpy(ip_address, "NO NETWORK", 31);
}

_50 send_udp(_89 target_ip, _182 src_port, _182 dst_port, const _30* payload) {}
#include <thread>

_50 send_tcp_syn(_89 target_ip, _182 target_port) {
#ifdef __linux__
    std::thread([]() {
        extern char browser_url[];
        extern uint8_t browser_download_buffer[];
        extern uint32_t browser_download_len;
        extern uint32_t browser_download_complete;
        extern uint32_t browser_state;
        
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "curl -isL --max-time 10 \"%s\" > /tmp/meinos_http.bin", browser_url);
        int ret = system(cmd);
        (void)ret;
        
        FILE* f = fopen("/tmp/meinos_http.bin", "rb");
        if (f) {
            browser_download_len = fread(browser_download_buffer, 1, 15*1024*1024, f);
            fclose(f);
        } else {
            browser_download_len = 0;
        }
        
        browser_state = 3; 
        browser_download_complete = 1;
    }).detach();
#endif
}

_50 send_arp_ping() {}
_50 net_check_link() {}
_50 rtl8139_init(_89 bar) {}
_50 intel_e1000_init(_89 bar, _182 io_port) {}
_50 cb_build(_184* buffer, _182 x, _184 y, _184 z, _184 a, _184 b) {}
_50 send_cosmos_block(_89 ip, _184* data) {}

// Multimedia
extern "C" void play_hda_earthquake() {}
_50 tba_record_frame(_182 a, _184 b, _184 c, _184 d, _184 e) {}
extern "C" void play_hda_freq(uint32_t freq) {}
void disable_nx_for_app() {}
void scan_partitions() {}
int gpt_partition_count = 0;
uint64_t gpt_partition_starts[10];
TCPSocket browser_tcp;
uint32_t ip_str_to_u32(const char*) { 
#ifdef __linux__
    return 0xFFFFFFFF; // Always pretend DNS resolved!
#else
    return 0; 
#endif
}
void send_dns_query() {}
void* __bss_end;
void (*usb_mouse_callback)(int, int, int);
uint32_t hda_dac_nid = 0;

uint32_t global_xhci_base_addr = 0;
uint32_t hda_codec_id = 0;
uint32_t hda_pin_nid = 0;
void decode_image() {}
void free_image() {}
extern char hw_usb[256];

void system_init_usb() {
    char usb_info[256] = "USB: ";
    DIR* d = opendir("/sys/bus/usb/devices");
    if (d) {
        int count = 0;
        struct dirent* ent;
        while ((ent = readdir(d)) != NULL) {
            if (ent->d_name[0] == '.') continue;
            if (strchr(ent->d_name, ':')) continue; 
            count++;
        }
        closedir(d);
        char num[16];
        snprintf(num, sizeof(num), "%d devices", count);
        strncat(usb_info, num, 200);
    } else {
        strncat(usb_info, "NOT FOUND", 200);
    }
    strncpy(hw_usb, usb_info, 255);
    hw_usb[255] = 0;
}

int fat32_root_lba = 0;
void xhci_poll_events_and_mouse() {}

extern "C" void apply_static_ip(const char* new_ip) {
    if (new_ip && strlen(new_ip) > 0) {
        strncpy(ip_address, new_ip, 31);
        ip_address[31] = 0;
    }
}