#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"

/*
 * Terminal-based plotting using drawille character, aka drawille.
 */

/* parameters used to draw a line */
struct line {
	int x0, y0, x1, y1;		/* point of the line */
	int dx, dy, sx, sy, err;	/* parameters for the algorythm */
};

/*
 * Turn on the bit at position (row, col) of a single cell.  The
 * pattern is not linear (1-4-2-5-3-6-7-8), because it matches the
 * drawille pattern.
 */
static void
drawille_cell_dot(uint8_t *cell, int row, int col)
{
	uint8_t flags[4][2] = {
		{ 0x01, 0x08 },
		{ 0x02, 0x10 },
		{ 0x04, 0x20 },
		{ 0x40, 0x80 },
	};

	*cell |= flags[row][col];
}

static size_t
drawille_cell_utf(uint8_t cell, char *utf)
{
	long rune;

	rune = 10240 + cell;
	utf[0] = (char)(0xe0 | (0x0f & (rune >> 12)));	/* 1110xxxx */
	utf[1] = (char)(0x80 | (0x3f & (rune >> 6)));	/* 10xxxxxx */
	utf[2] = (char)(0x80 | (0x3f & (rune)));	/* 10xxxxxx */
	return 3;
}

static uint8_t
drawille_get(struct drawille *drw, int row, int col)
{
	return drw->buf[row * drw->col + col];
}

size_t
drawille_fmt_row(struct drawille *drw, char *buf, size_t sz, int row)
{
	char txt[] = "xxx";
	size_t n;

	n = 0;
	for (int col = 0; col < drw->col; col++) {
		drawille_cell_utf(drawille_get(drw, row, col), txt);
		n += snprintf(buf+n, sz-n, "%s", txt);
	}
	return n;
}

/*
 * Coordinates are passed as (x, y), but the canvas stores bits as
 * (row, col).  Conversion is made by this function.
 */
void
drawille_dot(struct drawille *drw, int x, int y)
{
	if (x < 0 || x / 2 >= drw->col || y < 0 || y / 4 >= drw->row)
		return;
	drawille_cell_dot(drw->buf + (drw->row - y / 4 - 1) * drw->col + (x / 2),
	    3 - y % 4,
	    x % 2);
}

struct drawille *
drawille_new(int row, int col)
{
	struct drawille *drw;

	if ((drw = calloc(sizeof(struct drawille) + row * col, 1)) == NULL)
		return NULL;
	drw->row = row;
	drw->col = col;
	return drw;
}

static void
drawille_line_init(struct line *l, int x0, int y0, int x1, int y1)
{
	l->x0 = x0;
	l->y0 = y0;
	l->x1 = x1;
	l->y1 = y1;
	l->sx = x0 < x1 ? 1 : -1;
	l->sy = y0 < y1 ? 1 : -1;
	l->dx = abs(x1 - x0);
	l->dy = abs(y1 - y0);
	l->err = (l->dx > l->dy ? l->dx : -l->dy) / 2;
}

static int
drawille_line_next(struct line *l)
{
	int e;

	if (l->x0 == l->x1 && l->y0 == l->y1)
		return 0;

	e = l->err;
	if (e > -l->dx) {
		l->x0 += l->sx;
		l->err -= l->dy;
	}
	if (e < l->dy) {
		l->y0 += l->sy;
		l->err += l->dx;
	}
	return 1;
}

void
drawille_line(struct drawille *drw, int x0, int y0, int x1, int y1)
{
	struct line l;

	drawille_line_init(&l, x0, y0, x1, y1);
	do {
		drawille_dot(drw, l.x0, l.y0);
	} while (drawille_line_next(&l));
}

void
drawille_line_hist(struct drawille *drw, int x0, int y0, int x1, int y1, int zero)
{
	struct line l;
	int sign;

	drawille_line_init(&l, x0, y0, x1, y1);
	do {
		sign = (l.y0 > zero) ? (-1) : (+1);
		for (int y = l.y0; y != zero + sign; y += sign)
			drawille_dot(drw, l.x0, y);
	} while (drawille_line_next(&l));
}

void
drawille_dot_hist(struct drawille *drw, int x, int y, int zero)
{
	int sign;

	sign = (y > zero) ? (-1) : (+1);
	for (; y != zero + sign; y += sign)
		drawille_dot(drw, x, y);
}

static int
drawille_text_glyph(struct drawille *drw, int x, int y, struct font *font, char c)
{
	int width;
	char *glyph;

	if ((unsigned)c > 127)
		glyph = font->glyph[0];
	else
		glyph = font->glyph[(unsigned)c];

	width = strlen(glyph) / font->height;

	for (int ix = 0; ix < width; ix++)
	for (int iy = 0; iy < font->height; iy++) {
		if (glyph[ix + (font->height - 1) * width - iy * width] == 3)
			drawille_dot(drw, x + ix, y + iy);
	}

	return width;
}

char *
drawille_text(struct drawille *drw, int x, int y, struct font *font, char *s)
{
	if (drw->row*4 < font->height)
		return NULL;

	for (; *s != '\0' && x < drw->col/2; s++, x++)
		x += drawille_text_glyph(drw, x, y, font, *s);
	return s;
}
