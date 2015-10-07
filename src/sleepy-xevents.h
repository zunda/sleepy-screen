/* sleepy-xevents.h - detects x11 events through xinput2 */
/* vim: set ts=2 sw=2 noet: */

#ifndef SLEEPY_XEVENTS_H
#define SLEEPY_XEVENTS_H

/* return value */
typedef enum {
	ok = 0,	/* got SIGINT */
	error_xopendisplay,
} sleepy_xevents_result_t;
	
/* callback when an event is detected - should return 0 to continue the loop */
typedef int (*callback_for_event_t)(void *);

/* blocks until an event is detected, calls callback, and continue */
/* ends with SIGINT */
sleepy_xevents_result_t sleepy_xevents_loop(callback_for_event_t callback, void *callback_arg);

#endif	/* SLEEPY_XEVENTS_H */
