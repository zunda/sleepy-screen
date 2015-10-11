/* sleepy-xevents.c - detects x11 events through xinput2 */
/* vim: set ts=2 sw=2 noet: */
/*

Copyright (C) 2015 zunda <zundan@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/
#include "sleepy-xevents.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <X11/extensions/XInput2.h>

#include <signal.h>

int continuing;
void sigint_handler(int signum);

void
sigint_handler(int signum)
{
	if (signum == SIGINT) continuing = 0;
}

sleepy_xevents_result_t
sleepy_xevents_loop(callback_for_event_t callback)
{
	int r;
	Display *display;
	XIEventMask mask[2];
	XIEventMask *m;
	Window win;
	XEvent ev;
	XGenericEventCookie *cookie;

	display = XOpenDisplay(NULL);
	if (!display) return xevents_error_xopendisplay;

	win = DefaultRootWindow(display);

	m = &mask[0];
	m->deviceid = XIAllDevices;
	m->mask_len = XIMaskLen(XI_LASTEVENT);
	m->mask = calloc(m->mask_len, sizeof(char));
	XISetMask(m->mask, XI_KeyPress);
	XISetMask(m->mask, XI_KeyRelease);

	m = &mask[1];
	m->deviceid = XIAllMasterDevices;
	m->mask_len = XIMaskLen(XI_LASTEVENT);
	m->mask = calloc(m->mask_len, sizeof(char));
	XISetMask(m->mask, XI_RawKeyPress);
	XISetMask(m->mask, XI_RawKeyRelease);

	XISelectEvents(display, win, &mask[0], 2);
	XSync(display, False);

	free(mask[0].mask);
	free(mask[1].mask);

	continuing = 1;
	signal(SIGINT, sigint_handler);

	while(continuing)
		{
			cookie = (XGenericEventCookie*) &ev.xcookie;
			XNextEvent(display, (XEvent*) &ev);
			XFreeEventData(display, cookie);
			r = callback();
			if (r != 0) continuing = 0;
		}

	XDestroyWindow(display, win);
	XSync(display, False);
	XCloseDisplay(display);

	return xevents_ok;
}
