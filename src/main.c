/* sleepy-screen : a simple screen lock that only look for keyboard activity */
/* vim: set ts=2 sw=2 noet: */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "sleepy-xevents.h"
#include "sleepy-dbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void lock(void);
int mark(void *arg);
void check_activity(int signum);

long wait_sec;
time_t last_active_time;

OrgFreedesktopScreenSaver *proxy;

void
lock(void)
{
	GError *error;
	fputs("L", stderr);
	error = NULL;
	sleepy_dbus_lock_screen(proxy, &error);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			sleepy_dbus_finish(proxy);
			exit(EXIT_FAILURE);
		}
}

int
mark(void *arg)
{
	fputs(".", stderr);
	time(&last_active_time);
	return 0;
}

void
check(int signum)
{
	time_t now;
	fputs("C", stderr);
	time(&now);
	if (last_active_time + wait_sec <= now)
		{
			int saver_active;
			GError *error;
			error = NULL;
			saver_active = sleepy_dbus_saver_active(proxy, &error);
			if (error)
				{
					fprintf(stderr, "%s\n", error->message);
					sleepy_dbus_finish(proxy);
					exit(EXIT_FAILURE);
				}
			if (!saver_active) lock();
			alarm(wait_sec);
		}
	else
		{
			alarm(last_active_time + wait_sec - now);
		}
}

int
main(int argc, char *argv[])
{
	GError *error;
	sleepy_xevents_result_t r;
	wait_sec = 10;

	proxy = sleepy_dbus_init_and_get_proxy(&error);
  if (error)
    {
			fprintf(stderr, "%s\n", error->message);
			return EXIT_FAILURE;
    }

	signal(SIGALRM, check);
	alarm(wait_sec);
	r = sleepy_xevents_loop(mark, NULL);
	switch(r)
		{
			case xevents_error_xopendisplay:
				fputs("Could not connect to X server.\n", stderr);
				sleepy_dbus_finish(proxy);
				return EXIT_FAILURE;
		}

	sleepy_dbus_finish(proxy);
	return EXIT_SUCCESS;
}
