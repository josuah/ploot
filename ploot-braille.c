#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "drawille.h"
#include "scale.h"
#include "tool.h"
#include "log.h"

char const *arg0 = NULL;

/*
 * Plot the body as an histogram interpolating the gaps and include
 * a vertical and horizontal axis.
 */
int
braille_histogram(struct vlist *vl, struct drawille *drw,
	time_t tmin, time_t tmax, double vmin, double vmax)
{
	int		x, xprev, y, yprev, zero;
	double		*v;
	time_t		*t;
	size_t		n;

	zero = scale_ypos(0, vmin, vmax, drw->row*4);
	v = vl->v;
	t = vl->t;
	n = vl->n;
	for (; n > 0; n--, t++, v++) {
		if (isnan(*v))  /* XXX: better handling? */
			continue;
		y = scale_ypos(*v, vmin, vmax, drw->row * 4);
		x = scale_xpos(*t, tmin, tmax, drw->col * 2);
		if (n < vl->n)
			drawille_histogram_line(drw, xprev, yprev, x, y, zero);
		xprev = x;
		yprev = y;
	}
	return 0;
}

static int
braille_axis_x(FILE *fp, time_t tmin, time_t tmax, time_t tstep, int col)
{
	int x, o, prec;
	char tmp[sizeof("MM/DD HH:MM")], *fmt;
	size_t n;
	time_t t;

	fmt =
	  (tstep < 3600 * 12) ? "^%H:%M:%S" :
	  (tstep < 3600 * 24) ? "^%m/%d %H:%M" :
	  "^%Y/%m/%d";
	n = x = 0;

	t = tmin + tstep - tmin % tstep;
	for (; t < tmax; t += tstep) {
		x = (t - tmin) * col / (tmax - tmin);
		strftime(tmp, sizeof tmp, fmt, localtime(&t));
		prec = x - n + strlen(tmp);
		if ((o = fprintf(fp, "%*s", prec, tmp)) < 0)
			return -1;
		n += o;
	}
	fprintf(fp, "\n");
	return 0;
}

/*
 * Plot a single line out of the y axis, at row <r> out of <rows>.
 */
static void
braille_axis_y(FILE *fp, double vmin, double vmax, int r, int rows)
{
	char tmp[10] = "", *s;
	double val;

	val = (rows - r) * (vmax - vmin) / rows;
	humanize(tmp, val);
	s =
	  (r == 0) ? "┌" :
	  (r == rows - 1) ? "└" :
	  "├";
	fprintf(fp, "%s%-6s ", s, tmp);
}

static int
braille_render(struct drawille *drw, FILE *fp, double vmin, double vmax)
{
	for (int row = 0; row < drw->row; row++) {
		drawille_put_row(drw, fp, row);
		braille_axis_y(fp, vmin, vmax, row, drw->row);
		fprintf(fp, "\n");
	}
	return 0;
}

static void
plot(struct vlist *vl, FILE *fp, size_t ncol, int row, int col)
{
	size_t len;
	double vmin, vmax, vstep;
	time_t tmin, tmax, tstep;
	struct drawille *drw;

	len = 500;
	col -= 8;

	scale(vl, ncol, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);
	warn("vstep=%lf vstep=%ld", vstep, tstep);

	if ((drw = drawille_new(row, col)) == NULL)
		fatal(1, "allocating drawille canvas");
	if (braille_histogram(vl, drw, tmin, tmax, vmin, vmax) == -1)
		fatal(1, "allocating drawille canvas");
	if (braille_render(drw, fp, vmin, vmax) == -1)
		fatal(1, "rendering braille canvas");
	if (braille_axis_x(fp, tmin, tmax, tstep, col) == -1)
		fatal(1, "printing x axis");;
	free(drw);
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s\n", arg0);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct vlist *vl;
	size_t ncol;
	int c;

	optind = 0;
	while ((c = getopt(argc, argv, "")) > -1) {
		switch (c) {
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		usage();

	csv_labels(stdin, &vl, &ncol);
	csv_values(stdin, vl, ncol);

	plot(vl, stdout, ncol, 20, 80);

	free(vl);
	return 1;
}
