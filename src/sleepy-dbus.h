/* sleepy-dbus.h - talks with the freedesktop screen saver */
/* vim: set ts=2 sw=2 noet: */

#ifndef SLEEPY_DBUS_H
#define SLEEPY_DBUS_H

#include "sleepy-dbus-gen.h"

OrgFreedesktopScreenSaver* sleepy_dbus_init_and_get_proxy(GError **error);
void sleepy_dbus_finish(OrgFreedesktopScreenSaver *proxy);
void sleepy_dbus_lock_screen(OrgFreedesktopScreenSaver *proxy, GError **error);
int sleepy_dbus_saver_active(OrgFreedesktopScreenSaver *proxy, GError **error);


#endif	/* SLEEPY_DBUS_H */
