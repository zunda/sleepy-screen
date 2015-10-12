sleepy-screen
=============

A simple screen locker that monitors only keyboard

Usage
-----
This program monitors keyboard activity and locks screen after a while
through freedesktop.org compatible screen lock.

Start this program when logging in through, e.g. the Session and Startup menu.
Run `sleepy-screen -h` to see command line options.

Installation
------------
Programmed and tested on Ubuntu 15.04 and xfce.

```
$ autoreconf
$ ./configure
$ make
# make install
```

Packages you may need to install to build this program:
- build-essential
- autoconf
- libglib2.0-dev
- libxext-dev
- libxi-dev

License
-------
Copyright (C) 2015 zunda <zundan@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
