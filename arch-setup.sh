# Wi-Fi setup.
iwctl
[iwd] device list
[iwd] station <device> scan
[iwd] station <device> get-networks
[iwd] station <device> connect <SSID>

# Create partitions.
cgdisk /dev/sda
    # Create /dev/sda1 (type ef00)
    # Create /dev/sda2 (type 8300)

# or with gdisk
gdisk /dev/sda
    # Create a new partition table: o
    # Create a new partition: n
    # Verify that there are no problems: v
    # Write table to disk: w

# Setup EFI partition.
mkfs.fat -F32 /dev/sda1

# Setup btrfs partition.
mkfs.btrfs /dev/sda2
mount /dev/sda2 /mnt

btrfs subvolume create /mnt/@
btrfs subvolume create /mnt/@home
btrfs subvolume create /mnt/@var
umount /mnt

mount -o subvol=@ /dev/sda2 /mnt
mkdir /mnt/{boot,home,var}
mount /dev/sda1 /mnt/boot
mount -o subvol=@home /dev/sda2 /mnt/home
mount -o subvol=@var /dev/sda2 /mnt/var

# Install base system.
pacstrap -K /mnt base base-devel linux linux-firmware
genfstab -U /mnt >> /mnt/etc/fstab

# Configure system.
arch-chroot /mnt
pacman -S iwd intel-ucode vim

# Install bootloader.
bootctl install
vim /boot/loader/loader.conf
    # default arch
    # timeout 5
    # editor 0
vim /boot/loader/entries/arch.conf
    # title   Arch Linux
    # linux   /vmlinuz-linux
    # initrd  /intel-ucode.img
    # initrd  /initramfs-linux.img
    # options root=UUID=<...> rootflags=subvol=@ rw
    #
    # HINT: use the following in vim: `read ! blkid /dev/sda2`

# Configure hostname.
hostnamectl set-hostname lithium

# Configure time.
timedatectl set-ntp true
timedatectl set-timezone Europe/Kyiv

# Configure locale.
vim /etc/locale.gen
locale-gen
localectl set-locale en_US.UTF-8

# Configure sudo (uncomment `%wheel ALL=(ALL) ALL`)
vim /etc/sudoers

# Build initramfs.
mkinitcpio -p linux

# Set root password.
passwd

# Install apps.
sudo pacman -Syu
sudo pacman -S \
    plasma \
    ark \
    dolphin \
    ffmpegthumbs \
    gwenview \
    kate \
    kdegraphics-thumbnailers \
    kdialog \
    konsole \
    okular \
    spectacle \
    bluez-utils \
    clang \
    cmake \
    cups \
    curl \
    firefox \
    fish \
    flatpak \
    fwupd \
    gdb \
    gimp \
    git \
    go \
    gst-plugins-base \
    gst-plugins-good \
    gvfs \
    hplip \
    htop \
    imagemagick \
    keepassxc \
    libreoffice-fresh \
    lldb \
    llvm \
    meson \
    mpv \
    ninja \
    noto-fonts \
    noto-fonts-emoji \
    openssh \
    otf-fira-mono \
    otf-fira-sans \
    ripgrep \
    rsync \
    thunderbird \
    tmux \
    ttf-croscore \
    ttf-liberation \
    ttf-ubuntu-font-family \
    ufw \
    unzip \
    wireless_tools \
    xcursor-vanilla-dmz \
    xcursor-vanilla-dmz-aa \
    xdg-user-dirs \
    xterm

# Enable SDDM.
sudo systemctl enable sddm

# Enable systemd-resolved.
sudo systemctl enable systemd-resolved

# Enable ssh daemon.
sudo systemctl enable sshd

# Enable NetworkManager.
sudo systemctl enable NetworkManager

# Enable CUPS.
sudo systemctl enable org.cups.cupsd

# Enable bluetooth.
sudo systemctl enable bluetooth

# Configure ufw.
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw limit ssh
sudo ufw enable
sudo systemctl enable ufw

# Configure home.
sudo systemctl enable --now systemd-homed
sudo homectl create vlad --shell=/usr/bin/fish --storage=luks

# Use subpixel font rendering.
sudo rm /etc/fonts/conf.d/10-hinting-slight.conf
sudo ln -s /etc/fonts/conf.avail/10-hinting-none.conf /etc/fonts/conf.d/10-hinting-none.conf
sudo ln -s /etc/fonts/conf.avail/10-sub-pixel-rgb.conf /etc/fonts/conf.d/10-sub-pixel-rgb.conf
sudo ln -s /etc/fonts/conf.avail/11-lcdfilter-default.conf /etc/fonts/conf.d/11-lcdfilter-default.conf
sudo cp fonts.conf /etc/fonts/local.conf

# Setup ccache.
sudo pacman -S ccache
ccache --set-config=max_size=<max cache size, e.g. 100.0G>

# Install yay.
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si

# Install ScanGearMP.
yay -S scangearmp2

# Configure the SSH key agent.
mkdir -p ~/.config/systemd/user/
ln -s config/systemd/user/ssh-agent.service ~/.config/systemd/user/
systemctl --user enable --now ssh-agent
