/* sleepy-dbus.c - talks with the freedesktop screen saver */
/* vim: set ts=2 sw=2 noet: */
#include "sleepy-dbus.h"
#include "sleepy-dbus-gen.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

sleepy_dbus_result_t
sleepy_dbus_lock_screen(void)
{
	sleepy_dbus_result_t r;
	OrgFreedesktopScreenSaver *proxy;
	GError *error;

	r = dbus_ok;
	error = NULL;
	proxy = org_freedesktop_screen_saver_proxy_new_for_bus_sync(
		G_BUS_TYPE_SESSION,
		G_DBUS_PROXY_FLAGS_NONE,
		"org.freedesktop.ScreenSaver",
		"/ScreenSaver",
		NULL,
		&error
	);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			r = dbus_error;
			goto done;
		}

	org_freedesktop_screen_saver_call_lock_sync(proxy, NULL, &error);
	if (error)
		{
			fprintf(stderr, "%s\n", error->message);
			r = dbus_error;
			goto done;
		}

done:
	g_object_unref(proxy);
}
