#include <arpa/inet.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#include "arg.h"
#include "util.h"
#include "font.h"

#define MARGIN 4

#define XDENSITY	7	/* nb of values on x axis */
#define YDENSITY	7	/* nb of values on y axis */

#define TITLE_X		(IMAGE_H - TITLE_H)
#define TITLE_Y		(XLABEL_W)
#define TITLE_H		(FONT_H * 2)
#define TITLE_W		(PLOT_W)

#define XLABEL_X	(PLOT_X)
#define XLABEL_Y	(0)
#define XLABEL_H	(PLOT_H)
#define XLABEL_W	(FONT_W * 9 + MARGIN)

#define YLABEL_X	(0)
#define YLABEL_Y	(PLOT_Y)
#define YLABEL_H	(FONT_H * 2)
#define YLABEL_W	(PLOT_W)

#define PLOT_X		(YLABEL_H)
#define PLOT_Y		(XLABEL_W)
#define PLOT_W		700
#define PLOT_H		160

#define LEGEND_X	(YLABEL_H)
#define LEGEND_Y	(IMAGE_W - LEGEND_W)
#define LEGEND_W	(FONT_W + 150 + FONT_W)
#define LEGEND_H	(PLOT_H)

#define IMAGE_H		(TITLE_H + PLOT_H + YLABEL_H)
#define IMAGE_W		(XLABEL_W + PLOT_W + LEGEND_W)

typedef uint16_t	Color[4];
typedef struct clist	Clist;
typedef struct vlist	Vlist;
typedef struct canvas	Canvas;
typedef struct font	Font;

struct vlist {
	Color col;	/* color to use to draw the line */
	time_t *t;	/* array of timestamps */
	double *v;	/* array of values */
	int n;		/* number of values */
	char *label;	/* for the legend */
};

struct canvas {
	int w;		/* width */
	int h;		/* height */
	int x;		/* x offset */
	int y;		/* x offset */
	Color b[IMAGE_W * IMAGE_H];
};

struct font {
	int w;		/* width */
	int h;		/* height */
	char **b;	/* buffer */
};

struct clist {
	char *name;
	Color col;
};

char *argv0;
char *tflag	= "";
char *uflag	= "";

Clist clist[] = {
	/* name       red     green   blue    alpha */
	{ "red",    { 0xffff, 0x4444, 0x4444, 0xffff } },
	{ "orange", { 0xffff, 0x9999, 0x4444, 0xffff } },
	{ "yellow", { 0xffff, 0xffff, 0x4444, 0xffff } },
	{ "green",  { 0x2222, 0xffff, 0x5555, 0xffff } },
	{ "cyan",   { 0x0000, 0xffff, 0xdddd, 0xffff } },
	{ "blue",   { 0x2222, 0x9999, 0xffff, 0xffff } },
	{ NULL, { 0, 0, 0, 0 } }
};

Font font = { FONT_W, FONT_H, glyph };

static int
color(Color *col, char *name)
{
	Clist *c;
 
	for (c = clist; c->name != NULL; c++) {
		if (strcmp(name, c->name) == 0) {
			memcpy(col, c->col, sizeof(*col));
			return 0;
		}
	}
 
	return -1;
}

static void
scale_minmax(Vlist *v, int n,
	double *vmin, double *vmax,
	time_t *tmin, time_t *tmax)
{
	int i;

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
		3600*24*100, 3600*24*365
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
	double dv, *s, scale[] = { 1, 2, 3, 5 };
	int i;

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
scale(Vlist *v, int n,
	double *vmin, double *vmax, double *vstep,
	time_t *tmin, time_t *tmax, time_t *tstep)
{
	scale_minmax(v, n, vmin, vmax, tmin, tmax);
	scale_tstep(tstep, YDENSITY, *tmin, *tmax);
	scale_vstep(vstep, XDENSITY, *vmin, *vmax);
}

/*
 * Convert (x,y) coordinates to (row,col) for printing into the buffer.
 * The buffer only contain one number, so the coordinate is a single integer:
 *	width * x + y.
 * The coordinates are shifted by offx and offy to permit relative coordinates.
 *
 * The convention used:                                      y
 * - (0,0) is at the lower left corner of the canvas.        |
 * - (0,1) is above it.                                      +--x
 */
static void
ff_pixel(Canvas *can, Color *col,
	int x, int y)
{
	x += can->x;
	y += can->y;
	if (x < 0 || x >= can->h || y < 0 || y >= can->w)
		return;
	memcpy(can->b + can->w * (can->h - 1 - x) + y, col, sizeof(*can->b));
}

static void
ff_rectangle(Canvas *can, Color *col,
	int x1, int y1,
	int x2, int y2)
{
	int x, y, xmin, ymin, xmax, ymax;

	xmin = MIN(x1, x2); xmax = MAX(x1, x2);
	ymin = MIN(y1, y2); ymax = MAX(y1, y2);

	for (x = xmin; x <= xmax; x++)
		for (y = ymin; y <= ymax; y++)
			ff_pixel(can, col, x, y);
}

/*
 * From Bresenham's line algorithm and dcat's tplot.
 */
static void
ff_line(Canvas *can, Color *col,
	int x0, int y0,
	int x1, int y1)
{
	int dx, dy, sx, sy, err, e;

	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	err = (dx > dy ? dx : -dy) / 2;

	for (;;) {
		ff_pixel(can, col, x0, y0);

		if (x0 == x1 && y0 == y1)
			break;

		e = err;
		if (e > -dx) {
			x0 += sx;
			err -= dy;
		}
		if (e < dy) {
			y0 += sy;
			err += dx;
		}
	}
}

/*
 * Draw a coloured glyph from font f centered on x.
 */
static void
ff_char(Canvas *can, Color *col, char c, Font *f,
	int x, int y)
{
	int xf, yf;

	if (c & 0x80)
		c = '\0';


	x -= f->h / 2;

	for (xf = 0; xf < f->h; xf++)
		for (yf = 0; yf < f->w; yf++)
			if (f->b[(int)c][f->w * (f->h - xf) + yf] == 1)
				ff_pixel(can, col, x + xf, y + yf);
}

/*
 * Draw a left aligned string without wrapping it.
 */
static void
ff_str_left(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	for (; *s != '\0'; y += f->w, s++)
		ff_char(can, col, *s, f, x, y);
}

/*
 * Draw a center aligned string without wrapping it.
 */
static void
ff_str_center(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	y -= f->w * strlen(s) / 2;
	ff_str_left(can, col, s, f, x, y);
}

/*
 * Draw a right aligned string without wrapping it.
 */
static void
ff_str_right(Canvas *can, Color *col, char *s, Font *f,
	int x, int y)
{
	y -= f->w * strlen(s);
	ff_str_left(can, col, s, f, x, y);
}

static void
ff_print(Canvas *can)
{
	uint32_t w, h;

	w = htonl(can->w);
	h = htonl(can->h);

	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	fwrite(can->b, can->w * can->h, sizeof(*can->b), stdout);
}

static int
ff_t2y(time_t t, time_t tmin, time_t tmax)
{
	return (t - tmin) * PLOT_W / (tmax - tmin);
}

static int
ff_v2x(double v, double vmin, double vmax)
{
	return (v - vmin) * PLOT_H / (vmax - vmin);
}

static void
ff_xaxis(Canvas *can, Color *label, Color *grid,
	double vmin, double vmax, double vstep)
{
	double v;
	int x;
	char str[8 + 1];

	for (v = vmax - fmod(vmax, vstep); v >= vmin; v -= vstep) {
		x = ff_v2x(v, vmin, vmax);

		ff_line(can, grid,
			x, XLABEL_W,
			x, XLABEL_W + PLOT_W);

		humanize(str, v);
		ff_str_right(can, label, str, &font,
			x, XLABEL_W - MARGIN);
	}
}

static void
ff_yaxis(Canvas *can, Color *label, Color *grid,
	time_t tmin, time_t tmax, time_t tstep)
{
	time_t t;
	int y;
	char str[sizeof("MM/DD HH/MM")], *fmt;

	if (tstep < 3600 * 12)
		fmt = "%H:%M:%S";
	else if (tstep < 3600 * 24)
		fmt = "%m/%d %H:%M";
	else
		fmt = "%Y/%m/%d";

	for (t = tmax - tmax % tstep; t >= tmin; t -= tstep) {
		y = ff_t2y(t, tmin, tmax);

		ff_line(can, grid,
			YLABEL_H, y,
			YLABEL_H + PLOT_H, y);

		strftime(str, sizeof(str), fmt, localtime(&t));
		ff_str_center(can, label, str, &font,
			YLABEL_H / 2, y);
	}
}

static void
ff_title(Canvas *can,
	Color *ct, char *title,
	Color *cu, char *unit)
{
	ff_str_left(can, ct, title, &font,
		TITLE_H / 2, 0);
	ff_str_right(can, cu, unit, &font,
		TITLE_H / 2, TITLE_W);
}

static void
ff_plot(Canvas *can, Vlist *v,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	time_t *tp;
	double *vp;
	int x, y, n, xlast, ylast, first;

	first = 1;
	for (tp = v->t, vp = v->v, n = v->n; n > 0; n--, vp++, tp++) {
		x = ff_v2x(*vp, vmin, vmax);
		y = ff_t2y(*tp, tmin, tmax);

		if (!first)
			ff_line(can, &v->col, xlast, ylast, x, y);

		xlast = x;
		ylast = y;
		first = 0;
	}
}

static void
ff_values(Canvas *can, Vlist *v, int n,
	double vmin, double vmax,
	time_t tmin, time_t tmax)
{
	for (; n > 0; n--, v++)
		ff_plot(can, v, vmin, vmax, tmin, tmax);
}

static void
ff_legend(Canvas *can, Color *label_fg, Vlist *v, int n)
{
	int i, x, y;

	for (i = 0; i < n; i++, v++) {
		x = LEGEND_H - i * (FONT_H + MARGIN) - FONT_H / 2;

		y = MARGIN + FONT_W;
		ff_str_left(can, &v->col, "\1", &font, x, y);

		y += FONT_W * 2;
		ff_str_left(can, label_fg, v->label, &font, x, y);
	}
}

/*
 * Plot the 'n' values list of the 'v' array with title 'name' and
 * 'units' label.
 *
 *	       Title       (units)
 *	     y ^                    Legend
 *	 label |- + - + - + - + -    ....
 *	  here |- + - + - + - + -    ....
 *	       +--+---+---+---+-->
 *	        x label here        
 */
static void
ff(Vlist *v, int n, char *name, char *units)
{
	Canvas can	= { IMAGE_W, IMAGE_H, 0, 0, { { 0 }, { 0 } } };
	Color plot_bg	= { 0x2222, 0x2222, 0x2222, 0xffff };
	Color grid_bg	= { 0x2929, 0x2929, 0x2929, 0xffff };
	Color grid_fg	= { 0x3737, 0x3737, 0x3737, 0xffff };
	Color label_fg	= { 0x8888, 0x8888, 0x8888, 0xffff };
	Color title_fg	= { 0xdddd, 0xdddd, 0xdddd, 0xffff };
	double vmin, vmax, vstep;
	time_t tmin, tmax, tstep;

	scale(v, n, &vmin, &vmax, &vstep, &tmin, &tmax, &tstep);

	can.x = 0;
	can.y = 0;
	ff_rectangle(&can, &plot_bg, 0, 0, IMAGE_H - 1, IMAGE_W - 1);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	ff_rectangle(&can, &grid_bg, 0, 0, PLOT_H, PLOT_W);

	can.x = YLABEL_X;
	can.y = YLABEL_Y;
	ff_yaxis(&can, &label_fg, &grid_fg, tmin, tmax, tstep);

	can.x = XLABEL_X;
	can.y = XLABEL_Y;
	ff_xaxis(&can, &label_fg, &grid_fg, vmin, vmax, vstep);

	can.x = TITLE_X;
	can.y = TITLE_Y;
	ff_title(&can, &title_fg, name, &label_fg, units);

	can.x = PLOT_X;
	can.y = PLOT_Y;
	ff_values(&can, v, n, vmin, vmax, tmin, tmax);

	can.x = LEGEND_X;
	can.y = LEGEND_Y;
	ff_legend(&can, &label_fg, v, n);

	ff_print(&can);
}
 
static void
csv_labels(Vlist *v, char **argv, char *buf)
{
	if (esfgets(buf, LINE_MAX, stdin) == NULL)
		fputs("missing label line\n", stderr), exit(1);
 
	if (strcmp(strsep(&buf, ","), "epoch") != 0)
		fputs("first label must be \"epoch\"\n", stderr), exit(1);
 
	for (; *argv != NULL; v++, argv++) {
		if ((v->label = strsep(&buf, ",")) == NULL)
			fputs("more arguments than columns\n", stderr), exit(1);
		else if (color(&v->col, *argv) == -1)
			fprintf(stderr, "unknown color: %s\n", *argv), exit(1);
	}
 
	if (strsep(&buf, ",") != NULL)
		fputs("more columns than arguments\n", stderr), exit(1);
}

static int
csv_addval(Vlist *v, int bufsize, int nval, double field, time_t epoch)
{
	if (nval >= bufsize) {
		bufsize = bufsize * 2 + 1;
		if ((v->v = realloc(v->v, bufsize * sizeof(*v->v))) == NULL)
			perror("reallocating values buffer"), exit(1);
		if ((v->t = realloc(v->t, bufsize * sizeof(*v->t))) == NULL)
			perror("reallocating values buffer"), exit(1);
	}
	v->v[nval] = field;
	v->t[nval] = epoch;
	v->n = nval + 1;

	return bufsize;
}

/*
 * Add to each column the value on the current row.
 */
static int
csv_addrow(Vlist *v, int bufsize, int ncol, int nval, char *line)
{
	time_t epoch;
	int bs;
	char *field, *dot;

	if ((field = strsep(&line, ",")) == NULL)
		fprintf(stderr, "%d: missing epoch\n", nval), exit(1);

	if ((dot = strchr(field, '.')) != NULL)
		*dot = '\0';
	epoch = eatol(field);
	for (; (field = strsep(&line, ",")) != NULL; ncol--, v++) {
		if (ncol <= 0)
			fprintf(stderr, "%d: too many fields\n", nval), exit(1);
		bs = csv_addval(v, bufsize, nval, eatof(field), epoch);
	}
	if (ncol > 0)
		fprintf(stderr, "%d: too few fields\n", nval), exit(1);

	return bs;
}

/*
 *       < ncol >
 * epoch,a1,b1,c1  ^
 * epoch,a2,b2,c2 nval
 * epoch,a3,b3,c3  v
 */
static void
csv_values(Vlist *v, int ncol)
{
	int nval, bufsize;
	char line[LINE_MAX];

	bufsize = 0;
	for (nval = 0; esfgets(line, sizeof(line), stdin) != NULL; nval++)
		bufsize = csv_addrow(v, bufsize, ncol, nval, line);
	if (nval == 0)
		fputs("no value could be read\n", stderr), exit(1);
}

static void
usage(void)
{
	Clist *c;

	fprintf(stderr, "usage: %s [-t title] [-u unit] {", argv0);
	fputs(clist->name, stderr);
	for (c = clist + 1; c->name != NULL; c++)
		fprintf(stderr, ",%s", c->name);
	fputs("}...\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	Vlist *v;
	char labels[LINE_MAX];

	ARGBEGIN {
	case 't':
		tflag = EARGF(usage());
		break;
	case 'u':
		uflag = EARGF(usage());
		break;
	default:
		usage();
	} ARGEND;

	if ((v = calloc(argc, sizeof(*v))) == NULL)
		perror("calloc value list"), exit(1);

	csv_labels(v, argv, labels);
	csv_values(v, argc);

	ff(v, argc, tflag, uflag);

	return 0;
}
