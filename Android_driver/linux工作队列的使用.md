#Linux 工作队列的使用
###Linux中Workqueue机制分析
    什么是workqueue？Linux中的Workqueue机制就是为了简化内核线程的创建。通过
    调用workqueue的接口就能创建内核线程。并且可以根据当前系统CPU的个数创建线
    程的数量，使得线程处理的事务能够并行化。workqueue是内核中实现简单而有效的
    机制，他显然简化了内核daemon的创建，方便了用户的编程.
###Workqueue接口函数说明
``` c
//用于创建一个workqueue队列，为系统中的每个CPU都创建一个内核线程。
create_workqueue

//用于创建workqueue，只创建一个内核线程。
create_singlethread_workqueue

//释放workqueue队列,需要释放的workqueue队列指针
destroy_workqueue

//调度执行一个具体的任务，执行的任务将会被挂入Linux系统提供的workqueue-keventd_wq
schedule_work

//延迟一定时间去执行一个具体的任务，功能与schedule_work类似，多了一个延迟时间
schedule_delayed_work

//调度执行一个指定workqueue中的任务
queue_work

//延迟调度执行一个指定workqueue中的任务，功能与queue_work类似，输入参数多了一个delay
queue_delayed_work
```
###使用create_workqueue创建工作队列
``` c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/sysfs.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

//工作以队列结构组织成工作队列（workqueue），其数据结构为workqueue_struct，
static struct workqueue_struct *test_wq = NULL;
//把推后执行的任务叫做工作（work），描述它的数据结构为work_struct
static struct work_struct   work;
/*
 *定义工作队列调用函数
 */
void work_func(struct work_struct *work){
        while(1){
                printk(KERN_ERR "-----%s-----\n",__func__);
                //printk可以在多处理器上同时被调用
        }
}

static int __init test_init(void){

        /*创建工作队列workqueue_struct，该函数会为cpu创建内核线程*/
        test_wq = create_workqueue("test_wq");
        /*初始化工作work_struct，指定工作函数*/
        INIT_WORK(&work,work_func);
        /*将工作加入到工作队列中，最终唤醒内核线程*/
        queue_work(test_wq, &work);

        while(1){
                mdelay(1000);
                printk(KERN_ERR "-----%s-----\n",__func__);
        }
        return 0;
}

static void __exit test_exit(void){

}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("liujin725@outlook.com");
```
    运行结果：
    没有打印任何信息，系统直接卡死，卡死原因是因为所有的cpu都被printk占用，系统无法调用其他的进程
###使用create_singlethread_workqueue创建工作队列
``` c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/sysfs.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
 //工作以队列结构组织成工作队列（workqueue），其数据结构为workqueue_struct，
static struct workqueue_struct *test_wq = NULL;
//把推后执行的任务叫做工作（work），描述它的数据结构为work_struct
static struct work_struct   work;
/*
 *定义工作队列调用函数
 */
void work_func(struct work_struct *work){
        while(1){
                printk(KERN_ERR "-----%s-----\n",__func__);  //printk可以在多处理器上同时被调用
        }
}

static int __init test_init(void){
        /*创建工作队列workqueue_struct，该函数会为cpu创建内核线程*/
        test_wq = create_singlethread_workqueue("test_wq");
        /*初始化工作work_struct，指定工作函数*/
        INIT_WORK(&work,work_func);
        /*将工作加入到工作队列中，最终唤醒内核线程*/
        queue_work(test_wq, &work);
        while(1){
                mdelay(1000);
                printk(KERN_ERR "-----%s-----\n",__func__);
        }
        return 0;
}

static void __exit test_exit(void){

}
module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("liujin725@outlook.com");
```
    运行结果：
    <3>[ 124.050211] -----work_func-----　　 //work_func有打印
    <3>[ 124.244364] -----work_func-----
    <3>[ 124.341765] -----work_func-----
    <3>[ 124.537000] -----work_func-----
    <3>[ 124.630770] -----work_func-----
    <3>[ 124.801644] -----test_init-----　　　　//test_init有打印
    <3>[ 124.825084] -----work_func-----
    ...
    //循环打印
###总结
``` c
使用create_workqueue创建的工作队列在工作执行函数work_func中循环调用printk会导
致系统卡死，是因为create_workqueue创建工作队列时在每个cpu上都创了worker_thread
内核线程，worker_thread线程处理的事务能够并行化，导致所有的cpu都被printk函数所
占用，系统无法调用其他的进程，所以系统出现卡死并且无任何log信息打印.
--------------------------------------------------------------------------
使用create_singlethread_workqueue创建的工作队列只在一个cpu上创建worker_thread
内核线程，即使该cpu一直被printk占用也还有其他的cpu可以继续调用其他的进程，所以能
够一直打印log.
```
