/* global includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>

/* local includes */
#include "input.h"

/* constants */
#define NUM_BUTTONS 8
static char GAMEPAD_DRIVER[] = "/dev/tdt4258_gamepad";


static int fd;
static volatile char button_value;

static void (*button_down_function_pointers[NUM_BUTTONS]) (void);

static int
get_bit_position(char c)
{
	if (c == 1) return 0;
	if (c == 2) return 1;
	if (c == 4) return 2;
	if (c == 8) return 3;
	if (c == 16) return 4;
	if (c == 32) return 5;
	if (c == 64) return 6;
	if (c == 128) return 7;
	return -1;
}

const char* 
byte_to_binary(int x)
{
	static char b[9];
	b[0] = '\0';
	int z;
	for (z = 128; z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "1" : "0");
	}
	return b;
}

static char
get_button_value(void)
{
	char v;
	int retval;
	retval = read(fd, &v, 1);
	if (retval == 0) {
		perror("Could not read button value");
	}
	return v;
}

int input_init(void)
{
	int i;
	/* ønsker en gjennomgang på flaggene her */
	fd = open(GAMEPAD_DRIVER, O_RDWR, (mode_t)0600);
	
	for (i = 0; i < NUM_BUTTONS; ++i) {
		button_down_function_pointers[i] = NULL;
	}

	return fd;
}

void
process_input(void)
{
	printf("process_input called.\n");

	char new_button_value;
	new_button_value = get_button_value();

	/* xor */
	char value_diff = button_value ^ new_button_value;
	
	printf("Old value: %s\n", byte_to_binary(button_value));
	printf("New value: %s\n", byte_to_binary(new_button_value));
	printf("Diff value: %s\n", byte_to_binary(value_diff));
	
	if (!value_diff) {
		/* nothing has happend */
		return;
	}
	int z;
	int i = 7;
	for (z = 128; z > 0; z>>=1, --i) {
		if (z & value_diff) {
			if (z & new_button_value) {
				printf("Button %i was pressed\n", i + 1);
			} else {
				printf("Button %i was released\n", i + 1);
			}
		}
	}

	int position = get_bit_position(value_diff);
	if (position == -1)
		/* we have more than one button to update, choosing to skip */
		return;


	/* update value */
	button_value = new_button_value;


}

void
register_button_down_handler(int button, void (*handler)(void))
{
	if (button < 1 && button > NUM_BUTTONS) {
		return;
	}
	button_down_function_pointers[button-1] = handler;

}
