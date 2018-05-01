CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L
LDFLAGS = -static

SRC = main.c ffplot.c ffdraw.c font_14x7.c
OBJ = $(SRC:.c=.o)
LIB = -lm

all:x ploot

ploot: $(OBJ)
	${CC} $(LDFLAGS) -o $@ $(OBJ) $(LIB)

install:x ploot
	mkdir -p ${PREFIX}/bin
	cp ploot ${PREFIX}/bin/ploot

clean:x
	rm -f *.o ploot

x:

$(SRC): arg.h ploot.h font.h font_14x7.h
