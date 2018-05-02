#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "arg.h"
#include "ploot.h"
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
estriplf(char *line)
{
	char *lf;

	if ((lf = strchr(line, '\n')) == NULL || lf[1] != '\0')
		fputs("invalid input\n", stderr), exit(1);
	*lf = '\0';
}

static void
read_labels(Vlist *v, char **argv, char *buf)
{
	if (fgets(buf, LINE_MAX, stdin) == NULL) {
		if (ferror(stdin))
			perror("fread from stdin");
		else
			fputs("missing label line\n", stderr);
		exit(1);
	}
	estriplf(buf);

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

static double
eatof(char *str)
{
	char *s;

	for (s = str; *s != '\0'; s++)
		if (!isdigit(*s) && *s != '-' && *s != '.')
			fputs("invalid float format\n", stderr), exit(0);
	return atof(str);
}

static long
eatol(char *str)
{
	char *s;

	for (s = str; *s != '\0'; s++)
		if (!isdigit(*s) && *s != '-')
			fputs("invalid number format\n", stderr), exit(0);
	return atol(str);
}

static void
add_val(Vlist *v, int *bufsiz, int nval, double field, time_t epoch)
{
	if (nval >= *bufsiz) {
		*bufsiz = *bufsiz * 2 + 1;
		if ((v->v = realloc(v->v, *bufsiz * sizeof(*v->v))) == NULL)
			perror("reallocating values buffer"), exit(1);
		if ((v->t = realloc(v->t, *bufsiz * sizeof(*v->t))) == NULL)
			perror("reallocating values buffer"), exit(1);
	}
	v->v[nval] = field;
	v->t[nval] = epoch;
	v->n = nval + 1;
}

/*
 * Add to each column the value on the current row.
 */
static void
add_row(Vlist *v, int *bufsiz, int ncol, int nval, char *line)
{
	time_t epoch;
	int n;
	char *field, *dot;

	if ((field = strsep(&line, ",")) == NULL)
		fprintf(stderr, "%d: missing epoch\n", nval), exit(0);

	if ((dot = strchr(field, '.')) != NULL)
		*dot = '\0';
	epoch = eatol(field);
	for (n = 0; (field = strsep(&line, ",")) != NULL; n++, v++) {
		if (n > ncol)
			fprintf(stderr, "%d: too many fields\n", nval), exit(0);
		add_val(v, bufsiz, nval, eatof(field), epoch);
	}
	if (n < ncol)
		fprintf(stderr, "%d: too few fields\n", nval), exit(0);
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
	int nval, bufsiz;
	char line[LINE_MAX];

	bufsiz = 0;
	for (nval = 0; fgets(line, sizeof(line), stdin); nval++) {
		estriplf(line);
		add_row(v, &bufsiz, ncol, nval, line);
	}
}

static void
usage(void)
{
	ColorList *c;

	fprintf(stderr, "usage: %s [-t title] [-u unit] color...\n"
		"available colors as defined by \"config.h\":\n", argv0);
	for (c = colorlist; c->name != NULL; c++)
		fprintf(stderr, "- %s\n", c->name);
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
	} ARGEND;

	if ((v = calloc(argc, sizeof(*v))) == NULL)
		perror("calloc value list"), exit(1);

	read_labels(v, argv, labels);
	read_values(v, argc);

	ffplot(v, argc, tflag, uflag);

	return 0;
}
