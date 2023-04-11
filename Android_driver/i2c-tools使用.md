i2cdetect -y 1 //扫描i2c1 上的设备
i2cdump -f -y 1 0x3b //查看i2c1 上地址为0x3b的设备寄存器状态
i2cset -f -y 3 0x21 0x00 0x44 b
