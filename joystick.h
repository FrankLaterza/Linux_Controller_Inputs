/* 
    (C) Copyright 2007,2008, Stephen M. Cameron.
    This file is part of wordwarvi.
 */
#include <stdio.h>
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#define JOYSTICK_DEVNAME "/dev/input/js0"

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */


struct js_event {
	unsigned int time;	/* event timestamp in milliseconds */
	short value;   /* value */
	unsigned char type;     /* event type */
	unsigned char number;   /* axis/button number */
};

struct wwvi_js_event {
	int button[11];
	int stick_x1;
	int stick_y1;
	int stick_x2;
	int stick_y2;
	int trigger_L2;
	int trigger_R2;
};

extern int open_joystick(char *joystick_device);
extern int read_joystick_event(struct js_event *jse);
extern void set_joystick_y_axis(int axis);
extern void set_joystick_x_axis(int axis);
extern void close_joystick();
extern int get_joystick_status(struct wwvi_js_event *wjse);

#endif