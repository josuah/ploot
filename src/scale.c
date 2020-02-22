#include "scale.h"

#include <stddef.h>
#include <time.h>

#include "tool.h"
#include "log.h"

/*
 *	- <max   ^
 *	-        |	Translate the coordinates between double values
 *	- <val  szy and height in the plot of <row> rows.
 *	-        |
 *	- <min   v
 */
int
scale_ypos(double val, double min, double max, int szy)
{
	return szy * (val - min) / (max - min);
}

/*
 *	<---- szx ---->		Translate the coordinates between the time
 *				range and position in the plot of <col> cols.
 *	t1     t     t2
 *	 | . . | . . |
 */
int
scale_xpos(time_t t, time_t t1, time_t t2, int szx)
{
	return szx * (t - t1) / (t2 - t1);
}

static void
scale_minmax(struct vlist *vl, int ncol,
	time_t *tmin, time_t *tmax,
	double *vmin, double *vmax)
{
	double *v;
	time_t *t;
	size_t n;

	*vmin = *vmax = 0;
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
		fatal(1, "invalid time scale: min=%lld max=%lld", *tmin, *tmax);
}

static time_t
scale_tstep(time_t min, time_t max, int density)
{
        time_t dt, *s, scale[] = {
		1, 5, 2, 10, 20, 30, 60, 60*2, 60*5, 60*10, 60*20, 60*30, 3600, 
		3600*2, 3600*5, 3600*10, 3600*18, 3600*24, 3600*24*2, 
		3600*24*5, 3600*24*10, 3600*24*20, 3600*24*30, 3600*24*50,
		3600*24*100, 3600*24*365, 0
	};

	dt = max - min;
	for (s = scale; s < scale + LEN(scale); s++)
		if (dt < *s * density)
			return *s;
	return 0;
}

static double
scale_vstep(double min, double max, int density)
{
	double dv, d, *s, scale[] = { 1, 2, 3, 5 };

	dv = max - min;

	if (dv > 1)
		for (d = 1; d != 0; d *= 10)
			for (s = scale; s < scale + LEN(scale); s++)
				if (dv < *s * d * density)
					return *s * d;
	if (dv < 1)
		for (d = 1; d != 0; d *= 10)
			for (s = scale + LEN(scale) - 1; s >= scale; s--)
				if (dv > *s / d * density / 2)
					return *s / d;
	return 0;
}

/*
 * Adjust the vertical scale so that everything fits, with nice
 * scale values.
 */
void
scale_vminmax(double *min, double *max, int row)
{
	double unit, range, mi;

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

void
scale(struct vlist *vl, int ncol,
	time_t *tmin, time_t *tmax, time_t *tstep,
	double *vmin, double *vmax, double *vstep)
{
	scale_minmax(vl, ncol, tmin, tmax, vmin, vmax);
	*tstep = scale_tstep(*tmin, *tmax, SCALE_X);
	*vstep = scale_vstep(*vmin, *vmax, SCALE_Y);
}
