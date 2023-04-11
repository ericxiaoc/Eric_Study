#Qt中的类型转换
####QString转换成char*
``` cpp
QString tmp = "12345"
.toUtf8() 转换成QByteArray
.Data() 转换成char*

tmp.toUtf8().Data() 则为char*类型
```
