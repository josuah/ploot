CFLAGS	= -Wall -Wextra -std=c99 -pedantic -fPIC \
		-D_POSIX_C_SOURCE=200809L
LFLAGS	= -static
BIN	= ploot-ff ploot-feed
LIB	= -lm
MANDIR	= $(PREFIX)/share/man

SRC	= csv.c drawille.c font.c font7.c font8.c font13.c util.c scale.c

all: $(BIN)

${BIN}: ${SRC:.c=.o} ${BIN:=.o}
	${CC} $(LFLAGS) -o $@ $@.o ${SRC:.c=.o} $(LIB)

install: $(BIN)
	mkdir -p ${PREFIX}/bin $(MANDIR)/man1 $(MANDIR)/man7
	cp $(BIN) ${PREFIX}/bin
	cp ploot-ff.1 ploot-feed.1 $(MANDIR)/man1
	cp ploot-csv.7 $(MANDIR)/man7

clean:
	rm -f *.o
