CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all: ploot.o config.h
	${CC} -o ploot ploot.o
