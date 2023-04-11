#RK平台刷屏参
###如何刷入屏参
    将屏参参数包里面对应的屏幕型号的屏参文件rk_lcd_parameters拷贝到U盘或者TF卡根目录,开机上电插入U盘或者TF都行，等待系统重启，正常显示logo，刷屏参完毕，可以拔掉U盘或TF卡了。
###屏参的参数说明
``` shell
#屏幕类型
# 0 - DSI0 ; 1 - DSI1  2-edp 3-LVDS
panel-type = 2;

# 延时参数，一般不用调整
unprepare-delay-ms = 100;
enable-delay-ms = 100;  //出现上电闪白屏，可调整该参数试试
disable-delay-ms = 100;
prepare-delay-ms = 100;
reset-delay-ms = 100;
init-delay-ms = 100;
bus-format = 4113;
width-mm = 0;
height-mm = 24;

# panel timing
clock-frequency = 147700000;  //输出时钟
hactive = 1920;  //分辨率
hfront-porch = 48;
hsync-len = 32;
hback-porch = 132;
vactive = 1080; //分辨率
vfront-porch = 3;
vsync-len = 6;
vback-porch = 66;
hsync-active = 0;
vsync-active = 0;
de-active = 0;    //一般不需要调整
pixelclk-active = 0;  //时钟极性 0或1
display_rotation = 0; //系统显示方向，包括recovery显示方向 0,90,180,270
touchscreen_rotation = 0;//触摸方向，一般跟系统方向一致;0,90,180,270

# for dsi panel
#mipi 的传输模式，一般常见为 MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST | MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_EOT_PACKET
dsi,flags = 2563; //一般不需要更改

#mipi 颜色，一般都是选择0;
dsi,format = 0;
可配置如下:
    # 0:     MIPI_DSI_FMT_RGB888,
    # 1:     MIPI_DSI_FMT_RGB666,
    # 2:     MIPI_DSI_FMT_RGB666_PACKED,
    # 3:     MIPI_DSI_FMT_RGB565,

#mipi lanes数，一般不需要修改;
dsi,lanes = 4;

#mipi序列，需供应商提供，点屏需修改
panel-init-sequence = 29 00 06 14 01 08 00 00 00 ff aa 01 02 03 04 05 06 07 ff aa AA bb ff;
```
``` shell
屏参刷新的算法

(hactive + hfront-porch + hback-porch + hsync-len) * (vactive + vfront-porch + vback-porch + vsync-len) * 刷新率 = 时钟频率;

刷新率一般需要调整为 55 ~ 60之间最佳，组装屏可能需要高于60;


1. VBP(vertical back porch)：
    表示在一帧图像开始时，垂直同步信号以后的无效的行数，

2. VFP(vertical front porch)：
    表示在一帧图像结束后，垂直同步信号以前的无效的行数，

3. VSPW(vertical sync pulse width)：
    表示垂直同步脉冲的宽度，用行数计算，

4. HBP(horizontal back porch)：
    表示从水平同步信号开始到一行的有效数据开始之间的VCLK的个数，

5. HFP(horizontal front porth)：
    表示一行的有效数据结束到下一个水平同步信号开始之间的VCLK的个数，

6. HSPW(horizontal sync pulse width)：
    表示水平同步信号的宽度，用VCLK计算，

bus-format 用于调整lvds屏的数据格式，出现颜色严重失真，花屏，可尝试调整该参数;
常用的如下
#define MEDIA_BUS_FMT_RGB888_1X7X4_SPWG		4113
#define MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA	4114
#define MEDIA_BUS_FMT_RGB666_1X7X3_SPWG		4112
#define MEDIA_BUS_FMT_RGB565_1X16		    4119
```

###注意事项
* 刷入的屏参升级固件的时候(正常升级固件不用每次都擦除升级)，只要不擦除，就不会消失;
* U盘或者TF卡中的屏参文件只能有一个，不管是在哪个目录,以及造成烧写屏参错误;
* 屏参文件名一定是"rk_lcd_parameters",其他名称不识别;
* 目前支持平台ly101m + ly102e,ly101m + ly101e;
