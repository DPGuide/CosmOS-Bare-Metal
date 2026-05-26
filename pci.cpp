#include "pci.h"
#include "schneider_lang.h"
/// ==========================================
/// BARE METAL: DIE ECHTE 64-BIT IDT (MIT PIC REMAP)
/// ==========================================
#ifdef __x86_64__
struct IDT64_Entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct IDT64_Pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

IDT64_Entry idt[256];
IDT64_Pointer idtp;

extern "C" void isr_trap(); /// Für CPU-Crashes (0-31)
extern "C" void isr_hw();   /// Für Hardware wie USB/Maus (32-255)

/// Mini-Helfer für die Chip-Kommunikation
static inline void outb_idt(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void idt_set_gate(uint8_t num, uint64_t handler) {
    uint16_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    idt[num].offset_low = (uint16_t)handler;
    idt[num].selector = cs; 
    idt[num].ist = 0;
    idt[num].types_attr = 0x8E; 
    idt[num].offset_mid = (uint16_t)(handler >> 16);
    idt[num].offset_high = (uint32_t)(handler >> 32);
    idt[num].zero = 0;
}

void init_idt() {
    /// 1. PIC REMAPPING: Der wichtigste Schritt der Bare-Metal-Welt!
    /// Verschiebt die Hardware-Interrupts weg von den CPU-Crashes auf Platz 32+
    outb_idt(0x20, 0x11); outb_idt(0xA0, 0x11);
    outb_idt(0x21, 0x20); outb_idt(0xA1, 0x28); /// Master auf 32 (0x20), Slave auf 40
    outb_idt(0x21, 0x04); outb_idt(0xA1, 0x02);
    outb_idt(0x21, 0x01); outb_idt(0xA1, 0x01);
    outb_idt(0x21, 0x00); outb_idt(0xA1, 0x00); /// Masken auf 0: Alle IRQs durchlassen

    idtp.limit = (sizeof(IDT64_Entry) * 256) - 1;
    idtp.base = (uint64_t)&idt;

    /// 2. DIE TABELLE AUFBAUEN
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint64_t)isr_trap); /// Crash-Netz aufspannen
    }
    for (int i = 32; i < 256; i++) {
        idt_set_gate(i, (uint64_t)isr_hw);   /// Hardware-Handler (Maus/Keyboard) setzen
    }

    /// 3. ZÜNDUNG
    __asm__ volatile("lidt %0" : : "m"(idtp));
    __asm__ volatile("sti"); /// Interrupts auf der CPU scharfschalten!
}
#else
void init_idt() {} 
#endif

/// --- XHCI DMA POSTAMT (HARDCODED RAM) ---
/// Wir reservieren uns einfach die 10-Megabyte-Marke im Arbeitsspeicher!
/// 0x00A00000 ist exakt durch 64 teilbar (perfekt aligned) und hier stören wir niemanden.
extern int init_xhci_probe(uint32_t bar0, int id);
/// BARE METAL FIX: DCBAA und Command Ring (128 MB Marke)
uint64_t* xhci_dcbaa = (uint64_t*)0x08100000; 
uint64_t* xhci_cmd_ring = (uint64_t*)0x08010000;

/// NEU: Der Briefkasten (Event Ring & Segment Table)
uint64_t* xhci_ev_ring = (uint64_t*)0x08020000; 
uint64_t* xhci_erst    = (uint64_t*)0x08030000;
/// BARE METAL FIX: Globale Zähler für die Förderbänder!
_43 usb_out_idx = 0;
_43 usb_in_idx = 0;
/// BARE METAL FIX: Unser Lese-Zeiger für den Briefkasten
uint32_t xhci_ev_idx = 0;
uint32_t xhci_ev_cycle = 1;
/// --- XHCI TRB STRUKTUR (16 Byte) ---
struct XHCI_TRB {
    uint32_t param1;
    uint32_t param2;
    uint32_t status;
    uint32_t control;
};

/// Das xHCI Kommunikations-Zentrum
uint64_t xhci_db_base = 0;   /// <--- WICHTIG: Das hier muss xhci_db_base heißen!
uint32_t xhci_cmd_idx = 0;   /// Aktuelle Zeile im Postausgang
uint32_t xhci_cmd_cycle = 1; /// Das magische Toggle-Bit
_202 OracleEntry {
    _30 type[8];     
    _182 vendor;     
    _182 device;     
    _89 bar_addr;    
};

#ifdef __x86_64__
  /// Im 64-Bit Modus (OS2) ist pci.cpp der Chef und reserviert den RAM
  HardwareRegistry sys_hw; 
#else
  /// Im 32-Bit Modus (OS1) ist pci.cpp nur Gast und nutzt die sys_hw aus deiner kernel.cpp
  extern HardwareRegistry sys_hw; 
#endif
_172 OracleEntry oracle_db[16];
_172 _43 oracle_entry_count;
_172 _50 mmio_write32(_89 addr, _89 val);
_172 _30 hw_storage_list[4][48];
_172 _43 storage_count;
_172 _43 current_storage_idx;
_172 _30 hw_gpu_list[2][48];
_172 _43 gpu_count;
_172 _43 current_gpu_idx;
_172 _89 mmio_read32(_89 addr);
_172 _50 int_to_str(_43 val, _30* str);
_172 _30 hw_storage[48];
_172 NICInfo found_nics[5];
_172 _43 nic_count;
_172 _43 active_nic_idx;
_172 _89 ata_base;
_172 _44 usb_detected;
_172 _89 usb_io_base;
_172 _44 logitech_found;
_172 _44 webcam_active;
_172 _30 hw_gpu[48];
_172 _30 hw_usb[48];
_172 _30 webcam_model[40];
_172 _30 hw_net[48];
_172 _50 outl(_182 p, _89 v);
_172 _89 inl(_182 p);
_172 _50 str_cpy(_30* d, _71 _30* s);
_172 _50 nic_select_next();
_172 _50 ahci_init();
_172 _50 byte_to_hex(_184 b, _30* out);
_202 PhysicalDrive;
_172 PhysicalDrive drives[8];
_172 _43 drive_count;
_184 xhci_send_enable_slot(); /// Dem Compiler sagen: "Hier kommt gleich eine Funktion, die eine Zahl zurückgibt!"
_50 xhci_send_address_device(_184 slot_id, _184 port_id, _89 speed);
_50 xhci_configure_endpoints(_184 slot_id, _89 speed);
_89* xhci_ep_out_ring = (_89*)0x04090000; /// Förderband HIN zum Stick
_89* xhci_ep_in_ring  = (_89*)0x040A0000;
/// ==========================================
/// BARE METAL FIX: 64-BIT PAGING (KUGELSICHER)
/// ==========================================
#ifdef __x86_64__
/// Wir nutzen RAM bei glatt 60 Megabyte, um Kern-Kollisionen zu meiden
uint64_t next_free_page_table = 0x07800000; 

void map_mmio_64(uint64_t phys_addr) {
    uint64_t virt_addr = phys_addr;
    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    
    /// ULL verhindert, dass der Compiler die Bits abschneidet
    uint64_t* pml4 = (uint64_t*)(cr3 & 0xFFFFFFFFFFFFF000ULL); 

    uint64_t pml4_idx = (virt_addr >> 39) & 0x1FF;
    uint64_t pdpt_idx = (virt_addr >> 30) & 0x1FF;
    uint64_t pd_idx   = (virt_addr >> 21) & 0x1FF;

    if ((pml4[pml4_idx] & 1) == 0) {
        uint64_t* new_pdpt = (uint64_t*)next_free_page_table;
        next_free_page_table += 4096;
        for(int i=0; i<512; i++) new_pdpt[i] = 0;
        pml4[pml4_idx] = (uint64_t)new_pdpt | 3;
    }
    
    uint64_t* pdpt = (uint64_t*)(pml4[pml4_idx] & 0xFFFFFFFFFFFFF000ULL);
    if ((pdpt[pdpt_idx] & 1) == 0) {
        uint64_t* new_pd = (uint64_t*)next_free_page_table;
        next_free_page_table += 4096;
        for(int i=0; i<512; i++) new_pd[i] = 0;
        pdpt[pdpt_idx] = (uint64_t)new_pd | 3;
    }
    
    uint64_t* pd = (uint64_t*)(pdpt[pdpt_idx] & 0xFFFFFFFFFFFFF000ULL);
    uint64_t aligned_phys = phys_addr & 0xFFFFFFFFFFE00000ULL;
    
    /// FIX 1: MMIO Cache Disable! (0x9B statt 0x83)
    /// Hardware friert oft ein, wenn man Register im Cache behält.
    pd[pd_idx] = aligned_phys | 0x9B; 
    
    /// FIX 2: Absolut sicherer TLB Flush (CR3 Reload) statt invlpg
    __asm__ volatile("mov %0, %%cr3" : : "r"(cr3) : "memory");
}
#else
void map_mmio_64(uint64_t phys_addr) {}
#endif

/// BARE METAL FIX: Zugriff auf die Sound-Ports aus der kernel.cpp
extern void init_xhci(uint32_t bar0);
extern _182 ac97_mixer_port;
extern _182 ac97_bus_port;
/// ==========================================
/// BARE METAL: PCIe ECAM (MEMORY MAPPED I/O)
/// ==========================================
_89 pcie_base_addr = 0;
/// Eine einfache Warteschleife für die Hardware-Latenz
_50 wait_hardware(_89 cycles) {
    /// 'volatile' verhindert, dass der Compiler die Schleife weg-optimiert
    _39(volatile _89 i = 0; i < cycles; i++) {
        asm volatile("nop"); /// 'No Operation' - Die CPU macht kurz Pause
    }
}
/// ==========================================
/// BARE METAL: DYNAMISCHES ACPI RADAR
/// ==========================================
_50 init_acpi_and_pcie() {
    _89* ptr = (_89*)0x000E0000;
    _89* end = (_89*)0x000FFFFF;
    _39(; ptr < end; ptr++) {
        /// "RSD PTR " Signatur
        _15(ptr[0] EQ 0x20445352 AND ptr[1] EQ 0x20525450) {
            _89 rsdt_addr = ptr[4];
            _89* rsdt = (_89*)rsdt_addr;
            _15(rsdt[0] NEQ 0x54445352) _96;
            _89 length = rsdt[1];
            _43 num_tables = (length - 36) / 4;
            _89* table_ptrs = rsdt + 9;
            _39(_43 i = 0; i < num_tables; i++) {
                _89* table = (_89*)table_ptrs[i];
                _89 sig = table[0];
                /// "MCFG" Signatur gefunden!
                _15(sig EQ 0x4746434D) {
                    _89* mcfg = (_89*)table_ptrs[i];
                    _89 base_low = mcfg[11];
                    _89 base_high = mcfg[12];
                    /// Wenn die Adresse unter 4GB liegt, speichern wir sie!
                    _15(base_high EQ 0 AND base_low NEQ 0) {
                        pcie_base_addr = base_low;
                        _96; /// Mission erfüllt!
                    }
                }
            }
            _96;
        }
    }
}
_89 pcie_read_mmio(_89 bus, _89 dev, _89 func, _89 offset) {
    /// BARE METAL LOCK: Wenn die Adresse 0 ist, sofort abbrechen! (Schützt die IDT)
    _15(pcie_base_addr EQ 0) _96 0xFFFFFFFF;
    unsigned long long calc = (unsigned long long)pcie_base_addr + (bus << 20) + (dev << 15) + (func << 12) + (offset & 0xFFF);
    _15(calc >= 0xFFFFFFF0) _96 0xFFFFFFFF;
    _96 *((volatile _89*)((_89)calc));
}
_50 pcie_write_mmio(_89 bus, _89 dev, _89 func, _89 offset, _89 value) {
    /// BARE METAL LOCK: Niemals das eigene Krankenhaus sprengen!
    _15(pcie_base_addr EQ 0) _96;
    unsigned long long calc = (unsigned long long)pcie_base_addr + (bus << 20) + (dev << 15) + (func << 12) + (offset & 0xFFF);
    _15(calc >= 0xFFFFFFF0) _96;
    *((volatile _89*)((_89)calc)) = value;
}
/// BARE METAL: 64-Bit Hardware in 32-Bit RAM einblenden
_50 map_memory(_89 virt_addr, unsigned long long phys_addr) {
    /// Wir berechnen, in welchem Directory und in welchem 2MB-Slot die virtuelle Adresse liegt
    _89 pdpt_idx = virt_addr >> 30;           /// Welches Gigabyte? (0 bis 3)
    _89 pd_idx = (virt_addr >> 21) & 0x1FF;   /// Welcher 2MB Block? (0 bis 511)
    unsigned long long* target_pd = (unsigned long long*)(0x00B01000 + (pdpt_idx * 0x1000));
    /// Die gigantische 64-Bit Physische Adresse mit den Flags (Present | R/W | 2MB) eintragen
    target_pd[pd_idx] = phys_addr | 0x83;
    /// Befiehl der CPU, ihren Adress-Cache (TLB) für diese Adresse zu leeren!
    asm volatile("invlpg (%0)" ::"r" (virt_addr) : "memory");
}
/// --- STRUKTUR FÜR DAS GFX-ORAKEL ---
_202 GraphicsCardInfo {
    _182 vendor_id;
    _182 device_id;
    _184 bus, slot, func;
    _94  framebuffer_addr; /// Die heilige Adresse (32-Bit)
};
GraphicsCardInfo main_gfx_card;
/// --- DER SCANNER ---
_50 pci_find_gfx_card() {
    main_gfx_card.framebuffer_addr = 0;
    /// BARE METAL FIX: Auch hier auf _43 (int) ändern!
    _39(_43 bus = 0; bus < 256; bus++) {
        _39(_184 slot = 0; slot < 32; slot++) {
            _39(_184 func = 0; func < 8; func++) {
                /// Nutzt DEINE pci_read Funktion (Offset 0x00 für IDs)
                _89 id = pci_read(bus, slot, func, 0);
                _182 vendor = id & 0xFFFF;
                _15(vendor EQ 0xFFFF) _37;
                /// Klasse und Subklasse liegen bei Offset 0x08
                _89 class_rev = pci_read(bus, slot, func, 0x08);
                _184 cls = (class_rev >> 24) & 0xFF;
                _184 sub = (class_rev >> 16) & 0xFF;
				_184 prog_if = (class_rev >> 8) & 0xFF;
                /// 0x03 = Display Controller, 0x00 = VGA Compatible
                _15(cls EQ 0x03 AND sub EQ 0x00) {
                    main_gfx_card.vendor_id = vendor;
                    main_gfx_card.device_id = (id >> 16) & 0xFFFF;
                    main_gfx_card.bus = bus;
                    main_gfx_card.slot = slot;
                    main_gfx_card.func = func;
                    /// HIER IST DAS GOLD: Lese BAR 0 (Offset 0x10)
                    _89 bar0 = pci_read(bus, slot, func, 0x10);
                    /// Bereinige die Speichertyp-Flags (unterste 4 Bits abschneiden)
                    main_gfx_card.framebuffer_addr = bar0 & 0xFFFFFFF0;
                    /// Rückkehr zum Kernel - wir haben das Ziel!
                    _96; 
                }
            }
        }
    }
}
_50 xhci_bios_handoff(_89 cap_base) {
    _89 hccparams1 = mmio_read32(cap_base + 0x10);
    _43 xecp_offset = (hccparams1 >> 16) & 0xFFFF;
    _15(xecp_offset EQ 0) _96;
    _89 ext_cap_ptr = cap_base + (xecp_offset << 2);
    
    _39(_43 i = 0; i < 20; i++) {
        _89 val = mmio_read32(ext_cap_ptr);
        _184 cap_id = val & 0xFF;
        _184 next_ptr = (val >> 8) & 0xFF;
        
        _15(cap_id EQ 1) {
            /// 1. OS-Flagge setzen (Wir bitten höflich um Kontrolle)
            mmio_write32(ext_cap_ptr, val | 0x01000000);
            
            /// 2. Kurz warten (Dein sicheres 1-Mio-Limit)
            _39(_43 wait = 0; wait < 1000000; wait++) {
                _15((mmio_read32(ext_cap_ptr) & 0x00010000) EQ 0) _37;
                asm volatile("pause");
            }
            
            /// ==========================================
            /// BARE METAL FIX: GEWALT-ÜBERNAHME!
            /// Wenn das BIOS die Flagge nicht freiwillig löscht,
            /// löschen wir sie jetzt mit Gewalt (Bit 16 = 0)!
            /// ==========================================
            _89 force_val = mmio_read32(ext_cap_ptr);
            _15(force_val & 0x00010000) {
                mmio_write32(ext_cap_ptr, force_val & ~0x00010000);
            }
            
            /// SMI (System Management Interrupts) für USB abwürgen
            _89 ctrl = mmio_read32(ext_cap_ptr + 4);
            mmio_write32(ext_cap_ptr + 4, ctrl & 0x1FFFFE1);
            _96;
        }
        _15(next_ptr EQ 0) _37; 
        ext_cap_ptr = ext_cap_ptr + (next_ptr << 2);
    }
}
_50 xhci_reset(_89 op_base) {
    /// 1. Den aktuellen Befehls-Status (USBCMD) auslesen
    _89 cmd = mmio_read32(op_base);
    /// 2. Controller ANHALTEN (Bit 0 - Run/Stop auf 0 setzen)
    cmd = cmd & 0xFFFFFFFE;
    mmio_write32(op_base, cmd);
    /// 3. Warten, bis der Controller den Stopp bestätigt (USBSTS Bit 0 = HCHalted)
    _89 status_reg = op_base + 0x04;
    _39(_43 wait = 0; wait < 100000; wait++) {
        _15(mmio_read32(status_reg) & 0x01) _37; 
    }
    /// 4. RESET durchführen (Bit 1 - HCRST setzen)
    mmio_write32(op_base, cmd | 0x02);
    /// 5. Warten, bis der Reset fertig ist (Hardware löscht Bit 1 automatisch)
    _39(_43 wait = 0; wait < 100000; wait++) {
        _15((mmio_read32(op_base) & 0x02) EQ 0) _37;
    }
}

_50 xhci_power_on_ports(_89 op_base, _43 max_ports) {
    _39(_43 i = 1; i <= max_ports; i++) {
        _89 portsc_addr = op_base + 0x400 + ((i - 1) * 0x10);
        _89 portsc = mmio_read32(portsc_addr);
        /// 1. DIE FALLE ENTSCHÄRFEN
        /// Bits 17-23 (0x00FE0000) sind Write-1-to-clear.
        /// Mit der Maske 0xFF01FFFF setzen wir genau diese Bits auf 0, 
        /// lassen den Rest aber unangetastet.
        portsc = portsc & 0xFF01FFFF;
        /// 2. STROM EINSCHALTEN
        /// Bit 9 (0x200) ist der Port Power (PP) Pin.
        portsc = portsc | 0x200;
        /// 3. ZURÜCKSCHREIBEN
        mmio_write32(portsc_addr, portsc);
    }
    /// Kurze Pause, damit die Hardware die Spannung (5V) aufbauen kann
    /// und der USB-Stick Zeit hat, seinen Chip hochzufahren.
    _39(_43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }
}
/// ------------------------------------------
/// PCI IMPLEMENTIERUNG
/// ------------------------------------------
_89 pci_read(_184 bus, _184 slot, _184 func, _184 offset) { 
    _89 address = (_89)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((_89)0x80000000)); 
    outl(0xCF8, address); 
    _96 inl(0xCFC); 
}
_50 pci_write(_184 bus, _184 slot, _184 func, _184 offset, _89 value) { 
    _89 address = (_89)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((_89)0x80000000)); 
    outl(0xCF8, address); 
    outl(0xCFC, value); 
}
/// ==========================================
/// BARE METAL: xHCI (USB 3.0) ORAKEL
/// ==========================================
inline unsigned char mmio_read8(_89 address) {
    return *((volatile unsigned char*)address);
}
_184 xhci_check_ports(_89 op_base, _43 max_ports) {
    /// 1. DIE GEDENKSEKUNDE: Wir warten, bis der Stick gebootet hat
    _39(_43 wait = 0; wait < 20000000; wait++) { asm volatile("nop"); }

    _44 found = _86;
    _39(_43 i = 1; i <= max_ports; i++) {
        _89 portsc_addr = op_base + 0x400 + ((i - 1) * 0x10);
        _89 portsc = mmio_read32(portsc_addr);

        /// Bit 0 ist der "Current Connect Status"
        _15(portsc & 0x01) {
            /// Wir lesen die Geschwindigkeit des Sticks (Bits 10-13)
            _89 port_speed = (portsc >> 10) & 0x0F;
            
            /// 1. Slot anfragen (Tor 1)
            _184 slot_id = xhci_send_enable_slot(); 
            
            /// 2. Gerät taufen (Tor 2)
            _15(slot_id > 0) {
                xhci_send_address_device(slot_id, i, port_speed);
				/// BARE METAL FIX: Hier werden die Förderbänder gestartet!
                xhci_configure_endpoints(slot_id, port_speed);
            }
            
            found = _128;
            _96 slot_id; /// Wir haben ihn, Abbruch!
        }
	}
    
    _15(!found) {
        str_cpy(hw_usb, "NO DEVICE PLUGGED");
    }
    _96 0;
}

_184 xhci_send_enable_slot() {
    volatile XHCI_TRB* ring = (volatile XHCI_TRB*)0x08010000 ;
    ring[xhci_cmd_idx].param1 = 0;
    ring[xhci_cmd_idx].param2 = 0;
    ring[xhci_cmd_idx].status = 0;
    ring[xhci_cmd_idx].control = (9 << 10) | xhci_cmd_cycle;
    xhci_cmd_idx++;
    mmio_write32(xhci_db_base, 0); /// Klingeln!
    volatile uint32_t* event_ring = (volatile uint32_t*)0x04030000;
    _39(_43 wait = 0; wait < 1000000; wait++) {
        /// Wir lesen genau den Slot, auf den unser Radar (xhci_ev_idx) zeigt!
        _89 ctrl = event_ring[(xhci_ev_idx * 4) + 3];
        _15((ctrl & 1) EQ xhci_ev_cycle) { 
            _184 comp_code = (event_ring[(xhci_ev_idx * 4) + 2] >> 24) & 0xFF;
            _184 slot_id = (ctrl >> 24) & 0xFF;
            xhci_ev_idx++; /// Briefkasten-Zeiger für den nächsten Befehl weiterschieben!
            _15(comp_code EQ 1) { _96 slot_id; } /// Erfolg! ID zurückgeben
            _96 0; /// Fehler
        }
    }
    _96 0; /// Timeout
}
_50 xhci_send_address_device(_184 slot_id, _184 port_id, _89 speed) {
    /// 1. RAM-Fächer für den Stick reservieren
    _89* out_ctx  = (_89*)0x04060000;
    _89* in_ctx   = (_89*)0x04070000;
    _89* ep0_ring = (_89*)0x04080000;

    _39(_43 i=0; i<512; i++) { out_ctx[i] = 0; in_ctx[i] = 0; }
    _39(_43 i=0; i<64; i++)  { ep0_ring[i] = 0; }
    /// BARE METAL FIX: Echte 64-Bit Zuweisung in die Pyramide!
    xhci_dcbaa[slot_id] = (uint64_t)out_ctx;

    /// 2. Input Context Formular ausfüllen
    in_ctx[1] = 0x03; /// Enable Slot (Bit 0) & EP0 (Bit 1)

    /// Slot Context (Offset 0x20 = DWORD 8)
    in_ctx[8] = (1 << 27); /// Context Entries = 1
    in_ctx[9] = (port_id << 16); /// Root Hub Port Number eintragen

    /// Endpoint 0 Context (Offset 0x40 = DWORD 16)
    _89 max_packet = (speed EQ 4) ? 512 : 64; /// USB 3.0 = 512, USB 2.0 = 64
    in_ctx[16 + 1] = (3 << 1) | (4 << 16) | (max_packet << 16); /// CErr=3, Typ=Control
    
    /// 64-Bit Fix: Ring-Adresse anheften
    in_ctx[16 + 2] = (_89)(uint64_t)ep0_ring | 0x01; 
    
    in_ctx[16 + 3] = 0; 
    in_ctx[16 + 4] = 8; /// Average TRB Length

    /// 3. Befehl (Typ 11) abfeuern!
    volatile XHCI_TRB* ring = (volatile XHCI_TRB*)0x04010000;
    
    /// 64-Bit Fix
    ring[xhci_cmd_idx].param1 = (uint32_t)((uint64_t)in_ctx & 0xFFFFFFFF);
	ring[xhci_cmd_idx].param2 = (uint32_t)(((uint64_t)in_ctx >> 32) & 0xFFFFFFFF);
    ring[xhci_cmd_idx].status = 0;
    ring[xhci_cmd_idx].control = (11 << 10) | (slot_id << 24) | xhci_cmd_cycle;
    xhci_cmd_idx++;
    mmio_write32(xhci_db_base, 0);

    /// 4. Auf die Taufe warten
    volatile uint32_t* event_ring = (volatile uint32_t*)0x04030000;
    _39(_43 wait = 0; wait < 1000000; wait++) {
        _89 ctrl = event_ring[(xhci_ev_idx * 4) + 3];
        _15((ctrl & 1) EQ xhci_ev_cycle) { 
            _184 comp_code = (event_ring[(xhci_ev_idx * 4) + 2] >> 24) & 0xFF;
            xhci_ev_idx++; /// Weiterschieben
            
            _15(comp_code EQ 1) { 
                str_cpy(hw_usb, "USB ADDR ASSIGNED");
            } _41 {
                str_cpy(hw_usb, "ADDRESS ERR");
            }
            _37;
        }
    }
}
/// ==========================================
/// BARE METAL FIX: FÖRDERBÄNDER EINRICHTEN (TOR 3)
/// ==========================================
extern _89 global_mouse_slot;
extern _50 xhci_submit_mouse_read();

_50 xhci_configure_endpoints(_184 slot_id, _89 speed) {
    _89* in_ctx = (_89*)0x04070000;
    _89* ep_out_ring = (_89*)0x04090000;
    _89* ep_in_ring  = (_89*)0x040A0000;
    _39(_43 i=0; i<256; i++) { ep_out_ring[i] = 0; ep_in_ring[i] = 0; }
    
    in_ctx[0] = 0; 

    in_ctx[8] = (in_ctx[8] & ~(0xF << 20)) | (speed << 20);

    _15(speed EQ 2) { 
        in_ctx[17] = (in_ctx[17] & 0x0000FFFF) | (8 << 16);
    } _41 _15(speed EQ 1) { 
        in_ctx[17] = (in_ctx[17] & 0x0000FFFF) | (64 << 16); 
    } _41 {
        in_ctx[17] = (in_ctx[17] & 0x0000FFFF) | (512 << 16); 
    }

    _15(speed <= 2) { 
        global_mouse_slot = slot_id;
        
        in_ctx[1] = (1 << 0) | (1 << 1) | (1 << 3); 
        in_ctx[8] = (in_ctx[8] & 0x07FFFFFF) | (3 << 27); 

        in_ctx[32 + 0] = (8 << 16); 

        _15(speed EQ 2) { 
            in_ctx[32 + 1] = (3 << 1) | (7 << 3) | (8 << 16); 
        } _41 {
            in_ctx[32 + 1] = (3 << 1) | (7 << 3) | (64 << 16); 
        }

        /// 64-Bit Fixes
        in_ctx[32 + 2] = (_89)(uint64_t)ep_in_ring | 0x01;
        in_ctx[32 + 3] = 0; 
        in_ctx[32 + 4] = 8;
        
        ep_in_ring[63 * 4 + 0] = (_89)(uint64_t)ep_in_ring;
        ep_in_ring[63 * 4 + 1] = 0; 
        ep_in_ring[63 * 4 + 2] = 0;
        ep_in_ring[63 * 4 + 3] = (6 << 10) | 2; 

    } _41 {
        in_ctx[1] = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
        in_ctx[8] = (in_ctx[8] & 0x07FFFFFF) | (3 << 27);
        
        in_ctx[24 + 1] = (3 << 1) | (2 << 16) | (512 << 16); 
        /// 64-Bit Fix
        in_ctx[24 + 2] = (_89)(uint64_t)ep_out_ring | 0x01;
        in_ctx[24 + 3] = 0; in_ctx[24 + 4] = 8;
        
        in_ctx[32 + 1] = (3 << 1) | (6 << 16) | (512 << 16);
        /// 64-Bit Fix
        in_ctx[32 + 2] = (_89)(uint64_t)ep_in_ring | 0x01;
        in_ctx[32 + 3] = 0; in_ctx[32 + 4] = 8;
        
        /// 64-Bit Fixes
        ep_out_ring[63 * 4 + 0] = (_89)(uint64_t)ep_out_ring;
        ep_out_ring[63 * 4 + 1] = 0; ep_out_ring[63 * 4 + 2] = 0;
        ep_out_ring[63 * 4 + 3] = (6 << 10) | 2;
        
        ep_in_ring[63 * 4 + 0] = (_89)(uint64_t)ep_in_ring;
        ep_in_ring[63 * 4 + 1] = 0; ep_in_ring[63 * 4 + 2] = 0;
        ep_in_ring[63 * 4 + 3] = (6 << 10) | 2;
    }

    volatile XHCI_TRB* ring = (volatile XHCI_TRB*)0x04010000;
    /// 64-Bit Fix
    ring[xhci_cmd_idx].param1 = (_89)(uint64_t)in_ctx;
    ring[xhci_cmd_idx].param2 = 0;
    ring[xhci_cmd_idx].status = 0;
    ring[xhci_cmd_idx].control = (12 << 10) | (slot_id << 24) | xhci_cmd_cycle;
    xhci_cmd_idx++;
    mmio_write32(xhci_db_base, 0); 

    volatile uint32_t* event_ring = (volatile uint32_t*)0x04030000;
    _39(_43 wait = 0; wait < 1000000; wait++) {
        _89 ctrl = event_ring[(xhci_ev_idx * 4) + 3];
        _15((ctrl & 1) EQ xhci_ev_cycle) { 
            _184 comp_code = (event_ring[(xhci_ev_idx * 4) + 2] >> 24) & 0xFF;
            xhci_ev_idx++; 
            _15(comp_code EQ 1) { 
                str_cpy(hw_usb, "ENDPOINTS READY!"); 
                
                _15(speed <= 2) { 
                    uint32_t* ep0_ring = (uint32_t*)0x04080000;
                    
                    ep0_ring[0] = 0x00000B21; 
                    ep0_ring[1] = 0x00000000; 
                    ep0_ring[2] = 0x00000008; 
                    ep0_ring[3] = (2 << 10) | (1 << 6) | 1; 

                    ep0_ring[4] = 0; ep0_ring[5] = 0; ep0_ring[6] = 0;
                    ep0_ring[7] = (4 << 10) | (1 << 16) | 1; 

                    ep0_ring[8]  = 0x00000A21;
                    ep0_ring[9]  = 0x00000000;
                    ep0_ring[10] = 0x00000008; 
                    ep0_ring[11] = (2 << 10) | (1 << 6) | 1; 

                    ep0_ring[12] = 0; ep0_ring[13] = 0; ep0_ring[14] = 0;
                    ep0_ring[15] = (4 << 10) | (1 << 16) | 1; 

                    volatile uint32_t* db_array = (volatile uint32_t*)xhci_db_base;
                    db_array[slot_id] = 1; 

                    _39(volatile _43 w = 0; w < 5000000; w++) { asm volatile("pause"); }

                    xhci_submit_mouse_read();
                }
            } _41 {
                str_cpy(hw_usb, "EP CONFIG ERR");
            }
            
            _37;
        }
    }
}
/// ==========================================
/// 1. KAPAZITÄT LESEN (TOR 4)
/// ==========================================
extern "C" _43 xhci_bot_get_capacity(_184 slot_id) { // <--- HIER EINGEFÜGT!
    _89* cbw_ram = (_89*)0x040B0000;
    _39(_43 i=0; i<28; i++) cbw_ram[i] = 0;
    
    cbw_ram[0] = 0x43425355; cbw_ram[1] = 0x02; cbw_ram[2] = 8; cbw_ram[3] = 0x0A000080;
    _184* scsi_cmd = (_184*)(cbw_ram + 3) + 3; scsi_cmd[0] = 0x25;

    volatile XHCI_TRB* out_ring = (volatile XHCI_TRB*)xhci_ep_out_ring;
    /// 64-Bit Fix
    out_ring[usb_out_idx].param1 = (_89)(uint64_t)cbw_ram; 
    out_ring[usb_out_idx].param2 = 0;
    out_ring[usb_out_idx].status = 31; 
    out_ring[usb_out_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 2);
    usb_out_idx++; _15(usb_out_idx > 60) usb_out_idx = 0; 

    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    _89* cap_ram = (_89*)0x040C0000;
    volatile XHCI_TRB* in_ring = (volatile XHCI_TRB*)xhci_ep_in_ring;
    /// 64-Bit Fix
    in_ring[usb_in_idx].param1 = (_89)(uint64_t)cap_ram; 
    in_ring[usb_in_idx].param2 = 0;
    in_ring[usb_in_idx].status = 8; 
    in_ring[usb_in_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 3);
    usb_in_idx++; _15(usb_in_idx > 60) usb_in_idx = 0;

    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    _89 max_lba_be = cap_ram[0];
    _89 max_lba = ((max_lba_be >> 24) & 0xFF) | ((max_lba_be << 8) & 0xFF0000) | ((max_lba_be >> 8) & 0xFF00) | ((max_lba_be << 24) & 0xFF000000);
    _43 mb = max_lba / 2048; 
    _15(mb EQ 0 OR mb > 500000) _96 16; 
    _96 mb;
}

/// ==========================================
/// 2. SEKTOREN LESEN (TOR 5)
/// ==========================================
_44 xhci_bot_read_sectors(_184 slot_id, uint32_t lba, uint64_t dest_ram) {
    _89* cbw_ram = (_89*)0x040B0000; _89* csw_ram = (_89*)0x040D0000;
    _39(_43 i=0; i<28; i++) { cbw_ram[i] = 0; csw_ram[i] = 0; }
    
    cbw_ram[0] = 0x43425355; cbw_ram[1] = 0x03; cbw_ram[2] = 512; cbw_ram[3] = 0x0A000080;
    _184* scsi_cmd = (_184*)(cbw_ram + 3) + 3; scsi_cmd[0] = 0x28;
    scsi_cmd[2] = (lba >> 24) & 0xFF; scsi_cmd[3] = (lba >> 16) & 0xFF; scsi_cmd[4] = (lba >> 8) & 0xFF; scsi_cmd[5] = lba & 0xFF;
    scsi_cmd[8] = 1; scsi_cmd[9] = 0;

    volatile XHCI_TRB* out_ring = (volatile XHCI_TRB*)xhci_ep_out_ring;
    volatile XHCI_TRB* in_ring  = (volatile XHCI_TRB*)xhci_ep_in_ring;

    /// COMMAND (OUT)
    out_ring[usb_out_idx].param1 = (_89)(uint64_t)cbw_ram; 
    out_ring[usb_out_idx].param2 = 0; 
    out_ring[usb_out_idx].status = 31; 
    out_ring[usb_out_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 2);
    usb_out_idx++; _15(usb_out_idx > 60) usb_out_idx = 0;
    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    /// DATA (IN) - BARE METAL FIX: 64-Bit Pointer aufteilen!
    in_ring[usb_in_idx].param1 = (uint32_t)(dest_ram & 0xFFFFFFFF); 
    in_ring[usb_in_idx].param2 = (uint32_t)((dest_ram >> 32) & 0xFFFFFFFF); 
    in_ring[usb_in_idx].status = 512; 
    in_ring[usb_in_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 3);
    usb_in_idx++; _15(usb_in_idx > 60) usb_in_idx = 0;
    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    /// STATUS (IN)
    in_ring[usb_in_idx].param1 = (_89)(uint64_t)csw_ram; 
    in_ring[usb_in_idx].param2 = 0; 
    in_ring[usb_in_idx].status = 13; 
    in_ring[usb_in_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 3);
    usb_in_idx++; _15(usb_in_idx > 60) usb_in_idx = 0;
    
    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }
    _15(csw_ram[0] EQ 0x53425355 AND csw_ram[3] EQ 0) _96 _128;
    _96 _86;
}

/// ==========================================
/// 3. SEKTOREN SCHREIBEN (TOR 6)
/// ==========================================
_44 xhci_bot_write_sectors(_184 slot_id, uint32_t lba, uint64_t src_ram) {
    _89* cbw_ram = (_89*)0x040B0000; _89* csw_ram = (_89*)0x040D0000;
    _39(_43 i=0; i<28; i++) { cbw_ram[i] = 0; csw_ram[i] = 0; }
    
    cbw_ram[0] = 0x43425355; cbw_ram[1] = 0x04; cbw_ram[2] = 512; cbw_ram[3] = 0x0A000000; /// OUT!
    _184* scsi_cmd = (_184*)(cbw_ram + 3) + 3; scsi_cmd[0] = 0x2A; /// WRITE 10
    scsi_cmd[2] = (lba >> 24) & 0xFF; scsi_cmd[3] = (lba >> 16) & 0xFF; scsi_cmd[4] = (lba >> 8) & 0xFF; scsi_cmd[5] = lba & 0xFF;
    scsi_cmd[8] = 1; scsi_cmd[9] = 0;

    volatile XHCI_TRB* out_ring = (volatile XHCI_TRB*)xhci_ep_out_ring;
    volatile XHCI_TRB* in_ring  = (volatile XHCI_TRB*)xhci_ep_in_ring;

    /// COMMAND (OUT)
    out_ring[usb_out_idx].param1 = (_89)(uint64_t)cbw_ram; 
    out_ring[usb_out_idx].param2 = 0; 
    out_ring[usb_out_idx].status = 31; 
    out_ring[usb_out_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 2);
    usb_out_idx++; _15(usb_out_idx > 60) usb_out_idx = 0;
    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    /// DATA (OUT!) - BARE METAL FIX: 64-Bit Pointer aufteilen!
    out_ring[usb_out_idx].param1 = (uint32_t)(src_ram & 0xFFFFFFFF); 
    out_ring[usb_out_idx].param2 = (uint32_t)((src_ram >> 32) & 0xFFFFFFFF); 
    out_ring[usb_out_idx].status = 512; 
    out_ring[usb_out_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 2);
    usb_out_idx++; _15(usb_out_idx > 60) usb_out_idx = 0;
    _39(volatile _43 wait = 0; wait < 1000000; wait++) { asm volatile("nop"); }

    /// STATUS (IN)
    in_ring[usb_in_idx].param1 = (_89)(uint64_t)csw_ram; 
    in_ring[usb_in_idx].param2 = 0; 
    in_ring[usb_in_idx].status = 13; 
    in_ring[usb_in_idx].control = (1 << 10) | (1 << 5) | 1;
    mmio_write32(xhci_db_base + (slot_id * 32), 3);
    usb_in_idx++; _15(usb_in_idx > 60) usb_in_idx = 0;
    
    _43 wait = 0;
    _114(csw_ram[0] NEQ 0x53425355 AND wait < 1000000) { wait++; asm volatile("nop"); }

    _15(csw_ram[0] EQ 0x53425355 AND csw_ram[3] EQ 0) _96 _128;
    _96 _86;
}
/// ==========================================
/// BARE METAL: DER NEUE, SICHERE USB-SCANNER
/// ==========================================
_44 usb_initialized = _86;
/// Globaler Zähler, der sich merkt, welchen Chip wir gerade anschauen
static int xhci_iterator = 0;
/// ==========================================
/// BARE METAL TOOL: HEX TO STRING
/// ==========================================
void hex_to_str(uint32_t val, char* str) {
    const char hex_chars[] = "0123456789ABCDEF";
    str[0] = '0'; str[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        str[i + 2] = hex_chars[val & 0xF];
        val >>= 4;
    }
    str[10] = 0;
}

/// Füge das oben in deine pci.cpp ein, falls Swap noch nicht bekannt ist:
extern void Swap(); 

/// BARE METAL DEBUG: Direkter Zugriff auf deinen Framebuffer!
extern uint32_t* fb; 

void xhci_debug_color(uint32_t hex_color) {
    if (!fb) return;
    /// Zeichnet ein fettes 50x50 Quadrat oben links in die Ecke (unaufhaltsam!)
    for(int y=0; y<50; y++) {
        for(int x=0; x<50; x++) {
            fb[y*800 + x] = hex_color;
        }
    }
}

/// ==========================================
/// PASSIVER USB SCAN (BIOS BEHÄLT DIE KONTROLLE)
/// ==========================================
void system_init_usb() {
    uint32_t bus = 0; uint32_t dev = 0; uint32_t func = 0; 
    bool found = false;
    
    for(uint32_t b=0; b<256 && !found; b++) {
        for(uint32_t d=0; d<32 && !found; d++) {
            for(uint32_t f=0; f<28 && !found; f++) {
                uint32_t id = pci_read(b,d,f,0);
                if ((id & 0xFFFF) != 0xFFFF) {
                    uint32_t cls = pci_read(b,d,f,0x08);
                    if ((cls >> 8) == 0x0C0330) { bus = b; dev = d; func = f; found = true; }
                }
            }
        }
    }

    if(!found) { 
        str_cpy(hw_usb, "xHCI NOT FOUND"); 
    } else {
        str_cpy(hw_usb, "BIOS LEGACY USB ACTIVE"); 
    }

    /// WICHTIG: Wir fassen ab hier KEINE Register mehr an!
    /// Kein BIOS-Handoff, kein Run/Stop, kein Strom-Reset.
    /// Das BIOS denkt, wir haben keinen Treiber, und emuliert 
    /// die USB-Geräte munter als PS/2-Geräte weiter.
}
extern "C" void hda_init_controller(uint32_t hda_base);

_50 pci_scan_all() {
    /// 1. ALLE Zähler sauber resetten, damit Arrays niemals überlaufen!
    nic_count = 0;
    storage_count = 0;
    gpu_count = 0;

    _39(_43 bus = 0; bus < 256; bus++) { 
        _39(_184 dev = 0; dev < 32; dev++) {
            _39(_184 func = 0; func < 8; func++) {
                _89 id = pci_read(bus, dev, func, 0);
                
                /// BARE METAL FIX: Nur EINMAL auf gültiges Gerät prüfen!
                _15((id & 0xFFFF) NEQ 0xFFFF) { 
                    _89 class_rev = pci_read(bus, dev, func, 0x08); 
                    _184 cls = (class_rev >> 24) & 0xFF; 
                    _184 sub = (class_rev >> 16) & 0xFF;
                    _184 prog_if = (class_rev >> 8) & 0xFF;
                    _182 vendor = id & 0xFFFF; 
                    _182 device_id = (id >> 16) & 0xFFFF;

                    /// 2. ORAKEL CHECK
                    _44 oracle_match = _86;
                    _39(_43 o = 0; o < oracle_entry_count; o++) {
                        _15(oracle_db[o].vendor EQ vendor AND oracle_db[o].device EQ device_id) {
                            oracle_match = _128;
                        }
                    }

                    /// 3. GPU SCAN
                    _15(cls EQ 0x03) {
                        _15(gpu_count < 2) {
                            str_cpy(hw_gpu_list[gpu_count], "GPU: ");
                            _30* p = hw_gpu_list[gpu_count] + 5;
                            byte_to_hex(vendor >> 8, p); p+=2; byte_to_hex(vendor & 0xFF, p); p+=2;
                            *p++ = '-';
                            byte_to_hex(device_id >> 8, p); p+=2; byte_to_hex(device_id & 0xFF, p); p+=2;
                            *p = 0; /// Null Terminator
                            _15(oracle_match) { str_cpy(hw_gpu_list[gpu_count], "ORACLE GPU"); }
                            gpu_count++;
                        }
                        _15(!oracle_match) { str_cpy(hw_gpu, "GPU DEV"); }
                    }

                    /// 4. NETZWERKKARTEN
                    _15(cls EQ 0x02) { 
                        _89 bar0 = pci_read(bus, dev, func, 0x10);
                        _15(bar0 & 1) { sys_hw.net_io_port = bar0 & 0xFFFFFFFC; } 
                        _41 { sys_hw.net_bar0 = bar0 & 0xFFFFFFF0; }
                        sys_hw.is_net_found = _128;

                        _89 cmd_reg = pci_read(bus, dev, func, 0x04);
                        pci_write(bus, dev, func, 0x04, cmd_reg | 0x0407);

                        _15(nic_count < 5) { 
                            found_nics[nic_count].address = bar0;
                            str_cpy(hw_net, "ID:");
                            _30* stat = hw_net + 3;
                            byte_to_hex(vendor >> 8, stat); stat+=2; byte_to_hex(vendor & 0xFF, stat); stat+=2;
                            *stat++ = '-';
                            byte_to_hex(device_id >> 8, stat); stat+=2; byte_to_hex(device_id & 0xFF, stat); stat+=2;
                            *stat = 0; /// Null Terminator
                            
                            _15(vendor EQ 0x10EC) { str_cpy(found_nics[nic_count].name, "REALTEK"); found_nics[nic_count].type=1; } 
                            _41 _15(vendor EQ 0x8086) { str_cpy(found_nics[nic_count].name, "INTEL"); found_nics[nic_count].type=2; } 
                            _41 { str_cpy(found_nics[nic_count].name, "GENERIC"); found_nics[nic_count].type=0; }
                            nic_count++; 
                        } 
                    }

                    /// 5. AHCI / SATA CONTROLLER
                    _15(cls EQ 0x01 AND sub EQ 0x06) {
                        _89 bar5 = pci_read(bus, dev, func, 0x24);
                        sys_hw.ahci_bar5 = bar5 & 0xFFFFFFF0; 
                        sys_hw.is_ahci_found = _128;
                        _89 cmd_reg = pci_read(bus, dev, func, 0x04);
                        pci_write(bus, dev, func, 0x04, cmd_reg | 0x07); 
                    }

                    /// 6. USB CONTROLLER (xHCI)
                    _15(cls EQ 0x0C AND sub EQ 0x03) {
                        sys_hw.is_usb_found = _128;
                        usb_detected = _128;
                        
                        _15(prog_if EQ 0x30) {
                            static int xhci_discovery_count = 0;
                            xhci_discovery_count++;

                            _15(vendor EQ 0x8086) {
                                /// 1. Aufwachen (D0 State)
                                _184 cap_ptr = pci_read(bus, dev, func, 0x34) & 0xFF;
                                _39(_43 steps = 0; steps < 10 AND cap_ptr NEQ 0; steps++) {
                                    _89 cap_reg = pci_read(bus, dev, func, cap_ptr);
                                    _15((cap_reg & 0xFF) EQ 0x01) { 
                                        _89 pmcsr = pci_read(bus, dev, func, cap_ptr + 4);
                                        pci_write(bus, dev, func, cap_ptr + 4, pmcsr & 0xFFFFFFFC);
                                        _39(volatile _43 wait = 0; wait < 10000000; wait++) { asm volatile("pause"); }
                                        _37; 
                                    }
                                    cap_ptr = (cap_reg >> 8) & 0xFF;
                                }
                                
                                /// 2. Rechte (Memory & Bus Master) UND alte Interrupts blockieren (0x0406 statt 0x06)
								_89 cmd_reg = pci_read(bus, dev, func, 0x04);
								pci_write(bus, dev, func, 0x04, cmd_reg | 0x0406);
								
								/// HIER ENTSCHÄRFEN WIR DIE MODERNE MSI-BOMBE VORHER!
								_184 cap_ptr_msi = pci_read(bus, dev, func, 0x34) & 0xFF;
								_39(_43 steps = 0; steps < 10 AND cap_ptr_msi NEQ 0; steps++) {
									_89 cap_reg = pci_read(bus, dev, func, cap_ptr_msi);
									_184 cap_id = cap_reg & 0xFF;
									
									_15(cap_id EQ 0x05) { /// MSI gefunden -> Abschalten!
										_89 msi_ctrl = pci_read(bus, dev, func, cap_ptr_msi);
										pci_write(bus, dev, func, cap_ptr_msi, msi_ctrl & ~0x00010000);
									} _41 _15(cap_id EQ 0x11) { /// MSI-X gefunden -> Abschalten!
										_89 msix_ctrl = pci_read(bus, dev, func, cap_ptr_msi);
										pci_write(bus, dev, func, cap_ptr_msi, msix_ctrl & ~0x80000000);
									}
									cap_ptr_msi = (cap_reg >> 8) & 0xFF;
								}
                                
                                /// 3. Adresse (BAR0 + BAR1) komplett lesen
                                _89 bar0 = pci_read(bus, dev, func, 0x10);
                                uint32_t clean_bar0 = bar0 & 0xFFFFFFF0;
                                
                                /// Wir speichern auch BAR1 (falls das Mainboard es nutzt)
                                _89 bar1 = pci_read(bus, dev, func, 0x14);
                                
                                /// WIR IGNORIEREN NIEMANDEN MEHR!
                                if (clean_bar0 != 0) {
                                    /// Wir übergeben BAR0 an unsere Diagnose-Sonde
                                    int result = init_xhci_probe(clean_bar0, xhci_discovery_count);
                                    
                                    if (result == 1) {
                                        sys_hw.usb_bar0 = clean_bar0;
                                        // Der "Auserwählte" wurde gefunden
                                    }
                                }
                            }
                        }
                    }

                    /// 7. STORAGE CATCH-ALL
                    _15(cls EQ 0x01) { 
                        _15(storage_count < 4) {
                            str_cpy(hw_storage_list[storage_count], "DRV: ");
                            _30* p = hw_storage_list[storage_count] + 5;
                            byte_to_hex(vendor >> 8, p); p+=2; byte_to_hex(vendor & 0xFF, p); p+=2;
                            *p++ = '-';
                            byte_to_hex(device_id >> 8, p); p+=2; byte_to_hex(device_id & 0xFF, p); p+=2;
                            *p = 0; /// Null Terminator
                            storage_count++;
                        }
                    }

                    /// 8. SOUNDKARTE (AC97)
                    _15(cls EQ 0x04 AND sub EQ 0x01) {
                        ac97_mixer_port = (_182)(pci_read(bus, dev, func, 0x10) & 0xFFFFFFFE);
                        ac97_bus_port = (_182)(pci_read(bus, dev, func, 0x14) & 0xFFFFFFFE);
                        _89 cmd_reg = pci_read(bus, dev, func, 0x04);
                        pci_write(bus, dev, func, 0x04, cmd_reg | 0x05);
                    }

                    /// 8.1 INTEL HD AUDIO (HDA)
                    _15(cls EQ 0x04 AND sub EQ 0x03) {
                        extern uint32_t hda_base_addr;
                        /// BAR0 ist bei HDA immer MMIO (Speicher)
                        hda_base_addr = pci_read(bus, dev, func, 0x10) & 0xFFFFFFF0;
                        _89 cmd_reg = pci_read(bus, dev, func, 0x04);
                        /// Bus Mastering (Bit 2) und Memory Space (Bit 1) aktivieren!
                        pci_write(bus, dev, func, 0x04, cmd_reg | 0x06);
                        
                        /// HDA Initialisierung aufrufen
                        _15(hda_base_addr != 0) {
                            hda_init_controller(hda_base_addr);
                        }
                    }

                    /// 9. WEBCAM
                    _15(vendor EQ 0x046D) { 
                        logitech_found = _128; 
                        webcam_active = _128; 
                        str_cpy(webcam_model, "LOGITECH C270"); 
                    }
                }
            }
        }
    }
    
    /// 10. NETZWERK ABSCHLUSS
    _15(nic_count > 0) { 
        active_nic_idx = -1; 
        nic_select_next(); 
    } _41 {
        str_cpy(hw_net, "NO NIC FOUND");
    }
}