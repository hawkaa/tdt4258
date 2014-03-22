/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* local includes */
#include "input.h"

/* constants */
static char GAMEPAD_DRIVER[] = "/dev/tdt4258_gamepad";

static int fd;
static char button_value;

static char
get_button_value(void)
{
	char v;
	read(fd, &v, 1);
	return v;
}

int input_init(void)
{
	/* ønsker en gjennomgang på flaggene her */
	fd = open(GAMEPAD_DRIVER, O_RDWR, (mode_t)0600);

	return fd;
}

void
process_input(void)
{
	printf("process_input called.\n");
	button_value = get_button_value();
	printf("Button value: %i\n", button_value);
}

void
register_button_down_handler(int button, void (*handler)(void))
{

}
