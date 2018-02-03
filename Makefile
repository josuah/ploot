CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all:ploot

ploot: ploot.o config.h
	${CC} -static -o ploot ploot.o

clean:
	rm -f *.o ploot

.PHONY: all clean
