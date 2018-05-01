/*
 * Draw a plot
 *
 *	       Title       (units)
 *	     y ^                    Legend
 *	 label |- + - + - + - + -
 *	  here |- + - + - + - + -
 *	       +--+---+---+---+-->
 *	        x label here        
 */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ploot.h" /* placed before for Font type declaration */
#include "font_14x7.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define MARGIN 4

#define FONT_H 14
#define FONT_W 7

#define TITLE_X (IMAGE_H - TITLE_H)
#define TITLE_Y (XLABEL_W)
#define TITLE_H (FONT_H * 2)
#define TITLE_W (PLOT_W)

#define XLABEL_X (PLOT_X)
#define XLABEL_Y (0)
#define XLABEL_H (PLOT_H)
#define XLABEL_W (FONT_W * 9 + MARGIN)

#define YLABEL_X (0)
#define YLABEL_Y (PLOT_Y)
#define YLABEL_H (FONT_H * 2)
#define YLABEL_W (PLOT_W)

#define PLOT_X (YLABEL_H)
#define PLOT_Y (XLABEL_W)
#define PLOT_W 700
#define PLOT_H 200

#define LEGEND_X (YLABEL_H)
#define LEGEND_Y (IMAGE_W - LEGEND_W)
#define LEGEND_W (FONT_W + 150 + FONT_W)
#define LEGEND_H (PLOT_H)

#define IMAGE_H (TITLE_H + PLOT_H + YLABEL_H)
#define IMAGE_W (XLABEL_W + PLOT_W + LEGEND_W)

Color buffer[IMAGE_W * IMAGE_H];

Color c_axis = { 0xffff, 0xffff, 0xffff, 0xfff };
Font *font = &font_14x7;

static int
t2y(time_t t, time_t tmin, time_t tmax)
{
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
v2x(double v, double vmin, double vmax)
{
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

/*
 * Set 'str' to a human-readable form of 'num' with always a width of 8 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.  Return the exponent.
 */
static int
humanize(char *str, double val)
{
	int exp, precision;
	char label[] = { '\0', 'M', 'G', 'T', 'E' };

	for (exp = 0; ABS(val) > 1000; exp++)
		val /= 1000;

	precision = (ABS(val) < 10) ? 2 : (ABS(val) < 100) ? 1 : 0;
	precision += (exp == 0);

	snprintf(str, 9, "%+.*f %c", precision, val, label[exp]);
	str[8] = '\0';
	if (val >= 0)
		str[0] = ' ';

	return exp * 3;
}

static void
xaxis(Canvas *can, Color *label, Color *grid,
	double vmin, double vmax, double vstep)
{
	double v;
	int x;
	char str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		x = v2x(v, vmin, vmax);

		ffdraw_line(can, grid,
			x, XLABEL_W,
			x, XLABEL_W + PLOT_W);

		humanize(str, v);
		ffdraw_str_right(can, label, str, font,
			x, XLABEL_W - MARGIN);
	}
}

static void
yaxis(Canvas *can, Color *label, Color *grid,
	time_t tmin, time_t tmax, time_t tstep)
{
	time_t t;
	int y;
	char str[sizeof("YYYY/MM/DD")], *fmt;

	fmt = (tstep < 3600 * 24) ? " %H:%M:%S " : " %Y/%m/%d ";

	for (t = tmax - tmax % tstep; t >= tmin; t -= tstep) {
		y = t2y(t, tmin, tmax);

		ffdraw_line(can, grid,
			YLABEL_H, y,
			YLABEL_H + PLOT_H, y);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ffdraw_str_center(can, label, str, font,
			YLABEL_H / 2, y);
	}
}

static void
title(Canvas *can,
	Color *ct, char *title,
	Color *cu, char *unit)
{
	ffdraw_str_left(can, ct, title, font,
		TITLE_H / 2, 0);
	ffdraw_str_right(can, cu, unit, font,
		TITLE_H / 2, TITLE_W);
}

static void
graph(Canvas *can, Vlist *v,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t *tp;
	double *vp;
	int x, y, n, xlast, ylast, first;

	first = 1;
	for (tp = v->t, vp = v->v, n = v->n; n-- > 0; n--, vp++, tp++) {
		x = v2x(*vp, vmin, vmax);
		y = t2y(*tp, tmin, tmax);

		if (!first)
			ffdraw_line(can, &v->col, xlast, ylast, x, y);

		xlast = x;
		ylast = y;
		first = 0;
	}
}

static void
plot(Canvas *can, Vlist *v, int n,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	for (; n > 0; n--, v++)
		graph(can, v, vmin, vmax, tmin, tmax);
}

static void
legend(Canvas *can, Color *label_fg, Vlist *v, int n)
{
	int i, x, y;

	for (i = 0; i < n; i++, v++) {
		x = LEGEND_H - n * (FONT_H + MARGIN) - FONT_H / 2;

		y = MARGIN + FONT_W;
		ffdraw_str_left(can, &v->col, "\1", font, x, y);

		y += FONT_W * 2;
		ffdraw_str_left(can, label_fg, v->label, font, x, y);
	}
}

void
ffdraw(char *name, char *units, Vlist *v, int n,
	double vmin, double vmax, double vstep,
	time_t tmin, time_t tmax, time_t tstep)
{
	Canvas can = { IMAGE_W, IMAGE_H, buffer, 0, 0 };
	Color plot_bg	= { 0x2222, 0x2222, 0x2222, 0xffff };
	Color grid_bg	= { 0x2929, 0x2929, 0x2929, 0xffff };
	Color grid_fg	= { 0x3737, 0x3737, 0x3737, 0xffff };
	Color label_fg	= { 0x8888, 0x8888, 0x8888, 0xffff };
	Color title_fg	= { 0xdddd, 0xdddd, 0xdddd, 0xffff };

	can.x = 0;
	can.y = 0;
	ffdraw_fill(&can, &plot_bg);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	ffdraw_rectangle(&can, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	can.x = YLABEL_X;
	can.y = YLABEL_Y;
	yaxis(&can, &label_fg, &grid_fg, tmin, tmax, tstep);

	can.x = XLABEL_X;
	can.y = XLABEL_Y;
	xaxis(&can, &label_fg, &grid_fg, vmin, vmax, vstep);

	can.x = TITLE_X;
	can.y = TITLE_Y;
	title(&can, &title_fg, name, &label_fg, units);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	plot(&can, v, n, vmin, vmax, tmin, tmax);

	can.x = LEGEND_X;
	can.y = LEGEND_Y;
	legend(&can, &label_fg, v, n);

	ffdraw_print(&can);
}
