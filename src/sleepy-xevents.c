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
	Display *display;
	display = XOpenDisplay(NULL);
	if (!display) return xevents_error_xopendisplay;

	Window win = DefaultRootWindow(display);

	XIEventMask m;
	m.deviceid = XIAllMasterDevices;
	m.mask_len = XIMaskLen(XI_LASTEVENT);
	m.mask = calloc(m.mask_len, sizeof(char));
	XISetMask(m.mask, XI_RawKeyPress);
	XISetMask(m.mask, XI_RawKeyRelease);
	XISetMask(m.mask, XI_RawButtonPress);
	XISetMask(m.mask, XI_RawButtonRelease);

	XISelectEvents(display, win, &m, 1);
	XSync(display, False);

	free(m.mask);

	continuing = 1;
	signal(SIGINT, sigint_handler);

	XEvent ev;
	XGenericEventCookie *cookie;
	while(continuing)
		{
			cookie = (XGenericEventCookie*) &ev.xcookie;
			XNextEvent(display, (XEvent*) &ev);
			XFreeEventData(display, cookie);
			if (callback() != 0) continuing = 0;
		}

	XDestroyWindow(display, win);
	XSync(display, False);
	XCloseDisplay(display);

	return xevents_ok;
}
