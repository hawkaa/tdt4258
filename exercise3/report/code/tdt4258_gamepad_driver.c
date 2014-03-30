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

