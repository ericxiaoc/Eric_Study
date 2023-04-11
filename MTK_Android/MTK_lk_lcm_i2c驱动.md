#IIC驱动的编写
芯片：MTK8163
平台: Andriod 8.1
内核：linux 4.4

###前言
    mtk的iic驱动的lk部分的编写，相比较于内核的标准流程要简单的很多，省了很多操作步骤，lk里面只需要iic地址，挂载的iic位置，需要发送的命令即可。
###LCM的IIC驱动如下（icn6211转换ic的驱动，用于mipi转rgb）:
``` c
//IIC读写操作
#define I2C_CH                 I2C0
#define ICN6211_I2C_ADDR       0x58 >> 1
int icn6211_reg_i2c_write(u16 addr, u32 data)
{
    u8 buffer[2];//8
    u8 lens;
    U32 ret_code = 0;

    static struct mt_i2c_t i2c;
    i2c.id = I2C_CH;
    i2c.addr = ICN6211_I2C_ADDR;	/* i2c API will shift-left this value to 0xc0 */
    i2c.mode = ST_MODE;
    i2c.speed = 100;
    i2c.dma_en = 0;

    buffer[0] = addr;
    buffer[1] = data;
    ret_code = i2c_write(&i2c, buffer, 2); // 0:I2C_PATH_NORMAL
    if (ret_code != 0)
    {
        return ret_code;
    }
    return 0;
}

u32 icn6211_reg_i2c_read(u16 addr)
{
    u8 buffer[1] = {0};//8
    u8 lens;
    u32 ret_code = 0;
    u32 data;
    static struct mt_i2c_t i2c;
    i2c.id = I2C_CH;
    i2c.addr = ICN6211_I2C_ADDR;	/* i2c API will shift-left this value to 0xc0 */
    i2c.mode = ST_MODE;
    i2c.speed = 100;
    i2c.dma_en = 0;
    buffer[0] = addr;
    ret_code = i2c_write(&i2c, buffer, 1);    // set register command
    if (ret_code != 0)
        return ret_code;
    ret_code = i2c_read(&i2c, buffer, 1);
    if (ret_code != 0)
    {
        return ret_code;
    }
    return buffer[0];
}

#define FRAME_WIDTH (1280)
#define FRAME_HEIGHT (720)
//lcm_get_params函数中主要是一些参数定义，例如屏的分辨率，屏的接口类型等等
static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS)); //先将LCM_PARAMS结构清空

    params->type = LCM_TYPE_DSI;  //lcm接口类型
    params->width = FRAME_WIDTH;  //lcm显示宽度
    params->height = FRAME_HEIGHT;  //lcm显示高度
    params->dsi.mode =SYNC_PULSE_VDO_MODE; // SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM = LCM_FOUR_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888; //LCM_DSI_FORMAT_RGB666;

    params->dsi.packet_size=256;

		// Video mode setting
    params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888; //LCM_PACKED_PS_18BIT_RGB666;

    params->dsi.vertical_sync_active     = 5;
    params->dsi.vertical_backporch       = 20;
    params->dsi.vertical_frontporch      = 5;
    params->dsi.vertical_active_line     = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active   = 20;
    params->dsi.horizontal_backporch     = 80;
    params->dsi.horizontal_frontporch    = 40;
    params->dsi.horizontal_active_pixel  = FRAME_WIDTH;

    //params->dsi.HS_ZERO = 6;
    //params->dsi.HS_PRPR = 9;
    params->dsi.PLL_CLOCK = 93;
    params->dsi.cont_clock= 1;
}

/*
在lcm_set_util_funcs这个函数中将util的内容复制给了lcm_util。
那么在LCM_UTIL_FUNCS这个类型中，主要定义了一些接口函数，这些接口
函数是mtk提供给lcm驱动开发者使用的，所以说不管是哪一款lcm驱动，
都需要这么一个接口，而且需要实现set_util_funcs接口，然后在mtk
lcm核心部分自然会调用这个接口函数，这样我们就可以使用其中的接口函数了。
*/

static LCM_UTIL_FUNCS lcm_util = {0};

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

#define ICN6211_REG_WRITE(add, data) icn6211_reg_i2c_write(add, data)
//icn6211转换ic的下发指令，转换1280x720的rgb信号；
static void init_icn6211_registers(void)
{

    printf("[LK/LCM] init_va7783_registers() \n");

	DSI_clk_HS_mode(1);

	ICN6211_REG_WRITE(0x20, 0x00);
	ICN6211_REG_WRITE(0x21, 0xD0);
	ICN6211_REG_WRITE(0x22, 0x25);
	ICN6211_REG_WRITE(0x23, 0x28);
	ICN6211_REG_WRITE(0x24, 0x14);
	ICN6211_REG_WRITE(0x25, 0x50);
	ICN6211_REG_WRITE(0x26, 0x00);
	ICN6211_REG_WRITE(0x27, 0x05);
	ICN6211_REG_WRITE(0x28, 0x05);
	ICN6211_REG_WRITE(0x29, 0x14);
	ICN6211_REG_WRITE(0x34, 0x80);
	ICN6211_REG_WRITE(0x36, 0x28);
	ICN6211_REG_WRITE(0xB5, 0xA0);
	ICN6211_REG_WRITE(0x5C, 0xFF);
	ICN6211_REG_WRITE(0x2A, 0x01);
	ICN6211_REG_WRITE(0x56, 0x92);//93 内部时钟
	ICN6211_REG_WRITE(0x6B, 0x71);//71
	ICN6211_REG_WRITE(0x69, 0x2B);//30
	ICN6211_REG_WRITE(0x10, 0x45);
	ICN6211_REG_WRITE(0x11, 0x88);
	ICN6211_REG_WRITE(0xB6, 0x20);
	ICN6211_REG_WRITE(0x51, 0x20);
	//ICN6211_REG_WRITE(0x14, 0x43);//彩条
	//ICN6211_REG_WRITE(0x2A, 0x49);//彩条
	ICN6211_REG_WRITE(0x09, 0x10);

}
#define GPIO_3430_LED_PWR           (GPIO3 | 0x80000000)
//lcm 的初始化
static void lcm_init(void)
{
    printf("[LK/LCM] TM050 lcm_init() \n");
    MDELAY(20);
    lcm_set_gpio_output(GPIO_3430_LED_PWR, GPIO_OUT_ONE);
    MDELAY(20);
    init_icn6211_registers();
}

//进行休眠的操作
static void lcm_suspend(void)
{
    printf("[LK/LCM] lcm_suspend() enter\n");
    lcm_set_gpio_output(GPIO_3430_LED_PWR, GPIO_OUT_ZERO);
}

//唤醒的操作，重新初始化
static void lcm_resume(void)
{
    // do nothing in LK
    printf("[LK/LCM] lcm_resume() enter\n");
    lcm_init();
}

/*compare_id函数对于lcm驱动来说不是必须的，但最好加上。这个函数主要是用于兼容lcm时，
lcm核心会调用该函数来匹配lcm驱动和屏*/
static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0;
	unsigned char buffer[2];
	unsigned int array[16];

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);
	MDELAY(10);

/*读取iic的寄存器地址*/
	array[0] = 0x00023700;
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10);

	read_reg_v2(0x04, buffer, 2);
	id = buffer[1];

	return (id == ICN6211_I2C_ADDR) ? 1 : 0;
}

//注册LCM 驱动
LCM_DRIVER tm043yvhg30_lcm_drv =
{
    .name             = "TM043YVHG30",
    .set_util_funcs   = lcm_set_util_funcs,
    .get_params       = lcm_get_params,
    .init             = lcm_init,
    .suspend          = lcm_suspend,
    .resume           = lcm_resume,
    .compare_id       = lcm_compare_id,
};
```
###添加到lcm驱动中
* 那么首先来看的是如何配置lcd屏（在mtk中叫做lcm）
device\mediateksample\a8312\ProjectConfig.mk中配置
```  makefile
LCM_HEIGHT = 720
LCM_WIDTH = 1280
```
vendor\mediatek\proprietary\bootable\bootloader\lk\project\a8312.mk中配置
``` makefile
CUSTOM_LK_LCM="icn6211"
```
kernel-4.4\arch\arm64\configs\a8312_defconfig和a8312_debug_defconfig中配置
``` makefile
CONFIG_CUSTOM_KERNEL_LCM="icn6211"
CONFIG_LCM_HEIGHT="720"
CONFIG_LCM_WIDTH="1280"
```
* 修改mt65xx_lcm_list.c
* 在\vendor\mediatek\proprietary\bootable\bootloader\lk\dev\lcm目录中创建一个目录icn6211（注意，目录的名字要和ProjectConfig.mk中的lcm配置相对应）
* 在该目录下创建一个文件icn6211.c
* 修改mt65xx_lcm_list.c，依照已有的格式在lcm_driver_list这个数组里面添加下面的内容：
``` makefile
LCM_DRIVER *lcm_driver_list[] = {
#if defined(ICN6211)
	&tm043yvhg30_lcm_drv,
#endif
#那么在这个数组的上面肯定还需要声明一下tm043yvhg30_lcm_drv
extern LCM_DRIVER tm043yvhg30_lcm_drv;
```
这样就对接上之前编写的驱动程序了，lk的驱动编译就ok了，kernel里面也需要添加一下；
kernel-4.4\drivers\misc\mediatek\lcm目录中如lk中一样操作即可；

注意：
SET_RESET_PIN方法的复位是标准的mtk方法，cpu自带lcm_rst功能，因此复位lcm，只需要如下
```  c
SET_RESET_PIN(1);
SET_RESET_PIN(0);
MDELAY(1);
SET_RESET_PIN(1);
```
