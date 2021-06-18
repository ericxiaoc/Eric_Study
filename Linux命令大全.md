###新增ubuntu用户
```
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
```
sudo adduser eric sudo
```
###samba重启
```
sudo /etc/init.d/smbd stop
sudo /etc/init.d/smbd start
```
###samba添加用户
```
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
```
testparm rlimit_max: increasing rlimit_max (1024) to minimum Windows limit (16384)
Fix:
......
sudo vim /etc/security/limits.conf
# End of file
* - nofile 16384
```
