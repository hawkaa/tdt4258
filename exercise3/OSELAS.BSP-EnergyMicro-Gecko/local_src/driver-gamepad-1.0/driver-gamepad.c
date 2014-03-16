/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <asm/io.h>

/* local includes */
#include "driver-gamepad.h"

/* static variables */
static int irq_gpio_even;
static int irq_gpio_odd;
static void *gpio_base;
static void *gpio_pc_base;

/*
 * Probe function
 */
static int
tdt4258_gamepad_probe(struct platform_device *dev)
{
	struct resource *res;

	int base, n;

	
	printk(KERN_INFO "Probing tdt4258_gamepad_driver...\n");

	res = platform_get_resource(dev, IORESOURCE_MEM, PLATFORM_MEM_INDEX_GPIO);

	/* save the variables */
	base = res->start;
	n = (res->end - res->start) / 4;
	printk(KERN_INFO "GPIO Base address: %#010x\n", base);
	printk(KERN_INFO "GPIO memory address size: %i\n", n);

	/* irq numbers */
	irq_gpio_even = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_EVEN);
	irq_gpio_odd = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_ODD);
	printk(KERN_INFO "GPIO even IRQ: %i\n", irq_gpio_even);
	printk(KERN_INFO "GPIO odd IRQ: %i\n", irq_gpio_odd);

	/* reserve */
	res = request_mem_region(base, n, "tdt4258_gamepad_driver");
	if (res == NULL) {
		printk(KERN_INFO "Could not allocate memory region...\n");
		return 1;
	} else {
		printk(KERN_INFO "Memory region allocated!\n");
	}


	/* memory map */
	gpio_base = ioremap_nocache(base, n);
	
	/* save pointers for gpio pc base so we dont have to calculate it all the time*/
	gpio_pc_base = gpio_base + GPIO_PC;


	/* set pins 8-15 to input */
	iowrite32(0x33333333, gpio_pc_base + GPIO_MODEL);

	/* enable internal pull up register */
	iowrite32(0xff, gpio_pc_base + GPIO_DOUT);

	/*
	for (;;) {
		printk(KERN_INFO "%i\n", ioread32(gpio_pc_base + GPIO_DIN));
	}
	*/
	return 0;

}

/*
 * Remove function
 */
static int
tdt4258_gamepad_remove(struct platform_device *dev)
{
	printk("my_remove called\n");

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
	
}
module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

