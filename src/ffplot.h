#ifndef FFPLOT_H
#define FFPLOT_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "font.h"

struct ffcolor {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t alpha;
};

struct ffplot {
	int w;		/* width */
	int h;		/* height */
	int x;		/* x offset */
	int y;		/* y offset */
	struct ffcolor *buf;
};

/**/
void		ffplot_pixel		(struct ffplot *, struct ffcolor *, int, int);
void		ffplot_rectangle	(struct ffplot *, struct ffcolor *, int, int, int, int);
void		ffplot_line		(struct ffplot *, struct ffcolor *, int, int, int, int);
int		ffplot_char		(struct ffplot *, struct ffcolor *, struct font *, char, int, int);
size_t		ffplot_text_left	(struct ffplot *, struct ffcolor *, struct font *, char *, int, int);
size_t		ffplot_text_center	(struct ffplot *, struct ffcolor *, struct font *, char *, int, int);
size_t		ffplot_text_right	(struct ffplot *, struct ffcolor *, struct font *, char *, int, int);
void		ffplot_print		(FILE *, struct ffplot *);

#endif
