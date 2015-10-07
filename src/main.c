/* sleepy-screen : a simple screen lock that only look for keyboard activity */
/* vim: set ts=2 sw=2 noet: */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "sleepy-xevents.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void lock(void);
int unlock(void *arg);

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
	fputs("L", stderr);
	alarm(0);
	r = system(LOCK_COMMAND);
	if (r) exit(r);
	locked = 1;
}

int
unlock(void *arg)
{
	fputs(".", stderr);
	time(&last_active_time);
	if (locked)
		{
			locked = 0;
			update_alarm();
		}
	return 0;
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
	fputs("C", stderr);
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
	sleepy_xevents_result_t r;
	wait_sec = 10;

	locked = 1;
	signal(SIGALRM, check_activity);
	unlock(NULL);

	r = sleepy_xevents_loop(unlock, NULL);
	switch(r)
		{
			case error_xopendisplay:
				fputs("Could not connect to X server.\n", stderr);
				return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
}
