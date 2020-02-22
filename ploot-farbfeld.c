#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "csv.h"
#include "ffplot.h"
#include "font.h"
#include "log.h"
#include "tool.h"
#include "scale.h"

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

struct colorname {
	char *name;
	struct ffcolor color;
};

char const *arg0 = NULL;
static char *tflag = "";
static char *uflag = "";
static struct font *font = &font13;

static struct colorname colorname[] = {
	/* name       red     green   blue    alpha */
	{ "red",    { 0xffff, 0x4444, 0x4444, 0xffff } },
	{ "orange", { 0xffff, 0x9999, 0x4444, 0xffff } },
	{ "yellow", { 0xffff, 0xffff, 0x4444, 0xffff } },
	{ "green",  { 0x2222, 0xffff, 0x5555, 0xffff } },
	{ "cyan",   { 0x0000, 0xffff, 0xdddd, 0xffff } },
	{ "blue",   { 0x2222, 0x9999, 0xffff, 0xffff } },
	{ NULL, { 0, 0, 0, 0 } }
};

static int
farbfeld_t2x(time_t t, time_t tmin, time_t tmax)
{
	if (tmin == tmax)
		return PLOT_W;
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
farbfeld_v2y(double v, double vmin, double vmax)
{
	if (vmin == vmax)
		return PLOT_H;
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

static void
farbfeld_xaxis(struct ffplot *plot, struct ffcolor *label, struct ffcolor *grid,
	time_t tmin, time_t tmax, time_t tstep)
{
	time_t t;
	int x;
	char str[sizeof("MM/DD HH/MM")], *fmt;

	if (tstep < 3600 * 12)
		fmt = "%H:%M:%S";
	else if (tstep < 3600 * 24)
		fmt = "%m/%d %H:%M";
	else
		fmt = "%X/%m/%d";

	for (t = tmax - tmax % tstep; t >= tmin; t -= tstep) {
		x = farbfeld_t2x(t, tmin, tmax);

		ffplot_line(plot, grid,
			x, XLABEL_H,
			x, XLABEL_H + PLOT_H);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ffplot_text_center(plot, label, font, str,
			x, XLABEL_H / 2);
	}
}

static void
farbfeld_yaxis(struct ffplot *plot, struct ffcolor *label, struct ffcolor *grid,
	double vmin, double vmax, double vstep)
{
	double v;
	int y;
	char str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		y = farbfeld_v2y(v, vmin, vmax);

		ffplot_line(plot, grid,
			YLABEL_W, y,
			YLABEL_W + PLOT_W, y);

		humanize(str, v);
		ffplot_text_right(plot, label, font, str,
			YLABEL_W - MARGIN, y);
	}
}

static void
farbfeld_title(struct ffplot *plot,
	struct ffcolor *ct, char *title,
	struct ffcolor *cu, char *unit)
{
	ffplot_text_left(plot, ct, font, title, TITLE_H / 2, 0);
	ffplot_text_right(plot, cu, font, unit, TITLE_H / 2, TITLE_W);
}

static void
farbfeld_plot(struct ffplot *plot, struct csv *vl, struct ffcolor *color,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t *tp;
	double *vp;
	int x, y, n, ylast, xlast, first;

	first = 1;
	for (tp = vl->t, vp = vl->v, n = vl->n; n > 0; n--, vp++, tp++) {
		y = farbfeld_v2y(*vp, vmin, vmax);
		x = farbfeld_t2x(*tp, tmin, tmax);

		if (!first)
			ffplot_line(plot, color, xlast, ylast, x, y);

		ylast = y;
		xlast = x;
		first = 0;
	}
}

static void
farbfeld_values(struct ffplot *plot, struct csv *vl, struct ffcolor **cl, size_t ncol,
	time_t tmin, time_t tmax,
	double vmin, double vmax)
{
	for (; ncol > 0; ncol--, vl++, cl++)
		farbfeld_plot(plot, vl, *cl, vmin, vmax, tmin, tmax);
}

static void
farbfeld_legend(struct ffplot *plot, struct ffcolor *fg, struct csv *vl, struct ffcolor **cl, size_t ncol)
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
	time_t tmin, tmax, tstep;

	scale(vl, ncol, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);

	assert(plot.buf = calloc(IMAGE_H * IMAGE_W, sizeof *plot.buf));

	plot.y = 0;
	plot.x = 0;
	ffplot_rectangle(&plot, &plot_bg, 0, 0, IMAGE_H - 1, IMAGE_W - 1);

	plot.x = PLOT_X;
	plot.y = PLOT_Y;
	ffplot_rectangle(&plot, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	plot.x = XLABEL_X;
	plot.y = XLABEL_Y;
	farbfeld_xaxis(&plot, &label_fg, &grid_fg, tmin, tmax, tstep);

	plot.x = YLABEL_X;
	plot.y = YLABEL_Y;
	farbfeld_yaxis(&plot, &label_fg, &grid_fg, vmin, vmax, vstep);

	plot.x = TITLE_X;
	plot.y = TITLE_Y;
	farbfeld_title(&plot, &title_fg, name, &label_fg, units);

	plot.x = PLOT_X;
	plot.y = PLOT_Y;
	farbfeld_values(&plot, vl, cl, ncol, tmin, tmax, vmin, vmax);

	plot.x = LEGEND_X;
	plot.y = LEGEND_Y;
	farbfeld_legend(&plot, &label_fg, vl, cl, ncol);

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
			fatal(1, "unknown color name: %s", *argv);
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

	optind = 0;
	while ((c = getopt(argc, argv, "")) > -1) {
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

	assert(cl = calloc(argc, sizeof(*cl)));

	csv_labels(stdin, &vl, &ncol);
	if (ncol > (size_t)argc)
		fatal(1, "too many columns or not enough arguments");
	else if (ncol < (size_t)argc)
		fatal(1, "too many arguments or not enough columns");
	csv_values(stdin, vl, ncol);
	argv_to_color(cl, argv);

	plot(vl, cl, argc, tflag, uflag);

	free(vl);
	free(cl);
	return 0;
}
