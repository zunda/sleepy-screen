/* sleepy-dbus.c - talks with the freedesktop screen saver */
/* vim: set ts=2 sw=2 noet: */
/*

Copyright (C) 2015 zunda <zundan@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/

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
	org_freedesktop_screen_saver_call_lock_sync(proxy, NULL, error);
	org_freedesktop_screen_saver_call_set_active_sync(proxy, 1, NULL, NULL, error);
}

int
sleepy_dbus_saver_active(OrgFreedesktopScreenSaver *proxy, GError **error)
{
	gboolean r;
	org_freedesktop_screen_saver_call_get_active_sync(proxy, &r, NULL, error);
	return r;
}
