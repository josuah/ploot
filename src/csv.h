#ifndef CSV_H
#define CSV_H

#include <stdio.h>

/*
 * List of values and timestamps.  Both have their dedicated buffer
 * so that the timestamp buffer can be shared across vlist objects.
 */
struct vlist {
	time_t		*t;		/* array of timestamps */
	double		*v;		/* array of values */
	size_t		n;		/* number of values */
	char		label[64];	/* for the legend */
};

/**/
void		csv_addrow		(struct vlist *, size_t, char *);
void		csv_labels		(FILE *, struct vlist **, size_t *);
void		csv_values		(FILE *, struct vlist *, size_t);

#endif
