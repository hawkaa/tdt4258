/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/siginfo.h>


/* local includes */
#include "driver-gamepad.h"

/* constasts */
static char DEVICE_NAME[] = "tdt4258_gamepad";

/* static variables */
static int irq_gpio_even;
static int irq_gpio_odd;
static void *gpio_base;
static void *gpio_pc_base;

static int memory_region_base;
static int memory_region_size;

static dev_t device_number;

/* checks if device is open or not */
static int device_open = 0;

/* pid for the current process that has the file open */
pid_t pid = 0;

/* device class */
static struct class *cl;

/* for testing only */
static char button_value;
static int is_eof;


/*
 * Signals the user application that opened the file with SIGUSR1 
 * The signal contains no data.
 */
static void
signal_user_application(void)
{
	struct siginfo info;
	struct task_struct *t;
	int retval;

	/* info */
	info.si_signo = SIGUSR1;
	info.si_code = SI_QUEUE;

	/* task */
	t = pid_task(find_pid_ns(pid, &init_pid_ns), PIDTYPE_PID);

	/* send the signal */
	retval = send_sig_info(SIGUSR1, &info, t);    //send the signal

	if (retval < 0) {
		printk(KERN_INFO "Error sending signal(%i)\n", SIGUSR1);
	}
}

irqreturn_t
gpio_interrupt_handler(unsigned int irq, struct pt_reg *reg)
{
	button_value = ioread32(gpio_pc_base + GPIO_DIN);
	is_eof = 0;

	/* signal user application that new input is available */
	signal_user_application();

	/* clear interrupt */
	iowrite32(0xff, gpio_base + GPIO_IFC);
	return IRQ_HANDLED;	
}

static int
tdt4258_gamepad_open(struct inode *inode, struct file *filp)
{
	/* TODO */
	printk(KERN_INFO "tdt4258_gamepad_open called...\n");
	


	/* only one can have the device open at once */
	if (device_open) {
		printk(KERN_INFO "The device is already opened by another process.");
		return -EBUSY;
	}

	/*
	 * Device is now ready to open
	 */

	/* register interrupts */	
	if(device_open == 0) {
		if(request_irq(irq_gpio_odd, gpio_interrupt_handler, 0, DEVICE_NAME, NULL) || 
	           request_irq(irq_gpio_even, gpio_interrupt_handler, 0 , DEVICE_NAME, NULL)){
			printk(KERN_INFO "The device cannot register the IRQ's: %d, %d\n", irq_gpio_odd, irq_gpio_even);
			return -EIO;
		}
	}

	printk(KERN_INFO "Interrupts enabled. ");



	/* save pid of the process that opened the driver */
	pid = current->pid;
	printk(KERN_INFO "Registered PID %i\n", pid);
	
	/* reset values */
	is_eof = 0;

	printk(KERN_INFO "Device successfully opened!");

	/* log that the device is open */
	++device_open;

	return 0;
}

static int
tdt4258_gamepad_release(struct inode *inode, struct file *filp)
{
	/* TODO */
	printk(KERN_INFO "tdt4258_gamepad_release called...\n");

	--device_open;
	if(device_open == 0){
		/* free interrupt channels */
		free_irq(irq_gpio_even, NULL);
		free_irq(irq_gpio_odd, NULL);
		printk(KERN_INFO "Interrupts disabled.");
	}

	printk(KERN_INFO "Device successfully released!");

	return 0;
}

static ssize_t
tdt4258_gamepad_read(struct file *filp, char __user *buff,
		size_t count, loff_t *offp)
{
	/* TODO */
	printk(KERN_INFO "tdt4258_gamepad_read called...\n");
	
	printk(KERN_INFO "Reading %i data.\n", count);

	if (count && !is_eof) {
		put_user(button_value, buff);
		++is_eof;
		return 1;
	} else {
		return 0;
	}

}

static ssize_t
tdt4258_gamepad_write(struct file *filp, const char __user *buff,
		size_t count, loff_t *offp)
{
	/* TODO */
	printk(KERN_INFO "tdt4258_gamepad_write called...\n");

	printk(KERN_INFO "Writing to the driver is not supported!");
	return -EINVAL;
}

static struct file_operations tdt4258_gamepad_fops = {
	.owner = THIS_MODULE,
	.read = tdt4258_gamepad_read,
	.write = tdt4258_gamepad_write,
	.open = tdt4258_gamepad_open,
	.release = tdt4258_gamepad_release
};

static struct cdev tdt4258_gamepad_cdev;


/*
 * Probe function
 */
static int
tdt4258_gamepad_probe(struct platform_device *dev)
{
	struct resource *res;
	struct class *cl;
	
	printk(KERN_INFO "Probing tdt4258_gamepad_driver...\n");

	res = platform_get_resource(dev, IORESOURCE_MEM,
				PLATFORM_MEM_INDEX_GPIO);

	/* save the variables */
	memory_region_base = res->start;
	memory_region_size = (res->end - res->start) / 4;
	printk(KERN_INFO "GPIO Base address: %#010x\n", memory_region_base);
	printk(KERN_INFO "GPIO memory address size: %i\n", memory_region_size);

	/* irq numbers */
	irq_gpio_even = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_EVEN);
	irq_gpio_odd = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_ODD);
	printk(KERN_INFO "GPIO even IRQ: %i\n", irq_gpio_even);
	printk(KERN_INFO "GPIO odd IRQ: %i\n", irq_gpio_odd);

	/* reserve */
	res = request_mem_region(memory_region_base,
				memory_region_size, DEVICE_NAME);
	if (res == NULL) {
		printk(KERN_INFO "Could not allocate memory region...\n");
		return 1;
	} else {
		printk(KERN_INFO "Memory region allocated!\n");
	}


	/* memory map */
	gpio_base = ioremap_nocache(memory_region_base, memory_region_size);
	
	/* save pointers for gpio pc base so we dont have to calculate it all the time*/
	gpio_pc_base = gpio_base + GPIO_PC;


	/* set pins 8-15 to input */
	iowrite32(0x33333333, gpio_pc_base + GPIO_MODEL);

	/* enable internal pull up register */
	iowrite32(0xff, gpio_pc_base + GPIO_DOUT);
	
	/* enable interrupt generation */
	iowrite32(0x22222222, gpio_base + GPIO_EXTIPSELL);
	iowrite32(0xff, gpio_base + GPIO_EXTIFALL);
	iowrite32(0xff, gpio_base + GPIO_EXTIRISE);
	iowrite32(0xff, gpio_base + GPIO_IEN);

	/* allocate char region */
	alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME);
	printk(KERN_INFO "Major: %i\n", MAJOR(device_number));
	printk(KERN_INFO "Minor: %i\n", MINOR(device_number));
	
	cdev_init(&tdt4258_gamepad_cdev, &tdt4258_gamepad_fops);
	cdev_add(&tdt4258_gamepad_cdev, device_number, 1);

	cl = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cl, NULL, device_number, NULL, DEVICE_NAME);

	return 0;

}

/*
 * Remove function
 */
static int
tdt4258_gamepad_remove(struct platform_device *dev)
{
	printk("my_remove called\n");


	/* remove memory region alloc */
	printk(KERN_INFO "Releasing base address: %#010x\n",
					memory_region_base);
	printk(KERN_INFO "Release size: %i\n", memory_region_size);
	release_mem_region(memory_region_base, memory_region_size);

	/* Release memory map  */
        iounmap(memory_region_base);
 
        /* Release char region */
        unregister_chrdev_region(device_number, 1);
 
        /* Remove cdev  */
        cdev_del(&tdt4258_gamepad_cdev);
 
        /* Destroy class and device */
        device_destroy(cl, device_number);
        class_destroy(cl);

	return 0;

}



static const struct of_device_id my_of_match[] = {
	{ .compatible = "tdt4258", },
	{ },
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver tdt4258_gamepad_driver = {
	.probe = tdt4258_gamepad_probe,
	.remove = tdt4258_gamepad_remove,
	.driver = {
		.name = "tdt4258_gamepad_driver",
		.owner = THIS_MODULE,
		.of_match_table = my_of_match,
	},
};



/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init template_init(void)
{

	printk("Hello World, here is your module speaking\n");
	
	/* initiate gamepad driver */
	platform_driver_register(&tdt4258_gamepad_driver);

	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	printk("Short life for a small module...\n");
	platform_driver_unregister(&tdt4258_gamepad_driver);
	
	
}
module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

