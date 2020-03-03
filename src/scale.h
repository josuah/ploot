#ifndef SCALE_H
#define SCALE_H

#include <stddef.h>
#include <time.h>

#include "csv.h"

/**/
int		scale_ypos		(double, double, double, int);
int		scale_xpos		(time_t, time_t, time_t, int);
void		scale_minmax		(struct csv *, int, time_t *, time_t *, double *, double *);
time_t		scale_tstep		(time_t, time_t, int);
double		scale_vstep		(double, double, int);

#endif
