/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#define GPIO_PA_BASE 0x40006000
#define GPIO_PA_DOUT ((unsigned int*)GPIO_PA_BASE + 0x0c)

#define GPIO_PC_BASE 0x40006048
#define GPIO_PC_DIN ((unsigned long)(GPIO_PC_BASE + 0x1c))

static int
my_probe(struct platform_device *dev)
{
	struct resource *res = platform_get_resource(dev, IORESOURCE_MEM, 0);

	printk("IO Start: %i, IO End: %i.", res->start, res->end);
	printk("my_probe called\n");

}

static int
my_remove(struct platform_device *dev)
{
	printk("my_remove called\n");

}

static const struct of_device_id my_of_match[] = {
	{ .compatible = "tdt4258", },
	{ },
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_driver = {
	.probe = my_probe,
	.remove = my_remove,
	.driver = {
		.name = "driver_gamepad",
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
	
	platform_driver_register(&my_driver);

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

