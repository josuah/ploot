CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L

.PHONY: all
all:ploot

ploot.o: config.h arg.h
ploot: ploot.o
	${CC} -static -o ploot ploot.o

.PHONY: install
install: ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

.PHONY: clean
clean:
	rm -f *.o ploot
