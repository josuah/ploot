#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "arg.h"
#include "ploot.h"
#include "util.h"
#include "config.h"	/* after ploot.h for type definitions */

#define LEN(x) (sizeof(x) / sizeof(*x))

char *argv0;
char *tflag = "";
char *uflag = "";

static int
color(Color *col, char *name)
{
	ColorList *c;

	for (c = colorlist; c->name != NULL; c++) {
		if (strcmp(name, c->name) == 0) {
			memcpy(col, &c->col, sizeof(*col));
			return 0;
		}
	}

	return -1;
}

static void
read_labels(Vlist *v, char **argv, char *buf)
{
	if (esfgets(buf, LINE_MAX, stdin) == NULL)
		fputs("missing label line\n", stderr), exit(1);

	if (strcmp(strsep(&buf, ","), "epoch") != 0)
		fputs("first label must be \"epoch\"\n", stderr), exit(1);

	for (; *argv != NULL; v++, argv++)
		if ((v->label = strsep(&buf, ",")) == NULL)
			fputs("more arguments than columns\n", stderr), exit(1);
		else if (color(&v->col, *argv) == -1)
			fprintf(stderr, "unknown color: %s\n", *argv), exit(1);

	if (strsep(&buf, ",") != NULL)
		fputs("more columns than arguments\n", stderr), exit(1);
}

static int
add_val(Vlist *v, int bufsize, int nval, double field, time_t epoch)
{
	if (nval >= bufsize) {
		bufsize = bufsize * 2 + 1;
		if ((v->v = realloc(v->v, bufsize * sizeof(*v->v))) == NULL)
			perror("reallocating values buffer"), exit(1);
		if ((v->t = realloc(v->t, bufsize * sizeof(*v->t))) == NULL)
			perror("reallocating values buffer"), exit(1);
	}
	v->v[nval] = field;
	v->t[nval] = epoch;
	v->n = nval + 1;

	return bufsize;
}

/*
 * Add to each column the value on the current row.
 */
static int
add_row(Vlist *v, int bufsize, int ncol, int nval, char *line)
{
	time_t epoch;
	int bs;
	char *field, *dot;

	if ((field = strsep(&line, ",")) == NULL)
		fprintf(stderr, "%d: missing epoch\n", nval), exit(0);

	if ((dot = strchr(field, '.')) != NULL)
		*dot = '\0';
	epoch = eatol(field);
	for (; (field = strsep(&line, ",")) != NULL; ncol--, v++) {
		if (ncol <= 0)
			fprintf(stderr, "%d: too many fields\n", nval), exit(0);
		bs = add_val(v, bufsize, nval, eatof(field), epoch);
	}
	if (ncol > 0)
		fprintf(stderr, "%d: too few fields\n", nval), exit(0);

	return bs;
}

/*
 *       < ncol >
 * epoch,a1,b1,c1  ^
 * epoch,a2,b2,c2 nval
 * epoch,a3,b3,c3  v
 */
static void
read_values(Vlist *v, int ncol)
{
	int nval, bufsize;
	char line[LINE_MAX];

	bufsize = 0;
	for (nval = 0; esfgets(line, sizeof(line), stdin) != NULL; nval++)
		bufsize = add_row(v, bufsize, ncol, nval, line);
}

static void
usage(void)
{
	ColorList *c;

	fprintf(stderr, "usage: %s [-t title] [-u unit] {", argv0);
	fputs(colorlist->name, stderr);
	for (c = colorlist + 1; c->name != NULL; c++)
		fprintf(stderr, ",%s", c->name);
	fputs("}...\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	Vlist *v;
	char labels[LINE_MAX];

	ARGBEGIN {
	case 't':
		tflag = EARGF(usage());
		break;
	case 'u':
		uflag = EARGF(usage());
		break;
	default:
		usage();
	} ARGEND;

	if ((v = calloc(argc, sizeof(*v))) == NULL)
		perror("calloc value list"), exit(1);

	read_labels(v, argv, labels);
	read_values(v, argc);

	ffplot(v, argc, tflag, uflag);

	return 0;
}
