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
#include "efm32gg.h"
#include "driver-gamepad.h"

/* static variables */
static struct port_range port_range_gpio;
static int irq_gpio_even;
static int irq_gpio_odd;

/*
 * Probe function
 */
static int
tdt4258_gamepad_probe(struct platform_device *dev)
{
	struct resource *res;
	
	printk(KERN_INFO "Probing tdt4258_gamepad_driver...\n");

	res = platform_get_resource(dev, IORESOURCE_MEM, PLATFORM_MEM_INDEX_GPIO);

	/* save the variables */
	port_range_gpio.start = res->start;
	port_range_gpio.end = res->end;
	printk(KERN_INFO "GPIO start port: %i\n", port_range_gpio.start);
	printk(KERN_INFO "GPIO end port: %i\n", port_range_gpio.end);

	/* irq numbers */
	irq_gpio_even = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_EVEN);
	irq_gpio_odd = platform_get_irq(dev, PLATFORM_IRQ_INDEX_GPIO_ODD);
	printk(KERN_INFO "GPIO even IRQ: %i\n", irq_gpio_even);
	printk(KERN_INFO "GPIO odd IRQ: %i\n", irq_gpio_odd);

	/*	
	res = request_region(port_range_gpio.start, 1, "tdt4258_gamepad_driver");
	if (res == NULL) {
		printk(KERN_INFO "Could not gain access to GPIO ports...\n");
		return 1;
	} else {
		printk(KERN_INFO "Got access to GPIO ports...\n");
	}*/
	
	
	/* set high drive strength */
	iowrite32(1, GPIO_PA_CTRL);

	iowrite32(0x55555555, GPIO_PA_MODEH); /* set pins A8-15 as output */

	/* turn off all LEDs */
	iowrite32(0x0f0f, GPIO_PA_DOUT);

	/* set pins 8-15 to input */
	iowrite32(0x33333333, GPIO_PC_MODEL);

	/* enable internal pull up register */
	iowrite32(0xff, GPIO_PC_DOUT);

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

