/* sleepy-screen : a simple screen lock that only look for keyboard activity */
/* vim: set ts=2 sw=2 noet: */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/XInput2.h>

#include <time.h>
#include <signal.h>

void lock(void);
void unlock(void);

void check_activity(int signum);
void update_alarm(void);

int locked;
time_t wait_sec;
time_t last_active_time;
time_t check_time;

#define LOCK_COMMAND \
"dbus-send --print-reply --dest=org.freedesktop.ScreenSaver /ScreenSaver org.freedesktop.ScreenSaver.Lock"

void lock(void)
{
	int r;
	alarm(0);
	r = system(LOCK_COMMAND);
	if (r) exit(r);
	locked = 1;
}

void unlock(void)
{
	time(&last_active_time);
	if (locked)
		{
			locked = 0;
			update_alarm();
		}
}

void
update_alarm(void)
{
	time_t now;
	check_time = last_active_time + wait_sec;
	time(&now);
	alarm(check_time - now);
}

void
check_activity(int signum)
{
	time_t now;
	time(&now);
	if (last_active_time + wait_sec <= now)
		lock();
	else
		update_alarm();
}

int
main(int argc, char *argv[])
{
	Display *display;
	XIEventMask mask[2];
	XIEventMask *m;
	Window win;
	XEvent ev;
	XGenericEventCookie *cookie;

	wait_sec = 10;

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

	locked = 1;
	signal(SIGALRM, check_activity);
	unlock();

	while(1)
		{
			cookie = (XGenericEventCookie*) &ev.xcookie;
			XNextEvent(display, (XEvent*) &ev);
			XFreeEventData(display, cookie);
			unlock();
		}

	XDestroyWindow(display, win);
	XSync(display, False);
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
