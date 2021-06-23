#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "csv.h"
#include "font.h"
#include "scale.h"
#include "util.h"

#ifndef __OpenBSD__
#define pledge(...) 0
#endif

#define MARGIN		4

#define IMAGE_H		(TITLE_H + PLOT_H + XLABEL_H)
#define IMAGE_W		(YLABEL_W + PLOT_W + LEGEND_W)

#define TITLE_X		(YLABEL_W)
#define TITLE_Y		(IMAGE_H - TITLE_H)
#define TITLE_H		((font)->height * 2)
#define TITLE_W		(PLOT_W)

#define YLABEL_X	(0)
#define YLABEL_Y	(PLOT_Y)
#define YLABEL_H	(PLOT_H)
#define YLABEL_W	(40 + MARGIN)

#define XLABEL_X	(PLOT_X)
#define XLABEL_Y	(0)
#define XLABEL_H	((font)->height * 2)
#define XLABEL_W	(PLOT_W)

#define PLOT_X		(YLABEL_W)
#define PLOT_Y		(XLABEL_H)
#define PLOT_W		(700)
#define PLOT_H		(160)

#define LEGEND_X	(IMAGE_W - LEGEND_W)
#define LEGEND_Y	(TITLE_H + PLOT_H - (font)->height)
#define LEGEND_W	(100)
#define LEGEND_H	(PLOT_H)

struct ffcolor {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t alpha;
};

struct ffplot {
	int w, h, x, y; /* width, height and coordinamtes */
	struct ffcolor *buf;
};

static struct colorname {
	char *name;
	struct ffcolor color;
} colorname[] = {
	/* name       red     green   blue    alpha */
	{ "red",    { 0xffff, 0x4444, 0x4444, 0xffff } },
	{ "orange", { 0xffff, 0x9999, 0x4444, 0xffff } },
	{ "yellow", { 0xffff, 0xffff, 0x4444, 0xffff } },
	{ "green",  { 0x2222, 0xffff, 0x5555, 0xffff } },
	{ "cyan",   { 0x0000, 0xffff, 0xdddd, 0xffff } },
	{ "blue",   { 0x2222, 0x9999, 0xffff, 0xffff } },
	{ NULL, { 0, 0, 0, 0 } }
};

static char *tflag = "";
static char *uflag = "";
static struct font *font = &font13;

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
static void
ffplot_pixel(struct ffplot *plot, struct ffcolor *color,
	int x, int y)
{
	x += plot->x;
	y += plot->y;
	if (x < 0 || x >= plot->w || y < 0 || y >= plot->h)
		return;
	memcpy(plot->buf + plot->w * (plot->h - 1 - y) + x, color, sizeof(*plot->buf));
}

static void
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
static void
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
static int
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
static size_t
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
static size_t
ffplot_text_center(struct ffplot *plot, struct ffcolor *color, struct font *ft,
	char *s, int x, int y)
{
	x -= font_strlen(ft, s) / 2;
	return ffplot_text_left(plot, color, ft, s, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
static size_t
ffplot_text_right(struct ffplot *plot, struct ffcolor *color, struct font *ft,
	char *s, int x, int y)
{
	x -= font_strlen(ft, s);
	return ffplot_text_left(plot, color, ft, s, x, y);
}

static void
ffplot_print(FILE *fp, struct ffplot *plot)
{
	uint32_t w, h;

	w = htonl(plot->w);
	h = htonl(plot->h);

	fprintf(stdout, "ffplot");
	fwrite(&w, sizeof(w), 1, fp);
	fwrite(&h, sizeof(h), 1, fp);
	fwrite(plot->buf, plot->w * plot->h, sizeof(*plot->buf), fp);
}

static int
ffplot_t2x(time_t t, time_t tmin, time_t tmax)
{
	if (tmin == tmax)
		return PLOT_W;
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
ffplot_v2y(double v, double vmin, double vmax)
{
	if (vmin == vmax)
		return PLOT_H;
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

static void
ffplot_xaxis(struct ffplot *plot, struct ffcolor *label, struct ffcolor *grid,
	time_t tmin, time_t tmax, time_t csvep)
{
	time_t t;
	int x;
	char str[sizeof("MM/DD HH/MM")], *fmt;

	if (csvep < 3600 * 12)
		fmt = "%H:%M:%S";
	else if (csvep < 3600 * 24)
		fmt = "%m/%d %H:%M";
	else
		fmt = "%X/%m/%d";

	for (t = tmax - tmax % csvep; t >= tmin; t -= csvep) {
		x = ffplot_t2x(t, tmin, tmax);

		ffplot_line(plot, grid,
			x, XLABEL_H,
			x, XLABEL_H + PLOT_H);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ffplot_text_center(plot, label, font, str,
			x, XLABEL_H / 2);
	}
}

static void
ffplot_yaxis(struct ffplot *plot, struct ffcolor *label, struct ffcolor *grid,
	double vmin, double vmax, double vstep)
{
	double v;
	int y;
	char str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		y = ffplot_v2y(v, vmin, vmax);

		ffplot_line(plot, grid,
			YLABEL_W, y,
			YLABEL_W + PLOT_W, y);

		humanize(str, v);
		ffplot_text_right(plot, label, font, str,
			YLABEL_W - MARGIN, y);
	}
}

static void
ffplot_title(struct ffplot *plot,
	struct ffcolor *ct, char *title,
	struct ffcolor *cu, char *unit)
{
	ffplot_text_left(plot, ct, font, title, TITLE_H / 2, 0);
	ffplot_text_right(plot, cu, font, unit, TITLE_H / 2, TITLE_W);
}

static void
ffplot_plot(struct ffplot *plot, struct csv *vl, struct ffcolor *color,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t *tp;
	double *vp;
	int x, y, n, ylast, xlast, first;

	first = 1;
	for (tp = vl->t, vp = vl->v, n = vl->n; n > 0; n--, vp++, tp++) {
		y = ffplot_v2y(*vp, vmin, vmax);
		x = ffplot_t2x(*tp, tmin, tmax);

		if (!first)
			ffplot_line(plot, color, xlast, ylast, x, y);

		ylast = y;
		xlast = x;
		first = 0;
	}
}

static void
ffplot_values(struct ffplot *plot, struct csv *vl, struct ffcolor **cl, size_t ncol,
	time_t tmin, time_t tmax,
	double vmin, double vmax)
{
	for (; ncol > 0; ncol--, vl++, cl++)
		ffplot_plot(plot, vl, *cl, vmin, vmax, tmin, tmax);
}

static void
ffplot_legend(struct ffplot *plot, struct ffcolor *fg, struct csv *vl, struct ffcolor **cl, size_t ncol)
{
	size_t x, y;

	for (; ncol > 0; ncol--, vl++, cl++) {
		y = -(ncol - 1) * (font->height + MARGIN);
		x = MARGIN * 2;
		x = ffplot_text_left(plot, *cl, font, "-", x, y) + MARGIN;
		x = ffplot_text_left(plot, fg, font, vl->label, x, y);
	}
}

/*
 * Plot the 'n' values list of the 'v' arrax with title 'name' and
 * 'units' label.
 *
 *	       Title       (units)
 *	     x ^                    Legend
 *	 label | - + - + - + - + -   ....
 *	  here | - + - + - + - + -   ....
 *	       +---+---+---+---+-->
 *	        x label here        
 */
static void
plot(struct csv *vl, struct ffcolor **cl, size_t ncol, char *name, char *units)
{
	struct ffplot plot = { IMAGE_W, IMAGE_H, 0, 0, NULL };
	struct ffcolor plot_bg = { 0x2222, 0x2222, 0x2222, 0xffff };
	struct ffcolor grid_bg = { 0x2929, 0x2929, 0x2929, 0xffff };
	struct ffcolor grid_fg = { 0x3737, 0x3737, 0x3737, 0xffff };
	struct ffcolor label_fg = { 0x8888, 0x8888, 0x8888, 0xffff };
	struct ffcolor title_fg = { 0xdddd, 0xdddd, 0xdddd, 0xffff };
	double vmin, vmax, vstep;
	time_t tmin, tmax, csvep;

	scale_minmax(vl, ncol, &tmin, &tmax, &vmin, &vmax);
	csvep = scale_csvep(tmin, tmax, 7);
	vstep = scale_vstep(vmin, vmax, 7);

	if ((plot.buf = calloc(IMAGE_H * IMAGE_W, sizeof *plot.buf)) == NULL)
		err(1, "calloc: %s", strerror(errno));

	plot.y = 0;
	plot.x = 0;
	ffplot_rectangle(&plot, &plot_bg, 0, 0, IMAGE_H - 1, IMAGE_W - 1);

	plot.x = PLOT_X;
	plot.y = PLOT_Y;
	ffplot_rectangle(&plot, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	plot.x = XLABEL_X;
	plot.y = XLABEL_Y;
	ffplot_xaxis(&plot, &label_fg, &grid_fg, tmin, tmax, csvep);

	plot.x = YLABEL_X;
	plot.y = YLABEL_Y;
	ffplot_yaxis(&plot, &label_fg, &grid_fg, vmin, vmax, vstep);

	plot.x = TITLE_X;
	plot.y = TITLE_Y;
	ffplot_title(&plot, &title_fg, name, &label_fg, units);

	plot.x = PLOT_X;
	plot.y = PLOT_Y;
	ffplot_values(&plot, vl, cl, ncol, tmin, tmax, vmin, vmax);

	plot.x = LEGEND_X;
	plot.y = LEGEND_Y;
	ffplot_legend(&plot, &label_fg, vl, cl, ncol);

	ffplot_print(stdout, &plot);
}

static struct ffcolor *
name_to_color(char *name)
{
	struct colorname *cn;

	for (cn = colorname; cn->name != NULL; cn++)
		if (strcmp(name, cn->name) == 0)
			return &cn->color;
	return NULL;
}

static void
argv_to_color(struct ffcolor **cl, char **argv)
{
	for (; *argv != NULL; cl++, argv++)
		if ((*cl = name_to_color(*argv)) == NULL)
			err(1, "unknown color name: %s", *argv);
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-t title] [-u unit] {", arg0);
	fputs(colorname->name, stderr);
	for (struct colorname *cn = colorname + 1; cn->name != NULL; cn++)
		fprintf(stderr, ",%s", cn->name);
	fputs("}...\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct csv *vl;
	struct ffcolor **cl;
	size_t ncol;
	int c;

	if (pledge("stdio", "") < 0)
		err(1, "pledge: %s", strerror(errno));

	arg0 = *argv;
	while ((c = getopt(argc, argv, "t:u:")) > -1) {
		switch (c) {
		case 't':
			tflag = optarg;
			break;
		case 'u':
			uflag = optarg;
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	if ((cl = calloc(argc, sizeof *cl)) == NULL)
		err(1, "calloc: %s", strerror(errno));

	csv_labels(stdin, &vl, &ncol);
	if (ncol > (size_t)argc)
		err(1, "too many columns or not enough arguments");
	else if (ncol < (size_t)argc)
		err(1, "too many arguments or not enough columns");
	csv_values(stdin, vl, ncol);
	argv_to_color(cl, argv);

	plot(vl, cl, argc, tflag, uflag);

	free(vl);
	free(cl);
	return 0;
}
