/// ==========================================
/// COSMOS OS - USB BULK-ONLY TRANSPORT (BOT)
/// ==========================================
#include "schneider_lang.h"
#pragma pack(push, 1)

/// ==========================================
/// BARE METAL FIX: xHCI Globale Statusvariablen (64-BIT SAFE)
/// ==========================================
uint64_t global_xhci_base_addr = 0;
uint64_t global_xhci_event_ring_base = 0;
uint32_t global_xhci_event_dequeue_idx = 0;
uint32_t global_xhci_event_cycle = 1; 
uint64_t global_xhci_runtime_regs = 0;

struct USB_CBW {
    _89 signature;       
    _89 tag;             
    _89 transfer_len;    
    _184 flags;          
    _184 lun;            
    _184 cmd_len;        
    _184 scsi_cmd[16];   
};
struct USB_CSW {
    _89 signature;       
    _89 tag;             
    _89 data_residue;    
    _184 status;         
};
#pragma pack(pop)

uint32_t global_mouse_slot = 0;
uint32_t mouse_trb_idx = 0;
uint32_t mouse_trb_cycle = 1;
extern uint32_t xhci_db_base;

void xhci_submit_mouse_read() {
    if (global_mouse_slot == 0) return;
    
    volatile uint32_t* in_ring = (volatile uint32_t*)0x040A0000;
    
    /// BARE METAL FIX: Puffer Adresse ist jetzt 64-Bit!
    uint64_t buffer_addr = 0x06500000 + (mouse_trb_idx * 8);
    
    volatile uint8_t* prep_buf = (volatile uint8_t*)buffer_addr;
    for(int i=0; i<28; i++) prep_buf[i] = 0;
    
    /// BARE METAL FIX: Die 64-Bit Adresse sauber in zwei 32-Bit Blöcke (TRB 0 und 1) aufteilen!
    in_ring[mouse_trb_idx * 4 + 0] = (uint32_t)(buffer_addr & 0xFFFFFFFF); 
    in_ring[mouse_trb_idx * 4 + 1] = (uint32_t)(buffer_addr >> 32);
    in_ring[mouse_trb_idx * 4 + 2] = 8;          
    in_ring[mouse_trb_idx * 4 + 3] = (1 << 10) | (1 << 5) | mouse_trb_cycle; 
    
    mouse_trb_idx++;
    
    if (mouse_trb_idx == 63) {
        uint32_t link_ctrl = in_ring[63 * 4 + 3];
        in_ring[63 * 4 + 3] = (link_ctrl & ~1) | mouse_trb_cycle;
        mouse_trb_idx = 0;
        mouse_trb_cycle ^= 1;
    }
    
    volatile uint32_t* db_array = (volatile uint32_t*)xhci_db_base;
    db_array[global_mouse_slot] = 3; 
}

/// ==========================================
/// UHCI HOST CONTROLLER (LEGACY)
/// ==========================================
_172 _89 usb_io_base; 
_172 _50 outw(_182 p, _182 v);
_172 _182 inw(_182 p);

struct UHCI_TD {
    _89 link_ptr;      
    _89 ctrl_status;   
    _89 token;         
    _89 buffer_ptr;    
    _89 pad[4];        
} __attribute__((aligned(16)));

_89* uhci_frame_list = (_89*)0x02000000; 
UHCI_TD* current_td  = (UHCI_TD*)0x02001000;

_89 bulk_toggle_out = 0;
_89 bulk_toggle_in = 0;

_44 uhci_execute_td(_89 pid, _43 dev_addr, _43 ep, void* buffer, _43 len, _89 toggle) {
    _15(usb_io_base EQ 0) _96 _86;
    outw(usb_io_base + 8, 0x02000000 & 0xFFFF);
    outw(usb_io_base + 10, 0x02000000 >> 16);
    current_td->link_ptr = 1;
    current_td->ctrl_status = (1 << 23) | (3 << 27);
    _89 max_len = (len > 0) ? (len - 1) : 0x7FF; 
    current_td->token = (max_len << 21) | ((toggle & 1) << 19) | ((ep & 0x0F) << 15) | (dev_addr << 8) | pid;
    current_td->buffer_ptr = (_89)(uint64_t)buffer;
    _39(_43 i=0; i<1024; i++) uhci_frame_list[i] = (_89)(uint64_t)current_td;
    outw(usb_io_base, inw(usb_io_base) | 0x0001);
    _43 timeout = 1000000;
    _114((current_td->ctrl_status & (1 << 23)) AND timeout > 0) {
        __asm__ _192("pause");
        timeout--;
    }
    outw(usb_io_base, inw(usb_io_base) & ~0x0001);
    _15(timeout EQ 0 OR (current_td->ctrl_status & 0x7E0000)) _96 _86;
    _96 _128; 
}

_44 usb_bulk_out(_43 dev_addr, _43 ep, void* buffer, _43 len) {
    _44 res = uhci_execute_td(0xE1, dev_addr, ep, buffer, len, bulk_toggle_out); 
    bulk_toggle_out ^= 1;
    _96 res;
}
_44 usb_bulk_in(_43 dev_addr, _43 ep, void* buffer, _43 len) {
    _44 res = uhci_execute_td(0x69, dev_addr, ep, buffer, len, bulk_toggle_in); 
    bulk_toggle_in ^= 1;
    _96 res;
}

/// ==========================================
/// XHCI INITIALISIERUNG (FINAL WIRE-UP)
/// ==========================================
extern char hw_usb[48]; 
extern char cmd_status[256];
extern void str_cpy(char* d, const char* s); 

extern uint32_t xhci_db_base;
extern uint32_t* xhci_dcbaa;
extern void xhci_power_on_ports(uint32_t op_base, int max_ports);
extern uint8_t xhci_check_ports(uint32_t op_base, int max_ports);
extern "C" uint32_t xhci_bot_get_capacity(uint8_t slot_id);

void init_xhci(uint32_t bar0) {}
extern _50 xhci_bios_handoff(_89 cap_base);

extern void map_mmio_64(uint64_t phys_addr);

int init_xhci_probe(uint32_t bar0, int id) {
    global_xhci_base_addr = bar0;
    
    /// BARE METAL FIX: xHCI MMIO muss in den 64-Bit Page Tables gemappt sein!
    map_mmio_64(bar0);
    /// Der xHCI BAR kann mehrere 2MB-Regionen umfassen
    map_mmio_64(bar0 + 0x200000);
    
    xhci_bios_handoff(global_xhci_base_addr);
    uint8_t caplength = *((volatile uint8_t*)global_xhci_base_addr);
    
    const char hex_chars[] = "0123456789ABCDEF";
    char diag[32];
    diag[0] = 'I'; diag[1] = 'D'; diag[2] = ':';
    diag[3] = '0' + id; diag[4] = ' '; 
    diag[5] = 'C'; diag[6] = ':';
    diag[7] = hex_chars[(caplength >> 4) & 0xF];
    diag[8] = hex_chars[caplength & 0xF];
    diag[9] = ' '; diag[10] = 0;

    int offset = 0;
    while(hw_usb[offset] != 0) offset++; 
    int i = 0;
    while(diag[i] != 0) { hw_usb[offset + i] = diag[i]; i++; }
    hw_usb[offset + i] = 0;              

    if (caplength != 0xFF) return 1; 
    return 0; 
}

/// ==========================================
/// USB HANDSCHLAG V2 & V3 (LEGACY)
/// ==========================================
#pragma pack(push, 1)
struct USB_SETUP_PKT {
    _184 request_type;
    _184 request;
    _182 value;
    _182 index;
    _182 length;
};
#pragma pack(pop)

USB_SETUP_PKT* global_setup = (USB_SETUP_PKT*)0x02002000;
_44 uhci_execute_control(_43 dev_addr) {
    outw(usb_io_base + 8, 0x02000000 & 0xFFFF);
    outw(usb_io_base + 10, 0x02000000 >> 16);
    current_td->link_ptr = 1;
    current_td->ctrl_status = (1 << 23) | (3 << 27);
    current_td->token = (7 << 21) | (0 << 19) | (0 << 15) | (dev_addr << 8) | 0x2D; 
    current_td->buffer_ptr = (_89)(uint64_t)global_setup;
    _39(_43 i=0; i<1024; i++) uhci_frame_list[i] = (_89)(uint64_t)current_td;
    outw(usb_io_base, inw(usb_io_base) | 0x0001); 
    _43 timeout = 1000000;
    _114((current_td->ctrl_status & (1 << 23)) AND timeout > 0) { __asm__ _192("pause"); timeout--; }
	// DEBUG-INFO HINZUFÜGEN:
    _15(current_td->ctrl_status & 0x7E0000) {
        // Hier könntest du dir den Error-Code ausgeben lassen:
        // Bit 17=CRC, 18=Bitstuff, 19=STALL, 20=NAK, 21=Babble, 22=DB
        str_cpy(cmd_status, "USB: CONTROL ERROR DETECTED");
    }
    outw(usb_io_base, inw(usb_io_base) & ~0x0001); 
    _15(timeout EQ 0 OR (current_td->ctrl_status & 0x7E0000)) _96 _86;
    current_td->link_ptr = 1;
    current_td->ctrl_status = (1 << 23) | (3 << 27);
    current_td->token = (0x7FF << 21) | (1 << 19) | (0 << 15) | (dev_addr << 8) | 0x69; 
    current_td->buffer_ptr = 0;
    _39(_43 i=0; i<1024; i++) uhci_frame_list[i] = (_89)(uint64_t)current_td;
    outw(usb_io_base, inw(usb_io_base) | 0x0001); 
    timeout = 1000000;
    _114((current_td->ctrl_status & (1 << 23)) AND timeout > 0) { __asm__ _192("pause"); timeout--; }
    outw(usb_io_base, inw(usb_io_base) & ~0x0001); 
    _15(timeout EQ 0 OR (current_td->ctrl_status & 0x7E0000)) _96 _86;
    _96 _128;
}

_50 uhci_safe_delay(_43 loops) { _39(volatile _43 i = 0; i < loops; i++) { __asm__ _192("pause"); } }

_44 usb_enumerate_device(_43 port_idx, _43 new_address) {
    _182 port_reg = usb_io_base + 0x10 + (port_idx * 2);
    _182 safe_p = inw(port_reg) & 0xFFD5;
    outw(port_reg, safe_p | 0x0200); 
    uhci_safe_delay(1000000);
    safe_p = inw(port_reg) & 0xFFD5;
    outw(port_reg, safe_p & ~0x0200);
    uhci_safe_delay(250000);
    safe_p = inw(port_reg) & 0xFFD5;
    outw(port_reg, safe_p | 0x0004); 
    uhci_safe_delay(250000);
    _15(!(inw(port_reg) & 0x0004)) _96 _86;
    global_setup->request_type = 0x00; 
    global_setup->request = 0x05;      
    global_setup->value = new_address; 
    global_setup->index = 0;
    global_setup->length = 0;
    _15(!uhci_execute_control(0)) _96 _86;
    uhci_safe_delay(250000);
    global_setup->request_type = 0x00;
    global_setup->request = 0x09;      
    global_setup->value = 1;           
    global_setup->index = 0;
    global_setup->length = 0;
    _15(!uhci_execute_control(new_address)) _96 _86;
    _96 _128;
}

_44 usb_bot_read_sectors(_43 dev_addr, _43 ep_out, _43 ep_in, _89 lba, _43 num_sectors, _184* buffer) {
    USB_CBW cbw;
    _39(_43 i=0; i<sizeof(cbw); i++) ((_184*)&cbw)[i] = 0;
    cbw.signature = 0x43425355; cbw.tag = 0x00000001; cbw.transfer_len = num_sectors * 512;
    cbw.flags = 0x80; cbw.lun = 0; cbw.cmd_len = 10;
    cbw.scsi_cmd[0] = 0x28; cbw.scsi_cmd[2] = (lba >> 24) & 0xFF; cbw.scsi_cmd[3] = (lba >> 16) & 0xFF;
    cbw.scsi_cmd[4] = (lba >> 8) & 0xFF; cbw.scsi_cmd[5] = lba & 0xFF;
    cbw.scsi_cmd[7] = (num_sectors >> 8) & 0xFF; cbw.scsi_cmd[8] = num_sectors & 0xFF;
    _15(!usb_bulk_out(dev_addr, ep_out, &cbw, sizeof(cbw))) _96 _86;
    _15(!usb_bulk_in(dev_addr, ep_in, buffer, cbw.transfer_len)) _96 _86;
    USB_CSW csw;
    _15(!usb_bulk_in(dev_addr, ep_in, &csw, sizeof(csw))) _96 _86;
    _15(csw.signature NEQ 0x53425355 OR csw.status NEQ 0) _96 _86;
    _96 _128; 
}

_43 usb_bot_get_capacity(_43 dev_addr, _43 ep_out, _43 ep_in) {
    USB_CBW cbw;
    _39(_43 i=0; i<sizeof(cbw); i++) ((_184*)&cbw)[i] = 0;
    cbw.signature = 0x43425355; cbw.tag = 0x00000002; cbw.transfer_len = 8;
    cbw.flags = 0x80; cbw.lun = 0; cbw.cmd_len = 10; cbw.scsi_cmd[0] = 0x25;
    _15(!usb_bulk_out(dev_addr, ep_out, &cbw, sizeof(cbw))) _96 0;
    _184 capacity_data[8];
    _15(!usb_bulk_in(dev_addr, ep_in, capacity_data, 8)) _96 0;
    USB_CSW csw;
    _15(!usb_bulk_in(dev_addr, ep_in, &csw, sizeof(csw))) _96 0;
    _89 last_lba = (capacity_data[0] << 24) | (capacity_data[1] << 16) | (capacity_data[2] << 8) | capacity_data[3];
    _43 size_mb = (last_lba + 1) / 2048;
    _96 size_mb;
}

/// ==========================================
/// BARE METAL FIX: DER XHCI EVENT-RING CLEANER & MAUS-TREIBER (64-BIT SAFE!)
/// ==========================================
void (*usb_mouse_callback)(int dx, int dy, int btn) = 0; 

void xhci_poll_events_and_mouse() {
    if (!global_xhci_event_ring_base) return; 

    uint32_t curr_idx = global_xhci_event_dequeue_idx;
    uint32_t* trb = (uint32_t*)(global_xhci_event_ring_base + (curr_idx * 16));

    int total_dx = 0;
    int total_dy = 0;
    int final_btn = 0; 
    bool has_update = false;

    while ((trb[3] & 0x01) == global_xhci_event_cycle) {
        uint32_t trb_type = (trb[3] >> 10) & 0x3F;
        uint32_t ev_slot_id = (trb[3] >> 24) & 0xFF;
        uint32_t ev_ep_id   = (trb[3] >> 16) & 0x1F;
        
        if (trb_type == 32) { 
            uint32_t completion_code = (trb[2] >> 24) & 0xFF;
            
            if (ev_slot_id == global_mouse_slot && ev_ep_id == 3) {
                if (completion_code == 1 || completion_code == 13) { 
                    uint32_t remaining = trb[2] & 0xFFFFFF;
                    uint32_t bytes_read = 8 - remaining;
                    
                    /// BARE METAL FIX: 64-Bit Pointer Rekonstruktion!
                    uint64_t transfer_trb_addr = ((uint64_t)trb[1] << 32) | trb[0];
                    uint32_t* transfer_trb = (uint32_t*)transfer_trb_addr;
                    
                    uint64_t mouse_buf_addr = ((uint64_t)transfer_trb[1] << 32) | transfer_trb[0];
                    volatile uint8_t* mouse_buf = (volatile uint8_t*)mouse_buf_addr; 
                    
                    int btn = 0; int dx = 0; int dy = 0;
                    
                    if (mouse_buf[0] == 1 && bytes_read >= 4) {
                        btn = mouse_buf[1]; dx = (int8_t)mouse_buf[2]; dy = (int8_t)mouse_buf[3];
                    } else {
                        btn = mouse_buf[0]; dx = (int8_t)mouse_buf[1]; dy = (int8_t)mouse_buf[2];
                    }
                    
                    total_dx += dx; total_dy += dy; final_btn |= btn; 
                    has_update = true;
                    xhci_submit_mouse_read();
                }
            } 
        }
        
        curr_idx++;
        if (curr_idx >= 256) { 
            curr_idx = 0;
            global_xhci_event_cycle ^= 1; 
        }
        trb = (uint32_t*)(global_xhci_event_ring_base + (curr_idx * 16));
    }

    if (curr_idx != global_xhci_event_dequeue_idx) {
        global_xhci_event_dequeue_idx = curr_idx;
        volatile uint32_t* erdp_reg = (volatile uint32_t*)(global_xhci_runtime_regs + 0x38); 
        
        /// BARE METAL FIX: Den Ring-Pointer 64-Bit sicher in das Controller-Register schreiben!
        uint64_t phys_addr = global_xhci_event_ring_base + (curr_idx * 16);
        erdp_reg[0] = (uint32_t)(phys_addr & 0xFFFFFFFF) | 0x08; 
        erdp_reg[1] = (uint32_t)(phys_addr >> 32);
    }

    if (has_update && usb_mouse_callback != 0) {
        total_dx = total_dx / 2;
        total_dy = total_dy / 2;
        if (total_dx != 0 || total_dy != 0 || final_btn != 0) {
            usb_mouse_callback(total_dx, total_dy, final_btn); 
        }
    }
}
/// ==========================================
/// BARE METAL FIX: USB ROOT HUB AUTO-SCANNER
/// ==========================================

/// 1. Dem Compiler beibringen, wie ein DriveInfo aussieht!
_202 DriveInfo { _43 type; _43 size_mb; _43 base_port; _30 model[41]; };

/// 2. Saubere 'extern' (_172) Referenzen auf die globalen Variablen!
_172 DriveInfo drives[8];
_172 _43 drive_count;
_172 _30 cmd_status[256];

extern "C" _50 usb_scan_and_mount() {
    _43 usb_found = 0;
    
    /// BARE METAL FIX: xHCI (USB 3.0) SCAN FIRST
    _15(global_xhci_base_addr NEQ 0 AND drive_count < 8) {
        uint8_t caplength = *((volatile uint8_t*)global_xhci_base_addr);
        uint32_t op_base = global_xhci_base_addr + caplength;
        
        str_cpy(cmd_status, "xHCI: POWERING PORTS...");
        xhci_power_on_ports(op_base, 8); 
        
        str_cpy(cmd_status, "xHCI: SCANNING PORTS...");
        uint8_t slot_id = xhci_check_ports(op_base, 8);
        
        _15(slot_id > 0) {
            str_cpy(cmd_status, "xHCI: READING CAPACITY...");
            _43 size_mb = xhci_bot_get_capacity(slot_id);
            
            _15(size_mb > 0 AND drive_count < 8) {
                /// Only add on full success
                drives[drive_count].type = 3;
                drives[drive_count].size_mb = size_mb;
                drives[drive_count].base_port = slot_id;
                str_cpy(drives[drive_count].model, "USB 3.0 FLASH DRIVE");
                drive_count++;
                usb_found++;
            } _41 {
                str_cpy(cmd_status, "xHCI: SLOT OK, NO CAPACITY");
            }
        } _41 {
            str_cpy(cmd_status, "xHCI: NO DEVICE FOUND");
        }
    }

    /// UHCI (USB 1.1) SCAN
    _15(usb_io_base NEQ 0) {
        /// Ein UHCI-Root-Hub hat standardmäßig 2 Ports
        _39(_43 p = 0; p < 2; p++) {
            _182 port_reg = usb_io_base + 0x10 + (p * 2);
            
            /// Bit 0: Ist ein Gerät eingesteckt?
            _15(inw(port_reg) & 0x0001 AND drive_count < 8) { 
                
                /// --- PORT RESET ---
                outw(port_reg, 0x0200);
                _39(_43 wait=0; wait<1000000; wait++) __asm__ ("pause"); 
                outw(port_reg, 0x0000);
                _39(_43 wait=0; wait<1000000; wait++) __asm__ ("pause"); 
                outw(port_reg, 0x0004 | 0x0002);
                _39(_43 wait=0; wait<1000000; wait++) __asm__ ("pause"); 

                /// Port enabled?
                _15(inw(port_reg) & 0x0004) {
                    _43 dev_addr = p + 1;
                    
                    /// Enumerate (assign address)
                    _15(usb_enumerate_device(p, dev_addr)) {
                        _43 size_mb = usb_bot_get_capacity(dev_addr, 0x01, 0x81);
                        
                        _15(size_mb > 0) {
                            /// Only add to list on full success
                            drives[drive_count].size_mb = size_mb;
                            drives[drive_count].type = 3; 
                            drives[drive_count].base_port = dev_addr;
                            str_cpy(drives[drive_count].model, "USB FLASH DRIVE");
                            drive_count++;
                            usb_found++;
                        }
                    }
                }
            }
        }    
    }
    _15(usb_found > 0) {
        str_cpy(cmd_status, "USB: FLASH DRIVES MOUNTED!");
    }
}