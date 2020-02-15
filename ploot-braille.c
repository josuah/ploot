#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "def.h"
#include "arg.h"

char const	*arg0 = NULL;

/*
 * Return the step between two values.
 */
static int
braille_time_interval(time_t step)
{
	time_t		scale[] = {
		1, 5, 2, 10, 20, 30, 60, 60*2, 60*5, 60*10, 60*20, 60*30,
		3600, 3600*2, 3600*5, 3600*10, 3600*18, 3600*24, 3600*24*2,
		3600*24*5, 3600*24*10, 3600*24*20, 3600*24*30, 3600*24*50,
		3600*24*100, 3600*24*365, 0
	};

	for (time_t *s = scale; *s != 0; s++)
		if (*s >= 20 * step)
			return *s;
	return 1;
}

static size_t
braille_axis_x(FILE *fp, time_t step, time_t tmax, int col)
{
	int		x, prec;
	char		tmp[sizeof("MM/DD HH:MM")], *fmt;
	size_t		n;
	time_t		t, interval;

	interval = braille_time_interval(step);
	fmt = (step < 3600 * 12) ? "^%H:%M:%S" :
	    (step < 3600 * 24) ? "^%m/%d %H:%M" :
	    "^%Y/%m/%d";
	n = x = 0;

	t = tmax - col * 2 * step;
	t += interval - t % interval;
	for (; t < tmax; t += interval) {
		strftime(tmp, sizeof tmp, fmt, localtime(&t));
		x = ((t - tmax) / 2 + col * step) / step;
		prec = x - n + strlen(tmp);
		fprintf(fp, "%*s", prec, tmp);
	}
	fputc('\n', fp);
	return 1;
}

/*
 * Plot a single line out of the y axis, at row <r> out of <rows>.
 */
static void
braille_axis_y(FILE *fp, double min, double max, int r, int rows)
{
	char		tmp[10] = "", *s;
	double		val;

	val = (max - min) * (rows - r) / rows + min;
	humanize(tmp, val);
	s = (r == 0) ? "┌" :
	    (r == rows - 1) ? "└" :
	    "├";
	fprintf(fp, "%s%-6s ", s, tmp);
}

static int
braille_render(struct drawille *drw, FILE *fp, time_t tmin, time_t tmax)
{
	char		buf[LINE_MAX];

	/* Render the plot line by line. */
	for (int row = 0; row < drw->row; row++) {
		drawille_fmt_row(drw, buf, sizeof buf, row);
		braille_axis_y(fp, tmin, tmax, row, drw->row);
		fputc('\n', fp);
	}
	return 0;
}

/*
 * Plot the body as an histogram interpolating the gaps and include
 * a vertical and horizontal axis.
 */
static int
braille_hist(struct vlist *vl, FILE *fp, time_t tmin, time_t tmax, int row, int col)
{
	int		x, y, zero, shift;
	double		*v, vmin, vmax;
	time_t		*t;
	size_t		n;
	struct drawille	*drw;

	if ((drw = drawille_new(row, col)) == NULL)
		err(1, "allocating drawille canvas");

	shift = (drw->row > 1) ? (2) : (0);  /* center values on "|-" marks */
	vmin = vmax = 0;
	zero = scale_ypos(0, vmin, vmax, drw->row*4) - shift;
	v = vl->v;
	t = vl->t;
	n = vl->n;
	for (; n > 0; n--, t++, v++) {
		if (isnan(*v))  /* XXX: better handling? */
			continue;
		y = scale_ypos(*v, vmin, vmax, drw->row * 4) - shift;
		x = scale_xpos(*t, tmin, tmax, drw->col * 2);
		drawille_dot_hist(drw, x, y, zero);
	}
	if (braille_render(drw, fp, tmin, tmax) == -1)
		err(1, "rendering braille canvas");
	free(drw);
	return 0;
}

static int
plot(struct vlist *vl, FILE *fp, size_t ncol, int row, int col)
{
	size_t		len;
	double		vmin, vmax, vstep;
	time_t		tmin, tmax, tstep;

	len = 500;
	col -= 8;

	scale(vl, ncol, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);

	if (braille_hist(vl, fp, tmin, tmax, row, col) == -1)
		err(1, "allocating drawille canvas");
	braille_axis_x(fp, tstep, tmax, col);
	return 0;
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
	struct vlist	*vl;
	char		labels[LINE_MAX];
	size_t		ncol;

	ARG_SWITCH(argc, argv) {
	default:
		usage();
	}

	if (argc > 0)
		usage();

	csv_labels(stdin, labels, &vl, &ncol);
	csv_values(stdin, vl, ncol);

	plot(vl, stdout, ncol, 20, 80);

	free(vl);
	return 1;
}
