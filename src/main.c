/* sleepy-screen : a simple screen lock that only look for keyboard activity */
/* vim: set ts=2 sw=2 noet: */
/*

Copyright (C) 2015 zunda <zundan@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "sleepy-xevents.h"
#include "sleepy-dbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

void call_sleepy_dbus(void (*  func)(OrgFreedesktopScreenSaver *, GError **), char const* logstring);
void lock(void);
void blank(void);
int mark(void);
void usage(FILE* stream);

int verbosity = 0;
long wait_sec = 1800;
long blank_sec = 60;
time_t last_active_time;

OrgFreedesktopScreenSaver *proxy;

void
call_sleepy_dbus(void ( *func)(OrgFreedesktopScreenSaver *, GError **), char const* logstring)
{
	GError *error = NULL;
	if (verbosity > 0) fputs(logstring, stderr);
	error = NULL;
	func(proxy, &error);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			sleepy_dbus_finish(proxy);
			exit(EXIT_FAILURE);
		}
}

void
lock(void)
{
	call_sleepy_dbus(sleepy_dbus_lock_screen, "L");
}

void
blank(void)
{
	call_sleepy_dbus(sleepy_dbus_blank_screen, "B");
}

int
mark(void)
{
	if (verbosity > 0) fputs(".", stderr);
	time(&last_active_time);
	return 0;
}

void
check(int signum)
{
	time_t now;
	if (verbosity > 0) fputs("C", stderr);
	time(&now);
	if (last_active_time + wait_sec + blank_sec <= now)
		{
			lock();
			alarm(blank_sec);
		}
	else if (last_active_time + wait_sec <= now)
		{
			blank();
			alarm(blank_sec);
		}
	else
		{
			alarm(last_active_time + wait_sec - now);
		}
}

void
usage(FILE* stream)
{
	fprintf(stream,
		"sleepy-screen - A simple screen locker that monitors only keyboard\n\n"
		"options:\n"
		"\t-w wait : sepcify idle time before blank [%ld sec]\n"
		"\t-l wait : sepcify idle time after blank before lock [%ld sec]\n"
		"\t-v      : increase verbosity\n"
		"\t-h      : show help\n",
	 wait_sec, blank_sec);

	if (verbosity > 0)
	fputs("\n"
		PACKAGE_STRING "\n"
		"Copyright (C) 2015 zunda <zundan@gmail.com>\n\n"
		"This program is free software: you can redistribute it and/or modify\n"
		"it under the terms of the GNU General Public License as published by\n"
		"the Free Software Foundation, either version 3 of the License, or\n"
		"(at your option) any later version.\n\n"
		"This program is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"GNU General Public License for more details.\n\n"
		"You should have received a copy of the GNU General Public License\n"
		"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n",
	stream);
}

int
main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "hvw:l:")) != -1)
		{
			switch (opt)
				{
				case 'h':
					usage(stdout);
					return EXIT_SUCCESS;
				case 'v':
					verbosity += 1;
					break;
				case 'w':
					wait_sec = atol(optarg);
					break;
				case 'l':
					blank_sec = atol(optarg);
					break;
				default:
					usage(stderr);
					return EXIT_FAILURE;
				}
		}

	GError *error = NULL;
	proxy = sleepy_dbus_init_and_get_proxy(&error);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			return EXIT_FAILURE;
		}

	signal(SIGALRM, check);
	alarm(wait_sec);
	if (sleepy_xevents_loop(mark) == xevents_error_xopendisplay)
		{
			fputs("Could not connect to X server.\n", stderr);
			sleepy_dbus_finish(proxy);
			return EXIT_FAILURE;
		}

	sleepy_dbus_finish(proxy);
	return EXIT_SUCCESS;
}
