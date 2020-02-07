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

#define XDENSITX	7		/* nb of values on x axis */
#define YDENSITX	7		/* nb of values on y axis */

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
#define LEGEND_Y	(XLABEL_H)
#define LEGEND_W	(150)
#define LEGEND_H	(PLOT_H)

struct color {
	uint16_t	red;
	uint16_t	green;
	uint16_t	blue;
	uint16_t	alpha;
};

struct vlist {
	struct color	color;		/* color to use to draw the line */
	time_t		*t;		/* array of timestamps */
	double		*v;		/* array of values */
	int		n;		/* number of values */
	char		*label;		/* for the legend */
};

struct canvas {
	int		w;		/* width */
	int		h;		/* height */
	int		x;		/* x offset */
	int		y;		/* y offset */
	struct color	*buf;
};

struct clist {
	char		*name;
	struct color	color;
};

char const		*arg0;
static char		*tflag	= "";
static char		*uflag	= "";
static struct font	*font = &font13;

struct clist clist[] = {
	/* name       red     green   blue    alpha */
	{ "red",    { 0xffff, 0x4444, 0x4444, 0xffff } },
	{ "orange", { 0xffff, 0x9999, 0x4444, 0xffff } },
	{ "yellow", { 0xffff, 0xffff, 0x4444, 0xffff } },
	{ "green",  { 0x2222, 0xffff, 0x5555, 0xffff } },
	{ "cyan",   { 0x0000, 0xffff, 0xdddd, 0xffff } },
	{ "blue",   { 0x2222, 0x9999, 0xffff, 0xffff } },
	{ NULL, { 0, 0, 0, 0 } }
};

static struct color *
name_to_color(char *name)
{
	for (struct clist *c = clist; c->name != NULL; c++)
		if (strcmp(name, c->name) == 0)
			return &c->color;
	return NULL;
}

static void
scale_minmax(struct vlist *v, int n,
	time_t *tmin, time_t *tmax,
	double *vmin, double *vmax)
{
	int		i;

	*vmin = *vmax = 0;
	*tmin = *tmax = *v->t;

	for (; n-- > 0; v++) {
		for (i = 0; i < v->n; i++) {
			if (v->v[i] < *vmin)
				*vmin = v->v[i];
			if (v->v[i] > *vmax)
				*vmax = v->v[i];
			if (v->t[i] < *tmin)
				*tmin = v->t[i];
			if (v->t[i] > *tmax)
				*tmax = v->t[i];
		}
	}
}

static void
scale_tstep(time_t *step, int density, time_t min, time_t max)
{
        time_t dt, *s, scale[] = {
		1, 5, 2, 10, 20, 30, 60, 60*2, 60*5, 60*10, 60*20, 60*30, 3600, 
		3600*2, 3600*5, 3600*10, 3600*18, 3600*24, 3600*24*2, 
		3600*24*5, 3600*24*10, 3600*24*20, 3600*24*30, 3600*24*50,
		3600*24*100, 3600*24*365, 0
	};

	dt = max - min;

	for (s = scale; s < scale + LEN(scale); s++) {
		if (dt < *s * density) {
			*step = *s;
			break;
		}
	}
}

static void
scale_vstep(double *step, int density, double min, double max)
{
	double		 dv, *s, scale[] = { 1, 2, 3, 5 };
	int		i;

	dv = max - min;

	if (dv > 1) {
		for (i = 1; i != 0; i *= 10) {
			for (s = scale; s < scale + LEN(scale); s++) {
				if (dv < *s * i * density) {
					*step = *s * i;
					return;
				}
			}
		}
	} else {
		for (i = 1; i != 0; i *= 10) {
			for (s = scale + LEN(scale) - 1; s >= scale; s--) {
				if (dv > *s / i * density / 2) {
					*step = *s / i;
					return;
				}
			}
		}
	}
}

static void
scale(struct vlist *v, int n,
	time_t *tmin, time_t *tmax, time_t *tstep,
	double *vmin, double *vmax, double *vstep)
{
	scale_minmax(v, n, tmin, tmax, vmin, vmax);
	scale_tstep(tstep, XDENSITX, *tmin, *tmax);
	scale_vstep(vstep, YDENSITX, *vmin, *vmax);
}

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
ff_pixel(struct canvas *can, struct color *col,
	int x, int y)
{
	x += can->x;
	y += can->y;
	if (x < 0 || x >= can->w || y < 0 || y >= can->h)
		return;
	memcpy(can->buf + can->w * (can->h - 1 - y) + x, col, sizeof(*can->buf));
}

static void
ff_rectangle(struct canvas *can, struct color *col,
	int y1, int x1,
	int y2, int x2)
{
	int		x, y, ymin, xmin, ymax, xmax;

	ymin = MIN(y1, y2); ymax = MAX(y1, y2);
	xmin = MIN(x1, x2); xmax = MAX(x1, x2);

	for (y = ymin; y <= ymax; y++)
		for (x = xmin; x <= xmax; x++)
			ff_pixel(can, col, x, y);
}

/*
 * From Bresenham's line algorithm and dcat's tplot.
 */
static void
ff_line(struct canvas *can, struct color *col,
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
		ff_pixel(can, col, x0, y0);

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
ff_char(struct canvas *can, struct color *col, char c,
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
				ff_pixel(can, col, x + xf, y + yf);
	return wf + 1;
}

/*
 * Draw a left aligned string without wrapping it.
 */
static size_t
ff_text_left(struct canvas *can, struct color *col, char *s,
	int x, int y)
{
	for (; *s != '\0'; s++)
		x += ff_char(can, col, *s, x, y);
	return x;
}

/*
 * Draw a center aligned string without wrapping it.
 */
static size_t
ff_text_center(struct canvas *can, struct color *col, char *s,
	int x, int y)
{
	x -= font_strlen(font, s) / 2;
	return ff_text_left(can, col, s, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
static size_t
ff_text_right(struct canvas *can, struct color *col, char *s,
	int x, int y)
{
	x -= font_strlen(font, s);
	return ff_text_left(can, col, s, x, y);
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
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
ff_v2y(double v, double vmin, double vmax)
{
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
ff_plot(struct canvas *can, struct vlist *v,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t		*tp;
	double		*vp;
	int		x, y, n, ylast, xlast, first;

	first = 1;
	for (tp = v->t, vp = v->v, n = v->n; n > 0; n--, vp++, tp++) {
		y = ff_v2y(*vp, vmin, vmax);
		x = ff_t2x(*tp, tmin, tmax);

		if (!first)
			ff_line(can, &v->color, xlast, ylast, x, y);

		ylast = y;
		xlast = x;
		first = 0;
	}
}

static void
ff_values(struct canvas *can, struct vlist *v, int n,
	time_t tmin, time_t tmax,
	double vmin, double vmax)
{
	for (; n > 0; n--, v++)
		ff_plot(can, v, vmin, vmax, tmin, tmax);
}

static void
ff_legend(struct canvas *can, struct color *label_fg, struct vlist *v, int n)
{
	int i, x, y;

	for (i = 0; i < n; i++, v++) {
		x = MARGIN;
		x = ff_text_left(can, &v->color, "\1", x, y);
		x = ff_text_left(can, label_fg, v->label, x, y);
		y = LEGEND_H - i * (font->height + MARGIN) - font->height / 2;
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
ff(struct vlist *v, int n, char *name, char *units)
{
	struct canvas	can = { IMAGE_W, IMAGE_H, 0, 0, NULL };
	struct color	plot_bg = { 0x2222, 0x2222, 0x2222, 0xffff };
	struct color	grid_bg = { 0x2929, 0x2929, 0x2929, 0xffff };
	struct color	grid_fg = { 0x3737, 0x3737, 0x3737, 0xffff };
	struct color	label_fg = { 0x8888, 0x8888, 0x8888, 0xffff };
	struct color	title_fg = { 0xdddd, 0xdddd, 0xdddd, 0xffff };
	double		vmin, vmax, vstep;
	time_t		tmin, tmax, tstep;

	scale(v, n, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);

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
	ff_values(&can, v, n, tmin, tmax, vmin, vmax);

	can.x = LEGEND_X;
	can.y = LEGEND_Y;
	ff_legend(&can, &label_fg, v, n);

	ff_print(&can);
}
 
static void
csv_labels(struct vlist *v, char **argv, char *buf)
{
	struct color	*color;

	if (esfgets(buf, LINE_MAX, stdin) == NULL)
		err(1, "missing label line");
 
	if (strcmp(strsep(&buf, ","), "epoch") != 0)
		err(1, "first label must be \"epoch\"");
 
	for (; *argv != NULL; v++, argv++) {
		if ((v->label = strsep(&buf, ",")) == NULL)
			err(1, "more arguments than columns");
		else if ((color = name_to_color(*argv)) == NULL)
			err(1, "unknown color: %s", *argv);
		v->color = *color;
	}
 
	if (strsep(&buf, ",") != NULL)
		err(1, "more columns than arguments");
}

static int
csv_addval(struct vlist *v, size_t sz, size_t nval, double field, time_t epoch)
{
	if (nval >= sz) {
		sz = sz * 2 + 1;
		if ((v->v = realloc(v->v, sz * sizeof(*v->v))) == NULL)
			err(1, "reallocating values buffer");
		if ((v->t = realloc(v->t, sz * sizeof(*v->t))) == NULL)
			err(1, "reallocating values buffer");
	}
	v->v[nval] = field;
	v->t[nval] = epoch;
	v->n = nval + 1;

	return sz;
}

/*
 * Add to each column the value on the current row.
 */
static int
csv_addrow(struct vlist *v, size_t sz, size_t ncol, size_t nval, char *line)
{
	time_t		epoch;
	int		bs;
	char		*field, *dot;

	if ((field = strsep(&line, ",")) == NULL)
		err(1, "%d: missing epoch", nval);

	if ((dot = strchr(field, '.')) != NULL)
		*dot = '\0';
	epoch = eatol(field);
	for (; (field = strsep(&line, ",")) != NULL; ncol--, v++) {
		if (ncol <= 0)
			err(1, "%d: too many fields", nval);
		bs = csv_addval(v, sz, nval, eatof(field), epoch);
	}
	if (ncol > 0)
		err(1, "%d: too few fields", ncol);

	return bs;
}

/*
 *       < ncol >
 * epoch,a1,b1,c1  ^
 * epoch,a2,b2,c2 nval
 * epoch,a3,b3,c3  v
 */
static void
csv_values(struct vlist *v, size_t ncol)
{
	int		nval, sz;
	char		line[LINE_MAX];

	sz = 0;
	for (nval = 0; esfgets(line, sizeof(line), stdin) != NULL; nval++)
		sz = csv_addrow(v, sz, ncol, nval, line);
	if (nval == 0)
		err(1, "no value could be read\n");
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-t title] [-u unit] {", arg0);
	fputs(clist->name, stderr);
	for (struct clist *c = clist + 1; c->name != NULL; c++)
		fprintf(stderr, ",%s", c->name);
	fputs("}...\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct vlist	*v;
	char		labels[LINE_MAX];

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

	fflush(stdout);

	if ((v = calloc(argc, sizeof(*v))) == NULL)
		err(1, "calloc value list");

	csv_labels(v, argv, labels);
	csv_values(v, argc);

	ff(v, argc, tflag, uflag);

	return 0;
}
