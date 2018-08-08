CFLAGS		= -Wall -Wextra -Werror -std=c89 -pedantic -fPIC \
		-D_POSIX_C_SOURCE=200809L
LDFLAGS		= -static
BIN		= ploot-ff ploot-feed
LIB		= -lm
MAN		= ploot-ff.1 ploot-feed.1
MANDIR		= $(PREFIX)/share/man

SRC_PLOOT_FF	= util.c ploot-ff.c
HDR_PLOOT_FF	= arg.h util.h font.h
OBJ_PLOOT_FF	= $(SRC_PLOOT_FF:.c=.o)

SRC_PLOOT_FEED	= util.c ploot-feed.c
HDR_PLOOT_FEED	= arg.h util.h
OBJ_PLOOT_FEED	= $(SRC_PLOOT_FEED:.c=.o)

all: $(BIN)

ploot-ff: $(OBJ_PLOOT_FF)
	${CC} $(LDFLAGS) -o $@ $(OBJ_PLOOT_FF) $(LIB)

ploot-feed: $(OBJ_PLOOT_FEED)
	${CC} $(LDFLAGS) -o $@ $(OBJ_PLOOT_FEED) $(LIB)

install: $(BIN)
	mkdir -p ${PREFIX}/bin $(MANDIR)/man1
	cp $(BIN) ${PREFIX}/bin
	cp $(MAN) $(MANDIR)/man1

clean:
	rm -f *.o

.PHONY: all install clean

$(SRC_PLOOT_FF): $(HDR_PLOOT_FF)
$(SRC_PLOOT_FEED): $(HDR_PLOOT_FEED)
