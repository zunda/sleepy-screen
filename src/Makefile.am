bin_PROGRAMS = sleepy-screen

AM_CFLAGS = $(XINPUT_CFLAGS)
sleepy_screen_LDADD = $(XINPUT_LIBS)
sleepy_screen_SOURCES = main.c sleepy-xevents.c sleepy-xevents.h

EXTRA_DIST = org.freedesktop.ScreenSaver.xml
CLEANFILES = \
	sleepy-screen-dbus-gen.c \
	sleepy-screen-dbus-gen.h
sleepy-screen-dbus-gen.c sleepy-screen-dbus-gen.h: org.freedesktop.ScreenSaver.xml
	$(GDBUS_CODEGEN) --generate-c-code sleepy-screen-dbus-gen $<
