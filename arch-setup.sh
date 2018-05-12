# Create partitions.
cgdisk /dev/sda
    # Create /dev/sda1 (type ef00)
    # Create /dev/sda2 (type 8e00)

# or with gdisk
gdisk /dev/sda
    # Create a new partition table: o
    # Create a new partition: n
    # Verify that there are no problems: v
    # Write table to disk: w

# Format EFI partition.
mkfs.fat -F32 /dev/sda1

# Setup crypto.
cryptsetup luksFormat /dev/sda2
cryptsetup open --type luks /dev/sda2 lvm

# Setup LVM.
pvcreate /dev/mapper/lvm
vgcreate arch /dev/mapper/lvm
lvcreate -n swap -L 10G arch
lvcreate -n root -L 64G arch
lvcreate -n home -l 100%FREE arch
mkfs.ext4 /dev/mapper/arch-root -L system
mkfs.ext4 /dev/mapper/arch-home -L data
mkswap /dev/mapper/arch-swap

# Check partitions.
lsblk

# Mount partitions.
mount /dev/mapper/arch-root /mnt
mkdir /mnt/home
mkdir /mnt/boot
mount /dev/mapper/arch-home /mnt/home
mount /dev/sda1 /mnt/boot
swapon /dev/mapper/arch-swap

# Install base system.
reflector --country 'Ukraine' --age 12 --protocol https --sort rate --save /etc/pacman.d/mirrorlist
pacstrap /mnt base base-devel
genfstab -p /mnt >> /mnt/etc/fstab

# Configure system.
arch-chroot /mnt
pacman -S vim intel-ucode wpa_supplicant
vim /etc/hostname
ln -sf /usr/share/zoneinfo/Europe/Kiev /etc/localtime
hwclock --systohc
vim /etc/locale.gen
locale-gen
locale > /etc/locale.conf
vim /etc/sudoers
    # Uncomment '%wheel ALL=(ALL) ALL'
vim /etc/mkinitcpio.conf
    # edit HOOKS
    # HOOKS="base udev autodetect modconf block keyboard encrypt lvm2 resume filesystems fsck"
mkinitcpio -p linux
passwd

# Install bootloader.
bootctl --path=/boot install
vim /boot/loader/loader.conf
    # default arch
    # timeout 5
    # editor 0
vim /boot/loader/entries/arch.conf
    # title   Arch Linux
    # linux   /vmlinuz-linux
    # initrd  /intel-ucode.img
    # initrd  /initramfs-linux.img
    # options cryptdevice=UUID=<...>:arch root=/dev/mapper/arch-root rw resume=/dev/mapper/arch-swap
    #
    # HINT: use the following in vim: `read ! blkid /dev/sda2`


# Add my repo.
sudo pacman-key --init
sudo pacman-key --recv-keys 95453E765FE509F4
pacman-key --finger 95453E765FE509F4
    # Check fingerprint.
sudo pacman-key --lsign-key 95453E765FE509F4
sudo vim /etc/pacman.conf
    # Add the following before [testing] repo
    #     [zzag]
    #     Server = https://zzag.github.io/repo/arch/

# Install apps.
sudo pacman -Syu
sudo pacman -S \
    amarok \
    ark \
    breeze-kde4 \
    dolphin \
    dolphin-plugins \
    extra-cmake-modules \
    ffmpegthumbs \
    gwenview \
    k3b \
    kaccounts-providers \
    kate \
    kcalc \
    kdegraphics-thumbnailers \
    kdialog \
    kget \
    kgpg \
    kmail \
    konsole \
    konversation \
    ktorrent \
    kwin-effects-appear2 \
    kwin-effects-disappear2 \
    kwin-effects-minimize1 \
    kwin-effects-unminimize1 \
    okular \
    plasma \
    plasma5-applets-redshift-control \
    plasma5-applets-thermal-monitor \
    plasma5-applets-weather-widget \
    spectacle \
    android-udev \
    autoconf \
    automake \
    avahi \
    cdparanoia \
    cdrdao \
    cdrtools \
    clang \
    cmake \
    cowsay \
    ctags \
    curl \
    devtools \
    dialog \
    diff-so-fancy \
    docker \
    dvd+rw-tools \
    emovix \
    figlet \
    firefox \
    gdb \
    gimp \
    git \
    go \
    gst-plugins-base \
    gst-plugins-good \
    gvfs \
    hddtemp \
    htop \
    imagemagick \
    inkscape \
    linux-lts \
    lldb \
    llvm \
    lm_sensors \
    mpv \
    neofetch \
    net-tools \
    nss-mdns \
    openssh \
    otf-fira-code \
    otf-fira-mono \
    p7zip \
    packagekit \
    packagekit-qt5 \
    perf \
    pkgfile \
    python-pip \
    ranger \
    rclone \
    redshift \
    ripgrep \
    rsync \
    telegram-desktop \
    the_silver_searcher \
    tldr \
    tmux \
    transcode \
    tree \
    ttf-croscore \
    ttf-liberation \
    ttf-meslo \
    ttf-meslo-dz \
    ttf-roboto-mono \
    ufw \
    unrar \
    unzip \
    valgrind \
    vcdimager \
    veracrypt \
    w3m \
    wireless_tools \
    wireshark-qt \
    xdg-user-dirs \
    xterm \
    youtube-dl \
    zsh

# Configure SDDM.
sddm --example-config | sudo tee /etc/sddm.conf.d/sddm.conf > /dev/null
    # Set MinimumVT to 7.
sudo systemctl enable sddm
sudo cp /etc/systemd/system/display-manager.service /etc/systemd/system/sddm.service
    # Delete Conflicts field in /etc/systemd/system/sddm.service
sudo systemctl disable sddm
sudo systemctl enable sddm

# Use a DNS cache.
sudo ln -s /run/systemd/resolve/stub-resolv.conf /etc/resolv.conf
sudo systemctl enable systemd-resolved
sudo systemctl start systemd-resolved

# Enable avahi.
sudo vim /etc/nsswitch.conf
    # Change the hosts line to include `mdns_minimal [NOTFOUND=return]`
    # before `resolve` and `dns`.
sudo cp /usr/share/doc/avahi/*.service /etc/avahi/services/
sudo systemctl enable avahi-daemon
sudo systemctl start avahi-daemon

# Configure ufw.
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow mdns
sudo ufw allow ssh

# Enable ssh daemon.
sudo systemctl enable sshd

# Use subpixel font rendering.
sudo vim /etc/profile.d/freetype2.sh
    # Add the following line:
    # export FREETYPE_PROPERTIES="truetype:interpreter-version=38"
sudo ln -s /etc/fonts/conf.avail/10-hinting-slight.conf /etc/fonts/conf.d/10-hinting-slight.conf
sudo ln -s /etc/fonts/conf.avail/10-sub-pixel-rgb.conf /etc/fonts/conf.d/10-sub-pixel-rgb.conf
sudo ln -s /etc/fonts/conf.avail/11-lcdfilter-default.conf /etc/fonts/conf.d/11-lcdfilter-default.conf
sudo cp fonts.conf /etc/fonts/local.conf

# Configure KMail
#     To generate an app password go to https://security.google.com/settings/security/apppasswords

# Use LTS kernel.
sudo cp /boot/loader/entries/arch.conf /boot/loader/entries/arch-lts.conf
sudo vim /boot/loader/entries/arch-lts.conf
    # Point initrd and linux to the LTS kernel.
sudo vim /boot/loader/loader.conf
    # Change default kernel.

# Install update notifier.
sudo pacman -S update-notifier
sudo systemctl enable update-notifier.timer
sudo systemctl start update-notifier.timer

# Enable docker.
sudo systemctl enable docker
