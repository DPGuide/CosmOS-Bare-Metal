#include <stdint.h>
extern "C" void disable_nx_for_app(unsigned long long a, unsigned long long b) {}
extern "C" void scan_partitions(void* port) {}
extern "C" void send_dns_query(const char* domain) {}
extern "C" unsigned char* decode_image(unsigned char* buffer, int len, int* w, int* h, int* comp, int req_comp) { return 0; }
extern "C" void free_image(unsigned char* ptr) {}

struct FAT32_ParsedFile {};
bool fat32_list_dir(int a, unsigned char* b, FAT32_ParsedFile* c, int d, unsigned char e, int f) { return false; }
