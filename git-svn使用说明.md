#Git-SVN说明
###前言
    为了更好的管理项目，由于服务器是svn服务器，所以这里推荐使用git-svn协同工作，在原来的项目工程基础上可以使用git，git更适合Android这种类型的项目，接下来介绍相关命令。
###git-svn常见命令
#####1. 克隆svn项目
```shell
git svn clone  [svn url]
```
例如：MT8163
``` shell
git svn clone svn://192.168.12.7/MT8163_O/MT8163_O1_MP5_V1
```
#####2. svn提交版本过多，采用下面的命令进行克隆，r1250表示从该版本开始克隆
``` shell
git svn clone -r1250:HEAD --prefix=svn/ svn://192.168.12.7/MT8163_O/MT8163_O1_MP5_V1
```
#####3. 查看当前版本库情况
``` shell
git svn info
```
#####4. 取回远程仓库所有分支的变化
``` shell
git svn fetch
```
####5. 取回远程仓库当前分支的变化，并与本地分支变基合并
``` shell
git svn rebase
```
####6. 上传当前分支的本地仓库到远程仓库
``` shell
git svn dcommit
```
####7. 清空当前项目的所有修改包括新增文件
``` shell
 git add --all && git reset --hard
```
####8. 同步 svn最新节点
``` shell
git svn fetch
git svn rebase
```
####8. 克隆失败处理方式（跟运行环境有关）
```
提示 error: git-svn died of signal 11
```
处理方法:
``` shell
rm -rf .git
git svn init svn://192.168.12.7/MT8163_O/MT8163_O1_MP5_V1
git svn fetch
```
####9. 提交修改到svn
``` shell
git add --all  #提交所有修改，包括新增到本地暂存区
git commit -am "20210709 1.测试提交"  #提交本地仓库
git svn dcommit #提交到svn服务器
```
####10. 常用git命令
``` shell
git status  #查看当前项目所有修改记录
git status <dir>  #查看指定目录的修改情况
git diff <file path>  #查看当前文件修改的内容
```
