#include "csv.h"

#include <errno.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "log.h"
#include "util.h"

/*
 * Read CSV data onto a set of (struct csv).
 */

static void
csv_addtime(struct csv *vl, time_t epoch)
{
	assert(vl->t = realloc(vl->t, (vl->n + 1) * sizeof(*vl->t)));
	vl->t[vl->n] = epoch;
}

static void
csv_addval(struct csv *vl, double field)
{
	assert(vl->v = realloc(vl->v, (vl->n + 1) * sizeof(*vl->v)));
	vl->v[vl->n] = field;
}

/*
 * Add to each column the value on the current row.  The time_t
 * buffer is shared among all fields.
 */
void
csv_addrow(struct csv *vl, size_t ncol, char *line)
{
	char *field;
	time_t *tbuf;
	long l;
	double d;

	field = strsep(&line, ",");
	if (!field)
		die(1, "missing epoch at row %zu", vl->n);

	l = strtol(field, NULL, 10);
	if (errno)
		die(100, "parsing number '%s'", field);
	csv_addtime(vl, l);
	tbuf = vl[0].t;
	for (; (field = strsep(&line, ",")); ncol--, vl->n++, vl++) {
		if (ncol == 0)
			die(1, "too many fields at line %zu", vl->n);
		d = strtod(field, NULL);
		if (errno)
			die(100, "parsing double '%s'", field);
		csv_addval(vl, d);
		vl->t = tbuf;
	}
	if (ncol > 0)
		die(1, "too few fields at line %zu", vl->n);
}
 
/*
 *       < *ncol >
 * epoch,label1,label2,label3
 */
void
csv_labels(FILE *fp, struct csv **vl, size_t *ncol)
{
	char *field, *line, *cp;
	struct csv *col;
	size_t sz;
	ssize_t r;

	sz = 0, line = NULL;
	r = getline(&line, &sz, fp);
	if (ferror(fp))
		die(111, "error while reading from file");
	if (r == -1)
		die(100, "missing label line");
	strchomp(line);

	cp = line;
	if (strcmp(strsep(&cp, ","), "epoch") != 0)
		die(1, "first label must be 'epoch'");

	*vl = NULL;
	*ncol = 0;
	while ((field = strsep(&cp, ","))) {
		assert(*vl = realloc(*vl, sz += sizeof(**vl)));
		col = (*vl) + (*ncol)++;
		strlcpy(col->label, field, sizeof(col->label));
	}

	free(line);
}

/*
 *       < ncol >
 * epoch,a1,b1,c1  ^
 * epoch,a2,b2,c2 vl->n
 * epoch,a3,b3,c3  v
 */
void
csv_values(FILE *fp, struct csv *vl, size_t ncol)
{
	char *line;
	size_t sz;

	sz = 0, line = NULL;
	while (getline(&line, &sz, fp) > -1)
		csv_addrow(vl, ncol, line);
	if (vl->n == 0)
		die(1, "no value could be read");
	if (vl->n == 1)
		die(1, "only one value could be read");

	free(line);
}
