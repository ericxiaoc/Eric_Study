# RK3399 ubuntu18.04 根文件系统
###使用环境
* rk3399_linux代码
* ubuntu18.04桌面系统
* ubuntu18.04 base代码
###修改根文件系统
####解压目录且拷贝文件
下载ubuntu基本裸系统
http://cdimage.ubuntu.com/ubuntu-base/releases/18.04.1/release/
``` shell
mkdir ubuntufs
sudo tar -xpf ubuntu-base-18.04.1-base-arm64.tar.gz -C ubuntufs
```
####安装qemu环境
``` shell
sudo apt-get install qemu-user-static
```
拷贝对应的系统结构文件到解压的目录,我这里是arm64.
``` shell
sudo cp /usr/bin/qemu-aarch64-static ubuntufs/usr/bin/
```
####准备网络
将本机的dns配置复制到目标rootfs
``` shell
sudo cp -b /etc/resolv.conf ubuntufs/etc/resolv.conf
```
####使用chroot命令进入根文件系统进行操作
``` shell
sudo chroot ubuntufs/
```
####更新及安装软件
``` shell
apt-get update
apt-get upgrade
```
####替换软件源
``` shell
deb http://ports.ubuntu.com/ubuntu-ports/ bionic main restricted
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates main restricted
deb http://ports.ubuntu.com/ubuntu-ports/ bionic universe
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates universe
deb http://ports.ubuntu.com/ubuntu-ports/ bionic multiverse
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-updates multiverse
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-backports main restricted universe multiverse
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-security main restricted
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-security universe
deb http://ports.ubuntu.com/ubuntu-ports/ bionic-security multiverse
```
####安装常用的工具
``` shell
apt install sudo language-pack-en-base ssh net-tools ethtool wireless-tools ifupdown iputils-ping htop vim kmod network-manager xorg  bash-completion  htop qt5-default openbox make --no-install-recommends
```
``` shell
--no-install-recommends
```
参数来避免安装非必须的文件,从而减小镜像的体积
####添加用户及设置密码
``` shell
useradd -s '/bin/bash' -m -G adm,sudo langyu
passwd langyu
passwd root
```
####设置可使用root用户登录ssh
安装ssh后，默认情况下不允许使用root用户登录，需作以下修改
``` shell
vi /etc/ssh/sshd_config
```
找到``` PermitRootLogin prohibit-password```，改成```PermitRootLogin yes```
####设置主机名
默认主机名为localhost.localdomain
``` shell
 echo 'ly_ubuntu' > /etc/hostname
```
添加主机入口
``` shell
root@lysver:/# vi /etc/hosts
127.0.0.1		localhost
127.0.0.1		ly_ubuntu
```
####允许root用户登录串口
``` shell
vi /etc/pam.d/login
```
搜索```pam_securetty.so```，找到改行在前面加#号注释掉
``` shell
#auth [success=ok new_authtok_reqd=ok ignore=ignore user_unknown=bad default=die] pam_securetty.so
```
###制作根文件系统
####退出文件系统;
``` shell
root@lysver:/# exit
```
####查看文件系统的大小
``` shell
sudo du -h --max-depth=0 ubuntufs/
859M	ubuntufs/
```
####用dd工具创建镜像文件
``` shell
dd if=/dev/zero of=linuxroot.img bs=1M count=2048
```
####格式化镜像文件
``` shell
mkfs.ext4 -F -L linuxroot linuxroot.img
```
####挂载镜像并往里面拷入修改后的文件系统
``` shell
mkdir rootfs
sudo mount linuxroot.img rootfs/
sudo cp -rfp ubuntufs/*  rootfs/
sudo umount rootfs/
```
####检查并修复linuxroot.img镜像的文件系统
``` shell
e2fsck -p -f linuxroot.img
```
####减小ubuntu.img镜像文件的大小
``` shell
resize2fs -M linuxroot.img
```
####查看镜像文件大小
``` shell
ls -lh linuxroot.img 
```
