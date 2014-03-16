/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>


#define PLATFORM_MEM_INDEX_GPIO 0
#define PLATFORM_IRQ_INDEX_GPIO_EVEN 0
#define PLATFORM_IRQ_INDEX_GPIO_ODD 1

struct port_range {
	int start;
	int end;
};


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
	printk(KERN_INFO "Probing tdt4258_gamepad_driver...\n");

	struct resource *res = platform_get_resource(dev, IORESOURCE_MEM, PLATFORM_MEM_INDEX_GPIO);

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

