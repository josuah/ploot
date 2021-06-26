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
#include "util.h"
#include "csv.h"

#ifndef __OpenBSD__
#define pledge(...) 0
#endif

static int
get_min_max(struct csv *vl, int ncol,
	time_t *tmin, time_t *tmax,
	double *vmin, double *vmax)
{
	double *v;
	time_t *t;
	size_t n;

	*vmin = *vmax = 0; /* always show 0 on the scale */
	*tmin = *tmax = *vl->t;

	for (; ncol > 0; ncol--, vl++) {
		for (t = vl->t, v = vl->v, n = vl->n; n > 0; t++, v++, n--) {
			if (*v < *vmin) *vmin = *v;
			if (*v > *vmax) *vmax = *v;
			if (*t < *tmin) *tmin = *t;
			if (*t > *tmax) *tmax = *t;
		}
	}
	if (*tmin == *tmax)
		return -1;
	return 0;
}

static time_t
time_mark_step(time_t min, time_t max, int dots)
{
	time_t dt, scale[] = {
		1, 5, 2, 10, 20, 30, 60, 60*2, 60*5, 60*10, 60*20, 60*30, 3600, 
		3600*2, 3600*6, 3600*12, 3600*24, 3600*24*2, 
		3600*24*7, 3600*24*14, 3600*24*20, 3600*24*21, 3600*24*28, 3600*24*50,
		3600*24*100, 3600*24*365, 0
	};

	dt = max - min;
	for (time_t *sc = scale; *sc > 0; sc++)
		if (dt < *sc * dots)
			return *sc;
	return dt / dots;
}

/*
 * Make the value scale aligned with round values by changing the
 * minimal and maximal values.
 */
static void
adjust_scale(double *min, double *max, int rows)
{
	double dv, step, scale[] = { 1, 2, 2.5, 5, };

	dv = *max - *min;

	step = 1;
	if (dv > 1) {
		for (double mant = 1;; mant *= 10) {
			double *sc = scale;
			for (; sc < scale + LEN(scale); sc++) {
				step = mant * *sc;
				if (dv < rows * step)
					goto end;
			}
		}
	} else {
		for (double mant = 1;; mant /= 10) {
			double *sc = scale + LEN(scale) - 1;
			for (; sc >= scale; sc--) {
				double tmp = mant * *sc;
				if (dv > rows * tmp)
					goto end;
				step = tmp;
			}
		}
	}
end:
	*min = (int)(*min / step) * step;
	*max = *min + step * rows;
}

/*
 * Plot the body as an histogram interpolating the gaps and include
 * a vertical and horizontal axis.
 */
static int
braille_histogram(struct csv *vl, struct drawille *drw,
	time_t tmin, time_t tmax, double vmin, double vmax)
{
	int		x, xprev, y, yprev, zero;
	double		*v;
	time_t		*t;
	size_t		n;

#define SHIFT (4 / 2)
#define POSITION(val, min, max, sz) ((sz) * ((val) - (min)) / ((max) - (min)) + SHIFT)

	zero = POSITION(0, vmin, vmax, drw->row*4);
	v = vl->v;
	t = vl->t;
	n = vl->n;
	for (; n > 0; n--, t++, v++) {
		if (isnan(*v)) /* XXX: better handling? */
			continue;
		y = POSITION(*v, vmin, vmax, drw->row * 4);
		x = POSITION(*t, tmin, tmax, drw->col * 2);
		if (n < vl->n) /* only plot when xprev, yprev are set */
			drawille_histogram_line(drw, xprev, yprev, x, y, zero);
		xprev = x;
		yprev = y;
	}

#undef POSITION

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
braille_axis_y(FILE *fp, double min, double max, int r, int rows)
{
	char buf[10] = "";

	humanize(buf, (rows - 1 - r) * (max - min) / rows);
	fprintf(fp, "├%s ", buf);
}

static int
braille_render(struct drawille *drw, FILE *fp, double min, double max)
{
	int row;

	for (row = 0; row < drw->row; row++) {
		drawille_put_row(fp, drw, row);
		braille_axis_y(fp, min, max, row, drw->row);
		fprintf(fp, "\n");
	}
	return 0;
}

static void
plot(struct csv *vl, size_t ncol, int rows, int cols, FILE *fp)
{
	double vmin, vmax;
	time_t tmin, tmax, tstep;
	struct drawille *drw;

	rows = MAX(rows, 2);	/* readable */

	if (get_min_max(vl, ncol, &tmin, &tmax, &vmin, &vmax) < 0)
		err(1, "invalid scale: tmin=%lld tmax=%lld vmin=%fd vmax=%fd",
		    (long long)tmin, (long long)tmax, vmin, vmax);
	adjust_scale(&vmin, &vmax, rows);
	tstep = time_mark_step(tmin, tmax, cols);

	for (; ncol > 0; vl++, ncol--) {
		if ((drw = drawille_new(rows, cols)) == NULL)
			err(1, "drawille_new: %s", strerror(errno));
		fprintf(fp, " %s\n", vl->label);
		if (braille_histogram(vl, drw, tmin, tmax, vmin, vmax) == -1)
			err(1, "allocating drawille canvas");
		if (braille_render(drw, fp, vmin, vmax) == -1)
			err(1, "rendering braille canvas");
		free(drw);
	}
	if (braille_axis_x(fp, tmin, tmax, tstep * 10, cols) == -1)
		err(1, "printing x axis");;
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-r rows] [-c cols]\n", arg0);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct csv *vl;
	size_t ncol;
	int c, rows, cols;

	if (pledge("stdio", "") < 0)
		err(1, "pledge: %s", strerror(errno));

	rows = 4, cols = 60;
	arg0 = *argv;
	while ((c = getopt(argc, argv, "r:c:")) > -1) {
		switch (c) {
		case 'r':
			rows = atoi(optarg);
			if (rows < 1) {
				warn("invalid number of rows");
				usage();
			}
			break;
		case 'c':
			cols = atoi(optarg);
			if (rows < 1) {
				warn("invalid number of columns");
				usage();
			}
			break;

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

	plot(vl, ncol, rows, cols, stdout);

	free(vl);
	return 0;
}
