/*
 * Render bitmapped font as a farbfeld image
 *
 * The convention used:                                      y
 * - (0,0) is at the lower left corner of the canvas.        |
 * - (0,1) is above it.                                      +--x
 */

#include <arpa/inet.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ploot.h"

/*
 * Convert (x,y) coordinates to (row,col) for printing into the buffer.
 * The buffer only contain one number, so the coordinate is a single integer:
 *	width * x + y.
 * The coordinates are shifted by offx and offy to permit relative coordinates.
 */
void
ffdraw_pixel(Canvas *can, Color *col,
	int x, int y)
{
	x += can->x;
	y += can->y;
	if (x < 0 || x >= can->h || y < 0 || y >= can->w)
		return;
	memcpy(can->b + can->w * (can->h - 1 - x) + y, col, sizeof(*can->b));
}

void
ffdraw_rectangle(Canvas *can, Color *col,
	int x1, int y1,
	int x2, int y2)
{
	int x, y, xmin, ymin, xmax, ymax;

	xmin = MIN(x1, x2); xmax = MAX(x1, x2);
	ymin = MIN(y1, y2); ymax = MAX(y1, y2);

	for (x = xmin; x <= xmax; x++)
		for (y = ymin; y <= ymax; y++)
			ffdraw_pixel(can, col, x, y);
}

/*
 * From Bresenham's line algorithm and dcat's tplot.
 */
void
ffdraw_line(Canvas *can, Color *col,
	int x0, int y0,
	int x1, int y1)
{
	int dx, dy, sx, sy, err, e;

	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	err = (dx > dy ? dx : -dy) / 2;

	for (;;) {
		ffdraw_pixel(can, col, x0, y0);

		if (x0 == x1 && y0 == y1)
			break;

		e = err;
		if (e > -dx) {
			x0 += sx;
			err -= dy;
		}
		if (e < dy) {
			y0 += sy;
			err += dx;
		}
	}
}

/*
 * Draw a coloured glyph from font f centered on x.
 */
void
ffdraw_char(Canvas *can, Color *col, char c, Font *f,
	int x, int y)
{
	int xf, yf;

	if (c & 0x80)
		c = '\0';

	x -= f->h / 2;

	for (xf = 0; xf < f->h; xf++)
		for (yf = 0; yf < f->w; yf++)
			if (f->b[(int)c][f->w * (f->h - xf - 1) + yf] > 0)
				ffdraw_pixel(can, col, x + xf, y + yf);
}

/*
 * Draw a left aligned string without wrapping it.
 */
void
ffdraw_str_left(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	for (; *s != '\0'; y += f->w, s++)
		ffdraw_char(can, col, *s, f, x, y);
}

/*
 * Draw a center aligned string without wrapping it.
 */
void
ffdraw_str_center(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	y -= f->w * strlen(s) / 2;
	ffdraw_str_left(can, col, s, f, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
void
ffdraw_str_right(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	y -= f->w * strlen(s);
	ffdraw_str_left(can, col, s, f, x, y);
}

void
ffdraw_fill(Canvas *can, Color *col)
{
	int x, y;

	x = can->x;	can->x = 0;
	y = can->y;	can->y = 0;

	ffdraw_rectangle(can, col, 0, 0, can->h - 1, can->w - 1);

	can->x = x;
	can->y = y;
}

void
ffdraw_print(Canvas *can)
{
	uint32_t w, h;

	w = htonl(can->w);
	h = htonl(can->h);

	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	fwrite(can->b, can->w * can->h, sizeof(*can->b), stdout);
}
