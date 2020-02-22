#include "csv.h"

#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "log.h"
#include "tool.h"

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

	if ((field = strsep(&line, ",")) == NULL)
		fatal(1, "missing epoch at row %zu", vl->n);

	csv_addtime(vl, eatol(field));
	for (; (field = strsep(&line, ",")) != NULL; ncol--, vl->n++, vl++) {
		if (ncol == 0)
			fatal(1, "too many fields at line %zu", vl->n);
		csv_addval(vl, eatof(field));
	}
	if (ncol > 0)
		fatal(1, "too few fields at line %zu", vl->n);

	/* the same time buffer can be used for all columns */
	for (tbuf = vl->t; ncol > 0; ncol--, vl++)
		vl->t = tbuf;
}
 
/*
 *       < *ncol >
 * epoch,label1,label2,label3
 */
void
csv_labels(FILE *fp, struct csv **vl, size_t *ncol)
{
	char *field, *line, *cp, *label;
	size_t sz;
	ssize_t r;

	r = getline(&line, &sz, fp);
	if (ferror(fp))
		fatal(111, "error while reading from file");
	if (r == -1)
		fatal(100, "missing label line");

	cp = line;
	if (strcmp(strsep(&cp, ","), "epoch") != 0)
		fatal(1, "first label must be 'epoch'");

	*vl = NULL;
	*ncol = 0;
	while ((field = strsep(&cp, ","))) {
		assert(*vl = realloc(*vl, sz += sizeof(**vl)));
		label = (*vl)[(*ncol)++].label;
		strlcpy(label, field, sizeof(label));
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

	sz = 0;
	while (getline(&line, &sz, fp) > -1)
		csv_addrow(vl, ncol, line);
	if (vl->n == 0)
		fatal(1, "no value could be read");
	if (vl->n == 1)
		fatal(1, "only one value could be read");

	free(line);
}
