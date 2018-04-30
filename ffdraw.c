/*
 * Render bitmapped font as a farbfeld image
 *
 * The convention used:                                      y
 * - (0,0) is at the lower left corner of the canvas.        |
 * - (0,1) is above it.                                      +--x
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "ffdraw.h"
#include "font-14x6.h"

#define WIDTH 100
#define HEIGHT 100

Color buffer[WIDTH * HEIGHT];

void
ffdraw_pixel(Canvas *can, Color col,
	int x, int y)
{
/* Make it segfault early. * /
	x = MIN(can->w - 1, x);
	y = MIN(can->h - 1, y);
/ **/
	memcpy(can->b + x + (can->h - 1 - y) * can->w, col, sizeof(*can->b));
}

void
ffdraw_rectangle(Canvas *can, Color col,
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
 * Adapted from Bresenham's line algorithm and dcat's tplot.
 */
void
ffdraw_line(Canvas *can, Color col,
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
 * Draw a coloured glyph from font f centerd on x, y
 */
void
ffdraw_char(Canvas *can, Color col, char c, Font *f,
	int x, int y)
{
	int xf, yf;

	x -= f->w / 2;
	y -= f->h / 2;

	for (xf = 0; xf < f->w; xf++)
		for (yf = 0; yf < f->h; yf++)
			if (f->b[(int)c][(f->h - yf - 1) * f->w + xf] > 0)
				ffdraw_pixel(can, col, x + xf, y + yf);
}

void
ffdraw_fill(Canvas *can, Color col)
{
	ffdraw_rectangle(can, col, 0, 0, can->w - 1, can->h - 1);
}
