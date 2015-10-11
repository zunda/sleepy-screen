/* sleepy-xevents.h - detects x11 events through xinput2 */
/* vim: set ts=2 sw=2 noet: */
/*

Copyright (C) 2015 zunda <zundan@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/

#ifndef SLEEPY_XEVENTS_H
#define SLEEPY_XEVENTS_H

/* return value */
typedef enum {
	xevents_ok = 0,	/* got SIGINT */
	xevents_error_xopendisplay,
} sleepy_xevents_result_t;
	
/* callback when an event is detected - should return 0 to continue the loop */
typedef int (*callback_for_event_t)(void);

/* blocks until an event is detected, calls callback, and continue */
/* ends with SIGINT */
sleepy_xevents_result_t sleepy_xevents_loop(callback_for_event_t callback);

#endif	/* SLEEPY_XEVENTS_H */
