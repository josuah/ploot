CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all:ploot

ploot: ploot.o config.h
	${CC} -static -o ploot ploot.o

install: ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

clean:
	rm -f *.o ploot

.PHONY: all clean
