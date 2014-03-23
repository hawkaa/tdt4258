/*
 * MODULE INPUT
 * 
 * This module is the interface from the gamepad to the rest of the
 * application. It will take care of all calculation and generating
 * the respective button up and button down events.
 */

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

/*
 * Constants
 */
#define NUM_BUTTONS 8
static char GAMEPAD_DRIVER[] = "/dev/tdt4258_gamepad";

/*
 * Static variables
 */

/* file descriptor for the driver */
static int fd;

/* current button value */
static volatile char button_value;

/* array of button up function pointers */
static void (*button_up_function_pointers[NUM_BUTTONS]) (void);

/* array of button down function pointers */
static void (*button_down_function_pointers[NUM_BUTTONS]) (void);

/* 
 * Returns the byte value of the buttons
 */
static char
get_button_value(void)
{
	char v;
	/* read one byte */
	if (read(fd, &v, 1)) {
		perror("Could not read button value");
	}
	return v;
}

/*
 * Button down
 * Will call the handler defined for the specific button
 */
static void
button_down(int button)
{
	if (button_down_function_pointers[button] != NULL) {
		(*button_down_function_pointers[button])();
	}
}

/*
 * Button up
 * Will call the handler defined for the specific button
 */
static void
button_up(int button)
{
	if (button_up_function_pointers[button] != NULL) {
		(*button_up_function_pointers[button])();
	}

}

/*
 * Initialize function
 * Opens the driver and initiates the function pointers
 */
int
input_init(void)
{
	int i;

	/* ønsker en gjennomgang på flaggene her */
	fd = open(GAMEPAD_DRIVER, O_RDWR, (mode_t)0600);
	
	/* set null pointers */
	for (i = 0; i < NUM_BUTTONS; ++i) {
		button_down_function_pointers[i] = NULL;
		button_up_function_pointers[i] = NULL;
	}

	return fd;
}

/*
 * Process input
 * Called on each driver signal, and will not be interrupted by another
 * gamepad driver signal. Will read the current button value and calculat
 * which button was pressed/released.
 */
void
process_input(void)
{

	/* get new button value */
	char new_button_value;
	new_button_value = get_button_value();

	/* get the difference between old and new value */
	char value_diff = button_value ^ new_button_value;
	
	
	if (!value_diff) {
		/*
		 * Nothing have happened. There is no good reason for this to
		 * happen, but nonetheless, we may just return.
		 */
		return;
	}


	/* iterate over bits */
	int z;
	int i = 7;
	for (z = 128; z > 0; z>>=1, --i) {
		if (z & value_diff) {
			/* current bit has changed */
			if (z & new_button_value) {
				/* current bit has been changed to 1 */
				button_down(i);
			} else {
				/* current bit has been changed to 0 */
				button_up(i);
			}
		}
	}


	/* update button value */
	button_value = new_button_value;


}

/*
 * Register button down handler
 */
void
register_button_down_handler(int button, void (*handler)(void))
{
	if (button < 1 && button > NUM_BUTTONS) {
		return;
	}
	button_down_function_pointers[button-1] = handler;

}

/*
 * Register button up handler
 */
void
register_button_up_handler(int button, void (*handler)(void))
{
	if (button < 1 && button > NUM_BUTTONS) {
		return;
	}
	button_up_function_pointers[button-1] = handler;

}
