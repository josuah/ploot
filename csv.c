#include "csv.h"
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "util.h"

/*
 * Read CSV data onto a set of (struct csv).
 */

static void
csv_add_time(struct csv *vl, time_t epoch)
{
	void *mem;

	if ((mem = realloc(vl->t, (vl->n + 1) * sizeof *vl->t)) == NULL)
		err(1, "realloc: %s", strerror(errno));
	vl->t = mem;
	vl->t[vl->n] = epoch;
}

static void
csv_add_val(struct csv *vl, double field)
{
	void *mem;

	if ((mem = realloc(vl->v, (vl->n + 1) * sizeof *vl->v)) == NULL)
		err(1, "", strerror(errno));
	vl->v = mem;
	vl->v[vl->n] = field;
}

/*
 * Add to each column the value on the current row.  The time_t
 * buffer is shared among all fields.
 */
static void
csv_add_row(struct csv *vl, size_t ncol, char *line)
{
	char *field;
	time_t *tbuf;
	long l;
	double d;

	if ((field = strsep(&line, ",")) == NULL)
		err(1, "missing epoch at row %zu", vl->n);

	l = strtol(field, NULL, 10);
	if (errno)
		err(100, "parsing number '%s'", field);

	csv_add_time(vl, l);
	tbuf = vl[0].t;
	for (; (field = strsep(&line, ",")); ncol--, vl->n++, vl++) {
		if (ncol == 0)
			err(1, "too many fields at line %zu", vl->n);
		d = strtod(field, NULL);
		if (errno)
			err(100, "parsing double '%s'", field);
		csv_add_val(vl, d);
		vl->t = tbuf;
	}
	if (ncol > 0)
		err(1, "too few fields at line %zu", vl->n);
}

/*
 *      < (ncol) >
 * label1,label2,label3
 */
void
csv_labels(FILE *fp, struct csv **vlp, size_t *ncol)
{
	char *field, *line, *cp;
	struct csv *vl, *col;
	size_t sz;
	ssize_t r;

	sz = 0, line = NULL;
	r = getline(&line, &sz, fp);
	if (ferror(fp))
		err(111, "error while reading from file");
	if (feof(fp))
		err(100, "missing label line");
	strchomp(line);

	cp = line;
	if (strcmp(strsep(&cp, ","), "epoch") != 0)
		err(1, "first label must be 'epoch'");

	sz = 0, vl = NULL, *ncol = 0;
	while ((field = strsep(&cp, ","))) {
		if ((vl = realloc(vl, sz += sizeof *vl)) == NULL)
			err(1, "realloc: %s", strerror(errno));
		col = vl + (*ncol)++;
		memset(col, 0, sizeof *vl);
		strlcpy(col->label, field, sizeof col->label);
	}
	free(line);
	*vlp = vl;
}

/*
 *    < (ncol) >
 * val1a,val1b,val1c    ^
 * val2a,val2b,val2c    |
 * val3a,val3b,val3c (vl->n)
 * val4a,val4b,val4c    |
 * val5a,val5b,val5c    v
 */
void
csv_values(FILE *fp, struct csv *vl, size_t ncol)
{
	char *line;
	size_t sz;

	sz = 0, line = NULL;
	while (getline(&line, &sz, fp) > -1)
		csv_add_row(vl, ncol, line);
	if (vl->n == 0)
		err(1, "no value could be read");
	if (vl->n == 1)
		err(1, "only one value could be read");
	free(line);
}
