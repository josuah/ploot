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

static int
braille_axis_x(FILE *fp, time_t tmin, time_t tmax, time_t tstep, int col)
{
	int		x, o, prec;
	char		tmp[sizeof("MM/DD HH:MM")], *fmt;
	size_t		n;
	time_t		t;

	fmt = (tstep < 3600 * 12) ? "^%H:%M:%S" :
	    (tstep < 3600 * 24) ? "^%m/%d %H:%M" :
	    "^%Y/%m/%d";
	n = x = 0;

	t = tmin;
	t += tstep - t % tstep;
	for (; t < tmax; t += tstep) {
		x = (t - tmin) * col / (tmax - tmin);
		strftime(tmp, sizeof tmp, fmt, localtime(&t));
		prec = x - n + strlen(tmp);
		if ((o = fprintf(fp, "%*s", prec, tmp)) < 0)
			return -1;
		n += o;
	}
	fputc('\n', fp);
	return 0;
}

/*
 * Plot a single line out of the y axis, at row <r> out of <rows>.
 */
static void
braille_axis_y(FILE *fp, double vmin, double vmax, int r, int rows)
{
	char		tmp[10] = "", *s;
	double		val;

	val = (rows - r) * (vmax - vmin) / rows;
	humanize(tmp, val);
	s = (r == 0) ? "┌" :
	    (r == rows - 1) ? "└" :
	    "├";
	fprintf(fp, "%s%-6s ", s, tmp);
}

static int
braille_render(struct drawille *drw, FILE *fp, double vmin, double vmax)
{
	/* Render the plot line by line. */
	for (int row = 0; row < drw->row; row++) {
		drawille_put_row(drw, fp, row);
		braille_axis_y(fp, vmin, vmax, row, drw->row);
		fputc('\n', fp);
	}
	return 0;
}

static void
plot(struct vlist *vl, FILE *fp, size_t ncol, int row, int col)
{
	size_t		len;
	double		vmin, vmax, vstep;
	time_t		tmin, tmax, tstep;
	struct drawille	*drw;

	len = 500;
	col -= 8;

	scale(vl, ncol, &tmin, &tmax, &tstep, &vmin, &vmax, &vstep);
	warn("vstep=%lf vstep=%ld", vstep, tstep);

	if ((drw = drawille_new(row, col)) == NULL)
		err(1, "allocating drawille canvas");
	if (drawille_histogram(vl, drw, tmin, tmax, vmin, vmax) == -1)
		err(1, "allocating drawille canvas");
	if (braille_render(drw, fp, vmin, vmax) == -1)
		err(1, "rendering braille canvas");
	if (braille_axis_x(fp, tmin, tmax, tstep, col) == -1)
		err(1, "printing x axis");;
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
