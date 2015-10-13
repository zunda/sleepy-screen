bin_PROGRAMS = sleepy-screen

sleepy_screen_CFLAGS = $(XINPUT_CFLAGS) $(XI2_CFLAGS) $(GIO_CFLAGS)
sleepy_screen_LDADD = $(XINPUT_LIBS) $(XI2_LIBS) $(GIO_LIBS)
sleepy_screen_SOURCES = main.c \
	sleepy-xevents.c sleepy-xevents.h \
	sleepy-dbus.c sleepy-dbus.h
nodist_sleepy_screen_SOURCES = sleepy-dbus-gen.c sleepy-dbus-gen.h
sleepy-screen.$(OBJEXT): sleepy-dbus-gen.c sleepy-dbus-gen.h

EXTRA_DIST = org.freedesktop.ScreenSaver.xml
CLEANFILES = sleepy-dbus-gen.c sleepy-dbus-gen.h
sleepy-dbus-gen.c sleepy-dbus-gen.h: org.freedesktop.ScreenSaver.xml
	$(GDBUS_CODEGEN) --generate-c-code $(basename $@) $<
