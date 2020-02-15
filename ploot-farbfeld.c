#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>

#include <math.h>

#include "arg.h"
#include "log.h"
#include "def.h"

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

struct color {
	uint16_t	red;
	uint16_t	green;
	uint16_t	blue;
	uint16_t	alpha;
};

struct cname {
	char		*name;
	struct color	color;
};

struct canvas {
	int		w;		/* width */
	int		h;		/* height */
	int		x;		/* x offset */
	int		y;		/* y offset */
	struct color	*buf;
};

char const		*arg0;
static char		*tflag	= "";
static char		*uflag	= "";
static struct font	*font = &font13;

static struct cname cname[] = {
	/* name       red     green   blue    alpha */
	{ "red",    { 0xffff, 0x4444, 0x4444, 0xffff } },
	{ "orange", { 0xffff, 0x9999, 0x4444, 0xffff } },
	{ "yellow", { 0xffff, 0xffff, 0x4444, 0xffff } },
	{ "green",  { 0x2222, 0xffff, 0x5555, 0xffff } },
	{ "cyan",   { 0x0000, 0xffff, 0xdddd, 0xffff } },
	{ "blue",   { 0x2222, 0x9999, 0xffff, 0xffff } },
	{ NULL, { 0, 0, 0, 0 } }
};

/*
 * Convert (x,y) coordinates to (row,col) for printing into the buffer.
 * The buffer only contain one number, so the coordinate is a single integer:
 *	width * y + y.
 * The coordinates are shifted by offx and offy to permit relative coordinates.
 *
 * The convention used:                                      y
 * - (0,0) is at the lower left corner of the canvas.        |
 * - (0,1) is above it.                                      +--x
 */
static void
ff_pixel(struct canvas *can, struct color *color,
	int x, int y)
{
	x += can->x;
	y += can->y;
	if (x < 0 || x >= can->w || y < 0 || y >= can->h)
		return;
	memcpy(can->buf + can->w * (can->h - 1 - y) + x, color, sizeof(*can->buf));
}

static void
ff_rectangle(struct canvas *can, struct color *color,
	int y1, int x1,
	int y2, int x2)
{
	int		x, y, ymin, xmin, ymax, xmax;

	ymin = MIN(y1, y2); ymax = MAX(y1, y2);
	xmin = MIN(x1, x2); xmax = MAX(x1, x2);

	for (y = ymin; y <= ymax; y++)
		for (x = xmin; x <= xmax; x++)
			ff_pixel(can, color, x, y);
}

/*
 * From Bresenham's line algorithm and dcat's tplot.
 */
static void
ff_line(struct canvas *can, struct color *color,
	int x0, int y0,
	int x1, int y1)
{
	int		dy, dx, sy, sx, err, e;

	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	err = (dy > dx ? dy : -dx) / 2;

	for (;;) {
		ff_pixel(can, color, x0, y0);

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
ff_char(struct canvas *can, struct color *color, char c,
	int x, int y)
{
	int		yf, xf, wf;

	if (c & 0x80)
		c = '\0';
	y -= font->height / 2;
	wf = font_width(font, c);
	for (xf = 0; xf < wf; xf++)
		for (yf = 0; yf < font->height; yf++)
			if (font->glyph[(int)c][wf * (font->height - yf) + xf] == 3)
				ff_pixel(can, color, x + xf, y + yf);
	return wf + 1;
}

/*
 * Draw a left aligned string without wrapping it.
 */
static size_t
ff_text_left(struct canvas *can, struct color *color, char *s,
	int x, int y)
{
	for (; *s != '\0'; s++)
		x += ff_char(can, color, *s, x, y);
	return x;
}

/*
 * Draw a center aligned string without wrapping it.
 */
static size_t
ff_text_center(struct canvas *can, struct color *color, char *s,
	int x, int y)
{
	x -= font_strlen(font, s) / 2;
	return ff_text_left(can, color, s, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
static size_t
ff_text_right(struct canvas *can, struct color *color, char *s,
	int x, int y)
{
	x -= font_strlen(font, s);
	return ff_text_left(can, color, s, x, y);
}

static void
ff_print(struct canvas *can)
{
	uint32_t		w, h;

	w = htonl(can->w);
	h = htonl(can->h);

	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	fwrite(can->buf, can->w * can->h, sizeof(*can->buf), stdout);
}

static int
ff_t2x(time_t t, time_t tmin, time_t tmax)
{
	if (tmin == tmax)
		return PLOT_W;
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
ff_v2y(double v, double vmin, double vmax)
{
	if (vmin == vmax)
		return PLOT_H;
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

static void
ff_xaxis(struct canvas *can, struct color *label, struct color *grid,
	time_t tmin, time_t tmax, time_t tstep)
{
	time_t		t;
	int		x;
	char		str[sizeof("MM/DD HH/MM")], *fmt;

	if (tstep < 3600 * 12)
		fmt = "%H:%M:%S";
	else if (tstep < 3600 * 24)
		fmt = "%m/%d %H:%M";
	else
		fmt = "%X/%m/%d";

	for (t = tmax - tmax % tstep; t >= tmin; t -= tstep) {
		x = ff_t2x(t, tmin, tmax);

		ff_line(can, grid,
			x, XLABEL_H,
			x, XLABEL_H + PLOT_H);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ff_text_center(can, label, str,
			x, XLABEL_H / 2);
	}
}

static void
ff_yaxis(struct canvas *can, struct color *label, struct color *grid,
	double vmin, double vmax, double vstep)
{
	double		v;
	int		y;
	char		str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		y = ff_v2y(v, vmin, vmax);

		ff_line(can, grid,
			YLABEL_W, y,
			YLABEL_W + PLOT_W, y);

		humanize(str, v);
		ff_text_right(can, label, str,
			YLABEL_W - MARGIN, y);
	}
}

static void
ff_title(struct canvas *can,
	struct color *ct, char *title,
	struct color *cu, char *unit)
{
	ff_text_left(can, ct, title, TITLE_H / 2, 0);
	ff_text_right(can, cu, unit, TITLE_H / 2, TITLE_W);
}

static void
ff_plot(struct canvas *can, struct vlist *vl, struct color *color,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t		*tp;
	double		*vp;
	int		x, y, n, ylast, xlast, first;

	first = 1;
	for (tp = vl->t, vp = vl->v, n = vl->n; n > 0; n--, vp++, tp++) {
		y = ff_v2y(*vp, vmin, vmax);
		x = ff_t2x(*tp, tmin, tmax);

		if (!first)
			ff_line(can, color, xlast, ylast, x, y);

		ylast = y;
		xlast = x;
		first = 0;
	}
}

static void
ff_values(struct canvas *can, struct vlist *vl, struct color **cl, size_t ncol,
	time_t tmin, time_t tmax,
	double vmin, double vmax)
{
	for (; ncol > 0; ncol--, vl++, cl++)
		ff_plot(can, vl, *cl, vmin, vmax, tmin, tmax);
}

static void
ff_legend(struct canvas *can, struct color *fg, struct vlist *vl, struct color **cl, size_t ncol)
{
	size_t		x, y;

	for (; ncol > 0; ncol--, vl++, cl++) {
		y = -(ncol - 1) * (font->height + MARGIN);
		x = MARGIN * 2;
		x = ff_text_left(can, *cl, "-", x, y) + MARGIN;
		x = ff_text_left(can, fg, vl->label, x, y);
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
ff(struct vlist *vl, struct color **cl, size_t ncol, char *name, char *units)
{
	struct canvas	can = { IMAGE_W, IMAGE_H, 0, 0, NULL };
	struct color	plot_bg = { 0x2222, 0x2222, 0x2222, 0xffff };
	struct color	grid_bg = { 0x2929, 0x2929, 0x2929, 0xffff };
	struct color	grid_fg = { 0x3737, 0x3737, 0x3737, 0xffff };
	struct color	label_fg = { 0x8888, 0x8888, 0x8888, 0xffff };
	struct color	title_fg = { 0xdddd, 0xdddd, 0xdddd, 0xffff };
	double		vmin, vmax, vstep;
	time_t		tmin, tmax, tstep;

	scale(vl, ncol, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);

	assert(can.buf = calloc(IMAGE_H * IMAGE_W, sizeof *can.buf));

	can.y = 0;
	can.x = 0;
	ff_rectangle(&can, &plot_bg, 0, 0, IMAGE_H - 1, IMAGE_W - 1);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	ff_rectangle(&can, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	can.x = XLABEL_X;
	can.y = XLABEL_Y;
	ff_xaxis(&can, &label_fg, &grid_fg, tmin, tmax, tstep);

	can.x = YLABEL_X;
	can.y = YLABEL_Y;
	ff_yaxis(&can, &label_fg, &grid_fg, vmin, vmax, vstep);

	can.x = TITLE_X;
	can.y = TITLE_Y;
	ff_title(&can, &title_fg, name, &label_fg, units);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	ff_values(&can, vl, cl, ncol, tmin, tmax, vmin, vmax);

	can.x = LEGEND_X;
	can.y = LEGEND_Y;
	ff_legend(&can, &label_fg, vl, cl, ncol);

	ff_print(&can);
}

static struct color *
name_to_color(char *name)
{
	struct cname	*cn;

	for (cn = cname; cn->name != NULL; cn++)
		if (strcmp(name, cn->name) == 0)
			return &cn->color;
	return NULL;
}

static void
argv_to_color(struct color **cl, char **argv)
{
	for (; *argv != NULL; cl++, argv++)
		if ((*cl = name_to_color(*argv)) == NULL)
			err(1, "unknown color name: %s", *argv);
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-t title] [-u unit] {", arg0);
	fputs(cname->name, stderr);
	for (struct cname *cn = cname + 1; cn->name != NULL; cn++)
		fprintf(stderr, ",%s", cn->name);
	fputs("}...\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct vlist	*vl;
	struct color	**cl;
	char		labels[LINE_MAX];
	size_t		ncol;

	ARG_SWITCH(argc, argv) {
	case 't':
		tflag = ARG;
		break;
	case 'u':
		uflag = ARG;
		break;
	default:
		usage();
	}

	if (argc == 0)
		usage();

	assert(cl = calloc(argc, sizeof(*cl)));

	csv_labels(stdin, labels, &vl, &ncol);
	if (ncol > (size_t)argc)
		err(1, "too many columns or not enough arguments");
	else if (ncol < (size_t)argc)
		err(1, "too many arguments or not enough columns");
	csv_values(stdin, vl, ncol);
	argv_to_color(cl, argv);

	ff(vl, cl, argc, tflag, uflag);

	free(vl);
	free(cl);
	return 0;
}
