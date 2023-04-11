##下载rk官方代码
####生成ssh密钥
``` shell
ssh-keygen -t rsa -C "your_email@example.com"
```
####替换有权限的密钥

``` shell
# 拷贝代理商密钥替换掉生成的 id_rsa id_rsa.pub
```
####下载代码
``` shell
#这里以px30为例
git clone ssh://git@www.rockchip.com.cn/repo/rk/tools/repo

#repo init
repo/repo init --repo-url ssh://git@www.rockchip.com.cn/repo/rk/tools/repo -u ssh://git@www.rockchip.com.cn/linux/rockchip/platform/manifests -b linux -m px30_linux_release.xml

#开始同步代码
.repo/repo/repo sync -l

#拉取到本地
.repo/repo/repo sync -c --no-tags
```
###遇到的问题
``` shell
# 执行ssh-add时出现Could not open a connection to your authentication
# agent,执行如下:
ssh-agent bash
ssh-add ~/.ssh/id_rsa
```

``` shell
#Try re-running with "-j1 --fail-fast" to exit at the first error
.repo/repo/repo sync -j1 --fail-fast
```
``` shell
#rockchip px30
repo/repo init --repo-url ssh://git@www.rockchip.com.cn/repo/rk/tools/repo -u ssh://git@www.rockchip.com.cn/linux/rockchip/platform/manifests -b linux -m px30_linux_release.xml
#rockchip rk3568
repo/repo init --repo-url ssh://git@www.rockchip.com.cn/repo/rk/tools/repo -u ssh://git@www.rockchip.com.cn/linux/rockchip/platform/manifests -b linux -m rk356x_linux_release.xml
#rockchip rk3399
repo/repo init --repo-url ssh://git@www.rockchip.com.cn/repo/rk/tools/repo -u ssh://git@www.rockchip.com.cn/linux/rk/platform/manifests -b linux -m rk3399_linux_release.xml
```

###代码克隆地址
``` shell
rk3399_linux:
git clone http://eric@192.168.4.168:8686/r/ly_rk3399_linux_sdk.git

rk3568_linux:
git clone http://eric@192.168.4.168:8686/r/ly_rk3568_linux_sdk.git

px30_linux:
git clone http://eric@192.168.4.168:8686/r/ly_px30_linux_sdk.git

Android 11(rk3399 rk3568 px30):
git clone http://eric@192.168.4.168:8686/r/ly_rockchip_Android11.0_sdk.git
```
