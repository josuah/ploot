#ifndef DRAWILLE_H
#define DRAWILLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "csv.h"
#include "font.h"

/*
 * Canvas to draw on with braille characters.
 */
struct drawille {
	int		col, row;	/* number of dots in total */
	uint8_t		buf[];		/* buffer of size (col * row) */
};

/**/
size_t		drawille_put_row	(FILE *, struct drawille *, int);
void		drawille_dot		(struct drawille *, int, int);
struct drawille *drawille_new		(int, int);
void		drawille_line		(struct drawille *, int, int, int, int);
void		drawille_histogram_dot	(struct drawille *, int, int, int);
void		drawille_histogram_line	(struct drawille *, int, int, int, int, int);
char *		drawille_text		(struct drawille *, int, int, struct font *, char *);

#endif
