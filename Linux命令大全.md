###新增ubuntu用户
``` shell
sudo adduser username

[sudo] password for lxp:
Adding user `eric' ...
Adding new group `eric' (1008) ...
Adding new user `eric' (1008) with group `eric' ...
Creating home directory `/home/eric' ...
Copying files from `/etc/skel' ...
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
Changing the user information for eric
Enter the new value, or press ENTER for the default
	Full Name []: eric
	Room Number []:
	Work Phone []:
	Home Phone []:
	Other []:
Is the information correct? [Y/n] y

```
###添加用户到指定的组
``` shell
sudo adduser eric sudo
```
###samba重启
``` shell
sudo /etc/init.d/smbd stop
sudo /etc/init.d/smbd start
```
###samba添加用户
``` shell
sudo smbpasswd -a eric
```
###修改现配置文件
  ``` shell
sudo gedit /etc/samba/smb.conf

[share]
 path = /home/xxx/share
 available = yes
 browseable = yes
 public = yes
 writable = yes
 valid users =myname
 create mask = 0700
 directory mask =0700
 force user =nobody
 force group = nogroup
 ```

	说明：
			valid users = myname　这个myname是自己起的，后面在添加用户名时就是添加的这个
	另外这个[share]名字也是可以随便起的，这个是你在windows下访问时显示的名字。

###ubuntu14.04 samba 无法访问。您可能没有权限使用网络资源
* testparm -s 命令使用
``` makefile
testparm rlimit_max: increasing rlimit_max (1024) to minimum Windows limit (16384)
Fix:
......
sudo vim /etc/security/limits.conf
# End of file
* - nofile 16384
```
###复制带.git 仓库的目录
* 当前的目录的A仓库拷贝到另外一个目录
``` shell
git clone  /xxx/xxx/A　　/tmp
```
* 局域网的拷贝
``` shell
//局域网服务器上的A仓库拷贝到本地tmp目录
git clone eric@192.168.12.9:/media/server09/user25/eric/MT8163_Android_8.1_debug/MT8163_O1_MP5_V1  ./MT8163_O1_MP5_V1/
```
###ubuntu卸载安装包
``` shell
sudo apt-get --purge remove 包名
```
###git-svn clone中断处理
``` shell
git svn clone --stdlayout svn地址
```
###复制文件到指定目录，并自动创建对应目录
``` shell
cp --path aps/customer/s2tek/public/panel/AUO_T420HW04_V0_1920x1080_99du/panel_setting.h  ./_product/V510/
```
###scp跨服务器拷贝文件
``` shell
scp -r mt8385_android_p_v1.345.tar.7z.001 eric@192.168.12.10://media/server10/user27/eric/mt8385
```
