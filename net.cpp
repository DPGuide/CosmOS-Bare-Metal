#include "net.h"
#define BIG_BLOCK_SIZE 256
#include "schneider_lang.h"
/// ========================================================
/// BARE METAL FIX: DIE HARTEN DMA-ZONEN FÜR INTEL & RTL
/// ========================================================
#ifdef __x86_64__
    /// OS2 (64-BIT): Wir zwingen die Intel-Ringe auf die absolut sichere 256-Megabyte-Marke!
    /// (14 MB war historisch oft durch BIOS oder Intel ME reserviert -> DMA Abort/Freeze!)
    #define e1000_rx_ring    ((volatile unsigned int*)0x10000000)
    #define e1000_tx_ring    ((volatile unsigned int*)0x10010000)
    #define e1000_rx_buffers ((volatile unsigned char (*)[4096])0x10020000)
    #define e1000_tx_buffers ((volatile unsigned char (*)[4096])0x10040000)
#else
    /// OS1 (32-BIT): Der alte Compiler macht das wie gewohnt.
    __attribute__((aligned(4096))) unsigned int e1000_rx_ring[32 * 4];
	__attribute__((aligned(4096))) unsigned int e1000_tx_ring[32 * 4];
	__attribute__((aligned(4096))) _184 e1000_rx_buffers[32][4096];
	__attribute__((aligned(4096))) _184 e1000_tx_buffers[32][4096];
#endif
_184 global_dhcp_buf[300];
_184 global_udp_buf[1500];
_184 global_ip_buf[1514];
_172 _89 mmio_read32(uintptr_t addr);
_172 _50 mmio_write32(uintptr_t addr, _89 val);
_172 _50 tba_master_stream(_184* network_payload);
#ifdef __x86_64__
_172 _50 map_mmio_64(uint64_t phys_addr);
#endif
/// --- PROTOTYPEN ---
_50 e1000_enable_rx();
_50 e1000_enable_tx();
_50 e1000_check_rx();
_202 undi_transmit_t {
    _182 status;
    _182 protocol;
    _182 len;
    _89  buffer_ptr;
} __attribute__((packed));
undi_transmit_t undi_tx_pkg;
_172 _89 rtl_io_base; 
_172 _89 intel_mem_base;
/// --- GLOBALE NETZWERK-ZÄHLER ---
_43 rx_cur_intel = 0;
_43 tx_cur_intel = 0;
_44 is_modern_intel = 0;
_172 _184 mac_addr[6];
_172 _30 mac_str[24];
#ifdef __x86_64__
    /// ==========================================
    /// OS2 (64-BIT): net.cpp ist der Chef und setzt harte Puffer!
    /// ==========================================
    _184* tx_buffer     = (_184*)0x00C10000; 
    _184* rx_buffer_rtl = (_184*)0x00C00000; 
    _30 ip_address[32]  = "10.0.2.15";
#else
    /// ==========================================
    /// OS1 (32-BIT): kernel.cpp ist der Chef, net.cpp ist nur Gast (_172)
    /// ==========================================
    _172 _184* tx_buffer;
    _172 _184* rx_buffer_rtl;
    _172 _30 ip_address[32];
#endif

_172 _43 tx_cur;
_172 _43 rx_idx_rtl;
extern void str_cat(char* dest, const char* src);

extern char cmd_status[256];
_30 net_mask[32] = "255.255.255.0";
_30 gateway_ip[32] = "10.0.2.2";
_30 dns_ip[32] = "8.8.8.8";
_184 router_mac[6] = {0, 0, 0, 0, 0, 0};
_172 _30 cmd_last_out[128];
_172 NICInfo found_nics[5];
_172 _43 active_nic_idx;
#ifdef __x86_64__
extern _30 browser_content[65536];
extern _30 browser_url[512];
extern _184 browser_download_buffer[3000000];
extern _43 browser_download_len;
extern _43 browser_content_length;
extern _44 browser_download_complete;
extern _44 pkg_download_active;
extern "C" void parse_html();
#else
_30 browser_content[65536];
_30 browser_url[512];
_184 browser_download_buffer[3000000];
_43 browser_download_len = 0;
_43 browser_content_length = 0;
_44 browser_download_complete = 0;
_44 pkg_download_active = 0;
#endif
TCPSocket browser_tcp = {0, 0, 0, 0, 0, 0};
_172 _44 str_starts(_71 _30* s1, _71 _30* s2);
_172 _89 random();
_172 _182 chk(_50* d, _43 l);
_172 _50 outl(_182 p, _89 v);
_172 _50 outw(_182 p, _182 v);
_172 _50 outb(_182 p, _184 v);
_172 _184 inb(_182 p);
_172 _50 str_cpy(_30* d, _71 _30* s);
_172 _43 str_len(_71 _30* s);
_172 _50 byte_to_hex(_184 b, _30* out);
_172 _182 hs(_182 v);
_172 _89 hl(_89 v);
_172 _50 int_to_str(_43 val, _30* str);
/// =======================================================
/// BARE METAL FIX: DER NUKLEARE HARDWARE-RESET
/// Tötet den BIOS-Zombie und resettet das Silizium!
/// =======================================================
_50 e1000_base_reset() {
    _15(intel_mem_base EQ 0) _96;
    /// 1. Interrupts aus
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF);
    /// 2. BARE METAL FIX: MAC Reset (Bit 26) UND PHY Reset (Bit 31) zünden!
    _89 ctrl = mmio_read32(intel_mem_base + 0x0000);
    ///mmio_write32(intel_mem_base + 0x0000, ctrl | 0x04000000);
    /// 3. Pause (Hardware bootet neu)
    _39(_89 wait = 0; wait < 10000000; wait++) { 
        __asm__ _192("nop"); 
    }
    /// 4. Interrupts NOCHMAL aus
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF);
    mmio_write32(intel_mem_base + 0x0400, 0); 
    mmio_write32(intel_mem_base + 0x0100, 0);
    /// 5. BARE METAL FIX: Das NEUE Register nach dem Reset auslesen, 
    /// DANN erst die Link-Bits hinzufügen, sonst zerstörst du das Routing!
    _89 new_ctrl = mmio_read32(intel_mem_base + 0x0000);
    mmio_write32(intel_mem_base + 0x0000, new_ctrl | 0x40 | 0x20 | 0x01 | 0x08);
}
/// ==========================================
/// FUNKTIONEN
/// ==========================================
_202 PXE_Struct {
    _184 signature[4];
    _184 length;
    _184 checksum;
    _184 revision;
    _184 reserved1;
    _89  entry_point_sp;
    _89  entry_point_esp;
} __attribute__((packed));
PXE_Struct* global_pxe = 0;
_81 _182 (*UNDI_ENTRY)(_182, _50*);
_50 find_undi_entry() {
    _39(_89 addr = 0x10000; addr < 0x9FFFF; addr += 16) {
        _184* p = (_184*)addr;
        _15(p[0] EQ '!' AND p[1] EQ 'P' AND p[2] EQ 'X' AND p[3] EQ 'E') {
            global_pxe = (PXE_Struct*)addr;
            str_cpy(cmd_status, "UNDI INTERFACE: FOUND");
            _96;
        }
    }
    str_cpy(cmd_status, "UNDI INTERFACE: NOT FOUND");
}
_50 net_raw(_50* d, _89 l) {
    _15(rtl_io_base > 0) {
        _39(_89 i=0;i<l;i++) tx_buffer[i]=((_184*)d)[i];
        outl(rtl_io_base+0x20+(tx_cur*4),(_89)(uintptr_t)tx_buffer); 
        outl(rtl_io_base+0x10+(tx_cur*4),l);
        tx_cur = (tx_cur + 1) % 4; /// BARE METAL FIX: Niemals denselben Descriptor direkt überschreiben!
        str_cpy(cmd_status, "RTL: NATIVE TX FIRED!");
    }
    _15(intel_mem_base > 0) {
        /// BARE METAL FIX: Niemals den Sendevorgang blockieren, nur weil der Link 
        /// "angeblich" noch down ist! Wir legen die Pakete einfach in den Puffer, 
        /// die Hardware sendet sie automatisch, sobald das Kabel bereit ist!
        _89 mac_status = mmio_read32(intel_mem_base + 0x0008);
        _15((mac_status & 0x02) EQ 0) {
            /// Wir geben nur einen Hinweis, aber brechen NICHT ab!
            str_cpy(cmd_status, "INTEL: QUEUED (LINK DOWN?)");
        }

        /// Den legalen RAM-Bereich für das aktuelle Paket holen
        _184* target_buf = (_184*)e1000_tx_buffers[tx_cur_intel];
        _39(_89 i = 0; i < l; i++) target_buf[i] = ((_184*)d)[i];
        _182 send_len = l;

        _15(send_len < 60) {
            _39(_43 i = send_len; i < 60; i++) target_buf[i] = 0;
            send_len = 60;
        }

        uint64_t phys_buf = (uint64_t)target_buf;
        
        volatile unsigned int* wait_desc;
        
        /// Wir verwenden ÜBERALL den Legacy Descriptor (Genau wie in QEMU!)
        /// Das zwingt die Bare-Metal-Karte, sich genau wie QEMU zu verhalten.
        _89* leg_desc = (_89*)&e1000_tx_ring[tx_cur_intel * 4];
        leg_desc[0] = (uint32_t)(phys_buf & 0xFFFFFFFF);
        leg_desc[1] = (uint32_t)((phys_buf >> 32) & 0xFFFFFFFF);
        leg_desc[2] = send_len | 0x0B000000; /// EOP, IFCS, RS
        leg_desc[3] = 0;
        
        wait_desc = (volatile unsigned int*)leg_desc;
        tx_cur_intel = (tx_cur_intel + 1) % 32;

        
        /// BARE METAL FIX: wbinvd WIEDERHERGESTELLT! 
        /// Ohne diesen Flush crasht x32 (da BSS Write-Back ist) und x64 
        /// verschluckt sich an MTRR-Konflikten (Hardware liest leere RAM-Adressen)!
        __asm__ _192("wbinvd" ::: "memory");
        
        mmio_write32(intel_mem_base + 0x3818, tx_cur_intel);
        
        /// =======================================================
        /// BARE METAL FIX 3: DER WAHRE LÜGENDETEKTOR (RAM statt MMIO)
        /// =======================================================
        _44 tx_done = _86; 
        _39(_89 wait = 0; wait < 1500000; wait++) {
            /// BARE METAL FIX: Zwinge die CPU, den echten RAM zu lesen!
            __asm__ _192("clflush (%0)" :: "r"(&wait_desc[3]));
            
            _15((wait_desc[3] & 0x01) NEQ 0) { 
                tx_done = _128; 
                _96; 
            }
            __asm__ _192("nop" ::: "memory");
        }

        _15(tx_done) {
            str_cpy(cmd_status, "TX SUCCESS: PAKET IST IM KABEL!");
        } _41 {
            str_cpy(cmd_status, "TX TIMEOUT (ABER TX=1 BEDEUTET GESENDET)");
        }
	}
}
/// ==========================================
/// BARE METAL FIX: IP STRING ZU UINT32 KONVERTER
/// ==========================================
_89 ip_str_to_u32(_71 _30* ip_str) {
    _89 result = 0;
    _89 octet = 0;
    _43 shift = 24;
    _114(*ip_str) {
        _15(*ip_str EQ '.') {
            result |= (octet << shift);
            shift -= 8;
            octet = 0;
        } _41 _15(*ip_str >= '0' AND *ip_str <= '9') {
            octet = octet * 10 + (*ip_str - '0');
        } _41 {
            _37;
        }
        ip_str++;
    }
    result |= (octet << shift);
    _96 result;
}
_50 net_ip(_89 dst, _50* p_data, _182 p_len, _184 proto) {
    /// BARE METAL FIX: Roher RAM (32 MB) - Kein Stack, keine BSS-Sektion!
    _184* b = global_ip_buf;
    EthernetFrame* e=(EthernetFrame*)b; 
    IPHeader* i=(IPHeader*)(b+14);
    
    _89 my_ip = ip_str_to_u32(ip_address);
    _39(_43 k=0;k<6;k++){
        _15(dst EQ 0xFFFFFFFF) e->dest_mac[k]=0xFF; 
        _41 e->dest_mac[k]=router_mac[k];
        e->src_mac[k]=mac_addr[k];
    }
    e->type=hs(0x0800);
    
    i->ver_ihl=0x45; i->len=hs(20+p_len); i->id=hs(random()); i->frag=hs(0x4000);
    i->ttl=64; i->proto=proto;
    _15(dst EQ 0xFFFFFFFF) i->src = 0; 
    _41 i->src = hl(ip_str_to_u32(ip_address));
    i->dst=hl(dst);
    
    /// =========================================================
    /// BARE METAL FIX: STRIKTE IPv4 CHECKSUMME FÜR ECHTE ROUTER!
    /// (Ersetzt dein altes i->chk = chk(...))
    /// =========================================================
    i->chk = 0; 
    _89 sum = 0;
    _184* hdr = (_184*)i;
    _39(_43 j = 0; j < 20; j += 2) {
        _182 word = (hdr[j] << 8) | hdr[j + 1];
        sum += word;
    }
    _114(sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    _182 final_sum = ~sum;
    i->chk = ((final_sum >> 8) & 0xFF) | ((final_sum << 8) & 0xFF00);
    /// =========================================================

    _39(_43 k=0;k<p_len;k++) b[34+k]=((_184*)p_data)[k];
    
    net_raw(b, 34+p_len);
}
_50 net_ipv6(_184* dst_ip6, _50* p_data, _182 p_len, _184 next_header) {
    /// Eigener Puffer für IPv6, da der Header größer ist (40 Bytes)
    static _184 global_ip6_buf[1514]; 
    _184* b = global_ip6_buf;
    EthernetFrame* e = (EthernetFrame*)b; 
    IPv6Header* i6 = (IPv6Header*)(b + 14);
    /// Ethernet Setup (Achtung: Eigener EtherType für IPv6!)
    _39(_43 k=0; k<6; k++) { e->dest_mac[k] = 0xFF; e->src_mac[k] = mac_addr[k]; } 
    e->type = hs(0x86DD); /// 0x86DD ist das Signal für IPv6 (statt 0x0800)
    /// IPv6 Header Setup
    /// Version 6 (0x60000000 in Network-Byte-Order)
    i6->vtc_flow = hl(0x60000000); 
    i6->payload_len = hs(p_len);
    i6->next_header = next_header;
    i6->hop_limit = 255;
    /// Adressen kopieren (16 Bytes)
    _39(_43 k=0; k<16; k++) {
        i6->src[k] = 0; /// Hier käme später deine eigene IPv6 rein
        i6->dst[k] = dst_ip6[k];
    }
    /// Daten anhängen (Offset ist hier 14 + 40 = 54)
    _39(_43 k=0; k<p_len; k++) b[54+k] = ((_184*)p_data)[k];
    /// Keine IP-Checksumme nötig! Direkt abfeuern!
    net_raw(b, 54 + p_len);
}
_50 send_big_cosmos_block(_89 ip, _184* block) {
    _184 pl[512];
    UDPHeader* u = (UDPHeader*)pl;
    u->src = hs(COSMOS_PORT);
    u->dst = hs(COSMOS_PORT);
    u->len = hs(8 + BIG_BLOCK_SIZE);
    u->chk = 0;
    _39(_43 k=0; k < BIG_BLOCK_SIZE; k++) {
        pl[8 + k] = block[k];
    }
    net_ip(ip, pl, 8 + BIG_BLOCK_SIZE, 17);
    str_cpy(cmd_status, "BIG BLOCK DISPATCHED");
}
_50 send_cosmos_block(_89 ip, _184* block) {
    _184 pl[512]; 
    UDPHeader* u = (UDPHeader*)pl;
    u->src = hs(COSMOS_PORT);
    u->dst = hs(COSMOS_PORT);
    u->len = hs(8 + BLOCK_SIZE);
    u->chk = 0;
    _39(_43 k=0; k < BLOCK_SIZE; k++) {
        pl[8 + k] = block[k];
    }
    net_ip(ip, pl, 8 + BLOCK_SIZE, 17);
    str_cpy(cmd_status, "COSMOS BLOCK SENT");
}
_50 net_handle_cosmos_packet(_184* data, _182 len) {
    _39(_43 k=0; k < len - 255; k++) {
        _15(data[k] EQ 0x2A AND data[k+255] EQ 0xFF) {
            _184* big_block = &data[k];
            _15(cb_validate(big_block)) {
                str_cpy(cmd_status, "BIG BLOCK RX: VALID");
                tba_master_stream(big_block);
                _37;
            }
        }
    }
}
extern "C" _50 send_arp_ping() {
    /// ========================================================
    /// BARE METAL FIX: DER LINK-GUARD (NUR FÜR INTEL!)
    /// ========================================================
    _15(intel_mem_base > 0) {
        _89 status = mmio_read32(intel_mem_base + 0x0008);
        _15((status & 0x02) EQ 0) {
            str_cpy(cmd_status, "FEHLER: KEIN LINK (KABEL DOWN)!");
        }
    }

    _184 frame[60]; 
    _39(_43 i = 0; i < 60; i++) frame[i] = 0;
    /// 1. Ethernet Header (Ziel: Broadcast)
    _39(_43 i = 0; i < 6; i++) frame[i] = 0xFF; 
    _39(_43 i = 0; i < 6; i++) frame[6+i] = mac_addr[i]; 
    frame[12] = 0x08; frame[13] = 0x06; /// ARP
    /// 2. ARP Header
    frame[14] = 0x00; frame[15] = 0x01; /// Ethernet
    frame[16] = 0x08; frame[17] = 0x00; /// IPv4
    frame[18] = 0x06; frame[19] = 0x04; /// HW len=6, Proto len=4
    frame[20] = 0x00; frame[21] = 0x01; /// Opcode: 1 (Request)
   /// 3. Sender MAC & IP (BARE METAL FIX: Echte IP verwenden!)
    _39(_43 i = 0; i < 6; i++) frame[22+i] = mac_addr[i]; 
    _89 my_ip = ip_str_to_u32(ip_address);
    frame[28] = (my_ip >> 24) & 0xFF; frame[29] = (my_ip >> 16) & 0xFF;
    frame[30] = (my_ip >> 8) & 0xFF; frame[31] = my_ip & 0xFF;
    
    /// 4. Target MAC (Null) & Target IP (BARE METAL FIX: Gateway aus DHCP!)
    _39(_43 i = 0; i < 6; i++) frame[32+i] = 0x00; 
    _89 gw_ip = ip_str_to_u32(gateway_ip);
    frame[38] = (gw_ip >> 24) & 0xFF; frame[39] = (gw_ip >> 16) & 0xFF;
    frame[40] = (gw_ip >> 8) & 0xFF; frame[41] = gw_ip & 0xFF;
    /// Ab dafür!
    net_raw(frame, 60);
}
/// =======================================================
/// BARE METAL FIX: DER LAN-PARTY MODUS (STATISCHE IP)
/// =======================================================
extern "C" _50 apply_static_ip(_71 _30* new_ip) {
    /// 1. Die eingegebene IP ist DEIN LAPTOP (z.B. 192.168.14.100)
    str_cpy(ip_address, new_ip);
    
    /// 2. Standard-Subnetz 
    str_cpy(net_mask, "255.255.255.0");
    
    /// 3. Gateway ist IMMER die FritzBox
    str_cpy(gateway_ip, "192.168.14.14"); 

    /// 4. Status auf ONLINE zwingen
    str_cpy(cmd_status, "ONLINE (STATIC IP SET)");

    /// 5. ARP Ping absenden (mit der richtigen Absender-IP!)
    send_arp_ping();
}

_50 send_udp(_89 ip, _182 p_src, _182 p_dst, _71 _30* msg) { 
    _43 ml=str_len(msg); _184 pl[1024]; UDPHeader* u=(UDPHeader*)pl;
    u->src=hs(p_src); u->dst=hs(p_dst); u->len=hs(8+2+ml); u->chk=0;
    pl[8]='S'; pl[9]=84; 
    _39(_43 k=0;k<ml;k++) pl[10+k]=msg[k]; 
    net_ip(ip, pl, 8+2+ml, 17);
    str_cpy(cmd_status, "UDP: SIGNED (ID 84)");
}
_50 send_udp_raw(_89 ip, _182 p_src, _182 p_dst, _184* payload, _182 payload_len) { 
    /// BARE METAL FIX: Roher RAM (32 MB + 4 KB)
    _184* pl = global_udp_buf;
    UDPHeader* u = (UDPHeader*)pl;
    u->src = hs(p_src); 
    u->dst = hs(p_dst); 
    u->len = hs(8 + payload_len);
    u->chk = 0;
    _39(_43 k = 0; k < payload_len; k++) {
        pl[8 + k] = payload[k]; 
    }

    /// =======================================================
    /// BARE METAL FIX: UDP CHECKSUMME (FritzBox Zicke Teil 2)
    /// Router droppen oft Pakete mit Checksumme 0x0000!
    /// =======================================================
    _89 sum = 0;
    
    /// 1. Pseudo-Header: IPs
    _15(ip EQ 0xFFFFFFFF) {
        /// Broadcast: Source IP ist 0.0.0.0, Dest IP ist 255.255.255.255
        sum += 0xFFFF; 
        sum += 0xFFFF;
    } _41 {
        /// BARE METAL FIX: Echte lokale IP verwenden (nicht QEMU 10.0.2.15)!
        _89 src_ip = ip_str_to_u32(ip_address);
        sum += (src_ip >> 16) & 0xFFFF; 
        sum += src_ip & 0xFFFF;
        sum += (ip >> 16) & 0xFFFF; 
        sum += ip & 0xFFFF;         
    }
    
    /// 2. Pseudo-Header: Protocol & Length
    sum += 17; 
    sum += (payload_len + 8); 
    
    /// 3. UDP Header & Payload summieren
    _43 total_udp_len = 8 + payload_len;
    _184* udp_bytes = (_184*)pl;
    _39(_43 j = 0; j < total_udp_len; j += 2) {
        _182 word = (udp_bytes[j] << 8);
        _15(j + 1 < total_udp_len) word |= udp_bytes[j + 1];
        sum += word;
    }
    
    /// 4. Overflow falten & schreiben
    _114(sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    _182 final_sum = ~sum;
    _15(final_sum EQ 0) final_sum = 0xFFFF; /// UDP Checksumme 0 wird als 0xFFFF gesendet
    
    u->chk = ((final_sum >> 8) & 0xFF) | ((final_sum << 8) & 0xFF00);

    net_ip(ip, pl, 8 + payload_len, 17);
}
_50 send_tcp_payload(_89 ip, _182 p_src, _182 p_dst, _89 seq, _89 ack_num, _184 flags, _184* payload, _182 payload_len) {
    _184 pl[1500]; 
    TCPHeader* t = (TCPHeader*)pl;
    t->src = hs(p_src); 
    t->dst = hs(p_dst); 
    t->seq = hl(seq); 
    t->ack = hl(ack_num);
    t->off = 0x50; 
    t->flg = flags;
    t->win = hs(8192); 
    t->chk = 0; 
    t->urg = 0;
    
    _39(_43 k=0; k < payload_len; k++) {
        pl[20 + k] = payload[k];
    }
    
    _89 sum = 0;
    _89 src_ip = ip_str_to_u32(ip_address);
    sum += (src_ip >> 16) & 0xFFFF; sum += src_ip & 0xFFFF;
    sum += (ip >> 16) & 0xFFFF;     sum += ip & 0xFFFF;         
    sum += 6; 
    sum += (20 + payload_len); 
    
    _43 total_tcp_len = 20 + payload_len;
    _184* tcp_bytes = (_184*)pl;
    _39(_43 j = 0; j < total_tcp_len; j += 2) {
        _182 word = (tcp_bytes[j] << 8);
        _15(j + 1 < total_tcp_len) word |= tcp_bytes[j + 1];
        sum += word;
    }
    
    _114(sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    _182 final_sum = ~sum;
    t->chk = ((final_sum >> 8) & 0xFF) | ((final_sum << 8) & 0xFF00);

    net_ip(ip, pl, total_tcp_len, 6);
}

_50 send_tcp_syn(_89 ip, _182 port) {
    _44 has_mac = _86;
    _39(_43 k=0; k<6; k++) _15(router_mac[k] NEQ 0) has_mac = _128;
    _15(!has_mac) {
        send_arp_ping();
        str_cpy(cmd_status, "WAITING FOR ARP... TRY AGAIN!");
        str_cpy(browser_content, "ARP IS MISSING!\nWAIT 1 SEC AND PRESS ENTER AGAIN!\n");
        _96;
    }

    browser_tcp.state = 1;
    browser_tcp.remote_ip = ip;
    browser_tcp.remote_port = port;
    browser_tcp.local_port = 49152 + (random() % 1000);
    browser_tcp.my_seq = random();
    browser_tcp.my_ack = 0;
    
    /// DIAGNOSTIK: TX Counter VOR dem Senden
    _89 tx_before = 0;
    _89 rx_before = 0;
    _89 link_status = 0;
    _15(intel_mem_base > 0) {
        tx_before = mmio_read32(intel_mem_base + 0x40D4);
        rx_before = mmio_read32(intel_mem_base + 0x40D0);
        link_status = mmio_read32(intel_mem_base + 0x0008);
    }
    
    send_tcp_payload(ip, browser_tcp.local_port, port, browser_tcp.my_seq, 0, 0x02, 0, 0);
    
    /// DIAGNOSTIK: TX Counter NACH dem Senden
    _89 tx_after = 0;
    _89 rx_after = 0;
    _15(intel_mem_base > 0) {
        tx_after = mmio_read32(intel_mem_base + 0x40D4);
        rx_after = mmio_read32(intel_mem_base + 0x40D0);
    }
    
    /// Alles in browser_content dumpen
    str_cpy(browser_content, "=== TCP SYN DIAGNOSTIK ===\n");
    
    /// Link Status
    str_cat(browser_content, "LINK STATUS: ");
    _30 hex_ls[12]; int_to_str(link_status, hex_ls);
    str_cat(browser_content, hex_ls);
    str_cat(browser_content, "\nSPEED: ");
    _43 speed = (link_status >> 6) & 3;
    _15(speed EQ 0) str_cat(browser_content, "10 MBIT");
    _41 _15(speed EQ 1) str_cat(browser_content, "100 MBIT");
    _41 _15(speed EQ 2) str_cat(browser_content, "1000 MBIT");
    _41 str_cat(browser_content, "UNKNOWN");
    str_cat(browser_content, " | DUPLEX: ");
    _15(link_status & 1) str_cat(browser_content, "FULL");
    _41 str_cat(browser_content, "HALF");
    str_cat(browser_content, " | LINK: ");
    _15(link_status & 2) str_cat(browser_content, "UP");
    _41 str_cat(browser_content, "DOWN");
    
    /// TX Counter
    str_cat(browser_content, "\nTX BEFORE: ");
    _30 tb[12]; int_to_str(tx_before, tb);
    str_cat(browser_content, tb);
    str_cat(browser_content, " | TX AFTER: ");
    _30 ta[12]; int_to_str(tx_after, ta);
    str_cat(browser_content, ta);
    
    /// RX Counter
    str_cat(browser_content, "\nRX BEFORE: ");
    _30 rb[12]; int_to_str(rx_before, rb);
    str_cat(browser_content, rb);
    str_cat(browser_content, " | RX AFTER: ");
    _30 ra[12]; int_to_str(rx_after, ra);
    str_cat(browser_content, ra);
    
    /// Router MAC (HEX)
    str_cat(browser_content, "\nROUTER MAC: ");
    _30 hx[4];
    _39(_43 k=0; k<6; k++) {
        byte_to_hex(router_mac[k], hx); hx[2]=0;
        str_cat(browser_content, hx);
        _15(k<5) str_cat(browser_content, ":");
    }
    
    /// Source IP
    str_cat(browser_content, "\nSRC IP: ");
    str_cat(browser_content, ip_address);
    
    /// Dest IP
    str_cat(browser_content, "\nDST IP: ");
    _30 d1[5],d2[5],d3[5],d4[5];
    int_to_str((ip >> 24) & 0xFF, d1);
    int_to_str((ip >> 16) & 0xFF, d2);
    int_to_str((ip >> 8) & 0xFF, d3);
    int_to_str(ip & 0xFF, d4);
    str_cat(browser_content, d1); str_cat(browser_content, ".");
    str_cat(browser_content, d2); str_cat(browser_content, ".");
    str_cat(browser_content, d3); str_cat(browser_content, ".");
    str_cat(browser_content, d4);
    
    str_cat(browser_content, "\n\nWAITING FOR SYN-ACK...\n");
    str_cpy(cmd_status, "TCP: SYN SENT (DIAG MODE)");
}

_50 send_tcp_ack(_89 ip, _182 p_src, _182 p_dst, _89 seq, _89 ack_num) {
    send_tcp_payload(ip, p_src, p_dst, seq, ack_num, 0x10, 0, 0);
    str_cpy(cmd_status, "TCP: ACK SENT");
}
_50 rtl_enable_rx() {
    outl(rtl_io_base + 0x30, (_89)(uintptr_t)rx_buffer_rtl);
    /// BARE METAL FIX: INTERRUPTS AUSSCHALTEN! (Polling-Modus)
    /// Das alte 0x0005 hat einen Interrupt-Sturm ausgelöst, der QEMU komplett einfrieren ließ!
    outw(rtl_io_base + 0x3C, 0x0000); 
    outl(rtl_io_base + 0x44, 0x0F | 0x80);
    outb(rtl_io_base + 0x37, 0x0C); 
}

extern _44 is_over(_43 mx, _43 my, _43 x, _43 y, _43 r);
extern _184 get_ascii_qwertz(_184 scancode);

#ifdef __x86_64__
/// --- HOLYSPIRIT SENTINEL EXTERNALS ---
extern _43 hs_radar_count;
struct DetectedIP {
    _184 ip[4];
    _30 threat[16];
    _43 hits;
    _44 isFriend;
    _30 proto[8];
};
extern DetectedIP hs_radar[100];
extern void hs_add_log(const char* msg);
#endif

/// =======================================================
/// DIE NETZWERK-FUNKTIONEN
/// =======================================================
_50 e1000_enable_tx() {
    _15(intel_mem_base EQ 0) _96;

    /// ASDE + SLU werden bereits in intel_e1000_init() nach dem PHY-Reset gesetzt.
    /// Hier lassen wir CTRL unangetastet, damit die Auto-Negotiation nicht gestört wird.
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF); /// Interrupts aus!

    /// BARE METAL FIX: TX Queue sauber stoppen!
    mmio_write32(intel_mem_base + 0x0400, 0); /// TCTL = 0
    _89 txdctl_old = mmio_read32(intel_mem_base + 0x3828);
    mmio_write32(intel_mem_base + 0x3828, txdctl_old & ~0x02000000); /// TX Queue Disable
    _39(_43 wait = 0; wait < 100000; wait++) {
        _15((mmio_read32(intel_mem_base + 0x3828) & 0x02000000) EQ 0) _37;
    }

    _39(_43 i = 0; i < 32; i++) {
        uint64_t phys_buf = (uint64_t)&e1000_tx_buffers[i][0];
        e1000_tx_ring[i*4 + 0] = (uint32_t)(phys_buf & 0xFFFFFFFF); 
        e1000_tx_ring[i*4 + 1] = (uint32_t)((phys_buf >> 32) & 0xFFFFFFFF);                     
        e1000_tx_ring[i*4 + 2] = 0;                     
        e1000_tx_ring[i*4 + 3] = 0;                     
    }
    
	uint64_t ring_phys_tx = (uint64_t)&e1000_tx_ring[0];
    mmio_write32(intel_mem_base + 0x3800, (uint32_t)(ring_phys_tx & 0xFFFFFFFF));
	/// =======================================================
    /// BARE METAL FIX: DIE ECHTE MAC-ADRESSE AUSLESEN
    /// RAL0 (0x5400) und RAH0 (0x5404) enthalten die Hardware-MAC
    /// =======================================================
    _89 mac_low = mmio_read32(intel_mem_base + 0x5400);
    _89 mac_high = mmio_read32(intel_mem_base + 0x5404);

    _30 mac_str[30] = "MAC: ";
    _30* m_ptr = mac_str + 5;
    byte_to_hex((mac_low >> 0) & 0xFF, m_ptr); m_ptr+=2; *m_ptr++ = ':';
    byte_to_hex((mac_low >> 8) & 0xFF, m_ptr); m_ptr+=2; *m_ptr++ = ':';
    byte_to_hex((mac_low >> 16) & 0xFF, m_ptr); m_ptr+=2; *m_ptr++ = ':';
    byte_to_hex((mac_low >> 24) & 0xFF, m_ptr); m_ptr+=2; *m_ptr++ = ':';
    byte_to_hex((mac_high >> 0) & 0xFF, m_ptr); m_ptr+=2; *m_ptr++ = ':';
    byte_to_hex((mac_high >> 8) & 0xFF, m_ptr); m_ptr+=2; *m_ptr = 0;

    /// Lass dir das irgendwo auf dem Bildschirm anzeigen!
    str_cpy(cmd_status, mac_str);
	
    mmio_write32(intel_mem_base + 0x3804, (uint32_t)((ring_phys_tx >> 32) & 0xFFFFFFFF));
    mmio_write32(intel_mem_base + 0x3808, 512);      
    mmio_write32(intel_mem_base + 0x3810, 0);         
    mmio_write32(intel_mem_base + 0x3818, 0);
    tx_cur_intel = 0;
    /// BARE METAL FIX: EXACTLY THE MEILENSTEIN CONFIG!
    /// Ein Read-Modify-Write hat die Karte zum Absturz gebracht. 
    /// Die Legacy-Werte (0x0103F0FA) haben im Meilenstein perfekt funktioniert (RX ging!).
    
    mmio_write32(intel_mem_base + 0x0400, 0x0103F0FA); 
    mmio_write32(intel_mem_base + 0x0410, 0x0060200A); 
    
    /// BARE METAL FIX: TX Queue starten! (Wie iPXE: NUR Enable-Bit setzen)
    /// Wir lassen WTHRESH und GRAN auf 0! 
    mmio_write32(intel_mem_base + 0x3828, 0x02000000);
    _39(_89 wait=0; wait<1000000; wait++) {
        _15((mmio_read32(intel_mem_base + 0x3828) & 0x02000000) NEQ 0) break;
        __asm__ _192("nop");
    }
}

_50 e1000_enable_rx() {
    _15(intel_mem_base EQ 0) _96;
    
    /// =========================================================
    /// BARE METAL FIX: WARTEN BIS DIE PXE-ROM-QUEUE GESTOPPT IST!
    /// Da wir keinen MAC-Reset mehr machen, läuft die alte PXE-Queue noch!
    /// Wenn wir RDBAL/RDBAH im laufenden Betrieb ändern, crasht der DMA-Controller!
    /// =========================================================
    mmio_write32(intel_mem_base + 0x0100, 0); /// RCTL.EN = 0
    _89 rxdctl_old = mmio_read32(intel_mem_base + 0x2828);
    mmio_write32(intel_mem_base + 0x2828, rxdctl_old & ~0x02000000); /// RX Queue Disable
    _39(_43 wait = 0; wait < 100000; wait++) {
        _15((mmio_read32(intel_mem_base + 0x2828) & 0x02000000) EQ 0) _37;
    }
    
    /// BARE METAL FIX: RFCTL LÖSCHEN! (EXTENDED DESCRIPTORS DEAKTIVIEREN)
    /// Die PXE ROM hat evtl. Advanced Descriptors (für IPv6/Checksums) aktiviert!
    /// Unser OS nutzt aber Legacy Descriptors (Word 3 = Status/Length).
    /// Wenn RFCTL nicht 0 ist, sucht die Karte das DD-Bit an der falschen Stelle!
    mmio_write32(intel_mem_base + 0x5008, 0);

    _39(_43 i=0; i<32; i++) {
        uint64_t phys_buf = (uint64_t)&e1000_rx_buffers[i][0];
        e1000_rx_ring[i*4 + 0] = (uint32_t)(phys_buf & 0xFFFFFFFF); 
        e1000_rx_ring[i*4 + 1] = (uint32_t)((phys_buf >> 32) & 0xFFFFFFFF); 
        e1000_rx_ring[i*4 + 2] = 0;                     
        e1000_rx_ring[i*4 + 3] = 0;                     
    }
	uint64_t ring_phys_rx = (uint64_t)&e1000_rx_ring[0];
    mmio_write32(intel_mem_base + 0x2800, (uint32_t)(ring_phys_rx & 0xFFFFFFFF));
    mmio_write32(intel_mem_base + 0x2804, (uint32_t)((ring_phys_rx >> 32) & 0xFFFFFFFF));        
    mmio_write32(intel_mem_base + 0x2808, 512);      
    mmio_write32(intel_mem_base + 0x2810, 0);        /// RDH = 0
    
    _89 rxdctl = mmio_read32(intel_mem_base + 0x2828);
    mmio_write32(intel_mem_base + 0x2828, rxdctl | 0x02000000); /// Enable Queue
    
    /// BARE METAL FIX: Warten, bis die Hardware die Queue wirklich aktiviert hat!
    /// Wenn wir RDT schreiben, bevor Bit 25 "1" ist, ignoriert die Karte das!
    _39(_89 wait = 0; wait < 1000000; wait++) {
        _15((mmio_read32(intel_mem_base + 0x2828) & 0x02000000) NEQ 0) _37;
        __asm__ _192("nop");
    }

    _39(_43 i = 0; i < 128; i++) { mmio_write32(intel_mem_base + 0x5200 + (i * 4), 0); }
    
    /// =========================================================
    /// BARE METAL FIX: RCTL KORREKT SETZEN!
    /// 0x0402801E =
    ///   Bit 1  (EN=1)    : Receiver Enable
    ///   Bit 2  (SBP=1)   : Store Bad Packets
    ///   Bit 3  (UPE=1)   : Unicast Promiscuous
    ///   Bit 4  (MPE=1)   : Multicast Promiscuous
    ///   Bit 15 (BAM=1)   : Broadcast Accept
    ///   Bit 16-17 (BSIZE=00) : 2048 Byte Buffers
    ///   Bit 25 (BSEX=1)  : Buffer Size Extension -> 2048 wird zu 4096! (Ist hier 0)
    ///   Bit 26 (SECRC=1) : Strip Ethernet CRC
    /// =========================================================
    /// BARE METAL FIX: 0x0400801E zwingt die Hardware auf 2048 Bytes (0x020000 war 512/1024!)
    mmio_write32(intel_mem_base + 0x0100, 0x0400801E);
    
    /// BARE METAL FIX: RDT (Tail) MUSS zwingend NACH dem RCTL.EN geschrieben werden, 
    /// sonst holt die I219-V keine Deskriptoren ab!
    mmio_write32(intel_mem_base + 0x2818, 31);       /// RDT = 31
}

_50 intel_e1000_init(_89 addr, _182 device_id) {
    intel_mem_base = addr & 0xFFFFFFF0;
    
    /// BARE METAL FIX: Erkennen, ob es eine moderne PCH-MAC (z.B. I219-V) ist.
    /// QEMU 82540EM hat device_id 0x100E. Alles andere betrachten wir als modern!
    is_modern_intel = (device_id NEQ 0x100E);
    _15(intel_mem_base EQ 0) _96;

    /// =========================================================
    /// BARE METAL FIX: DMA-ZONEN ALS UNCACHEABLE RE-MAPPEN!
    /// Die os2_entry.s Page Tables mappen ALLES mit 0x83 (Write-Back).
    /// Aber DMA-Puffer MÜSSEN Cache-Disabled sein, sonst sieht die CPU
    /// die NIC-DMA-Schreibvorgänge NIEMALS (sie liest nur ihren Cache)!
    /// Das ist DER Grund warum TX=1 und RX=0: Das Paket wird gesendet,
    /// die Antwort kommt per DMA im RAM an, aber die CPU liest cached 0.
    /// =========================================================
#ifdef __x86_64__
    map_mmio_64(0x00C00000); /// RTL RX buffer
    map_mmio_64(0x00C10000); /// RTL TX buffer
    map_mmio_64(0x10000000); /// E1000 RX ring + TX ring
    map_mmio_64(0x10020000); /// E1000 RX buffers
    map_mmio_64(0x10040000); /// E1000 TX buffers
#endif

    /// 1. MAC RETTEN (Dein Code ist perfekt!)
    _89 ral = mmio_read32(intel_mem_base + 0x5400);
    _89 rah = mmio_read32(intel_mem_base + 0x5404);
    _15(ral NEQ 0 AND ral NEQ 0xFFFFFFFF) {
        mac_addr[0] = (_184)(ral); mac_addr[1] = (_184)(ral >> 8); 
        mac_addr[2] = (_184)(ral >> 16); mac_addr[3] = (_184)(ral >> 24); 
        mac_addr[4] = (_184)(rah); mac_addr[5] = (_184)(rah >> 8);
    } _41 {
        mac_addr[0] = 0x52; mac_addr[1] = 0x54; mac_addr[2] = 0x00;
        mac_addr[3] = 0x12; mac_addr[4] = 0x34; mac_addr[5] = 0x56;
        /// BARE METAL FIX: Wenn die MAC 0 war, MÜSSEN wir sie der Hardware beibringen!
        mmio_write32(intel_mem_base + 0x5400, 0x12005452); /// 52 54 00 12 (Little Endian)
        mmio_write32(intel_mem_base + 0x5404, 0x80005634); /// 34 56 + Bit 31 (Address Valid)
    }
    _30* p = mac_str;
    _39(_43 i=0; i<6; i++) { byte_to_hex(mac_addr[i], p); p+=2; _15(i<5) *p++ = ':'; } *p = 0;

    /// =======================================================
    /// 2. BARE METAL FIX: THE EXTRA SAUSAGE (CTRL_EXT)
    /// =======================================================
    /// Wir sagen dem Stromspar-Controller: "OS Treiber ist aktiv!"
    /// Offset 0x0018 ist CTRL_EXT, Bit 28 (0x10000000) ist DRV_LOAD.
    _89 ctrl_ext = mmio_read32(intel_mem_base + 0x0018);
    mmio_write32(intel_mem_base + 0x0018, ctrl_ext | 0x10000000);

    /// =======================================================
    /// 3. BARE METAL FIX: HARD-RESET + LINK-UP WARTEN!
    /// =======================================================
    /// Da du per USB bootest (PXE aus), schläft die Netzwerkkarte!
    /// Wir MÜSSEN den MAC-Reset durchführen, sonst bleibt sie aus.
    /// BARE METAL FIX: Nur MAC-Reset (Bit 26: 0x04000000) durchführen!
    /// Ein PHY-Reset (Bit 31: 0x80000000) zwingt die I219-V auf 10 Mbit
    /// zurück, da wir keinen komplexen MDIO-Treiber haben, um sie wieder
    /// auf 1 Gbit hochzuhandeln.
    _89 ctrl_rst = mmio_read32(intel_mem_base + 0x0000);
    mmio_write32(intel_mem_base + 0x0000, ctrl_rst | 0x04000000);
    
    /// Kurz warten, bis der Reset durch ist
    _39(_192 _89 delay = 0; delay < 1000000; delay++) { __asm__ _192("nop"); }
    
    /// BARE METAL FIX: MAC ADRESSE WIEDERHERSTELLEN!
    /// Ein CTRL.RST löscht auf manchen NICs die RAL/RAH Register, 
    /// und wenn kein Auto-EEPROM-Read passiert, senden wir mit MAC 00:00:00:00:00:00!
    _89 new_ral = mac_addr[0] | (mac_addr[1] << 8) | (mac_addr[2] << 16) | (mac_addr[3] << 24);
    _89 new_rah = mac_addr[4] | (mac_addr[5] << 8) | 0x80000000; /// 0x80000000 = Address Valid!
    mmio_write32(intel_mem_base + 0x5400, new_ral);
    mmio_write32(intel_mem_base + 0x5404, new_rah);
    
    /// =======================================================
    /// BARE METAL FIX: ASDE + SLU WIEDERHERGESTELLT!
    /// Da Gigabit-Force das Senden blockiert hat (TX=0), MÜSSEN wir
    /// der MAC erlauben, auf 10 Mbit zu laufen. 10 Mbit reicht 
    /// völlig für TCP/HTTP! Der Fehler liegt nicht am Speed!
    /// =======================================================
    _39(_192 _89 settle = 0; settle < 5000000; settle++) { __asm__ _192("nop"); }
    _89 ctrl_post = mmio_read32(intel_mem_base + 0x0000);
    ctrl_post |= (1 << 5);   /// ASDE: Auto-Speed Detection Enable
    ctrl_post |= (1 << 6);   /// SLU: Set Link Up
    ctrl_post &= ~(1 << 11); /// FRCSPD aus: Geschwindigkeit NICHT erzwingen
    ctrl_post &= ~(1 << 12); /// FRCDPLX aus: Duplex NICHT erzwingen
    
    mmio_write32(intel_mem_base + 0x0000, ctrl_post);
    
    /// BARE METAL FIX: Warten auf den "Link Up"!
    /// Nach einem Reset dauert Auto-Negotiation bis zu 3 Sekunden.
    /// Wir MÜSSEN warten, bis das Kabel physikalisch verbunden ist!
    _30 link_str[50] = "WAITING FOR LINK...";
    str_cpy(cmd_status, link_str);
    
    _43 link_up = 0;
    _39(_43 timeout = 0; timeout < 50; timeout++) {
        _89 status = mmio_read32(intel_mem_base + 0x0008);
        _15(status & 0x02) { 
            link_up = 1; 
            break; 
        }
        /// Eine kleine Verzögerung (ca. 100ms)
        _39(_192 _89 delay = 0; delay < 20000000; delay++) { __asm__ _192("nop"); }
    }
    _15(link_up) {
        str_cpy(cmd_status, "LINK UP DETECTED!");
    } _41 {
        str_cpy(cmd_status, "LINK DOWN (CABLE UNPLUGGED?)");
    }

    /// Alte Interrupts löschen
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF);
    /// ICR auslesen, um pending Interrupts zu quittieren
    mmio_read32(intel_mem_base + 0x00C0);

	/// =======================================================
    /// BARE METAL FIX: WAKE-ON-LAN (ZOMBIE-MODUS) BEENDEN!
    /// =======================================================
    mmio_write32(intel_mem_base + 0x5800, 0); /// WUC (Wake Up Control) abschalten
    mmio_write32(intel_mem_base + 0x5808, 0); /// WUFC (Wake Up Filter) abschalten
    mmio_write32(intel_mem_base + 0x5810, 0); /// WUS (Wake Up Status) löschen
    
    /// --- SANFTER STOPP UND LINK UP ---
    mmio_write32(intel_mem_base + 0x0100, 0); 
    mmio_write32(intel_mem_base + 0x0400, 0); 
    _39(_192 _89 delay = 0; delay < 1000000; delay++) { __asm__ _192("nop"); }
    
    /// ASDE + SLU sind bereits in intel_e1000_init() gesetzt.
    /// Hier nur noch ICR auslesen, um pending Interrupts zu quittieren.
    mmio_read32(intel_mem_base + 0x00C0);
    
    e1000_enable_rx();
    e1000_enable_tx();
    str_cpy(cmd_status, "INTEL: READY & LISTENING");
}
_50 e1000_check_rx() {
    _15(intel_mem_base EQ 0) _96;

    _89 processed_any = 0;
    _89 last_processed = rx_cur_intel;

    _39(_89 i = 0; i < 32; i++) {
        
        /// =========================================================
        /// BARE METAL FIX: Auf x86 ist DMA-Write automatisch Cache-Coherent!
        /// Ein wbinvd in dieser Endlosschleife lockt den Memory-Bus 
        /// und verhindert, dass die NIC jemals in den RAM schreiben kann!
        /// =========================================================
        volatile unsigned int* rx_desc = (volatile unsigned int*)&e1000_rx_ring[rx_cur_intel * 4];
        __asm__ volatile("" ::: "memory"); /// Compiler memory barrier
        
        _89 status = rx_desc[3];

        /// Wenn das DD-Bit (0x01) NICHT gesetzt ist, ist hier kein Paket.
        _15((status & 0x01) EQ 0) {
            
            /// DIE ABSOLUTE HARDWARE-WAHRHEIT (TPR & TPT Zähler)
            _89 tpt = mmio_read32(intel_mem_base + 0x40D4); /// Gesendet
            _89 tpr = mmio_read32(intel_mem_base + 0x40D0); /// Empfangen
            
            _30 debug_str[60] = "HW STATS | TX: "; 
            _30* p = debug_str + 15;
            int_to_str(tpt, p); _114(*p) p++;
            *p++ = ' '; *p++ = 'R'; *p++ = 'X'; *p++ = ':'; *p++ = ' ';
            int_to_str(tpr, p); _114(*p) p++; *p = 0;

            /// Zeige es an, solange wir nicht "ONLINE" sind
            _15(!str_starts(cmd_status, "ONLINE")) {
                str_cpy(cmd_status, debug_str);
            }
            
            break; /// Schleife abbrechen
        }

        /// Paketlänge in DEINE Variable 'len' speichern
        _182 len = rx_desc[2] & 0xFFFF; 
        
		_184* raw_data = (_184*)e1000_rx_buffers[rx_cur_intel];

        /// --- BARE METAL FIX: DAS STETHOSKOP ---
        _30 hex_buf[40] = "RX RAW: L="; 
        _30* p_hex = hex_buf + 10;
        int_to_str(len, p_hex); _114(*p_hex) p_hex++;
        *p_hex++ = ' ';
        byte_to_hex(raw_data[12], p_hex); p_hex+=2; /// Ethernet Type (z.B. 08 für IPv4)
        byte_to_hex(raw_data[13], p_hex); p_hex+=2;
        *p_hex = 0;
        str_cpy(cmd_status, hex_buf);

        /// --- HOLYSPIRIT OMNI-CRAWLER INTERCEPTOR ---
#ifdef __x86_64__
        if (raw_data[12] == 0x08 && raw_data[13] == 0x00) { // IPv4
            _184 src_ip[4] = { raw_data[26], raw_data[27], raw_data[28], raw_data[29] };
            _184 proto = raw_data[23];
            
            // Find existing target
            int found = -1;
            for(int i = 0; i < hs_radar_count; i++) {
                if(hs_radar[i].ip[0] == src_ip[0] && hs_radar[i].ip[1] == src_ip[1] &&
                   hs_radar[i].ip[2] == src_ip[2] && hs_radar[i].ip[3] == src_ip[3]) {
                    found = i; break;
                }
            }
            if (found == -1 && hs_radar_count < 100) {
                found = hs_radar_count++;
                hs_radar[found].ip[0] = src_ip[0]; hs_radar[found].ip[1] = src_ip[1];
                hs_radar[found].ip[2] = src_ip[2]; hs_radar[found].ip[3] = src_ip[3];
                hs_radar[found].hits = 0;
                hs_radar[found].isFriend = (src_ip[0] == 192 && src_ip[1] == 168);
                if (proto == 1) str_cpy(hs_radar[found].proto, "ICMP");
                else if (proto == 6) str_cpy(hs_radar[found].proto, "TCP");
                else if (proto == 17) str_cpy(hs_radar[found].proto, "UDP");
                else str_cpy(hs_radar[found].proto, "UNK");
                
                // Add Log
                char log_msg[64];
                str_cpy(log_msg, "DETECTED NEW THREAT FROM ");
                char ip_str[16];
                int_to_str(src_ip[0], ip_str); str_cat(log_msg, ip_str); str_cat(log_msg, ".");
                int_to_str(src_ip[1], ip_str); str_cat(log_msg, ip_str); str_cat(log_msg, ".");
                int_to_str(src_ip[2], ip_str); str_cat(log_msg, ip_str); str_cat(log_msg, ".");
                int_to_str(src_ip[3], ip_str); str_cat(log_msg, ip_str);
                hs_add_log(log_msg);
            }
            if (found != -1) {
                hs_radar[found].hits++;
            }
        }
#endif

        /// --- VLAN-FILTER ---
        _43 off = 0;
        _15(raw_data[12] EQ 0x81 AND raw_data[13] EQ 0x00) off = 4;
        _44 packet_was_important = _86;

        /// --- 1. WEICHE: ARP ---
        _15(len >= 42+off AND raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x06) {
            /// =========================================================
            /// BARE METAL FIX: ARP REQUEST BEANTWORTEN! (Opcode 1)
            /// Ohne das kann die FritzBox unsere MAC nicht finden und
            /// wirft DNS/TCP-Antworten weg!
            /// =========================================================
            _15(raw_data[21+off] EQ 0x01) {
                /// Prüfe ob die Anfrage an UNSERE IP gerichtet ist
                _89 target_ip = (raw_data[38+off] << 24) | (raw_data[39+off] << 16) | (raw_data[40+off] << 8) | raw_data[41+off];
                _89 my_ip = ip_str_to_u32(ip_address);
                _15(target_ip EQ my_ip AND my_ip NEQ 0) {
                    /// ARP Reply bauen (im eigenen Buffer!)
                    _184 reply[60];
                    _39(_43 k=0; k<60; k++) reply[k] = 0;
                    /// 1. Ethernet: Ziel = Absender des Requests
                    _39(_43 k=0; k<6; k++) reply[k] = raw_data[6+off+k];
                    /// 2. Ethernet: Quelle = unsere MAC
                    _39(_43 k=0; k<6; k++) reply[6+k] = mac_addr[k];
                    /// 3. EtherType = ARP (0x0806)
                    reply[12] = 0x08; reply[13] = 0x06;
                    /// 4. ARP Header
                    reply[14] = 0x00; reply[15] = 0x01; /// Hardware: Ethernet
                    reply[16] = 0x08; reply[17] = 0x00; /// Protocol: IPv4
                    reply[18] = 0x06; reply[19] = 0x04; /// HW len=6, Proto len=4
                    reply[20] = 0x00; reply[21] = 0x02; /// Opcode: REPLY (2)
                    /// 5. Sender = WIR (MAC + IP)
                    _39(_43 k=0; k<6; k++) reply[22+k] = mac_addr[k];
                    reply[28] = (my_ip >> 24) & 0xFF;
                    reply[29] = (my_ip >> 16) & 0xFF;
                    reply[30] = (my_ip >> 8) & 0xFF;
                    reply[31] = my_ip & 0xFF;
                    /// 6. Target = der Fragesteller (MAC + IP aus dem Request)
                    _39(_43 k=0; k<6; k++) reply[32+k] = raw_data[22+off+k];
                    _39(_43 k=0; k<4; k++) reply[38+k] = raw_data[28+off+k];
                    /// 7. ABSCHIESSEN!
                    net_raw(reply, 60);
                    str_cpy(cmd_status, "ARP REPLY SENT!");
                    packet_was_important = _128;
                }
            }
            /// ARP Reply empfangen (Opcode 2) - Router-MAC lernen
            _41 _15(raw_data[21+off] EQ 0x02) { 
                _89 sender_ip = (raw_data[28+off] << 24) | (raw_data[29+off] << 16) | (raw_data[30+off] << 8) | raw_data[31+off];
                _89 gw_ip = ip_str_to_u32(gateway_ip);
                _15(sender_ip EQ gw_ip) {
                    _39(_43 k=0; k<6; k++) router_mac[k] = raw_data[22+off+k];
                    str_cpy(cmd_status, "ONLINE (ARP ROUTER OK)!");
                }
                packet_was_important = _128;
            }
        }

        /// --- 2. WEICHE: TCP PARSER ---
        _41 _15(raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x00 AND raw_data[23+off] EQ 0x06) {
            _43 ip_hl = (raw_data[14+off] & 0x0F) * 4;
            _182 ip_total_len = (raw_data[16+off] << 8) | raw_data[17+off];
            _43 tcp_start = 14 + off + ip_hl;
            
            _182 dst_port = (raw_data[tcp_start+2] << 8) | raw_data[tcp_start+3];
            _89 seq = (raw_data[tcp_start+4] << 24) | (raw_data[tcp_start+5] << 16) | (raw_data[tcp_start+6] << 8) | raw_data[tcp_start+7];
            _89 ack = (raw_data[tcp_start+8] << 24) | (raw_data[tcp_start+9] << 16) | (raw_data[tcp_start+10] << 8) | raw_data[tcp_start+11];
            
            _184 tcp_hl = (raw_data[tcp_start+12] >> 4) * 4;
            _184 flags = raw_data[tcp_start+13];
            
            _43 payload_start = tcp_start + tcp_hl;
            _43 tcp_payload_len = 0;
            _15(ip_total_len >= ip_hl + tcp_hl) {
                tcp_payload_len = ip_total_len - ip_hl - tcp_hl;
            }
            
            _15(browser_tcp.state NEQ 0 AND dst_port EQ browser_tcp.local_port) {
                _15((flags & 0x12) EQ 0x12) {
                    browser_tcp.state = 2; 
                    browser_tcp.my_ack = seq + 1;
                    browser_tcp.my_seq++; 
                    
                    send_tcp_ack(browser_tcp.remote_ip, browser_tcp.local_port, browser_tcp.remote_port, browser_tcp.my_seq, browser_tcp.my_ack);
                    
                    _30 host[64];
                    _30 path[512];
                    _43 s_idx = 0;
                    _15(str_starts(browser_url, "HTTP://") || str_starts(browser_url, "http://")) s_idx = 7;
                    _41 _15(str_starts(browser_url, "HTTPS://") || str_starts(browser_url, "https://")) s_idx = 8;
                    str_cpy(host, browser_url + s_idx);
                    str_cpy(path, "/");
                    for(int i=0; i<64; i++) {
                        if(host[i] == '/') {
                            host[i] = 0;
                            str_cpy(path, browser_url + s_idx + i);
                            break;
                        }
                    }
                    
                    _30 get_req[1024];
                    str_cpy(get_req, "GET ");
                    _43 g_idx = 4;
                    for (int k = 0; path[k] != 0 && g_idx < 900; k++) {
                        if (path[k] == ' ') {
                            get_req[g_idx++] = '+'; // Use + or %20. + is safer for queries
                        } else {
                            get_req[g_idx++] = path[k];
                        }
                    }
                    get_req[g_idx] = 0;
                    str_cat(get_req, " HTTP/1.0\r\nHost: ");
                    str_cat(get_req, host);
                    str_cat(get_req, "\r\nConnection: close\r\n\r\n");
                    _43 req_len = str_len(get_req);
                    send_tcp_payload(browser_tcp.remote_ip, browser_tcp.local_port, browser_tcp.remote_port, browser_tcp.my_seq, browser_tcp.my_ack, 0x18, (_184*)get_req, req_len);
                    browser_tcp.my_seq += req_len;
                    
                    browser_download_len = 0;
                    browser_content_length = 0;
                    browser_download_complete = 0;
                    str_cpy(cmd_status, "TCP: CONNECTED & GET SENT");
                }
                _41 _15(tcp_payload_len > 0) {
                    browser_tcp.my_ack = seq + tcp_payload_len;
                    send_tcp_ack(browser_tcp.remote_ip, browser_tcp.local_port, browser_tcp.remote_port, browser_tcp.my_seq, browser_tcp.my_ack);
                    
                    _15(browser_download_len + tcp_payload_len < 3000000) {
                        _39(_43 i=0; i<tcp_payload_len; i++) browser_download_buffer[browser_download_len+i] = raw_data[payload_start+i];
                        browser_download_len += tcp_payload_len;
                    }
                    
                    if (browser_content_length == 0 && browser_download_len > 20) {
                        for(uint32_t i=0; i<browser_download_len-16; i++) {
                            if (browser_download_buffer[i] == 'C' && browser_download_buffer[i+1] == 'o' && browser_download_buffer[i+8] == 'L' && browser_download_buffer[i+14] == 'h' && browser_download_buffer[i+15] == ':') {
                                uint32_t v = 0;
                                uint32_t j = i + 16;
                                while(browser_download_buffer[j] == ' ') j++;
                                while(browser_download_buffer[j] >= '0' && browser_download_buffer[j] <= '9') {
                                    v = v * 10 + (browser_download_buffer[j] - '0');
                                    j++;
                                }
                                browser_content_length = v;
                                break;
                            }
                        }
                    }
                    /// BARE METAL FIX: Call parse_html progressively since modern servers use keep-alive and never send FIN!
#ifdef __x86_64__
                    if (!pkg_download_active) {
                        parse_html();
                    }
#endif

                    str_cpy(cmd_status, "TCP: DATA RX");
                }
                _41 _15(flags & 0x01) {
                    browser_tcp.my_ack = seq + 1;
                    send_tcp_ack(browser_tcp.remote_ip, browser_tcp.local_port, browser_tcp.remote_port, browser_tcp.my_seq, browser_tcp.my_ack);
                    browser_tcp.state = 0; 
                    browser_download_complete = 1;
#ifdef __x86_64__
                    if (!pkg_download_active) {
                        parse_html();
                    }
#endif
                    str_cpy(cmd_status, "TCP: CLOSED");
                }
                _41 _15(flags & 0x04) {
                    browser_tcp.state = 0; 
                    browser_download_complete = 1;
                    str_cpy(cmd_status, "TCP: CONNECTION REFUSED (RST)");
                }
            }
            packet_was_important = _128;
        }




        /// --- 3. WEICHE: DHCP (THE ULTIMATE PARSER) ---
        /// BARE METAL FIX: Dynamische IP-Header-Länge berechnen!
        _41 _15(raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x00 AND raw_data[23+off] EQ 0x11) {
            _43 ip_hl = (raw_data[14+off] & 0x0F) * 4; /// Berechnet die echte Header-Länge
            _43 udp_start = 14 + off + ip_hl;
            _43 bootp_start = udp_start + 8;

              /// Ist es DNS? (Quell-Port ist 53 / 0x0035)
              _15(raw_data[udp_start] EQ 0x00 AND raw_data[udp_start+1] EQ 0x35) {
                  str_cpy(browser_content, "DNS RESPONSE RECEIVED!\nPARSING...\n");
                  _43 q_cnt = (raw_data[bootp_start+4] << 8) | raw_data[bootp_start+5];
                  _43 a_cnt = (raw_data[bootp_start+6] << 8) | raw_data[bootp_start+7];
                  
                  _15(a_cnt > 0) {
                      str_cpy(browser_content, "DNS RESPONSE RECEIVED!\nANSWERS > 0!\n");
                      _43 idx = bootp_start + 12;
                                              /// Skip Questions Safely
                        _39(_43 q = 0; q < q_cnt; q++) {
                            _44 q_done = _86;
                            _114(!q_done) {
                                _15((raw_data[idx] & 0xC0) EQ 0xC0) {
                                    idx += 2;
                                    q_done = _128;
                                } _41 _15(raw_data[idx] EQ 0) {
                                    idx += 1;
                                    q_done = _128;
                                } _41 {
                                    idx += raw_data[idx] + 1;
                                }
                            }
                            idx += 4; /// Skip type/class
                        }
                      
                      /// Parse Answers
                      _39(_43 a = 0; a < a_cnt; a++) {
                                                    /// Skip Name Safely
                          _44 a_done = _86;
                          _114(!a_done) {
                              _15((raw_data[idx] & 0xC0) EQ 0xC0) {
                                  idx += 2;
                                  a_done = _128;
                              } _41 _15(raw_data[idx] EQ 0) {
                                  idx += 1;
                                  a_done = _128;
                              } _41 {
                                  idx += raw_data[idx] + 1;
                              }
                          }
                          
                          _184 type_hi = raw_data[idx++]; _184 type_lo = raw_data[idx++];
                          _184 class_hi = raw_data[idx++]; _184 class_lo = raw_data[idx++];
                          idx += 4; /// Skip TTL
                          _184 dlen_hi = raw_data[idx++]; _184 dlen_lo = raw_data[idx++];
                          _43 dlen = (dlen_hi << 8) | dlen_lo;
                          
                          _15(type_hi EQ 0x00 AND type_lo EQ 0x01 AND class_hi EQ 0x00 AND class_lo EQ 0x01 AND dlen EQ 4) {
                              _89 ip1 = raw_data[idx++]; _89 ip2 = raw_data[idx++];
                              _89 ip3 = raw_data[idx++]; _89 ip4 = raw_data[idx++];
                              browser_tcp.remote_ip = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
                              str_cpy(cmd_status, "DNS: IP GEFUNDEN!");
                              send_tcp_syn(browser_tcp.remote_ip, 80);
                              _37;
                          } _41 {
                              idx += dlen;
                          }
                      }
                  }
                  }


            _15(len > bootp_start + 240) { /// Ist das Paket groß genug?
                /// Port 68 (0x44) und BOOTP Reply (0x02) Check
                _15(raw_data[udp_start+2] EQ 0x00 AND raw_data[udp_start+3] EQ 68 AND raw_data[bootp_start] EQ 0x02) {
                    
                    /// Magic Cookie Check an dynamischer Position
                    _15(raw_data[bootp_start+236] EQ 0x63 AND raw_data[bootp_start+237] EQ 0x82 AND raw_data[bootp_start+238] EQ 0x53 AND raw_data[bootp_start+239] EQ 0x63) {
                        
                        _30* ip_ptr = ip_address;
                        int_to_str(raw_data[bootp_start+16], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                        int_to_str(raw_data[bootp_start+17], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                        int_to_str(raw_data[bootp_start+18], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                        int_to_str(raw_data[bootp_start+19], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr = 0;
                        
                        _43 j = bootp_start + 240; 
                        _43 msg_type = 0;
                        _114(j < len AND raw_data[j] NEQ 255) {
                            _184 opt = raw_data[j];
                            _15(opt EQ 0) { j++; _101; } 
                            _184 opt_len = raw_data[j+1];
                            
                            _15(opt EQ 53) msg_type = raw_data[j+2];
                            
                            _15(opt EQ 1 AND opt_len EQ 4) {
                                _30* m_ptr = net_mask;
                                int_to_str(raw_data[j+2], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+3], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+4], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+5], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr = 0;
                            }
                            
                            _15(opt EQ 3 AND opt_len >= 4) {
                                _30* g_ptr = gateway_ip;
                                int_to_str(raw_data[j+2], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+3], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+4], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+5], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr = 0;
                            }
                            
                            _15(opt EQ 6 AND opt_len >= 4) {
                                _30* d_ptr = dns_ip;
                                int_to_str(raw_data[j+2], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+3], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+4], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+5], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr = 0;
                            }
                            j += 2 + opt_len; 
                        }
                        
                        _15(msg_type EQ 2) str_cpy(cmd_status, "DHCP OFFER RX (IP GEFUNDEN!)");
                        _41 _15(msg_type EQ 5) {
                            str_cpy(cmd_status, "DHCP ACK RX (ONLINE!)");
                            send_arp_ping();
                        }
                        _41 str_cpy(cmd_status, "DHCP RX: PROTOCOL OK");
                        packet_was_important = _128;
                    }



                }
            }
        }

        /// --- HEX-DEBUGGER MIT NOISE-FILTER ---
        _15(!packet_was_important AND !str_starts(cmd_status, "ONLINE")) {
            _44 is_noise = _86;
            _15(raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x06 AND raw_data[21+off] EQ 0x01) is_noise = _128;
            _15(raw_data[12+off] EQ 0x89 AND raw_data[13+off] EQ 0x12) is_noise = _128;
            _15(raw_data[12+off] EQ 0x86 AND raw_data[13+off] EQ 0xDD) is_noise = _128;
            _15(raw_data[12+off] EQ 0x88 AND raw_data[13+off] EQ 0xE1) is_noise = _128;
            _15(!is_noise) {
                _30 hex_buf2[40] = "RX: L="; _30* p2 = hex_buf2 + 6;
                int_to_str(len, p2); _114(*p2) p2++;
                *p2++ = ' '; *p2++ = 'T'; *p2++ = '=';
                byte_to_hex(raw_data[12+off], p2); p2+=2;
                byte_to_hex(raw_data[13+off], p2); p2+=2;
                *p2++ = ' '; *p2++ = 'O'; *p2++ = '=';
                byte_to_hex(raw_data[21+off], p2); p2+=2; *p2 = 0;
                str_cpy(cmd_status, hex_buf2);
            }
        }

        /// --- CLEANUP ---
        rx_desc[3] = 0; /// Status-Bit im echten RAM löschen!
        /// BARE METAL FIX: wbinvd zwingend nötig für x32 BSS und x64 MTRR Konflikte!
        __asm__ _192("wbinvd" ::: "memory");
        
        last_processed = rx_cur_intel;
        rx_cur_intel = (rx_cur_intel + 1) % 32;
        processed_any = 1; /// Wir haben erfolgreich etwas verarbeitet!
    }

    /// Der Hardware einmal am Ende sagen, welche Slots jetzt wieder frei sind
    _15(processed_any) {
        mmio_write32(intel_mem_base + 0x2818, last_processed);
    }
}
_50 check_incoming() {
    _15(intel_mem_base > 0) e1000_check_rx();
    _15(rtl_io_base > 0) {
        _30 cmd = inb(rtl_io_base + 0x37);
        _15((cmd & 1) EQ 0) {
            _89* hdr = (_89*)(rx_buffer_rtl + rx_idx_rtl);
            _89 rx_stat = hdr[0]; 
            _89 rx_len = (rx_stat >> 16) & 0xFFFF;
            _15(rx_len EQ 0 OR (rx_stat & 1) EQ 0) { outw(rtl_io_base + 0x38, rx_idx_rtl - 16); _96; }
            /// raw_data zeigt jetzt exakt auf Byte 0 des Ethernet-Pakets (Ziel-MAC)
            _30* raw_data = (_30*)(rx_buffer_rtl + rx_idx_rtl + 4);
            /// ====================================================
            /// 1. IST ES EIN ARP-PAKET? (EtherType 0x0806)
            /// ====================================================
            _15(raw_data[12] EQ 0x08 AND raw_data[13] EQ 0x06) {
                /// Ist es eine Frage (Request)? Opcode 0x0001
                _15(raw_data[20] EQ 0x00 AND raw_data[21] EQ 0x01) {
                    str_cpy(cmd_status, "PING EMPFANGEN! (ARP REQUEST)");
                    /// ==========================================
                    /// ARP REPLY BASTELN (Direkt im Buffer!)
                    /// ==========================================
                    /// 1. Ethernet-Header umdrehen
					_39 (_43 i = 0; i < 6; i++) {
						raw_data[i] = raw_data[i + 6];         /// Ziel wird zur alten Absender-MAC
						raw_data[i + 6] = inb(rtl_io_base + i); /// <--- BARE METAL MAGIC: Direkt aus dem Chip!
					}
                    /// 2. ARP-Opcode auf REPLY (2) setzen
                    raw_data[21] = 0x02;
                    /// 3. Alte Absender-IP der FritzBox merken
                    _89 temp_ip[4];
                    _39 (_43 i = 0; i < 4; i++) temp_ip[i] = raw_data[28 + i];
                    /// 4. Unsere Daten in den ARP-Body schreiben
					_39 (_43 i = 0; i < 6; i++) raw_data[22 + i] = inb(rtl_io_base + i); /// <--- Direkt aus dem Chip!
					_39 (_43 i = 0; i < 4; i++) raw_data[28 + i] = raw_data[38 + i];
                    /// 5. FritzBox-Daten als neues Ziel in den ARP-Body schreiben
                    _39 (_43 i = 0; i < 6; i++) raw_data[32 + i] = raw_data[i]; /// Ziel-MAC
                    _39 (_43 i = 0; i < 4; i++) raw_data[38 + i] = temp_ip[i];  /// Ziel-IP
                    /// 6. Paket abschiessen! (42 Bytes lang)
                    net_raw((_184*)raw_data, 42);
                    str_cpy(cmd_status, "ARP REPLY GESENDET!");
                }
                _41 _15(raw_data[20] EQ 0x00 AND raw_data[21] EQ 0x02) {
                    _89 sender_ip = (raw_data[28] << 24) | (raw_data[29] << 16) | (raw_data[30] << 8) | raw_data[31];
                    _89 gw_ip = ip_str_to_u32(gateway_ip);
                    _15(sender_ip EQ gw_ip) {
                        _39(_43 k=0; k<6; k++) router_mac[k] = raw_data[22+k];
                        str_cpy(cmd_status, "ONLINE (ARP ROUTER OK)!");
                    }
                }
            }
            /// ====================================================
            /// 2. IST ES EIN IPv4-PAKET? (EtherType 0x0800)
            /// ====================================================
            _15(raw_data[12] EQ 0x08 AND raw_data[13] EQ 0x00) {
                /// Ist es UDP? (Protocol-Feld im IP-Header ist 17)
                _15(raw_data[23] EQ 17) {
                    _43 ip_hl = (raw_data[14] & 0x0F) * 4;
                    _43 udp_start = 14 + ip_hl;
                    _43 bootp_start = udp_start + 8;

              /// Ist es DNS? (Quell-Port ist 53 / 0x0035)
              _15(raw_data[udp_start] EQ 0x00 AND raw_data[udp_start+1] EQ 0x35) {
                  str_cpy(browser_content, "DNS RESPONSE RECEIVED!\nPARSING...\n");
                  _43 q_cnt = (raw_data[bootp_start+4] << 8) | raw_data[bootp_start+5];
                  _43 a_cnt = (raw_data[bootp_start+6] << 8) | raw_data[bootp_start+7];
                  
                  _15(a_cnt > 0) {
                      str_cpy(browser_content, "DNS RESPONSE RECEIVED!\nANSWERS > 0!\n");
                      _43 idx = bootp_start + 12;
                                              /// Skip Questions Safely
                        _39(_43 q = 0; q < q_cnt; q++) {
                            _44 q_done = _86;
                            _114(!q_done) {
                                _15((raw_data[idx] & 0xC0) EQ 0xC0) {
                                    idx += 2;
                                    q_done = _128;
                                } _41 _15(raw_data[idx] EQ 0) {
                                    idx += 1;
                                    q_done = _128;
                                } _41 {
                                    idx += raw_data[idx] + 1;
                                }
                            }
                            idx += 4; /// Skip type/class
                        }
                      
                      /// Parse Answers
                      _39(_43 a = 0; a < a_cnt; a++) {
                                                    /// Skip Name Safely
                          _44 a_done = _86;
                          _114(!a_done) {
                              _15((raw_data[idx] & 0xC0) EQ 0xC0) {
                                  idx += 2;
                                  a_done = _128;
                              } _41 _15(raw_data[idx] EQ 0) {
                                  idx += 1;
                                  a_done = _128;
                              } _41 {
                                  idx += raw_data[idx] + 1;
                              }
                          }
                          
                          _184 type_hi = raw_data[idx++]; _184 type_lo = raw_data[idx++];
                          _184 class_hi = raw_data[idx++]; _184 class_lo = raw_data[idx++];
                          idx += 4; /// Skip TTL
                          _184 dlen_hi = raw_data[idx++]; _184 dlen_lo = raw_data[idx++];
                          _43 dlen = (dlen_hi << 8) | dlen_lo;
                          
                          _15(type_hi EQ 0x00 AND type_lo EQ 0x01 AND class_hi EQ 0x00 AND class_lo EQ 0x01 AND dlen EQ 4) {
                              _89 ip1 = raw_data[idx++]; _89 ip2 = raw_data[idx++];
                              _89 ip3 = raw_data[idx++]; _89 ip4 = raw_data[idx++];
                              browser_tcp.remote_ip = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
                              str_cpy(cmd_status, "DNS: IP GEFUNDEN!");
                              send_tcp_syn(browser_tcp.remote_ip, 80);
                              _37;
                          } _41 {
                              idx += dlen;
                          }
                      }
                  }
                  }

                    
                    /// Ist es DHCP? (Ziel-Port ist 68 / 0x0044)
                    _15(raw_data[udp_start+2] EQ 0x00 AND raw_data[udp_start+3] EQ 0x44) {
                        str_cpy(cmd_status, "DHCP OFFER EMPFANGEN!");
                        _43 ip1 = raw_data[bootp_start+16];
                        _43 ip2 = raw_data[bootp_start+17];
                        _43 ip3 = raw_data[bootp_start+18];
                        _43 ip4 = raw_data[bootp_start+19];
                        
                        ip_address[0] = 0; 
                        _30 tmp[10];
                        int_to_str(ip1, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip2, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip3, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip4, tmp); str_cat(ip_address, tmp);
                          
                        _43 j = bootp_start + 240; 
                        _43 msg_type = 0;
                        _114(j < rx_len AND raw_data[j] NEQ 255) {
                            _184 opt = raw_data[j];
                            _15(opt EQ 0) { j++; _101; } 
                            _184 opt_len = raw_data[j+1];
                            
                            _15(opt EQ 53) msg_type = raw_data[j+2];
                            
                            _15(opt EQ 1 AND opt_len EQ 4) {
                                _30* m_ptr = net_mask;
                                int_to_str(raw_data[j+2], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+3], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+4], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr++ = '.';
                                int_to_str(raw_data[j+5], m_ptr); _114(*m_ptr) m_ptr++; *m_ptr = 0;
                            }
                            
                            _15(opt EQ 3 AND opt_len >= 4) {
                                _30* g_ptr = gateway_ip;
                                int_to_str(raw_data[j+2], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+3], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+4], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr++ = '.';
                                int_to_str(raw_data[j+5], g_ptr); _114(*g_ptr) g_ptr++; *g_ptr = 0;
                            }
                            
                            _15(opt EQ 6 AND opt_len >= 4) {
                                _30* d_ptr = dns_ip;
                                int_to_str(raw_data[j+2], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+3], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+4], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr++ = '.';
                                int_to_str(raw_data[j+5], d_ptr); _114(*d_ptr) d_ptr++; *d_ptr = 0;
                            }
                            j += 2 + opt_len; 
                        }
                          
                        _15(msg_type EQ 2) str_cpy(cmd_status, "DHCP OFFER RX (IP GEFUNDEN!)");
                        _41 _15(msg_type EQ 5) {
                            str_cpy(cmd_status, "DHCP ACK RX (ONLINE!)");
                            send_arp_ping();
                        }
                        _41 str_cpy(cmd_status, "DHCP RX: PROTOCOL OK");
                    }



                }
            }
            /// BARE METAL FIX: Ringpuffer korrekt wrappen!
            rx_idx_rtl = (rx_idx_rtl + rx_len + 4 + 3) & ~3; 
            _114(rx_idx_rtl >= 8192) rx_idx_rtl -= 8192; 
            outw(rtl_io_base + 0x38, rx_idx_rtl - 16); 
            /// BARE METAL FIX: Interrupt-Status (ISR) IMMER bereinigen, um Stürme zu verhindern!
            outw(rtl_io_base + 0x3E, 0xFFFF);
        }
    }
}
_50 rtl8139_init(_89 io_addr) { 
    rtl_io_base = io_addr & ~3; outb(rtl_io_base + 0x52, 0); outb(rtl_io_base + 0x37, 0x10); 
    _114((inb(rtl_io_base + 0x37) & 0x10) NEQ 0) { } 
    outb(rtl_io_base + 0x37, 0x0C); 
    _30* p = mac_str; _39(_43 i=0; i<6; i++) { mac_addr[i] = inb(rtl_io_base + i); byte_to_hex(mac_addr[i], p); p+=2; _15(i<5) *p++ = ':'; } *p = 0; 
    rtl_enable_rx(); 
    str_cpy(cmd_status, "RTL8139 READY"); str_cpy(ip_address, "DHCP (RTL)..."); 
}

extern "C" _50 send_dns_query(_71 _30* domain) {
    _44 has_mac = _86;
    _39(_43 k=0; k<6; k++) _15(router_mac[k] NEQ 0) has_mac = _128;
    _15(!has_mac) {
        send_arp_ping();
        str_cpy(cmd_status, "WAITING FOR ARP... TRY AGAIN!");
        str_cpy(browser_content, "ARP IS MISSING!\nWAIT 1 SEC AND PRESS ENTER AGAIN!\n");
        return;
    }

    _15(dns_ip[0] EQ '0' AND dns_ip[1] EQ '.' AND dns_ip[2] EQ '0') {
        _15(gateway_ip[0] NEQ '0' AND gateway_ip[0] NEQ 0) {
            str_cpy(dns_ip, gateway_ip);
            str_cpy(cmd_status, "DNS: FALLBACK TO GATEWAY IP!");
        } _41 {
            str_cpy(dns_ip, "8.8.8.8");
            str_cpy(cmd_status, "DNS: FALLBACK TO 8.8.8.8!");
        }
    }
    
    _184 payload[512];
    _39(_43 i=0; i<512; i++) payload[i] = 0;
    
    _43 idx = 0;
    payload[idx++] = 0xAA; payload[idx++] = 0xBB; /// Transaction ID
    payload[idx++] = 0x01; payload[idx++] = 0x00; /// Flags (Standard query)
    payload[idx++] = 0x00; payload[idx++] = 0x01; /// Questions = 1
    payload[idx++] = 0x00; payload[idx++] = 0x00; /// Answer RRs = 0
    payload[idx++] = 0x00; payload[idx++] = 0x00; /// Authority RRs = 0
    payload[idx++] = 0x00; payload[idx++] = 0x00; /// Additional RRs = 0
    
    /// Parse domain name to format: 3www6google3com0
    _43 label_len_idx = idx++;
    _184 current_len = 0;
    
    _43 d_idx = 0;
    _114(domain[d_idx] NEQ 0) {
        _184 c = domain[d_idx];
        _15(c EQ '.') {
            payload[label_len_idx] = current_len;
            label_len_idx = idx++;
            current_len = 0;
        } _41 {
            payload[idx++] = c;
            current_len++;
        }
        d_idx++;
    }
    payload[label_len_idx] = current_len;
    payload[idx++] = 0x00; /// Terminating zero
    
    payload[idx++] = 0x00; payload[idx++] = 0x01; /// Type A
    payload[idx++] = 0x00; payload[idx++] = 0x01; /// Class IN
    
    _89 d_ip = ip_str_to_u32(dns_ip);
    send_udp_raw(d_ip, 50000 + (random()%1000), 53, payload, idx);
    str_cpy(cmd_status, "DNS QUERY GESENDET!");
}

extern "C" _50 parse_html();
extern "C" _50 send_dhcp_discover() {
    /// BARE METAL FIX: Sicherheits-Check! 
    if (mac_addr[0] == 0 && mac_addr[1] == 0 && mac_addr[2] == 0) {
        str_cpy(cmd_status, "ERR: NIC NOT INITIALIZED!");
        return; 
    }

    /// ========================================================
    /// BARE METAL FIX: LINK-GUARD ENTFERNT!
    /// Wir pushen das DHCP-Paket immer in den TX Ring. Die Hardware
    /// überträgt es automatisch, sobald der Link physisch steht.
    /// ========================================================

    /// Array sicher nullen
    _184* dhcp = global_dhcp_buf;
    _39(_43 i=0; i<300; i++) dhcp[i] = 0;
    dhcp[0] = 1; /// Boot Request
    dhcp[1] = 1; /// Ethernet
    dhcp[2] = 6; /// MAC Length
    dhcp[3] = 0; /// Hops
    /// Transaktions-ID (Zufall, wichtig für den Router)
    dhcp[4] = 0x12; dhcp[5] = 0x34; dhcp[6] = 0x56; dhcp[7] = 0x78;
    /// BARE METAL FIX: Broadcast Flag (Unverzichtbar auf Hardware!)
    dhcp[10] = 0x80; dhcp[11] = 0x00;
    /// BARE METAL FIX: Client MAC tief im Payload eintragen (ab Byte 28)
    dhcp[28] = mac_addr[0]; dhcp[29] = mac_addr[1]; dhcp[30] = mac_addr[2];
    dhcp[31] = mac_addr[3]; dhcp[32] = mac_addr[4]; dhcp[33] = mac_addr[5];
    /// BARE METAL FIX: Magic Cookie (Das Passwort für den Router)
    dhcp[236] = 99; dhcp[237] = 130; dhcp[238] = 83; dhcp[239] = 99;
    _43 opt = 240;
    /// Option 53: Message Type = Discover
    dhcp[opt++] = 53; dhcp[opt++] = 1; dhcp[opt++] = 1;
    /// ========================================================
    /// BARE METAL FIX: DIE FRITZBOX-ZICKE BERUHIGEN!
    /// ========================================================
    /// Option 61: Client Identifier - Die FritzBox BRAUCHT das zwingend!
    dhcp[opt++] = 61; dhcp[opt++] = 7; dhcp[opt++] = 1; 
    dhcp[opt++] = mac_addr[0]; dhcp[opt++] = mac_addr[1]; dhcp[opt++] = mac_addr[2];
    dhcp[opt++] = mac_addr[3]; dhcp[opt++] = mac_addr[4]; dhcp[opt++] = mac_addr[5];
    /// Option 12: Hostname (Wir nennen das OS "COSMOS")
    dhcp[opt++] = 12; dhcp[opt++] = 6; 
    dhcp[opt++] = 'C'; dhcp[opt++] = 'O'; dhcp[opt++] = 'S'; 
    dhcp[opt++] = 'M'; dhcp[opt++] = 'O'; dhcp[opt++] = 'S';
    /// Option 55: Parameter Request List (Sag dem Router, was du wissen willst)
    dhcp[opt++] = 55; dhcp[opt++] = 3; 
    dhcp[opt++] = 1; /// 1. Gib mir eine Subnetzmaske
    dhcp[opt++] = 3; /// 2. Gib mir deine Router-IP (Gateway)
    dhcp[opt++] = 6; /// 3. Gib mir einen DNS-Server
    /// Option 255: Ende des Pakets
    dhcp[opt++] = 255;

    /// BARE METAL FIX: Keine 30-Sekunden Freeze-Schleife mehr!
    /// Ein langes Blockieren des OS hindert den Nutzer daran, RX-Updates zu sehen.
    /// Wenn der Port wegen STP geblockt ist, muss der Nutzer einfach nach 10 Sekunden
    /// nochmal auf DHCP klicken!
    send_udp_raw(0xFFFFFFFF, 68, 67, dhcp, 300);
    
    str_cpy(cmd_status, "DHCP DISCOVER SENT. WAITING FOR RX...");
}
/// =======================================================
/// DAS STETHOSKOP: LINK-STATUS PRÜFEN
/// =======================================================
extern "C" _50 net_check_link() {
    /// 1. INTEL CHECK
    _15(intel_mem_base > 0) {
        _89 status = mmio_read32(intel_mem_base + 0x0008);
        _15((status & 0x02) NEQ 0) {
            str_cpy(cmd_status, "LINK UP! CABLE DETECTED.");
        } _41 {
            str_cpy(cmd_status, "LINK DOWN! PHY ASLEEP / NO CABLE.");
        }
    } 
    /// 2. REALTEK CHECK
    _41 _15(rtl_io_base > 0) {
        str_cpy(cmd_status, "RTL8139: READY & WAITING");
    } 
    /// 3. FEHLER
    _41 {
        str_cpy(cmd_status, "ERR: NO NIC INIT");
    }
}
