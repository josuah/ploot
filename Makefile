CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L

all:ploot

ploot: ploot.o config.h
	${CC} -static -o ploot ploot.o

install: ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

clean:
	rm -f *.o ploot

.PHONY: all clean
