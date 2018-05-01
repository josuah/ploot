#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

#include "ffdraw.h"
#include "font_14x7.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define LEN(x) (sizeof(x) / sizeof(*x))

/*
 * Sizes and positions:
 *
 *	 Title on the first row    legend
 *	     y ^                    here
 *	 label |- + - + - + - + -   with
 *	  here |- + - + - + - + -   the
 *	       +--+---+---+---+-->  full
 *	        x label here        height
 */

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

int
ffplot_t2y(time_t t, time_t tmin, time_t tmax)
{
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

int
ffplot_v2x(double v, double vmin, double vmax)
{
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

/*
 * Set 'str' to a human-readable form of 'num' with always a width of 8 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.  Return the exponent.
 */
int
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

void
ffplot_xaxis(Canvas *can, Color *label, Color *grid,
	double vmin, double vmax, double vstep)
{
	double v;
	int x;
	char str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		x = ffplot_v2x(v, vmin, vmax);

		ffdraw_line(can, grid,
			x, XLABEL_W,
			x, XLABEL_W + PLOT_W);

		humanize(str, v);
		ffdraw_str_right(can, label, str, font,
			x, XLABEL_W - MARGIN);
	}
}

void
ffplot_yaxis(Canvas *can, Color *label, Color *grid,
	time_t tmin, time_t tmax, time_t tstep)
{
	time_t t;
	int y;
	char str[sizeof("YYYY/MM/DD")], *fmt;

	fmt = (tstep < 3600 * 24) ? " %H:%M:%S " : " %Y/%m/%d ";

	for (t = tmax - tmax % tstep; t >= tmin; t -= tstep) {
		y = ffplot_t2y(t, tmin, tmax);

		ffdraw_line(can, grid,
			YLABEL_H, y,
			YLABEL_H + PLOT_H, y);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ffdraw_str_center(can, label, str, font,
			YLABEL_H / 2, y);
	}
}

void
ffplot_title(Canvas *can,
	Color *ct, char *title,
	Color *cu, char *unit)
{
	ffdraw_str_left(can, ct, title, font,
		TITLE_H / 2, 0);
	ffdraw_str_right(can, cu, unit, font,
		TITLE_H / 2, TITLE_W);
}

void
ffplot_graph(Canvas *can, Vlist *v,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t t;
	double *vp;
	int x, y, n, xlast, ylast, first;

	first = 1;
	t = tmin;
	for (vp = v->v, n = v->n; n-- > 0; n--, vp++) {
		x = ffplot_v2x(*vp, vmin, vmax);
		y = ffplot_t2y(t, tmin, tmax);

		if (!first)
			ffdraw_line(can, &v->col, xlast, ylast, x, y);

		xlast = x;
		ylast = y;
		t += v->step;
		first = 0;
	}
}

void
ffplot_plot(Canvas *can, Vlist **v,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	for (; *v != NULL; v++)
		ffplot_graph(can, *v, vmin, vmax, tmin, tmax);
}

void
ffplot_legend(Canvas *can, Color *label_fg, Vlist **v)
{
	int n, x, y;

	for (n = 0; *v != NULL; v++, n++) {
		x = LEGEND_H - n * (FONT_H + MARGIN) - FONT_H / 2;

		y = MARGIN + FONT_W;
		ffdraw_str_left(can, &(*v)->col, "\1", font, x, y);

		y += FONT_W * 2;
		ffdraw_str_left(can, label_fg, (*v)->name, font, x, y);
	}
}

void
ffdraw(Canvas *can, char *title, char *units, Vlist **vlistv,
	double vmin, double vmax, double vstep,
	time_t tmin, time_t tmax, time_t tstep)
{
	Color plot_bg	= { 0x2222, 0x2222, 0x2222, 0xffff };
	Color grid_bg	= { 0x2929, 0x2929, 0x2929, 0xffff };
	Color grid_fg	= { 0x3737, 0x3737, 0x3737, 0xffff };
	Color label_fg	= { 0x8888, 0x8888, 0x8888, 0xffff };
	Color title_fg	= { 0xdddd, 0xdddd, 0xdddd, 0xffff };

	can->x = 0;
	can->y = 0;
	ffdraw_fill(can, &plot_bg);

	can->x = PLOT_X;
	can->y = PLOT_Y;
	ffdraw_rectangle(can, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	can->x = YLABEL_X;
	can->y = YLABEL_Y;
	ffplot_yaxis(can, &label_fg, &grid_fg, tmin, tmax, tstep);

	can->x = XLABEL_X;
	can->y = XLABEL_Y;
	ffplot_xaxis(can, &label_fg, &grid_fg, vmin, vmax, vstep);

	can->x = TITLE_X;
	can->y = TITLE_Y;
	ffplot_title(can, &title_fg, title, &label_fg, units);

	can->x = PLOT_X;
	can->y = PLOT_Y;
	ffplot_plot(can, vlistv, vmin, vmax, tmin, tmax);

	can->x = LEGEND_X;
	can->y = LEGEND_Y;
	ffplot_legend(can, &label_fg, vlistv);
}

int
main(void)
{
	Canvas can = { IMAGE_W, IMAGE_H, buffer, 0, 0 };
	double v1[] = { 0.1, 30, -3, 42, 559, 343, 10, 345, 0 };
	double v2[] = { 30, -3, 42, 559, 343, 10, 345, 0, 0.3 };
        double v3[] = { 0, 0.3, 30, -3, 42, 5, 43, 345, 0, 10,
                0.3, 30, -3, 42, 59, 33, 35, 0, 40, 0.3, 30,
                0.3, 30, -3, 42, 55, 3, 5, 0, 100, 0.3, 30,
                95, 43, 45, 0, 40, 0.3, 30, 0.3, 30, -3, 42,
                0.3, 30, -3, 42, 5, 43, 3, 0, 100, 0.3, 30,
                0.3, 30, -3, 42, 59, 43, 45, 0, 4, 0.3, 30,
                -3, 42, 559, 343, 45, 0, 10 };
	Vlist vl1 = { { 0x0000, 0xffff, 0xdddd, 0xffff }, NULL, LEN(v1), 500, "available ravens" };
	Vlist vl2 = { { 0xffff, 0x9999, 0x4444, 0xffff }, NULL, LEN(v2), 500, "used ravens" };
	Vlist vl3 = { { 0x1111, 0xffff, 0x5555, 0xffff }, NULL, LEN(v3), 57,  "free ravens" };
	Vlist *vlistv[] = { NULL, NULL, NULL, NULL };
	double vmin, vmax, vstep;
	time_t tmin, tmax, tstep;
	uint32_t w, h;

	vlistv[0] = &vl1; vl1.v = v1;
	vlistv[1] = &vl2; vl2.v = v2;
	vlistv[2] = &vl3; vl3.v = v3;
	vlistv[3] = NULL;

	vmin = -30; vmax =  700; vstep = 120;
	tmin =   0; tmax = 2000; tstep = 300;

	ffdraw(&can, "Council of the ravens", "(feather per second)", vlistv,
		vmin, vmax, vstep,
		tmin, tmax, tstep);

	w = htonl(IMAGE_W);
	h = htonl(IMAGE_H);

	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	fwrite(can.b, IMAGE_W * IMAGE_H, sizeof(*can.b), stdout);

	return 0;
}
