NAME = ploot
VERSION = v0.1

D = -D_POSIX_C_SOURCE=200811L -D_BSD_SOURCE
CFLAGS = -Wall -Wextra -std=c99 -pedantic $W $D -fPIC
LFLAGS = -static -lm
PREFIX = /usr/local
MANOREFIX = $(PREFIX)/share/man

SRC = csv.c drawille.c font.c font13.c font8.c util.c
INC = csv.h drawille.h font.h util.h
BIN = ploot-feed ploot-braille ploot-text # ploot-farbfeld
OBJ = ${SRC:.c=.o}

all: ${BIN}

.c.o:
	${CC} -c ${CFLAGS} -o $@ $<

${OBJ} ${BIN:=.o}: ${INC} Makefile
${BIN}: ${OBJ} ${BIN:=.o}
	${CC} ${LFLAGS} -o $@ $@.o ${OBJ}

install: ${BIN}
	mkdir -p ${PREFIX}/bin ${MANDIR}/man1 ${MANDIR}/man5
	cp ${BIN} ${PREFIX}/bin
	cp *.1 ${MANDIR}/man1
	cp *.5 ${MANDIR}/man5

clean:
	rm -f *.o ${BIN}
