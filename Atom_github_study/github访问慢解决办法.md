###github 快速访问的设置方法
    * 首先打开http://tool.chinaz.com/dns 查询域名解析
    * 在请输入域名中输入`github.com`
    * 点击检测选择TTL值最低的IP和域名复制 比如:192.30.253.113 github.com
    * 打开自己的hosts文件在最后添加上此ip和域名
###具体步骤可以参考下面的图片
![a](assets/markdown-img-paste-20210121134027533.png)
![b](assets/markdown-img-paste-20210121134134356.png)
* 选择TTL值比较低的IP地址，填写到hosts文件中　
* C:\Windows\System32\drivers\etc目录中无hosts文件，就创建一个.
```
  127.0.0.1 www.1.com
  199.232.69.194 github.global.ssl.fastly.net
  13.229.188.59 github.com
  140.82.113.3 gist.github.com
  34.193.233.154 github.atom.io
```
　
