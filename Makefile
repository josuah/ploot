CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic -D_POSIX_C_SOURCE=200809L
LDFLAGS = -static

PLOOT_SRC = ploot.c ffplot.c ffdraw.c font_14x7.c util.c
PLOOT_OBJ = $(PLOOT_SRC:.c=.o)

PLOOTXT_SRC = plootxt.c util.c
PLOOTXT_OBJ = $(PLOOTXT_SRC:.c=.o)


LIB = -lm

all:V ploot plootxt

ploot: $(PLOOT_OBJ)
	${CC} $(LDFLAGS) -o $@ $(PLOOT_OBJ) $(LIB)

plootxt: $(PLOOTXT_OBJ)
	${CC} $(LDFLAGS) -o $@ $(PLOOTXT_OBJ) $(LIB)

install:V ploot plootxt
	mkdir -p ${PREFIX}/bin
	cp ploot plootxt ${PREFIX}/bin

clean:V
	rm -f *.o

V: # :V acts like .PHONY:

$(PLOOT_SRC) $(PLOOTXT_SRC): \
arg.h ploot.h util.h font.h font_14x7.h
