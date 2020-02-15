/*
 * Read CSV data onto a set of (struct vlist).
 */

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "def.h"

static void
csv_addtime(struct vlist *vl, time_t epoch)
{
	if ((vl->t = realloc(vl->t, (vl->n + 1) * sizeof(*vl->t))) == NULL)
		err(1, "reallocating values buffer");
	vl->t[vl->n] = epoch;
}

static void
csv_addval(struct vlist *vl, double field)
{
	if ((vl->v = realloc(vl->v, (vl->n + 1) * sizeof(*vl->v))) == NULL)
		err(1, "reallocating values buffer");
	vl->v[vl->n] = field;
}

/*
 * Add to each column the value on the current row.
 */
void
csv_addrow(struct vlist *vl, size_t ncol, char *line)
{
	char		*field;

	if ((field = strsep(&line, ",")) == NULL)
		err(1, "missing epoch at row %zu", vl->n);

	csv_addtime(vl, eatol(field));
	for (; (field = strsep(&line, ",")) != NULL; ncol--, vl->n++, vl++) {
		if (ncol == 0)
			err(1, "too many fields at line %zu", vl->n);
		csv_addval(vl, eatof(field));
	}
	if (ncol > 0)
		err(1, "too few fields at line %zu", vl->n);
}
 
/*
 *       < *ncol >
 * epoch,label1,label2,label3
 */
void
csv_labels(FILE *fp, char *buf,  struct vlist **vl, size_t *ncol)
{
	char		*field;
	size_t		sz;

	if (esfgets(buf, LINE_MAX, fp) == NULL)
		err(1, "missing label line");

	if (strcmp(strsep(&buf, ","), "epoch") != 0)
		err(1, "first label must be \"epoch\"");

	*vl = NULL;
	for (*ncol = 0; (field = strsep(&buf, ",")) != NULL; ++*ncol) {
		sz = (*ncol + 1) * sizeof **vl;
		if ((*vl = realloc(*vl, sz)) == NULL)
			err(1, "realloc");
		(*vl)[*ncol].label = field;
	}
}

/*
 *       < ncol >
 * epoch,a1,b1,c1  ^
 * epoch,a2,b2,c2 vl->n
 * epoch,a3,b3,c3  v
 */
void
csv_values(FILE *fp,  struct vlist *vl, size_t ncol)
{
	char		line[LINE_MAX];
	time_t		*tbuf;

	while (esfgets(line, sizeof(line), fp) != NULL)
		csv_addrow(vl, ncol, line);
	if (vl->n == 0)
		err(1, "no value could be read");
	if (vl->n == 1)
		err(1, "only one value could be read");

	/* The same time buffer can be used for all. */
	for (tbuf = vl->t; ncol > 0; ncol--, vl++)
		vl->t = tbuf;
}
