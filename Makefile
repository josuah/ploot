CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all: ploot.o config.h
	${CC} -static -o ploot ploot.o

clean:
	rm -f *.o ploot
