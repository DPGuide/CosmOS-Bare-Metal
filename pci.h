#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include "schneider_lang.h"

// Struktur aus kernel.cpp hierher verschoben
_202 NICInfo { _30 name[32]; _89 address; _43 type; };

// Globale Funktionen
_172 _89 pci_read(_184 bus, _184 slot, _184 func, _184 offset);
_172 _50 pci_scan_all();
/// 1. Den Bauplan (Struct) hier definieren, damit ihn jeder kennt
_202 HardwareRegistry {
    _44 is_ahci_found;
    uint64_t ahci_bar5; /// 64-Bit Fix

    _44 is_usb_found;
    uint64_t usb_bar0;  /// 64-Bit Fix

    _44 is_net_found;
    _89 net_bar0;
    _89 net_io_port;
}; 

/// 2. Danach erst die extern-Ankündigung
extern HardwareRegistry sys_hw;
/// ==========================================
/// AUTO-GENERATED PCI HEADER FUER OS2
/// ==========================================
#define PCI_VEN_INTEL 0x8086

/// Die ganzen extrahierten Devices:
#define PCI_DEV_153A 0x153A
#define PCI_DEV_153B 0x153B
#define PCI_DEV_155A 0x155A
#define PCI_DEV_1559 0x1559
#define PCI_DEV_15A0 0x15A0
#define PCI_DEV_15A1 0x15A1
#define PCI_DEV_15A2 0x15A2
#define PCI_DEV_15A3 0x15A3
#define PCI_DEV_156F 0x156F
#define PCI_DEV_1570 0x1570
#define PCI_DEV_15B7 0x15B7
#define PCI_DEV_15B8 0x15B8
#define PCI_DEV_15B9 0x15B9
#define PCI_DEV_15D7 0x15D7
#define PCI_DEV_15D8 0x15D8
#define PCI_DEV_15E3 0x15E3
#define PCI_DEV_15D6 0x15D6
#define PCI_DEV_15BB 0x15BB
#define PCI_DEV_15BC 0x15BC
#define PCI_DEV_15BD 0x15BD
#define PCI_DEV_15BE 0x15BE
#define PCI_DEV_15DF 0x15DF
#define PCI_DEV_15E0 0x15E0
#define PCI_DEV_15E1 0x15E1
#define PCI_DEV_15E2 0x15E2
#define PCI_DEV_0D4E 0x0D4E
#define PCI_DEV_0D4F 0x0D4F
#define PCI_DEV_0D4C 0x0D4C
#define PCI_DEV_0D4D 0x0D4D
#define PCI_DEV_0D53 0x0D53
#define PCI_DEV_0D55 0x0D55
#define PCI_DEV_15F9 0x15F9
#define PCI_DEV_15FA 0x15FA
#define PCI_DEV_15FB 0x15FB
#define PCI_DEV_15FC 0x15FC
#define PCI_DEV_15F4 0x15F4
#define PCI_DEV_100E 0x100E /// QEMU
#define PCI_DEV_1502 0x1502 /// Deine erste Testkarte
#define PCI_DEV_1503 0x1503 /// Deine zweite Testkarte

#endif