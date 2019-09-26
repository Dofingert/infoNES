#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/device.h>
#include <../arch/arm/mach-mx28/mx28_pins.h>
#include "gpio.h"


static struct class *joypad_drv_class;

/**
	 * FC手柄 bit 键位对应关系 真实手柄中有一个定时器，处理 连A  连B 
	 * 0  1   2       3       4    5      6     7
	 * A  B   Select  Start  Up   Down   Left  Right
*/
static ssize_t joypad_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	//利用 sszie_t 返回 手柄键值 
	//buf 是有符号无法保存 8 bit
	int i;
	ssize_t val = 0;
	if(!gpio_get_value(MXS_PIN_TO_GPIO(PINID_SSP0_DATA6))){
		//printk(KERN_INFO "key1 pressed!\n");
		val = 1<<0; //A
	}else{
		//printk(KERN_INFO "no key pressed!\n");
	}
	if(!gpio_get_value(MXS_PIN_TO_GPIO(PINID_SSP0_DATA5))){
		//printk(KERN_INFO "key2 pressed!\n");
		val = 1<<6;//LEFT
	}
	if(!gpio_get_value(MXS_PIN_TO_GPIO(PINID_SSP0_DATA4))){
		//printk(KERN_INFO "key3 pressed!\n");
		val = 1<<3;//start
	}
	if(!gpio_get_value(MXS_PIN_TO_GPIO(PINID_LCD_D18))){
		//printk(KERN_INFO "key4 pressed!\n");
		val = 1<<7;//RIGHT
	}
	if(!gpio_get_value(MXS_PIN_TO_GPIO(PINID_LCD_D17))){
		//printk(KERN_INFO "key5 pressed!\n");
		val = 1<<5;//UP
	}
	return val;
}
static struct file_operations joypad_drv_fops = {
 	.owner  =   THIS_MODULE,
	.read   =   joypad_drv_read,
};

int major;
//注册驱动程序
int joypad_drv_init(void)
{
	printk(KERN_INFO "joypad_drv_init\n");
	major = register_chrdev(0, "joypad_drv", &joypad_drv_fops);

	//自动创建 dev 节点
	joypad_drv_class = class_create(THIS_MODULE, "joypad_drv");
	device_create(joypad_drv_class, NULL, MKDEV(major, 0), NULL, "joypad");
	
	//P3.26 PINID_SAIF1_SDATA0配置为输入
	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_SAIF1_SDATA0));

	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_LCD_D17));
	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_LCD_D18));
	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_SSP0_DATA4));
	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_SSP0_DATA5));
	gpio_direction_input(MXS_PIN_TO_GPIO(PINID_SSP0_DATA6));
	return 0;
}

//卸载驱动程序
void joypad_drv_exit(void)
{
	printk(KERN_INFO "joypad_drv_exit\n");
	unregister_chrdev(major, "joypad_drv");
	device_destroy(joypad_drv_class, MKDEV(major, 0));
	class_destroy(joypad_drv_class);
}

module_init(joypad_drv_init);
module_exit(joypad_drv_exit);
MODULE_LICENSE("GPL");

