# Development Setup

## Embedded Systems

### RPi3

#### Rasbian

Select `Fake/Full KMS` in `raspi-config/advanced/gl` for OpenGLES 2.0. Reboot!

```sh
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev golang
```

- [https://gist.github.com/theramiyer/cb2b406128e54faa12c37e1a01f7ae15](https://gist.github.com/theramiyer/cb2b406128e54faa12c37e1a01f7ae15)

### Beaglebone Black

- [Beaglebone Black SGX Graphics](https://elinux.org/BeagleBoneBlack/SGX_%2B_Qt_EGLFS_%2B_Weston#Sources)
- [https://canox.net/2019/10/arch-linux-auf-dem-beaglebone-black-installieren/](https://canox.net/2019/10/arch-linux-auf-dem-beaglebone-black-installieren/)
- [https://archlinuxarm.org/platforms/armv7/ti/beaglebone-black](https://archlinuxarm.org/platforms/armv7/ti/beaglebone-black)

#### Post Install

```sh
sed -i 's/#Color/Color/' /etc/pacman.conf
pacman-key --init
pacman-key --populate archlinuxarm
pacman -S archlinux-keyring
pacman -Sy pacman
pacman -Syu
passwd
hostnamectl set-hostname $HOSTNAME
pacman -S sudo vim
EDITOR=vim visudo
useradd -d /home/$YOURUSERNAME -m -G wheel -s /bin/bash $YOURUSERNAME
passwd $YOURUSERNAME
reboot
userdel alarm
sudo pacman -S htop zsh rsync git screenfetch
sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
git clone https://github.com/tobiashienzsch/dotfiles.git
```
