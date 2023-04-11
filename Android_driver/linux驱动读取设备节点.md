#Linux驱动下读取设备节点
###例：驱动中读取电池充电状态
``` c
//创建工作队列，利用队列来循环读取设备节点的值
struct workqueue_struct *charge_wq;
struct delayed_work charge_work;
bool charge_check;
unsigned long charge_delay;

static void ly_read_file(char *pFilePath, u8 *pBuf, u16 nLength) {
	struct file *pFile = NULL;
	mm_segment_t old_fs;
	ssize_t nReadBytes = 0;

	old_fs = get_fs();
	set_fs(get_ds());

	pFile = filp_open(pFilePath, O_RDONLY, 0);
	if (IS_ERR(pFile)) {
		LANGYU_DEBUG("Open file failed: %s\n", pFilePath);
		return;
	}

	pFile->f_op->llseek(pFile, 0, SEEK_SET);
	nReadBytes = pFile->f_op->read(pFile, pBuf, nLength, &pFile->f_pos);
	LANGYU_DEBUG("Read %d bytes!\n", (int)nReadBytes);

	set_fs(old_fs);
	filp_close(pFile, NULL);
	return;
}
//循环读取电池设备节点的值
static void ly_charge_check(struct work_struct *work) {
  u8 ChargerStatus[20] = {0};
	if(ly_custom_pointer->charge_check){
		ly_read_file(POWER_SUPPLY_BATTERY_STATUS_PATCH, ChargerStatus, 20);
		LANGYU_DEBUG("Battery Status : %s\n", ChargerStatus);
		if (strstr(ChargerStatus, "Charging") != NULL || strstr(ChargerStatus, "Full") != NULL || strstr(ChargerStatus, "Fully charged") != NULL) {
				gpiod_set_value(ly_custom_pointer->boot_led,0); // charger plug-in
				mdelay(1000);
				gpiod_set_value(ly_custom_pointer->boot_led,1);
		}
		else { // Not charging
				gpiod_set_value(ly_custom_pointer->boot_led,1); // charger plug-out
		}
	}
	else{
		LANGYU_DEBUG("charger not need check ly_custom_pointer->esd_check is false!!!\n");
		goto ly_charge_check_out;
	}

ly_charge_check_out:
	ly_custom_pointer->charge_check = true;
	ly_custom_pointer->charge_delay = 1 * HZ;
	queue_delayed_work(ly_custom_pointer->charge_wq, &ly_custom_pointer->charge_work, ly_custom_pointer->charge_delay);
	return;
}

...
//初始化队列，创建create_singlethread_workqueue队列
INIT_DELAYED_WORK(&ly_custom_pointer->charge_work, ly_charge_check);
ly_custom_pointer->charge_wq = create_singlethread_workqueue("ilitek_charge_wq");
if (!ly_custom_pointer->charge_wq) {
  LANGYU_DEBUG("create workqueue charge work err\n");
}
else {
  ly_custom_pointer->charge_check = true;
  ly_custom_pointer->charge_delay = 2 * HZ;
  queue_delayed_work(ly_custom_pointer->charge_wq, &ly_custom_pointer->charge_work, ly_custom_pointer->charge_delay);
}
```
