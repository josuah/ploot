#ifndef FONT_H
#define FONT_H

#include <stddef.h>

/*
 * Bitmapped font saved as a '_' and 'X' pattern in a C source file.
 */
struct font {
	int		height;		/* The width is variable. */
	char		*glyph[128];	/* 0: end, 1: off, 2: on.  */
};

extern struct font font8;
extern struct font font13;

size_t	font_width(struct font *, int);
size_t	font_strlen(struct font *, char *);

#endif
