#include "net.h"
#define BIG_BLOCK_SIZE 256
#include "schneider_lang.h"
/// ========================================================
/// BARE METAL FIX: DIE HARTEN DMA-ZONEN FÜR INTEL & RTL
/// ========================================================
#ifdef __x86_64__
    /// OS2 (64-BIT): Wir zwingen die Intel-Ringe auf die absolut sichere 14-Megabyte-Marke!
    #define e1000_rx_ring    ((volatile unsigned int*)0x00E00000)
    #define e1000_tx_ring    ((volatile unsigned int*)0x00E10000)
    #define e1000_rx_buffers ((volatile unsigned char (*)[4096])0x00E20000)
    #define e1000_tx_buffers ((volatile unsigned char (*)[4096])0x00E40000)
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
_172 _50 mmio_write32(_89 addr, _89 val);
_172 _89 mmio_read32(_89 addr);
_172 _50 tba_master_stream(_184* network_payload);
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
_172 _184 mac_addr[6];
_172 _30 mac_str[24];
#ifdef __x86_64__
    /// ==========================================
    /// OS2 (64-BIT): net.cpp ist der Chef und setzt harte Puffer!
    /// ==========================================
    _184* tx_buffer     = (_184*)0x00C10000; 
    _184* rx_buffer_rtl = (_184*)0x00C00000; 
    _30 ip_address[32]  = "0.0.0.0 (OFFLINE)";
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
_172 _30 cmd_status[32];
_30 net_mask[32] = "255.255.255.0";
_30 gateway_ip[32] = "0.0.0.0";
_172 _30 cmd_last_out[128];
_172 NICInfo found_nics[5];
_172 _43 active_nic_idx;
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
        __asm__ _192("pause"); 
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
        str_cpy(cmd_status, "RTL: NATIVE TX FIRED!");
    }
    _15(intel_mem_base > 0) {
        _89 mac_status = mmio_read32(intel_mem_base + 0x0008);
        _15((mac_status & 0x02) EQ 0) {
            str_cpy(cmd_status, "INTEL ERR: LINK DOWN (WAIT!)");
            _96; 
        }
        /// Den legalen RAM-Bereich für das aktuelle Paket holen
        _184* target_buf = (_184*)e1000_tx_buffers[tx_cur_intel];
        _39(_89 i = 0; i < l; i++) target_buf[i] = ((_184*)d)[i];
        _182 send_len = l;

        _15(send_len < 60) {
            _39(_43 i = send_len; i < 60; i++) target_buf[i] = 0;
            send_len = 60;
        }

        volatile unsigned int* dma_desc = (volatile unsigned int*)&e1000_tx_ring[tx_cur_intel * 4];
        
        /// Hier ebenfalls den Cast-Trick anwenden
		uint64_t phys_buf = (uint64_t)&e1000_tx_buffers[tx_cur_intel][0];
        dma_desc[0] = (uint32_t)(phys_buf & 0xFFFFFFFF);
        dma_desc[1] = (uint32_t)((phys_buf >> 32) & 0xFFFFFFFF);
        dma_desc[2] = send_len | 0x0B000000; 
        dma_desc[3] = 0; 
        
        /// Cache zwingend leeren!
        __asm__ _192("wbinvd" ::: "memory");
        
        tx_cur_intel = (tx_cur_intel + 1) % 32;
        mmio_write32(intel_mem_base + 0x3818, tx_cur_intel);
        
        /// =======================================================
        /// BARE METAL FIX 3: DER WAHRE LÜGENDETEKTOR (RAM statt MMIO)
        /// =======================================================
        _44 tx_done = _86; 
        _39(_89 wait = 0; wait < 1500000; wait++) {
            /// BARE METAL FIX: Zwinge die CPU, den echten RAM zu lesen!
            __asm__ _192("clflush (%0)" :: "r"(&dma_desc[3]));
            
            _15((dma_desc[3] & 0x01) NEQ 0) { 
                tx_done = _128; 
                _96; 
            }
            __asm__ _192("pause" ::: "memory");
        }

        _15(tx_done) {
            str_cpy(cmd_status, "TX SUCCESS: PAKET IST IM KABEL!");
        } _41 {
            str_cpy(cmd_status, "TX TIMEOUT (ABER TX=1 BEDEUTET GESENDET)");
        }
	}
}
_50 net_ip(_89 dst, _50* p_data, _182 p_len, _184 proto) {
    /// BARE METAL FIX: Roher RAM (32 MB) - Kein Stack, keine BSS-Sektion!
    _184* b = global_ip_buf;
    EthernetFrame* e=(EthernetFrame*)b; 
    IPHeader* i=(IPHeader*)(b+14);
    
    _39(_43 k=0;k<6;k++){e->dest_mac[k]=0xFF; e->src_mac[k]=mac_addr[k];} 
    e->type=hs(0x0800);
    
    i->ver_ihl=0x45; i->len=hs(20+p_len); i->id=hs(random()); i->frag=hs(0x4000);
    i->ttl=64; i->proto=proto;
    _15(dst EQ 0xFFFFFFFF) i->src = 0; 
    _41 i->src = hl(0x0A00020F);
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
   /// 3. Sender MAC & Dummy IP (Wir tun so, als wären wir die .99)
    _39(_43 i = 0; i < 6; i++) frame[22+i] = mac_addr[i]; 
    frame[28] = 192; frame[29] = 168; frame[30] = 14; frame[31] = 99; 
    
    /// 4. Target MAC (Null) & Target IP (Die 14.14 - Deine FritzBox!)
    _39(_43 i = 0; i < 6; i++) frame[32+i] = 0x00; 
    frame[38] = 192; frame[39] = 168; frame[40] = 14; frame[41] = 14;
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
    net_ip(ip, pl, 8 + payload_len, 17);
}
_50 send_tcp_syn(_89 ip, _182 port) {
    _184 pl[64]; TCPHeader* t=(TCPHeader*)pl;
    t->src=hs(49152); t->dst=hs(port); t->seq=hl(random()); t->ack=0;
    t->off=0x50; t->flg=0x02; t->win=hs(8192); t->chk=0; t->urg=0;
    net_ip(ip, pl, 20, 6);
    str_cpy(cmd_status, "TCP: SYN SENT");
}
_50 send_tcp_ack(_89 ip, _182 p_src, _182 p_dst, _89 seq, _89 ack_num) {
    _184 pl[64]; TCPHeader* t = (TCPHeader*)pl;
    t->src = hs(p_src); 
    t->dst = hs(p_dst); 
    t->seq = hl(seq); 
    t->ack = hl(ack_num);
    t->off = 0x50; 
    t->flg = 0x10;
    t->win = hs(8192); 
    t->chk = 0; 
    t->urg = 0;
    net_ip(ip, pl, 20, 6);
    str_cpy(cmd_status, "TCP: CONNECTED (ACK)");
}
_50 rtl_enable_rx() {
    outl(rtl_io_base + 0x30, (_89)(uintptr_t)rx_buffer_rtl);
    outw(rtl_io_base + 0x3C, 0x0005); 
    outl(rtl_io_base + 0x44, 0x0F | 0x80);
    outb(rtl_io_base + 0x37, 0x0C); 
}
/// =======================================================
/// DIE NETZWERK-FUNKTIONEN
/// =======================================================
_50 e1000_enable_tx() {
    _15(intel_mem_base EQ 0) _96;

    _89 ctrl = mmio_read32(intel_mem_base + 0x0000);
    mmio_write32(intel_mem_base + 0x0000, ctrl | 0x40 | 0x20 | 0x01);
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF); /// Interrupts aus!

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
    
    mmio_write32(intel_mem_base + 0x0400, 0x0103F0FA); 
    mmio_write32(intel_mem_base + 0x0410, 0x0060200A); 
    
    /// BARE METAL FIX: TX Queue sauber starten (ohne BIOS-Altlasten)
    /// 0x02000000 = Queue Enable | 0x00010000 = Granularity
    mmio_write32(intel_mem_base + 0x3828, 0x02010000);
    _39(_89 wait=0; wait<1000000; wait++) {
        _15((mmio_read32(intel_mem_base + 0x3828) & 0x02000000) NEQ 0) break;
        __asm__ _192("pause");
    }
}

_50 e1000_enable_rx() {
    _15(intel_mem_base EQ 0) _96;
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
    mmio_write32(intel_mem_base + 0x2810, 0);        
    mmio_write32(intel_mem_base + 0x2818, 31);
    
    _89 rxdctl = mmio_read32(intel_mem_base + 0x2828);
    mmio_write32(intel_mem_base + 0x2828, rxdctl | 0x02000000); 
    
    _39(_43 i = 0; i < 128; i++) { mmio_write32(intel_mem_base + 0x5200 + (i * 4), 0); }
    mmio_write32(intel_mem_base + 0x0100, 0x0400801E);
}

_50 intel_e1000_init(_89 mmio_addr) {
    intel_mem_base = mmio_addr & 0xFFFFFFF0;
    _15(intel_mem_base EQ 0) _96;

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
    /// 3. BARE METAL FIX: HARD-RESET + KUPFER-RESET (PHY)
    /// =======================================================
    /// Dein 0x04000000 war ein Soft-Reset. Wir brauchen auf Gaming-PCs 
    /// zusätzlich 0x80000000 (PHY Reset), um das Kupferkabel zu wecken!
    _89 ctrl_rst = mmio_read32(intel_mem_base + 0x0000);
    ///mmio_write32(intel_mem_base + 0x0000, ctrl_rst | 0x04000000);
    
    /// Warten, bis der Chip neu gebootet ist
    _39(_192 _89 delay = 0; delay < 1000000; delay++) { __asm__ _192("pause"); }
    
    /// Alte Interrupts löschen
    mmio_write32(intel_mem_base + 0x00D8, 0xFFFFFFFF);
	/// =======================================================
    /// BARE METAL FIX: WAKE-ON-LAN (ZOMBIE-MODUS) BEENDEN!
    /// =======================================================
    mmio_write32(intel_mem_base + 0x5800, 0); /// WUC (Wake Up Control) abschalten
    mmio_write32(intel_mem_base + 0x5808, 0); /// WUFC (Wake Up Filter) abschalten
    mmio_write32(intel_mem_base + 0x5810, 0); /// WUS (Wake Up Status) löschen
    
    /// =======================================================
    /// 4. MAC-ADRESSE WIEDER IN DEN CHIP SCHREIBEN!
    /// (Der Reset hat sie aus dem Chip gelöscht!)
    /// =======================================================
    _89 new_ral = mac_addr[0] | (mac_addr[1] << 8) | (mac_addr[2] << 16) | (mac_addr[3] << 24);
    _89 new_rah = mac_addr[4] | (mac_addr[5] << 8) | 0x80000000; /// 0x80000000 = Address Valid!
    mmio_write32(intel_mem_base + 0x5400, new_ral);
    mmio_write32(intel_mem_base + 0x5404, new_rah);

    /// --- SANFTER STOPP UND LINK UP ---
    mmio_write32(intel_mem_base + 0x0100, 0); 
    mmio_write32(intel_mem_base + 0x0400, 0); 
    _39(_192 _89 delay = 0; delay < 1000000; delay++) { __asm__ _192("pause"); }
    
    /// Link Up (SLU) und Auto-Speed (ASDE) erzwingen
    _89 ctrl = mmio_read32(intel_mem_base + 0x0000);
    ///mmio_write32(intel_mem_base + 0x0000, ctrl | 0x00000060);
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
        
        /// BARE METAL FIX: DER HARDWARE-CACHE VERNICHTER!
        /// Bevor wir lesen, zwingen wir die CPU, ihren Cache wegzuwerfen!
        __asm__ _192("wbinvd" ::: "memory");

        volatile unsigned int* rx_desc = (volatile unsigned int*)&e1000_rx_ring[rx_cur_intel * 4];
        
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

        /// --- VLAN-FILTER ---
        _43 off = 0;
        _15(raw_data[12] EQ 0x81 AND raw_data[13] EQ 0x00) off = 4;
        _44 packet_was_important = _86;

        /// --- 1. WEICHE: ARP ---
        _15(len >= 42+off AND raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x06) {
            _15(raw_data[21+off] EQ 0x02) { 
                str_cpy(cmd_status, "ONLINE (ARP OK)!");
                _30* ip_ptr = ip_address;
                int_to_str(raw_data[28+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                int_to_str(raw_data[29+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                int_to_str(raw_data[30+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                int_to_str(raw_data[31+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr = 0;
                packet_was_important = _128;
            }
        }

        /// --- 2. WEICHE: DHCP (THE ULTIMATE PARSER) ---
        _41 _15(len > 280+off AND raw_data[12+off] EQ 0x08 AND raw_data[13+off] EQ 0x00 AND raw_data[23+off] EQ 0x11) {
            _15(raw_data[36+off] EQ 0x00 AND raw_data[37+off] EQ 68 AND raw_data[42+off] EQ 0x02) {
                _15(raw_data[278+off] EQ 0x63 AND raw_data[279+off] EQ 0x82 AND raw_data[280+off] EQ 0x53 AND raw_data[281+off] EQ 0x63) {
                    
                    _30* ip_ptr = ip_address;
                    int_to_str(raw_data[58+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                    int_to_str(raw_data[59+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                    int_to_str(raw_data[60+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr++ = '.';
                    int_to_str(raw_data[61+off], ip_ptr); _114(*ip_ptr) ip_ptr++; *ip_ptr = 0;
                    
                    _43 j = 282 + off; /// Geändert zu j, um Konflikte zu vermeiden
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
                        j += 2 + opt_len; 
                    }
                    
                    _15(msg_type EQ 2) str_cpy(cmd_status, "DHCP OFFER RX (IP GEFUNDEN!)");
                    _41 _15(msg_type EQ 5) str_cpy(cmd_status, "DHCP ACK RX (ONLINE!)");
                    _41 str_cpy(cmd_status, "DHCP RX: PROTOCOL OK");
                    packet_was_important = _128;
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
            }
            /// ====================================================
            /// 2. IST ES EIN IPv4-PAKET? (EtherType 0x0800)
            /// ====================================================
            _15(raw_data[12] EQ 0x08 AND raw_data[13] EQ 0x00) {
                /// Ist es UDP? (Protocol-Feld im IP-Header ist 17)
                _15(raw_data[23] EQ 17) {
                    /// Ist es DHCP? (Ziel-Port ist 68 / 0x0044)
                    _15(raw_data[36] EQ 0x00 AND raw_data[37] EQ 0x44) {
                        str_cpy(cmd_status, "DHCP OFFER EMPFANGEN!");
                        /// BINGO! Wir fischen die IP aus dem Paket (Offset 58)
                        _43 ip1 = raw_data[58];
                        _43 ip2 = raw_data[59];
                        _43 ip3 = raw_data[60];
                        _43 ip4 = raw_data[61];
                        /// Die alte IP löschen und die neue in den String bauen
                        ip_address[0] = 0; 
                        _30 tmp[10];
                        int_to_str(ip1, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip2, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip3, tmp); str_cat(ip_address, tmp); str_cat(ip_address, ".");
                        int_to_str(ip4, tmp); str_cat(ip_address, tmp);
                    }
                }
            }
            /// Ringpuffer weiterschieben (wie in deinem Original-Code)
            rx_idx_rtl = (rx_idx_rtl + rx_len + 4 + 3) & ~3; 
            _15(rx_idx_rtl > 8192) rx_idx_rtl = 0; 
            outw(rtl_io_base + 0x38, rx_idx_rtl - 16); 
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
extern "C" _50 send_dhcp_discover() {
    /// BARE METAL FIX: Sicherheits-Check! 
    /// Wenn die MAC-Adresse noch 00:00... ist, ist die Karte nicht initialisiert!
    if (mac_addr[0] == 0 && mac_addr[1] == 0 && mac_addr[2] == 0) {
        str_cpy(cmd_status, "ERR: NIC NOT INITIALIZED!");
        return; /// Sofort abbrechen, bevor die CPU abstürzt!
    }
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
    /// Absenden an Port 68 (Client) nach 67 (Server) via UDP Broadcast
    send_udp_raw(0xFFFFFFFF, 68, 67, dhcp, 300);
    /// KEIN STATUS-ÜBERSCHREIBEN MEHR HIER! 
    /// Wir lassen "INTEL: TX PUSHED" aus der net_raw() stehen!
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