/* sleepy-screen : a simple screen lock that only look for keyboard activity */
/* vim: set ts=2 sw=2 noet: */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/XInput2.h>

int
main(int argc, char *argv[])
{
	Display *display;
	XIEventMask mask[2];
	XIEventMask *m;
	Window win;
	XEvent ev;
	XGenericEventCookie *cookie;

	display = XOpenDisplay(NULL);
	if (!display)
		{
			fputs("Unable to connect to X server.\n", stderr);
			return EXIT_FAILURE;
		}

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

	while(1)
		{
			fputs(".", stderr);
			cookie = (XGenericEventCookie*) &ev.xcookie;
			XNextEvent(display, (XEvent*) &ev);
			XFreeEventData(display, cookie);
		}

	XDestroyWindow(display, win);
	XSync(display, False);
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
