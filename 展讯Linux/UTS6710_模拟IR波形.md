###模拟IR波形
#####1. 展讯提供的流程
``` c
GPIO 模拟NEC控制流程：
1. 开机loader0.c中注册函数
    enable_IR_OUT_isr ==> isr51_handler==> IR_OUT_isr_routine ==> IR_out
2. aps中调用tv_SendDVDKeycode()
3. drivers  SEND_DVD_IR_COMBO
  *(volatile UINT32 *)(0xa0000028) = ir_protocol;
  *(volatile UINT32 *)(0xa000002c) = ir_keycode;
  *(volatile ULONG *)(0xbe01011c) |= 0x00080000;//int51
4. ir_out.c中触发IR_out函数使用GPIO模拟NEC波形
   boot中使用delay_10u() api会非常准
```
#####2.配置对应的GPIO
``` c
aps\customer\s2tek\UTS_6710\adaptable\pin_config.h

/* DVD */
#define GPIO_DVD_IROUT					GPIO33 /*配置为需要的GPIO*/

```
``` c
aps\customer\s2tek\UTS_6710\adaptable\gpio_table.c

static GPIOAUXConfig_t gGPIOTableAux[35]=
{
  //......
  {GPIO_DVD_IROUT,GPIO_OUTPUT,GPIO_LEVEL_HIGH,GPIO_PULLUD_NO,GPIO_PULLUD_NO},
  //......
}

static GPIOMAINConfig_t gGPIOTableMain[]=
{
  //......
   {GPIO_33,GPIO_DVD_IROUT,GPIO_OUTPUT, GPIO_LEVEL_HIGH, GPIO_WITHOUT_INVERT,GPIO_PULLUD_NO, GPIO_PULLUD_NO},
   //......
}
```

######3. loader0中的流程，注册中断函数接口
``` c
#if IR_OUT_EN
/*
    Interrupt IR OUT
*/
#define IR_OUT_INT        51  //对应8051单片机的中断值，
void IR_OUT_isr_routine(void)
{
    // clear int51 event
    *(u32 *)(0xbe01011c) =     ((*(u32 *)(0xbe01011c)) ^ (0x00080000));
    printf("DVD_IR isr\n");
    IR_out();
}

void isr51_handler(void)
{
//    isr3_routine();
    __asm__ __volatile__(    \
        ".set noreorder\n"            \
        "la    $8, IR_OUT_isr_routine\n"            \
        "jal    $8\n"            \
        "nop\n"            \
        "eret\n"            \
        "nop\n"            \
        ".set reorder\n"
        :);                \
        //please rename  function "isr3_routine"
}

void enable_IR_OUT_isr(void)
{

    u32 ebase;
    #if System_CacheEnable
        ebase = CPU0_ISR_Address_cached;    //0x80020000
    #else
        ebase = CPU0_ISR_Address;    //0xa0020000
    #endif
    /* use isr 3 for example*/
    set_handler(ebase, 0x200 + IR_OUT_INT*VS_0x20 , &isr51_handler, 0x20);
    /* Enable INT 3*/
    cpu0_irq_unmask(IR_OUT_INT);

}
#endif
```
#####4 .配置用户层的IR程序
* 修改mconfig.config
``` makefile
CONFIG_SUPPORT_IR_TX=y
CONFIG_IR_TX_DVDMODELNAME1__S2TEK=y
```
* 修改对应的键值映射，如下定义了所有的键值，IR_TX_MAX对应没有功能，其他对应键值的位置填上下发的功能定义；
``` c
aps\customer\s2tek\public\ir_tx\IR_TX_DVDModelName1\ir_tx_mapping_table.c

//定义所有的键值，并映射需要的功能
const IR_TX_EVENT g_eIrTxMapTable[] = {
/*0x00, 0x01 ,0x02, 0x03*/	IR_TX_MAX, IR_TX_DISPLAY,IR_TX_STOP,IR_TX_MAX,
/*0x04, 0x05 ,0x06, 0x07*/	IR_TX_MAX, IR_TX_MAX, IR_TX_ZOOM_IN,  IR_TX_MAX,
/*0x08, 0x09 ,0x0a, 0x0b*/	IR_TX_EJECT,IR_TX_MAX, IR_TX_PLAY_PAUSE, IR_TX_UP,
/*0x0c, 0x0d ,0x0e, 0x0f*/	IR_TX_MAX, IR_TX_ENTER, IR_TX_MAX, IR_TX_DOWN,

/*0x10, 0x11 ,0x12, 0x13*/	IR_TX_PREV, IR_TX_NUM1, IR_TX_NUM5, IR_TX_NUM6,
/*0x14, 0x15 ,0x16, 0x17*/	IR_TX_NEXT, IR_TX_NUM4, IR_TX_NUM8, IR_TX_NUM9,
/*0x18, 0x19 ,0x1a, 0x1b*/	IR_TX_FR, IR_TX_NUM7, IR_TX_NUM0, IR_TX_MAX,
/*0x1c, 0x1d ,0x1e, 0x1f*/	IR_TX_FF, IR_TX_MAX, IR_TX_MAX, IR_TX_TITLE_MENU,

/*0x20, 0x21 ,0x22, 0x23*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x24, 0x25 ,0x26, 0x27*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x28, 0x29 ,0x2a, 0x2b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x2c, 0x2d ,0x2e, 0x2f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0x30, 0x31 ,0x32, 0x33*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x34, 0x35 ,0x36, 0x37*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x38, 0x39 ,0x3a, 0x3b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x3c, 0x3d ,0x3e, 0x3f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_ADD10,

/*0x40, 0x41 ,0x42, 0x43*/	IR_TX_SF, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x44, 0x45 ,0x46, 0x47*/	IR_TX_REPEAT,IR_TX_MENU_PBC,IR_TX_ANGLE, IR_TX_MAX,
/*0x48, 0x49 ,0x4a, 0x4b*/	IR_TX_PROGRAM, IR_TX_LEFT, IR_TX_RIGHT,  IR_TX_MAX,
/*0x4c, 0x4d ,0x4e, 0x4f*/	IR_TX_SUB_TITLE, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0x50, 0x51 ,0x52, 0x53*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x54, 0x55 ,0x56, 0x57*/	IR_TX_RETURN, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x58, 0x59 ,0x5a, 0x5b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_GOTO, IR_TX_MAX,
/*0x5c, 0x5d ,0x5e, 0x5f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_SET_A,  IR_TX_MAX,

/*0x60, 0x61 ,0x62, 0x63*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x64, 0x65 ,0x66, 0x67*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x68, 0x69 ,0x6a, 0x6b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x6c, 0x6d ,0x6e, 0x6f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0x70, 0x71 ,0x72, 0x73*/	IR_TX_INTRO, IR_TX_MAX, IR_TX_STEP, IR_TX_MAX,
/*0x74, 0x75 ,0x76, 0x77*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x78, 0x79 ,0x7a, 0x7b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x7c, 0x7d ,0x7e, 0x7f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0x80, 0x81 ,0x82, 0x83*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x84, 0x85 ,0x86, 0x87*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x88, 0x89 ,0x8a, 0x8b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x8c, 0x8d ,0x8e, 0x8f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0x90, 0x91 ,0x92, 0x93*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x94, 0x95 ,0x96, 0x97*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x98, 0x99 ,0x9a, 0x9b*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0x9c, 0x9d ,0x9e, 0x9f*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xa0, 0xa1 ,0xa2, 0xa3*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xa4, 0xa5 ,0xa6, 0xa7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xa8, 0xa9 ,0xaa, 0xab*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xac, 0xad ,0xae, 0xaf*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xb0, 0xb1 ,0xb2, 0xb3*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xb4, 0xb5 ,0xb6, 0xb7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xb8, 0xb9 ,0xba, 0xbb*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xbc, 0xbd ,0xbe, 0xbf*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xc0, 0xc1 ,0xc2, 0xc3*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xc4, 0xc5 ,0xc6, 0xc7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xc8, 0xc9 ,0xca, 0xcb*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xcc, 0xcd ,0xce, 0xcf*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xd0, 0xd1 ,0xd2, 0xd3*/	IR_TX_STANDBY, IR_TX_POWER, IR_TX_MAX, IR_TX_MAX,
/*0xd4, 0xd5 ,0xd6, 0xd7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xd8, 0xd9 ,0xda, 0xdb*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xdc, 0xdd ,0xde, 0xdf*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xe0, 0xe1 ,0xe2, 0xe3*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xe4, 0xe5 ,0xe6, 0xe7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xe8, 0xe9 ,0xea, 0xeb*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xec, 0xed ,0xee, 0xef*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,

/*0xf0, 0xf1 ,0xf2, 0xf3*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xf4, 0xf5 ,0xf6, 0xf7*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xf8, 0xf9 ,0xfa, 0xfb*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
/*0xfc, 0xfd ,0xfe, 0xff*/	IR_TX_MAX, IR_TX_MAX, IR_TX_MAX, IR_TX_MAX,
};

UINT16 g_wIrTxMapSize = sizeof(g_eIrTxMapTable)/sizeof(IR_TX_EVENT);
//解析所有的键值，有功能的则send
UINT8 APP_Ir_Tx_SendEvent(IR_TX_EVENT eEvent)
{
	UINT16 wIndex;
#ifdef CONFIG_DVD_COMBO
	UINT32 tempCommand;
#endif
	for(wIndex=0;wIndex<g_wIrTxMapSize;wIndex++)
	{
		if (eEvent == g_eIrTxMapTable[wIndex])
		{
			if (eEvent != IR_TX_MAX)
			{
#ifdef CONFIG_DVD_COMBO
//tempCommand等于16进制的用户码加键值码加键值反码；
				tempCommand=(((UINT32)IR_TX_VENDER_CUSTOMER<<16)&0xffff0000)|((UINT32)wIndex<<8)|(((UINT32)~wIndex)&0xFF);
				tv_SendDVDKeycode((UINT8)IR_TX_TYPE, tempCommand);
#endif
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}
```
#####5. 配置遥控的键值下发处理,APP_Ir_Tx_SendEvent就是第4步中的处理下发；
``` c
#ifdef CONFIG_SUPPORT_IR_TX
static void _HDMI2_IR_COOLX_Event_convert(UINT32 dSystemAppIndex, UINT32 *pbEvent)
{
	switch(*pbEvent)
	{
		case UI_EVENT_LEFT:
			APP_Ir_Tx_SendEvent(IR_TX_LEFT);
			break;
		case UI_EVENT_RIGHT:
			APP_Ir_Tx_SendEvent(IR_TX_RIGHT);
			break;
		case UI_EVENT_UP:
			APP_Ir_Tx_SendEvent(IR_TX_UP);
			break;
		case UI_EVENT_DOWN:
			APP_Ir_Tx_SendEvent(IR_TX_DOWN);
			break;
		case UI_EVENT_ENTER:
			APP_Ir_Tx_SendEvent(IR_TX_ENTER);
			break;
		case UI_EVENT_RETURN:
			APP_Ir_Tx_SendEvent(IR_TX_RETURN);	// == back
			break;
		default:
			break;
	}
}
#endif
```
#####6. 驱动层的按键响应处理
* 通过ioctl发送数据到驱动层
``` c
aps\middleware\ioctl_interface\umf_ioctl.c
#ifdef CONFIG_DVD_COMBO
void tv_SendDVDKeycode(UINT8 protocol, UINT32 keycode)
{
	DVDIR_t DVDKey;
	DVDKey.protocol= protocol;
	DVDKey.keycode = keycode;
	ioctl(kmfdev, KMF_IOC_SENDDVDKEY, &DVDKey);
}
#endif
```
* 驱动层接收处理
``` c
//====================================================
drivers\include\kmf_ioctl.h
#define KMF_IOC_SENDDVDKEY  _IOWR(KMF_IOC_MAGIC, KMF_IOC_INDEX_SENDDVDKEY, INT32)
//====================================================
drivers\kmf\basesrc_dvbeu\kmf_ioctl.c
#ifdef CONFIG_DVD_COMBO
	KmfIoctlFuncList[KMF_IOC_INDEX_SENDDVDKEY] = kmf_ioctl_SendDVDKey;
#endif

#ifdef CONFIG_DVD_COMBO
static INT32 kmf_ioctl_SendDVDKey(ULONG arg)
{
	INT32 retval = 0;
	DVDIR_t DVDKey;
//从用户层拷贝到内核层
	if (copy_from_user((void *)&DVDKey, (void *)arg, sizeof(DVDIR_t)))
	{
		retval = -EFAULT;
		return retval;
	}

	KMFDBG(0, "--- kmf_ioctl_SendDVDKey: %x  --- \n ", DVDKey.keycode);
  //通过SEND_DVD_IR_COMBO继续传递
	SEND_DVD_IR_COMBO(DVDKey.protocol, DVDKey.keycode);
	return retval;
}
//====================================================
drivers\IR\ir9561.c

#ifdef CONFIG_DVD_COMBO
INT32 SEND_DVD_IR_COMBO( UINT8 ir_protocol, UINT32 ir_keycode )
{
	*(volatile UINT32 *)(0xa0000028) = ir_protocol;
	*(volatile UINT32 *)(0xa000002c) = ir_keycode;
	DBG_MSG1(DBGCFG_IR, "ir protocol:0x%08x\tkeycode:0x%08x\n", (unsigned int)(*(volatile UINT32 *)(0xa0000028)), (unsigned int)(*(volatile UINT32 *)(0xa000002c)) );
   // trigger interrupt 51 触发中断,调用IR_out();
	*(volatile ULONG *)(0xbe01011c) |= 0x00080000;
	return 0;
}
EXPORT_SYMBOL(SEND_DVD_IR_COMBO);
#endif
```
* SEND_DVD_IR_COMBO来触发之前loader0中注册的中断函数，最后调用IR_out();
#####IR_out()怎么实现波形
* 通过函数IR_Send_Start来实现引导码，
```c
__cpu0 void IR_Send_Start(u8 ir_gpio)
{
	if ( ir_protocol == 1 ) {	// NEC
		// H:9ms, L:4.5ms
		irGPIOWriteFun(ir_gpio, 0);
		delay_10u(900);	//delay(9000);
		irGPIOWriteFun(ir_gpio, 1);
		delay_10u(450);	//delay(4500);
	}
}
```
* 数据0、数据1、代码实现；
``` c
__cpu0 void IR_Send_High(u8 ir_gpio)
{
	if ( ir_protocol == 1 ) {	// NEC
		// H:560us, L:2.25ms-560us
		irGPIOWriteFun(ir_gpio, 0);
		delay_10u(56);		//delay(560);
		irGPIOWriteFun(ir_gpio, 1);
		delay_10u(169);	//delay(1690);
	}
}
__cpu0 void IR_Send_Low(u8 ir_gpio)
{
	if ( ir_protocol == 1 ) {	// NEC
		// H:560us, L:1.12ms-560us
		irGPIOWriteFun(ir_gpio, 0);
		delay_10u(56);		//delay(560);
		irGPIOWriteFun(ir_gpio, 1);
		delay_10u(56);		//delay(560);
	}
}
```
* 停止位
``` c
__cpu0 void IR_Send_End(u8 ir_gpio)
{
	if ( ir_protocol == 1 ) {	// NEC
		// H:560us
		irGPIOWriteFun(ir_gpio, 0);
		delay_10u(56);		//delay(560);
		irGPIOWriteFun(ir_gpio, 1);
	}
}
```
* 长按键值发送
``` c
__cpu0 void IR_Repeat(u8 ir_gpio)
{
	if ( ir_protocol == 1 ) {	// NEC
		// H:9ms, L:2.25ms
		irGPIOWriteFun(ir_gpio, 0);
		delay_10u(900);	//delay(9000);
		irGPIOWriteFun(ir_gpio, 1);
		delay_10u(225);	//delay(2250);
		IR_Send_End(ir_gpio);
	}
}
```
* 数据的解析
``` c
__cpu0 void IR_out(void)
{
	u8 i, j;
	volatile u32 ir_input, tmp;
	volatile u32 ir_keycode;
	volatile u8 IR_OUT_GPIO;
	CUSTIMIZATION_TABLE *OptionShadowAddr = (void *)(Option_IRset_ShadowAddr);
	IR_OUT_GPIO = OptionShadowAddr->DVD_IROUT_GPIO_NO;
	ir_protocol = *(volatile u32 *)(0xa0000028);
	ir_keycode = *(volatile u32 *)(0xa000002c);
#if 0
	printf("IR_OUT_GPIO:%d\n", IR_OUT_GPIO);
	printf("ir_protocol:0x%08x\n", ir_protocol);
	printf("ir_keycode:0x%08x\n", ir_keycode);
#endif
	IR_Send_Start(IR_OUT_GPIO);
	for ( j=0;j<4;j++) {
		tmp = ir_keycode >> (8*(3-j));
		for ( i=0;i<8;i++ ) {
			if ( tmp & 0x00000001 ) {
				IR_Send_High(IR_OUT_GPIO);
			}
			else {
				IR_Send_Low(IR_OUT_GPIO);
			}
			tmp = tmp >> 1;
		}
	}
	IR_Send_End(IR_OUT_GPIO);
}
#endif	// endif IR_OUT_EN
```
* 例如```ir_keycode = 0x00bd4ab5```解析
``` c
1.j = 0;
tmp = 0x00bd4ab5 >> 24  = 0
tmp & 0x00000001 = 0;
IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0;
0 右移始终是 0; 所以最终是  0000 0000;

2.j = 1;
tmp = 0x00bd4ab5 >> 16  = 0x2

tmp & 0x00000001 = 1;  ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x1;
tmp & 0x00000001 = 1;  ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x0;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
所以最终是  0100 0000
3.j = 2;
tmp = 0x00bd4ab5 >> 8  = 0xbd4a

tmp & 0x00000001 = 1;  ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x5EA5;
tmp & 0x00000001 = 1;  ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x2F52;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x17A9;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0xBD4;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x5EA;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x2F5;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x17A;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
所以最终是  0101 0010
4.j = 3;
tmp = 0x00bd4ab5 >> 0  = 0x00bd4ab5

tmp & 0x00000001 = 1;  ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x5EA55A;
tmp & 0x00000001 = 1;  ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x2F52AD;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x17A956;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0xBD4AB;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x5EA55;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x2F5A2;
tmp & 0x00000001 = 1; ===> IR_Send_Low(IR_OUT_GPIO);
tmp = tmp >> 1 = 0x17A95;
tmp & 0x00000001 = 1; ===> IR_Send_High(IR_OUT_GPIO);
所以最终是  1010 1101
```
