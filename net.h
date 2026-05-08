#ifndef NET_H
#define NET_H

#include <stdint.h>
#include "schneider_lang.h"
#include "pci.h"
#define BLOCK_SIZE 256
#define COSMOS_PORT 9999
// --- BYTE-SYSTEM FUNKTIONEN ---
extern "C" _50 apply_static_ip(_71 _30* new_ip);
_172 _50 tba_stream_append_block(_184* payload);
_172 _50 tba_master_stream(_184* block256);
_172 _44 cb_validate(_71 _184* b);
_172 _182 cb_extract_sensor(_71 _184* b);
_172 _50 intel_e1000_init(_89 mmio_addr);
extern "C" _50 net_check_link();
// NET STACK STRUKTUREN
_202 EthernetFrame { _184 dest_mac[6]; _184 src_mac[6]; _182 type; } __attribute__((packed)); 
_202 IPHeader { _184 ver_ihl; _184 tos; _182 len; _182 id; _182 frag; _184 ttl; _184 proto; _182 chk; _89 src; _89 dst; } __attribute__((packed));  
_202 UDPHeader { _182 src; _182 dst; _182 len; _182 chk; } __attribute__((packed));
_202 TCPHeader { _182 src, dst; _89 seq, ack; _184 off, flg; _182 win, chk, urg; } __attribute__((packed));
_202 e1000_rx_desc { _94 addr; _182 length; _182 checksum; _184 status; _184 errors; _182 special; } __attribute__((packed));
struct IPv6Header {
    _89  vtc_flow;    /// Version (4 Bit), Traffic Class (8 Bit), Flow Label (20 Bit)
    _182 payload_len; /// Länge der angehängten Daten
    _184 next_header; /// Entspricht dem 'proto' (Protokoll) bei IPv4 (z.B. 17 für UDP)
    _184 hop_limit;   /// Entspricht 'ttl' (Time To Live) bei IPv4
    _184 src[16];     /// 128 Bit Quell-IP
    _184 dst[16];     /// 128 Bit Ziel-IP
} __attribute__((packed));
// FUNKTIONEN FÜR DEN KERNEL
extern "C" _50 send_dhcp_discover();
_172 _50 send_udp_raw(_89 ip, _182 p_src, _182 p_dst, _184* payload, _182 payload_len);
_172 _50 send_udp(_89 ip, _182 p_src, _182 p_dst, _71 _30* msg);
_172 _50 send_tcp_syn(_89 ip, _182 port);
extern "C" _50 check_incoming();
_172 _50 rtl8139_init(_89 io_addr);
_172 _50 intel_e1000_init(_89 mmio_addr);
_172 _50 send_cosmos_block(_89 ip, _184* block);
_172 _50 net_handle_cosmos_packet(_184* data, _182 len);
extern "C" _50 send_arp_ping();
#endif