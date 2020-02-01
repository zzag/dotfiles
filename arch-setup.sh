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

# Install apps.
sudo pacman -Syu
sudo pacman -S \
    apper \
    ark \
    dolphin \
    dolphin-plugins \
    extra-cmake-modules \
    ffmpegthumbs \
    gwenview \
    k3b \
    kaccounts-providers \
    kate \
    kcalc \
    kdeconnect \
    kdegraphics-thumbnailers \
    kdialog \
    kget \
    kgpg \
    kfind \
    kmines \
    konsole \
    ksudoku \
    okular \
    plasma \
    spectacle \
    android-udev \
    autoconf \
    automake \
    avahi \
    bind-tools \
    boost \
    cantata \
    cdparanoia \
    cdrdao \
    cdrtools \
    clang \
    cmake \
    cowsay \
    ctags \
    cups \
    curl \
    devtools \
    dialog \
    diff-so-fancy \
    dnscrypt-proxy \
    dvd+rw-tools \
    emovix \
    figlet \
    firefox \
    fish \
    flatpak \
    gdb \
    gimp \
    git \
    go \
    gst-plugins-base \
    gst-plugins-good \
    gvfs \
    gvim \
    hddtemp \
    hplip \
    htop \
    imagemagick \
    inkscape \
    keychain \
    libdbusmenu-glib \
    libreoffice-fresh \
    linux-headers \
    linux-lts \
    linux-lts-headers \
    lldb \
    llvm \
    lm_sensors \
    meson \
    mpd \
    mpv \
    neofetch \
    net-tools \
    ninja \
    noto-fonts \
    noto-fonts-emoji \
    nss-mdns \
    nvidia \
    nvidia-lts \
    openssh \
    otf-fira-code \
    otf-fira-mono \
    otf-fira-sans \
    p7zip \
    packagekit \
    packagekit-qt5 \
    pacman-contrib \
    perf \
    pkgfile \
    python-pip \
    qbittorrent \
    qt5-doc \
    qtcreator \
    quassel-monolithic \
    ranger \
    rclone \
    ripgrep \
    rsync \
    sshfs \
    synergy \
    the_silver_searcher \
    thunderbird \
    tldr \
    tmux \
    transcode \
    tree \
    ttf-croscore \
    ttf-liberation \
    ttf-ubuntu-font-family \
    ufw \
    unrar \
    unzip \
    valgrind \
    vcdimager \
    w3m \
    wireless_tools \
    wireshark-qt \
    xcursor-vanilla-dmz \
    xcursor-vanilla-dmz-aa \
    xdg-desktop-portal \
    xdg-desktop-portal-kde \
    xdg-user-dirs \
    xorg-server-xephyr \
    xorg-server-xvfb \
    xorg-server-xwayland \
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

# Configure dnscrypt-proxy.
sudo vim /etc/dnscrypt-proxy/dnscrypt-proxy.toml
    # Edit server_names = ['cloudflare', 'google'].
sudo vim /etc/resolvconf.conf
    # Uncomment name_servers=127.0.0.1.
sudo resolvconf -u
sudo vim /etc/NetworkManager/conf.d/rc-manager.conf
    # [main]
    # rc-manager=resolvconf
sudo systemctl enable --now dnscrypt-proxy.service

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
sudo ufw allow 1714:1764/udp # KDE Connect over UDP
sudo ufw allow 1714:1764/tcp # KDE Connect over TCP
sudo ufw allow 24800 # Synergy

# Enable ssh daemon.
sudo systemctl enable sshd

# Enable NetworkManager.
sudo systemctl enable NetworkManager

# Use subpixel font rendering.
sudo vim /etc/profile.d/freetype2.sh
    # Add the following line:
    # export FREETYPE_PROPERTIES="truetype:interpreter-version=38"
sudo rm /etc/fonts/conf.d/10-hinting-slight.conf
sudo ln -s /etc/fonts/conf.avail/10-hinting-none.conf /etc/fonts/conf.d/10-hinting-none.conf
sudo ln -s /etc/fonts/conf.avail/10-sub-pixel-rgb.conf /etc/fonts/conf.d/10-sub-pixel-rgb.conf
sudo ln -s /etc/fonts/conf.avail/11-lcdfilter-default.conf /etc/fonts/conf.d/11-lcdfilter-default.conf
sudo cp fonts.conf /etc/fonts/local.conf

# Use LTS kernel.
sudo cp /boot/loader/entries/arch.conf /boot/loader/entries/arch-lts.conf
sudo vim /boot/loader/entries/arch-lts.conf
    # Point initrd and linux to the LTS kernel.
sudo vim /boot/loader/loader.conf
    # Change default kernel.

# Update pkgfile metadata.
sudo pkgfile --update

# Enable systemd-timesyncd.
sudo timedatectl set-ntp true

# Configure keychain.
vim ~/.zprofile
    # export SSH_ASKPASS=/usr/bin/ksshaskpass
    # eval `keychain --eval --quiet --agents ssh <keys>`

# Configure Cantata.
mkdir ~/.config/mpd/playlists
ln -s path/to/dotfiles/mpd.conf ~/.config/mpd/mpd.conf
systemctl --user enable mpd
systemctl --user start mpd
cantata # Follow configuration wizard.

# Setup ccache.
sudo pacman -S ccache
echo 'export PATH="/usr/lib/ccache/bin:$PATH"' >> ~/.zprofile
ccache --set-config=max_size=<max cache size, e.g. 100.0G>

# Install yay.
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si

# Install ScanGearMP.
yay -S scangearmp2

# Install Google Chrome.
yay -S google-chrome

# Configure Synergy.
sudo vim /etc/synergy.conf
    # section: screens
    # 	MacMini:
    # 	Desktop:
    # end
    #
    # section: links
    # 	# Mac Mini is to the left of Desktop.
    # 	MacMini:
    # 		right = Desktop
    #
    # 	# Desktop is to the right of Mac Mini.
    # 	Desktop:
    # 		left = MacMini
    # end
    #
    # section: aliases
    # 	MacMini:
    # 		Vlads-Mac-mini.local
    #
    # 	Desktop:
    # 		lithium
    # end
    #
    # section: options
    # 	screenSaverSync = false
    # end

# Enable CUPS.
sudo systemctl enable org.cups.cupsd

# Configure Quassel.
# Interface:
#   * Check "Hide to tray on close button"
#
# Chat View:
#   * Check "Custom Colors"
#   * Check "Use Sender Coloring"
#   * Uncheck "Show previews of webpages on URL hover"
#
# Chat & Nick Lists:
#   * Check "Use Custom Colors" and use the following colors:
#     - Standard: #000000
#     - Inactive: #a0a0a4
#     - Unread messages: #64b5f6
#     - Highlight: #ff5500
#     - Other activity: #9ccc65
#   * Check "Display topic in tooltip"
#
# Input Widget:
#   * Uncheck "Show style buttons"

# Install RTL8812AU driver.
yay -S rtl88xxau-aircrack-dkms-git
