include config.mk

src = src/csv.c src/drawille.c src/ffplot.c src/font.c src/font13.c \
	src/font7.c src/font8.c src/log.c src/scale.c src/tool.c
inc = src/csv.h src/drawille.h src/ffplot.h src/font.h src/log.h \
	src/scale.h src/tool.h
bin = ploot-farbfeld ploot-feed ploot-braille
obj = ${src:.c=.o}
lib = -lm

all: $(bin)

.c.o:
	${CC} -c ${CFLAGS} -o $@ $<

${obj} ${bin:=.o}: ${inc} Makefile
${bin}: ${obj} ${bin:=.o}
	${CC} $(LFLAGS) -o $@ $@.o ${obj} $(lib)

install: $(bin)
	mkdir -p ${PREFIX}/bin $(MANDIR)/man1 $(MANDIR)/man7
	cp $(bin) ${PREFIX}/bin
	cp ploot-farbfeld.1 ploot-feed.1 $(MANDIR)/man1
	cp ploot-csv.7 $(MANDIR)/man7

clean:
	rm -f *.o */*.o ${bin}
