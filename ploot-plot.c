#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "def.h"

/*
 * Adjust the vertical scale so that it gets possible to 
 */
static void
plot_scale(double *min, double *max, int row)
{
	double		unit, range, mi;

	range = *max - *min;
	unit = 1;

	/* Zoom until it fills the canvas. */
	for (; (row - 1) * unit > range; unit /= 10)
		continue;

	/* Dezoom until it fits the canvas. */
	for (; (row - 1) * unit < range; unit *= 10)
		continue;

	/* Fine tune. */
	if ((row - 1) * unit / 5 > range)
		unit /= 5;
	if ((row - 1) * unit / 4 > range)
		unit /= 4;
	if ((row - 1) * unit / 2 > range)
		unit /= 2;

	/* Align the minimum (and the zero). */
	for (mi = 0; mi > *min - unit; mi -= unit)
		continue;

	/* Update the displayed minimal and maximal. */
	*min = mi;
	*max = mi + unit * row;
}

/*
 * Return the step between two values.
 */
static int
plot_time_interval(time_t step)
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
plot_axis_x(char *buf, size_t sz, time_t step, time_t t2, int col)
{
	int		x, prec;
	char		tmp[sizeof("MM/DD HH:MM")], *fmt;
	size_t		n;
	time_t		t, interval;

	interval = plot_time_interval(step);
	fmt = (step < 3600 * 12) ? "^%H:%M:%S" :
	    (step < 3600 * 24) ? "^%m/%d %H:%M" :
	    "^%Y/%m/%d";
	n = x = 0;

	t = t2 - col * 2 * step;
	t += interval - t % interval;
	for (; t < t2; t += interval) {
		strftime(tmp, sizeof tmp, fmt, localtime(&t));
		x = ((t - t2) / 2 + col * step) / step;
		prec = x - n + strlen(tmp);
		assert((n += snprintf(buf+n, sz-n, "%*s", prec, tmp)) <= sz);
	}
	assert((n += strlcpy(buf+n, "\n", sz-n)) < sz);
	return n;
}

/*
 * Plot a single line out of the y axis, at row <r> out of <rows>.
 */
static size_t
plot_axis_y(char *buf, size_t sz, double min, double max, int r, int rows)
{
	size_t		i;
	char		tmp[10] = "", *s;
	double		val;

	val = (max - min) * (rows - r) / rows + min;
	humanize(tmp, sizeof tmp, val);
	s = (r == 0) ? "┌" :
	    (r == rows - 1) ? "└" :
	    "├";
	i = snprintf(buf, sz, "%s%-6s ", s, tmp);
	return (i > sz) ? (sz) : (i);
}

static char *
plot_render(struct drawille *drw, double min, double max, time_t step, time_t t2)
{
	char		*buf;
	size_t		sz;
	size_t		n;

	/* Render the plot line by line. */
	sz = drw->row * (20 + drw->col * 3 + 1) + 1;
	sz += drw->col + 1 + 100000;
	if ((buf = calloc(sz, 1)) == NULL)
		goto err;
	n = 0;
	for (int row = 0; row < drw->row; row++) {
		n += drawille_fmt_row(drw, buf+n, sz-n, row);
		n += plot_axis_y(buf+n, sz-n, min, max, row, drw->row);
		n += strlcpy(buf+n, "\n", sz-n);
	}
	plot_axis_x(buf+n, sz-n, step, t2, drw->col);

	return buf;
err:
	errno = ENOBUFS;
	free(buf);
	return NULL;
}

/*
 * Plot the body as an histogram interpolating the gaps and include
 * a vertical and horizontal axis.
 */
static char *
plot_hist(struct timeserie *ts, time_t t2, struct drawille *drw)
{
	int		x, y, zero, shift;
	double		min, max, val;
	time_t		t1, t;

	/* Adjust the y scale. */
	shift = min = max = 0;
	timeserie_stats(ts, &min, &max);
	if (drw->row > 1) {
		shift = 2;  /* Align values to the middle of the scale: |- */
		plot_scale(&min, &max, drw->row);
	}
	zero = timeserie_ypos(0, min, max, drw->row*4) - shift;

	/* Adjust the x scale. */
	t2 = t2 + ts->step - t2 % ts->step;
	t1 = t2 - ts->step * ts->len;

	/* Plot the data in memory in <drw> starting from the end (t2). */
	t = t2;
	for (x = drw->col * 2; x > 0; x--) {
		val = timeserie_get(ts, t);
		if (!isnan(val)) {
			y = timeserie_ypos(val, min, max, drw->row*4) - shift;
			drawille_dot_hist(drw, x, y, zero);
		}
		t -= ts->step;
	}

	return plot_render(drw, min, max, ts->step, t2);
}

static char *
plot(struct timeserie *ts, time_t t2, int row, int col)
{
	struct drawille	*drw;
	size_t		len;
	char		*buf;

	len = 500;
	buf = NULL;
	drw = NULL;
	col -= 8;

	if (timeserie_read(ts) == -1)
		goto err;

	if ((drw = drawille_new(row, col)) == NULL)
		goto err;

	buf = plot_hist(ts, t2, drw);
err:
	if (buf == NULL)
		timedb_close(&ts->db);
	free(drw);
	return buf;
}
