bin_PROGRAMS = sleepy-screen

AM_CFLAGS = $(XINPUT_CFLAGS)
sleepy_screen_LDADD = $(XINPUT_LIBS)
sleepy_screen_SOURCES = main.c sleepy-xevents.c sleepy-xevents.h

EXTRA_DIST = org.freedesktop.ScreenSaver.xml
CLEANFILES = sleepy-dbus-gen.c sleepy-dbus-gen.h
sleepy-dbus-gen.c sleepy-dbus-gen.h: org.freedesktop.ScreenSaver.xml
	$(GDBUS_CODEGEN) --generate-c-code sleepy-dbus-gen $<
