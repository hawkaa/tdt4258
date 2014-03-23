#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=


all: game

game: signal.c input.c timer.c pong.c screen.c

clean:
	-rm -f game

install:


.PHONY: all install clean

# end of development makefile
