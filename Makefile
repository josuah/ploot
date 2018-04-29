CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L

SRC = font.c

OBJ = $(SRC:.c=.o)

all:x ploot

ploot.o: config.h arg.h
ploot: $(OBJ)
	${CC} -static -o $@ $(OBJ)

install:x ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

clean:x
	rm -f *.o ploot

x:
