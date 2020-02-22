#ifndef SCALE_H
#define SCALE_H

#include <stddef.h>
#include <time.h>

#include "csv.h"

#define SCALE_X 7	/* nb of values on x axis */
#define SCALE_Y 7	/* nb of values on y axis */

/**/
int		scale_ypos		(double, double, double, int);
int		scale_xpos		(time_t, time_t, time_t, int);
void		scale_vminmax		(double *, double *, int);
void		scale			(struct csv *, int, time_t *, time_t *, time_t *, double *, double *, double *);

#endif
