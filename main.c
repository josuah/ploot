#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ffdraw.h"
#include "font_14x7.h"

/* as you see, no css skills needed */

#define MARGIN 5
#define FONT_H 14
#define FONT_W 7

/* height */

#define TITLE_H (MARGIN + FONT_H + MARGIN)
#define PLOT_H 100
#define XLABEL_H (MARGIN + FONT_H + MARGIN)

#define IMAGE_H (TITLE_H + PLOT_H + XLABEL_H)

#define TITLE_B (0)
#define TITLE_E (TITLE_H)
#define PLOT_X_B (TITLE_H)
#define PLOT_X_E (IMAGE_H - XLABEL_H)
#define XLABEL_B (IMAGE_H - XLABEL_H)
#define XLABEL_E (IMAGE_H)

/* width */

#define YLABEL_W (MARGIN + 50 + MARGIN)
#define PLOT_W 500
#define LEGEND_W (MARGIN + 70 + MARGIN)

#define IMAGE_W (YLABEL_W + PLOT_W + LEGEND_W)

#define YLABEL_B (0)
#define YLABEL_E (YLABEL_W)
#define PLOT_Y_B (YLABEL_W)
#define PLOT_Y_E (IMAGE_W - LEGEND_W)
#define LEGEND_B (IMAGE_W - LEGEND_W)
#define LEGEND_E (IMAGE_W)

#define MID(x, y) ((x - y) / 2)

Color buffer[IMAGE_W * IMAGE_H];

Color c_axis = { 0xffff, 0xffff, 0xffff, 0xfff };
Font *font = &font_14x7;

void
ffplot_xaxis(Canvas *can, Color col, time_t tmax, time_t tstep)
{
	time_t t;
	int x, y, ystep, width;
	char str[sizeof("YYYY/MM/DD")], *fmt;

	if (tstep < 3600) {
		fmt = "%H:%M:%S";
		width = sizeof("HH:MM:SS");
	} else {
		fmt = "%Y/%m/%d";
		width = sizeof("YYYY/MM/DD");
	}

	ystep = MARGIN + FONT_W * width + MARGIN;

	t = tmax % tstep;
	x = XLABEL_B + FONT_H / 2;
	y = PLOT_Y_B + PLOT_W % ystep - width / 2;

	while (y > PLOT_Y_B) {
		strftime(str, sizeof(str), fmt, localtime(&t));
		ffdraw_str(can, col, str, font, x, y);

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
	Color col2 = { 0x3333, 0xffff, 0x8888, 0xffff };

	ffdraw_fill(can, col1);
	ffplot_xaxis(can, col2, 3600 * 24 * 30, 360);
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
