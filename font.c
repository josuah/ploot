/*
 * Render bitmapped font as a farbfeld image
 */

#include <arpa/inet.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "font-14x7.h"

#define WIDTH 100
#define HEIGHT 100

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef uint16_t Color[4];

typedef struct {
	Color *b;	/* buffer */
	int w;		/* width */
	int h;		/* height */
} Canvas;

Color buffer[WIDTH * HEIGHT];

void
ffdraw_pixel(Canvas *can, Color c,
	int x, int y)
{
	memcpy(can->b + x + (can->h - 1 - y) * can->w, c, sizeof(*can->b));
}

void
ffdraw_rectangle(Canvas *can, Color c,
	int x1, int y1,
	int x2, int y2)
{
	int x, y, xmin, ymin, xmax, ymax;

	xmin = MIN(x1, x2); xmax = MAX(x1, x2);
	ymin = MIN(y1, y2); ymax = MAX(y1, y2);

	for (x = xmin; x <= xmax; x++)
		for (y = ymin; y <= ymax; y++)
			ffdraw_pixel(can, c, x, y);
}

void
ffdraw_line(Canvas *can, Color c,
	int x1, int y1,
	int x2, int y2)
{
	int x, y;

	(void)c;
	(void)can;

	x = x1;
	y = y1;
	while (x < x2 && y < y2) {
		x++; y++;
	}
}

void
ffdraw_fill(Canvas *can, Color c)
{
	ffdraw_rectangle(can, c, 0, 0, can->w - 1, can->h - 1);
}

void
ffdraw(Canvas *can)
{
	Color c1 = { 0x2222, 0x2222, 0x2222, 0xffff };
	Color c2 = { 0x3333, 0xffff, 0x8888, 0xffff };

	ffdraw_fill(can, c1);
	ffdraw_rectangle(can, c2,
		0, 20,
		can->w - 10, 4);
}

void
usage(void)
{
	fprintf(stderr, "ploot <data.csv >image.ff\n");
	exit(1);
}

int
main(void)
{
	uint32_t w, h;
	Canvas can;

	can.b = buffer;
	can.w = WIDTH;
	can.h = HEIGHT;
	w = htonl(WIDTH);
	h = htonl(HEIGHT);
	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	ffdraw(&can);
	fwrite(can.b, WIDTH * HEIGHT, sizeof(*can.b), stdout);
	return 0;
}
