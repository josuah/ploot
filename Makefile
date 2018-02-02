CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all: plot.o config.h
	${CC} -o plot plot.o
