#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ffdraw.h"
#include "font_14x7.h"

#define FONT_H 14
#define FONT_W 7

/* as you see, no css skills needed */

#define MARGIN 2

/* height */

#define TITLE_H (MARGIN + FONT_H + MARGIN)
#define PLOT_H 100
#define XLABEL_H (MARGIN + FONT_H + MARGIN)

#define IMAGE_H (TITLE_H + PLOT_H + XLABEL_H)

#define TITLE_MAX (IMAGE_H)
#define TITLE_MIN (IMAGE_H - TITLE_H)
#define PLOT_X_MAX (IMAGE_H - TITLE_H)
#define PLOT_X_MIN (XLABEL_H)
#define XLABEL_MAX (XLABEL_H)
#define XLABEL_MIN (0)

/* width */

#define YLABEL_W (MARGIN + 20 + MARGIN)
#define PLOT_W 500
#define LEGEND_W (MARGIN + 70 + MARGIN)

#define IMAGE_W (YLABEL_W + PLOT_W + LEGEND_W)

#define LEGEND_MAX (IMAGE_W)
#define LEGEND_MIN (IMAGE_W - LEGEND_W)
#define PLOT_Y_MAX (IMAGE_W - LEGEND_W)
#define PLOT_Y_MIN (YLABEL_W)
#define YLABEL_MAX (YLABEL_W)
#define YLABEL_MIN (0)

#define MID(x, y) ((x - y) / 2)

Color buffer[IMAGE_W * IMAGE_H];

Color c_axis = { 0xffff, 0xffff, 0xffff, 0xfff };
Font *font = &font_14x7;

void
ffplot_xaxis(Canvas *can, Color label, Color grid, time_t tmax, time_t tstep)
{
	time_t t;
	int y, ystep;
	char str[sizeof("YYYY/MM/DD")], *fmt;

	if (tstep < 3600) {
		fmt = " %H:%M:%S ";
		ystep = sizeof(" HH:MM:SS ") * FONT_W;
	} else {
		fmt = " %Y/%m/%d ";
		ystep = sizeof(" YYYY/MM/DD ") * FONT_W;
	}

	t = tmax % tstep;
	y = PLOT_Y_MAX + PLOT_W % ystep - ystep;

	while (y > PLOT_Y_MIN) {
		strftime(str, sizeof(str), fmt, localtime(&t));
		ffdraw_str(can, label, str, font,
			XLABEL_MIN + XLABEL_H / 2, y - ystep / 2 + FONT_W);
		ffdraw_line(can, grid,
			PLOT_X_MIN, y,
			PLOT_X_MAX, y);
		y -= ystep;
		t -= tstep;
	}
}

void
ffplot_(Canvas *can)
{
	(void)can;
}

void
ffplot_graph(Canvas *can)
{
	(void)can;
}

void
ffplot_legend(Canvas *can)
{
	(void)can;
}

static void
ffdraw(Canvas *can)
{
	Color col1 = { 0x2222, 0x2222, 0x2222, 0xffff };
	Color label = { 0x3333, 0xffff, 0x8888, 0xffff };
	Color grid = { 0x4444, 0x4444, 0x4444, 0xffff };

	ffdraw_fill(can, col1);
	ffplot_xaxis(can, label, grid, 3600 * 24 * 30, 360);
/*
	ffdraw_line(can, col2, 49,1,9,79);
	ffdraw_str(can, col2, "R\\S`T'UaVbWcYdZe\nfghb\tjoi\rklmnopqrstuvwxyz{|}", font, 44, 10);
*/
}

int
main(void)
{
	uint32_t w, h;
	Canvas can;

	can.b = buffer;
	can.w = IMAGE_W;
	can.h = IMAGE_H;
	w = htonl(IMAGE_W);
	h = htonl(IMAGE_H);
	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	ffdraw(&can);
	fwrite(can.b, IMAGE_W * IMAGE_H, sizeof(*can.b), stdout);
	return 0;
}
