CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L

SRC = main.c ffdraw.c font-14x6.c

OBJ = $(SRC:.c=.o)

all:x ploot

ploot: $(OBJ)
	${CC} -static -o $@ $(OBJ)

install:x ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

clean:x
	rm -f *.o ploot

x:
