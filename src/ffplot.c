#include "ffplot.h"

#include <arpa/inet.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "font.h"
#include "util.h"

/*
 * Convert (x,y) coordinates to (row,col) for printing into the buffer.
 * The buffer only contain one number, so the coordinate is a single integer:
 *	width * y + y.
 * The coordinates are shifted by offx and offy to permit relative coordinates.
 *
 * The convention used:                                      y
 * - (0,0) is at the lower left corner of the plotvas.        |
 * - (0,1) is above it.                                      +--x
 */
void
ffplot_pixel(struct ffplot *plot, struct ffcolor *color,
	int x, int y)
{
	x += plot->x;
	y += plot->y;
	if (x < 0 || x >= plot->w || y < 0 || y >= plot->h)
		return;
	memcpy(plot->buf + plot->w * (plot->h - 1 - y) + x, color, sizeof(*plot->buf));
}

void
ffplot_rectangle(struct ffplot *plot, struct ffcolor *color,
	int y1, int x1,
	int y2, int x2)
{
	int x, y, ymin, xmin, ymax, xmax;

	ymin = MIN(y1, y2); ymax = MAX(y1, y2);
	xmin = MIN(x1, x2); xmax = MAX(x1, x2);

	for (y = ymin; y <= ymax; y++)
		for (x = xmin; x <= xmax; x++)
			ffplot_pixel(plot, color, x, y);
}

/*
 * From Bresenham's line algorithm and dcat's tplot.
 */
void
ffplot_line(struct ffplot *plot, struct ffcolor *color,
	int x0, int y0,
	int x1, int y1)
{
	int dy, dx, sy, sx, err, e;

	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	dx = ABS(x1 - x0);
	dy = ABS(y1 - y0);
	err = (dy > dx ? dy : -dx) / 2;

	for (;;) {
		ffplot_pixel(plot, color, x0, y0);

		if (y0 == y1 && x0 == x1)
			break;

		e = err;
		if (e > -dy) {
			y0 += sy;
			err -= dx;
		}
		if (e < dx) {
			x0 += sx;
			err += dy;
		}
	}
}

/*
 * Draw a coloured glyph from font f centered on y.
 */
int
ffplot_char(struct ffplot *plot, struct ffcolor *color, struct font *ft, char c,
	int x, int y)
{
	int yf, xf, wf;

	if (c & 0x80)
		c = '\0';
	y -= ft->height / 2;
	wf = font_width(ft, c);
	for (xf = 0; xf < wf; xf++)
		for (yf = 0; yf < ft->height; yf++)
			if (ft->glyph[(int)c][wf * (ft->height - yf) + xf] == 3)
				ffplot_pixel(plot, color, x + xf, y + yf);
	return wf + 1;
}

/*
 * Draw a left aligned string without wrapping it.
 */
size_t
ffplot_text_left(struct ffplot *plot, struct ffcolor *color, struct font *ft,
	char *s, int x, int y)
{
	for (; *s != '\0'; s++)
		x += ffplot_char(plot, color, ft, *s, x, y);
	return x;
}

/*
 * Draw a center aligned string without wrapping it.
 */
size_t
ffplot_text_center(struct ffplot *plot, struct ffcolor *color, struct font *ft,
	char *s, int x, int y)
{
	x -= font_strlen(ft, s) / 2;
	return ffplot_text_left(plot, color, ft, s, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
size_t
ffplot_text_right(struct ffplot *plot, struct ffcolor *color, struct font *ft,
	char *s, int x, int y)
{
	x -= font_strlen(ft, s);
	return ffplot_text_left(plot, color, ft, s, x, y);
}

void
ffplot_print(FILE *fp, struct ffplot *plot)
{
	uint32_t w, h;

	w = htonl(plot->w);
	h = htonl(plot->h);

	fprintf(stdout, "farbfeld");
	fwrite(&w, sizeof(w), 1, fp);
	fwrite(&h, sizeof(h), 1, fp);
	fwrite(plot->buf, plot->w * plot->h, sizeof(*plot->buf), fp);
}
