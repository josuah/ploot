CFLAGS	= -Wall -Wextra -Werror -std=c89 -pedantic

all: plot.o
	${CC} -o plot plot.o
