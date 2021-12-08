/* 
    (C) Copyright 2007,2008, Stephen M. Cameron.
    This file is part of wordwarvi.

 */


#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include "joystick.h"

//using namespace std;


static int joystick_fd = -1;

/* These are sensible on Logitech Dual Action Rumble and xbox360 controller. */
static int joystick_x1_axis = 0;
static int joystick_y1_axis = 1;
static int joystick_x2_axis = 2;
static int joystick_y2_axis = 5;
static int joystick_L2_axis = 3;
static int joystick_R2_axis = 4;



int open_joystick()
{
	joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
	if (joystick_fd < 0)
		return joystick_fd;

	/* maybe ioctls to interrogate features here? */

	return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse)); 

	if (bytes == -1)
		return 0;

	if (bytes == sizeof(*jse))
		return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}

void close_joystick()
{
	close(joystick_fd);
}

int get_joystick_status(struct wwvi_js_event *wjse)
{
	int rc;
	struct js_event jse;
	if (joystick_fd < 0)
		return -1;

	/* memset(wjse, 0, sizeof(*wjse)); */
	if ((rc = read_joystick_event(&jse) == 1)) {
		jse.type &= ~JS_EVENT_INIT; /* ignore synthetic events */
		if (jse.type == JS_EVENT_AXIS) {
			if (jse.number == joystick_x1_axis)
				wjse->stick_x1 = jse.value;
			if (jse.number == joystick_y1_axis)
				wjse->stick_y1 = jse.value;
			if (jse.number == joystick_x2_axis)
				wjse->stick_x2 = jse.value;
			if (jse.number == joystick_y2_axis)
				wjse->stick_y2 = jse.value;
			if (jse.number == joystick_L2_axis)
				wjse->trigger_L2 = jse.value;
			if (jse.number == joystick_R2_axis)
				wjse->trigger_R2 = jse.value;
		} else if (jse.type == JS_EVENT_BUTTON) {
			if (jse.number < 11) {
				switch (jse.value) {
				case 0:
				case 1: wjse->button[jse.number] = jse.value;
					break;
				default:
					break;
				}
			}
		}
	}
	return 0;
}



int main(int argc, char *argv[])
{
	//struct js_event jse;
	struct wwvi_js_event wjse;

	int fd = open_joystick();
	if (fd < 0) {
		printf("open failed.\n");
		exit(1);
	}

 	while (true) {


		usleep(1000);

		get_joystick_status(&wjse);
		
		printf("stick x1 %d", wjse.stick_x1);
		printf("   stick y1 %d", wjse.stick_y1);
		printf("   stick x2 %d", wjse.stick_x2);
		printf("   stick y2 %d", wjse.stick_y2);
		printf("   trigger L2 %d", wjse.trigger_L2);
		printf("   trigger R2 %d", wjse.trigger_R2);
		printf("   buttons ");
		for(int i=0; i<11; i++){
			printf("%d", wjse.button[i]);}

		printf("\n");
		}


}

