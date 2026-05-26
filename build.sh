#!/bin/bash
set -e

echo "=========================================="
echo "    COSMOS OS - BARE METAL BUILD SYSTEM   "
echo "=========================================="

echo "-> Erstelle automatisches Backup..."
BACKUP_DIR="backups/cosmos_$(date +%Y-%m-%d_%H-%M-%S)"
mkdir -p "$BACKUP_DIR"
cp *.cpp *.h *.s *.ld "$BACKUP_DIR/" 2>/dev/null || true
echo "-> Quellcodes sicher kopiert nach: $BACKUP_DIR"

echo "------------------------------------------"
echo "0. Alten Müll aufräumen..."
rm -f *.o *.elf *.bin cosmos.iso
rm -rf isodir
mkdir -p isodir/boot/grub

echo "------------------------------------------"
echo "1. Erstelle 64-Bit Linker-Skript..."
cat > linker64.ld << 'EOF'
ENTRY(os2_start)
SECTIONS
{
    . = 0x1000000;
    .text : ALIGN(4096) {
        *(.text.entry)
        *(.text .text.*)
    }
    .rodata : ALIGN(4096) { *(.rodata .rodata.*) }
    .data : ALIGN(4096) { *(.data .data.*) }
    .bss : ALIGN(4096) { *(COMMON) *(.bss .bss.*) }
}
EOF

echo "------------------------------------------"
echo "2. Kompiliere OS1 (32-Bit Bootloader & Disk Manager)..."
as --32 boot.s -o boot.o
g++ -m32 -O2 -c kernel.cpp -o kernel_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c pci.cpp -o pci_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c net.cpp -o net_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_bytes.cpp -o cosmos_bytes_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive
g++ -m32 -O2 -c cosmos_fs.cpp -o cosmos_fs_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_tba.cpp -o cosmos_tba_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive
g++ -m32 -O2 -c cosmos_ahci.cpp -o cosmos_ahci_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_cfs.cpp -o cosmos_cfs_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_usb.cpp -o cosmos_usb_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_hda.cpp -o cosmos_hda_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c cosmos_partition.cpp -o cosmos_partition_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m32 -O2 -c memory.cpp -o memory_32.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast

ld -m elf_i386 -T linker.ld -static -z noexecstack -o isodir/boot/kernel.bin boot.o kernel_32.o pci_32.o net_32.o cosmos_bytes_32.o cosmos_fs_32.o cosmos_tba_32.o cosmos_ahci_32.o cosmos_cfs_32.o cosmos_usb_32.o cosmos_hda_32.o cosmos_partition_32.o memory_32.o

echo "------------------------------------------"
echo "3. Kompiliere OS2 (64-Bit Payload Kernel)..."
as --64 os2_entry.s -o os2_entry.o
g++ -m64 -mno-red-zone -O2 -c kernel_main.cpp -o kernel_main_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c kernel.cpp -o kernel_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c pci.cpp -o pci_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c net.cpp -o net_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_bytes.cpp -o cosmos_bytes_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive
g++ -m64 -mno-red-zone -O2 -c cosmos_fs.cpp -o cosmos_fs_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_tba.cpp -o cosmos_tba_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive
g++ -m64 -mno-red-zone -O2 -c cosmos_ahci.cpp -o cosmos_ahci_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_cfs.cpp -o cosmos_cfs_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_usb.cpp -o cosmos_usb_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_hda.cpp -o cosmos_hda_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c arcade.cpp -o arcade.o -ffreestanding -fno-exceptions -fno-rtti -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c cosmos_partition.cpp -o cosmos_partition_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast
g++ -m64 -mno-red-zone -O2 -c memory.cpp -o memory_64.o -ffreestanding -fno-exceptions -fno-rtti -fpermissive -Wno-int-to-pointer-cast

echo "------------------------------------------"
echo "4. Linke OS2 (Die flache KERNEL.BIN)..."
ld -m elf_x86_64 -T linker64.ld -z noexecstack --allow-multiple-definition os2_entry.o kernel_main_64.o kernel_64.o pci_64.o net_64.o cosmos_bytes_64.o cosmos_fs_64.o cosmos_tba_64.o cosmos_ahci_64.o cosmos_cfs_64.o cosmos_usb_64.o cosmos_hda_64.o arcade.o cosmos_partition_64.o memory_64.o -o kernel_main.elf

objcopy -O binary kernel_main.elf isodir/KERNEL.BIN

echo "5. Externe App kompilieren..."
cat > app_linker.ld << 'EOF'
ENTRY(app_main)
SECTIONS
{
    . = 0x01100000;  /* BARE METAL FIX: Auf 64 Megabyte hochschieben! */
    .text : { *(.text.entry) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss : { *(.bss .bss.*) }
    /DISCARD/ : { *(.eh_frame) *(.comment) *(.note*) }
}
EOF

g++ -m64 -mno-red-zone -O2 -c app.cpp -o app.o -ffreestanding -fno-exceptions -fno-rtti
ld -m elf_x86_64 -T app_linker.ld -z noexecstack app.o -o app.elf
objcopy -O binary app.elf app.bin
echo "-> app.bin erfolgreich extrahiert!"

# ==============================================================
# BARE METAL FIX: DOPPEL-INJEKTION & FESTPLATTEN-ERWEITERUNG
# ==============================================================
echo "5. Injiziere app.bin in virtuelle Laufwerke..."

# Wir zwingen die app.bin auf exakt Sektor 10000
dd if=app.bin of=cosmos_drive.img bs=512 seek=10000 conv=notrunc status=none
dd if=app.bin of=cosmos_hdd.img bs=512 seek=10000 conv=notrunc status=none

echo "-> Injektion und Speicher-Erweiterung erfolgreich!"

echo "------------------------------------------"
echo "6. ISO zusammenbauen..."

cat > isodir/boot/grub/grub.cfg << 'EOF'
set timeout=0
set default=0
menuentry "Cosmos OS" {
    set gfxpayload=800x600x32
    multiboot /boot/kernel.bin
    module /KERNEL.BIN    # WICHTIG: Das ist dein Kernel-Payload, der bleibt!
    boot
}
EOF

# BARE METAL FIX: Alte ISO eiskalt löschen, damit xorriso/Windows nicht meckert!
rm -f cosmos.iso 

# Nur EINMAL ausführen und Fehler sichtbar lassen!
grub-mkrescue -o cosmos.iso isodir
echo "=========================================="
echo "          ISO ERFOLGREICH GEBAUT          "
echo "=========================================="