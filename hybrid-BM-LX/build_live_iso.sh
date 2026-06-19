#!/bin/bash
set -e

clear
echo "=========================================="
echo "    CosmOS Build-System"
echo "=========================================="
echo "Welche ISO-Version möchtest du bauen?"
echo " [1] FAST MODE    (Gzip: Baut in Sekunden, ca. 1.2 GB, für schnelle Tests)"
echo " [2] RELEASE MODE (XZ Max: Dauert sehr lange, kleinste Datei für Hardware)"
read -p "Wähle 1 oder 2 (Standard ist 1): " BUILD_MODE

if [ "$BUILD_MODE" == "2" ]; then
    # Extrem starke Komprimierung mit 1MB Blockgröße (Maximales Limit)
    COMP_ARGS="-comp xz -b 1M"
    echo "-> RELEASE MODE aktiviert. Hol dir einen Kaffee, das dauert jetzt..."
    sleep 2
else
    # Rasend schnelle Komprimierung
    COMP_ARGS="-comp gzip"
    echo "-> FAST MODE aktiviert. Turbogang eingelegt!"
    sleep 1
fi

echo "------------------------------------------"
echo "-> Erstelle automatisches Backup..."
BACKUP_DIR="backups/cosmos_$(date +%Y-%m-%d_%H-%M-%S)"
mkdir -p "$BACKUP_DIR"
cp *.cpp *.h *.s *.ld "$BACKUP_DIR/" 2>/dev/null || true
echo "-> Quellcodes sicher kopiert nach: $BACKUP_DIR"

echo "------------------------------------------"
echo "0. Alten Müll aufräumen..."

echo "1. Cleaning up old build files..."
rm -f CosmOS.iso
mkdir -p /tmp/meinos_build

echo "2. Compiling meinos.elf (Linux version)..."
make -f Makefile.linux clean
make -f Makefile.linux

echo "3. Creating minimal root filesystem (This will take a while)..."
export ROOTFS=/tmp/meinos_build/live_rootfs
mkdir -p $ROOTFS
if [ ! -f "$ROOTFS/bin/bash" ]; then
    debootstrap --variant=minbase --arch=amd64 noble $ROOTFS http://archive.ubuntu.com/ubuntu/
else
    echo "Base system already extracted, skipping debootstrap."
fi

echo "4. Configuring the root filesystem..."
mount -t proc none $ROOTFS/proc || true
mount -t sysfs none $ROOTFS/sys || true
mount -o bind /dev $ROOTFS/dev || true
mount -o bind /dev/pts $ROOTFS/dev/pts || true

# Install necessary packages inside chroot
chroot $ROOTFS /bin/bash -c "
export DEBIAN_FRONTEND=noninteractive
echo 'deb http://archive.ubuntu.com/ubuntu/ noble universe' > /etc/apt/sources.list.d/universe.list
apt-get update

# === COSMOS BASIS & INSTALLER PAKETE ===
apt-get install -y --no-install-recommends \
    linux-image-generic live-boot systemd systemd-sysv \
    xserver-xorg xserver-xorg-video-all xserver-xorg-input-all xinit \
    libsdl2-2.0-0 libgl1-mesa-dri libglx-mesa0 x11-xserver-utils ifupdown linux-firmware \
    xterm rsync os-prober parted sudo dosfstools ntfs-3g efibootmgr \
    openbox grub-pc-bin grub-efi-amd64-bin grub2-common

apt-get clean
# PROFI-TRICK: Löscht den ungenutzten Paket-Cache. Spart riesige Mengen an Platz in der finalen ISO!
rm -rf /var/lib/apt/lists/*
rm -rf /tmp/*
"

# Force create the interfaces file
mkdir -p $ROOTFS/etc/network
touch $ROOTFS/etc/network/interfaces

echo "4.5 Configuring Networking (DHCP)..."
mkdir -p $ROOTFS/etc/systemd/network
mkdir -p $ROOTFS/etc/network 

cat > $ROOTFS/etc/systemd/network/20-wired.network << 'EOF'
[Match]
Name=en* eth*

[Network]
DHCP=yes
EOF

# FIX: Voller Pfad zu systemctl, damit der Installer im chroot nicht crasht!
chroot $ROOTFS /usr/bin/systemctl enable systemd-networkd || true
chroot $ROOTFS /usr/bin/systemctl mask systemd-networkd-wait-online.service || true

# FIX: Prevent background updates from freezing the Live CD
rm -f $ROOTFS/lib/systemd/system/apt-daily.timer
rm -f $ROOTFS/lib/systemd/system/apt-daily.service
rm -f $ROOTFS/lib/systemd/system/apt-daily-upgrade.timer
rm -f $ROOTFS/lib/systemd/system/apt-daily-upgrade.service

umount $ROOTFS/dev/pts || true
umount $ROOTFS/sys || true
umount $ROOTFS/proc || true
umount $ROOTFS/dev || true

echo "5. Setting up Auto-Boot into MeinOS..."
mkdir -p $ROOTFS/opt/meinos
cp meinos.elf $ROOTFS/opt/meinos/
# APP.BIN als Trigger-Datei anlegen (Inhalt egal - die Engine ist in meinos.elf einkompiliert)
echo "MEINOS_APP_TRIGGER" > $ROOTFS/opt/meinos/APP.BIN
cp ROMS.tba $ROOTFS/opt/meinos/ || true
cp install_cosmos.sh $ROOTFS/opt/meinos/ || true
chmod +x $ROOTFS/opt/meinos/install_cosmos.sh || true

cat > $ROOTFS/root/.xinitrc << 'EOF'
#!/bin/sh
xset -dpms
xset s off
xset s noblank

# Startet den winzigen Window Manager im Hintergrund, damit Tastaturen in Apps funktionieren!
openbox &

cd /opt/meinos
exec ./meinos.elf
EOF
chmod +x $ROOTFS/root/.xinitrc

cat > $ROOTFS/etc/systemd/system/meinos.service << 'EOF'
[Unit]
Description=Start MeinOS X11 Environment
After=systemd-user-sessions.service plymouth-quit-wait.service getty@tty1.service
Conflicts=getty@tty1.service

[Service]
ExecStart=/usr/bin/xinit /root/.xinitrc -- /usr/bin/X :0 -nolisten tcp vt1
Restart=always
User=root
StandardInput=tty
TTYPath=/dev/tty1

[Install]
WantedBy=graphical.target
EOF

chroot $ROOTFS /usr/bin/systemctl enable meinos.service || true
chroot $ROOTFS /usr/bin/systemctl set-default graphical.target || true

chroot $ROOTFS passwd -d root

echo "6. Packaging the ISO..."
export ISO_DIR=/tmp/meinos_build/live_iso
rm -rf $ISO_DIR/live/filesystem.squashfs 
mkdir -p $ISO_DIR/live
mkdir -p $ISO_DIR/boot/grub

cp $ROOTFS/boot/vmlinuz-* $ISO_DIR/boot/vmlinuz
cp $ROOTFS/boot/initrd.img-* $ISO_DIR/boot/initrd.img

echo "Compressing filesystem (This will also take a while)..."

# HIER GREIFT JETZT DIE VARIABLE $COMP_ARGS AUS DEM MENÜ
# Ich habe dir außerdem noch die Anführungszeichen um proc/* etc. repariert!
mksquashfs $ROOTFS $ISO_DIR/live/filesystem.squashfs $COMP_ARGS -noappend -e "proc/*" "sys/*" "dev/*" "tmp/*" "run/*"

cat > $ISO_DIR/boot/grub/grub.cfg << 'EOF'
set timeout=0
set gfxpayload=text
menuentry "MeinOS Live" {
    linux /boot/vmlinuz boot=live quiet splash
    initrd /boot/initrd.img
}
EOF

grub-mkrescue -o CosmOS.iso $ISO_DIR

echo "=========================================="
echo "          ISO ERFOLGREICH GEBAUT          "
echo "=========================================="