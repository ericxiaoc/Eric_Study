#MTK上手指南
###驱动
####配置目录
#####Gpio配置：
``` c
vendor/mediatek/proprietary/scripts/dct/old_dct/DrvGen.exe(旧版本)
vendor/mediatek/proprietary/scripts/dct/DrvGen.exe(新版本)
//kernel
-->>vendor/mediatek/proprietary/custom/cre_2001/kernel/dct/dct/codegen.dws
//lk
-->>vendor/mediatek/proprietary/bootable/bootloader/lk/target/cre_2001/dct/codegen.dws
```
#####配置文件
######makefile 变量
* ProjectConfig.mk & BoardConfig.mk
######Android层的配置
* init.rc 初始化的脚本
* 系统属性system.prop
######kernel配置目录
* kernel-3.18/arch/arm64/configs/cre_2001_debug_defconfig
* kernel-3.18/arch/arm64/configs/cre_2001_defconfig
######编译信息
* buildinfo.sh
###UBOOT
