/* sleepy-dbus.h - talks with the freedesktop screen saver */
/* vim: set ts=2 sw=2 noet: */

#ifndef SLEEPY_DBUS_H
#define SLEEPY_DBUS_H

/* return value */
typedef enum {
	dbus_ok = 0,
	dbus_error,
} sleepy_dbus_result_t;
	
sleepy_dbus_result_t sleepy_dbus_lock_screen(void);

#endif	/* SLEEPY_DBUS_H */
