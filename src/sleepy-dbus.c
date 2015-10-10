/* sleepy-dbus.c - talks with the freedesktop screen saver */
/* vim: set ts=2 sw=2 noet: */
#include "sleepy-dbus.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

OrgFreedesktopScreenSaver*
sleepy_dbus_init_and_get_proxy(GError **error)
{
	OrgFreedesktopScreenSaver *proxy;
	*error = NULL;

	proxy = org_freedesktop_screen_saver_proxy_new_for_bus_sync(
		G_BUS_TYPE_SESSION,
		G_DBUS_PROXY_FLAGS_NONE,
		"org.freedesktop.ScreenSaver",
		"/ScreenSaver",
		NULL,
		error
	);

	return proxy;
}

void
sleepy_dbus_finish(OrgFreedesktopScreenSaver *proxy)
{
	g_object_unref(proxy);
}

void
sleepy_dbus_lock_screen(OrgFreedesktopScreenSaver *proxy, GError **error)
{
	org_freedesktop_screen_saver_call_set_active_sync(proxy, 1, NULL, NULL, error);
	org_freedesktop_screen_saver_call_lock_sync(proxy, NULL, error);
}
