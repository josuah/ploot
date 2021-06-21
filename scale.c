#include "scale.h"

#include <stddef.h>
#include <time.h>

#include "util.h"

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

void
scale_minmax(struct csv *vl, int ncol,
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
		err(1, "invalid time scale: min=%lld max=%lld", *tmin, *tmax);
}

time_t
scale_csvep(time_t min, time_t max, int nval)
{
        time_t dt, *sc, scale[] = {
		1, 5, 2, 10, 20, 30, 60, 60*2, 60*5, 60*10, 60*20, 60*30, 3600, 
		3600*2, 3600*5, 3600*10, 3600*18, 3600*24, 3600*24*2, 
		3600*24*5, 3600*24*10, 3600*24*20, 3600*24*30, 3600*24*50,
		3600*24*100, 3600*24*365, 0
	};

	dt = max - min;

	for (sc = scale; *sc > 0; sc++)
		if (dt < *sc * nval)
			return *sc;
	return dt / nval;
}

double
scale_vstep(double min, double max, int nval)
{
	double dv, d, *sc, scale[] = { 1, 2, 3, 5 };

	dv = max - min;

	if (dv > 1)
		for (d = 1; d != 0; d *= 10)
			for (sc = scale; sc < scale + LEN(scale); sc++)
				if (dv < *sc * d * nval)
					return *sc * d;
	if (dv < 1)
		for (d = 1; d != 0; d *= 10)
			for (sc = scale + LEN(scale) - 1; sc >= scale; sc--)
				if (dv > *sc / d * nval / 2)
					return *sc / d;
	return 0;
}
