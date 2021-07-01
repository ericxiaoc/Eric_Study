##LK与kernel传递参数
* 不管是uboot还是lk，传递参数一般常见的方法都是通过cmdline，然后kernel层去获取cmdline中的对应名的参数值；

####LK中添加变量参数到cmdline
``` c
\vendor\mediatek\proprietary\bootable\bootloader\lk\app\mt_boot\mt_boot.c
int boot_linux_fdt(void *kernel, unsigned *tags,
		   unsigned machtype,
		   void *ramdisk, unsigned ramdisk_sz)
{
  //...
  snprintf(tmpbuf, TMPBUF_SIZE, "%s%d", "screen.rotation=", screen_hwrotaton);
  		cmdline_append(tmpbuf);
  //...
```
####Kernel中获取到cmdline中的值
``` c
static char hwrotationbootstate[2];

static int __init screen_rotation_state_param(char *line)
{
	strlcpy(hwrotationbootstate, line, sizeof(hwrotationbootstate));
	return 1;
}

__setup("screen.rotation=", screen_rotation_state_param);
```
