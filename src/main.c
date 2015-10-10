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
int unlock(void *arg);

void check_activity(int signum);
void update_alarm(void);

int locked;
time_t wait_sec;
time_t last_active_time;
time_t check_time;

OrgFreedesktopScreenSaver *proxy;

void lock(void)
{
	GError *error;
	fputs("L", stderr);
	alarm(wait_sec);
	error = NULL;
	sleepy_dbus_lock_screen(proxy, &error);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			sleepy_dbus_finish(proxy);
			exit(EXIT_FAILURE);
		}
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
	gboolean r;
	GError *error;
	error = NULL;
	org_freedesktop_screen_saver_call_get_active_sync(proxy, &r, NULL, &error);
	fprintf(stderr, "C:%d", r);
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
	GError *error;
	sleepy_xevents_result_t r;
	wait_sec = 10;

	locked = 1;
	signal(SIGALRM, check_activity);
	unlock(NULL);

	proxy = sleepy_dbus_init_and_get_proxy(&error);
  if (error)
    {
			fprintf(stderr, "%s\n", error->message);
			return EXIT_FAILURE;
    }

	r = sleepy_xevents_loop(unlock, NULL);
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
