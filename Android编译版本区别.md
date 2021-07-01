###eng、user、userdebug区别简述
![a](assets/markdown-img-paste-2021062109305523.png)
如果目标构建变体为 user，则 adb 无法获取 root 权限。
|                    user                    |                 userdebug                  |                           eng                            |
|:------------------------------------------:|:------------------------------------------:|:--------------------------------------------------------:|
|          仅安装标签为 user 的模块          |       安装标签为 user、debug 的模块        |            安装标签为 user、debug、eng 的模块            |
|   设定属性 ro.secure=1，打开安全检查功能   |   设定属性 ro.secure=1，打开安全检查功能   |          设定属性 ro.secure=0，关闭安全检查功能          |
| 设定属性 ro.debuggable=0，关闭应用调试功能 | 设定属性 ro.debuggable=1，启用应用调试功能 |        设定属性 ro.debuggable=1，启用应用调试功能        |
|                                            |                                            | 设定属性 ro.kernel.android.checkjni=1，启用 JNI 调用检查 |
|             默认关闭 adb 功能              |             默认打开 adb 功能              |                    默认打开 adb 功能                     |
|            打开 Proguard 混淆器            |            打开 Proguard 混淆器            |                   关闭 Proguard 混淆器                   |
|        打开 DEXPREOPT 预先编译优化         |        打开 DEXPREOPT 预先编译优化         |               关闭 DEXPREOPT 预先编译优化                |

* userdebug版本如何挂载system
``` c
C:\Users\Eric>adb disable-verity
Verity disabled on /system
Verity disabled on /vendor
Verity already disabled on /system
Verity already disabled on /vendor
Now reboot your device for settings to take effect

C:\Users\Eric>adb root

C:\Users\Eric>adb remount
remount succeeded
```
