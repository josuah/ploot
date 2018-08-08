#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "arg.h"
#include "util.h"

#define WIDTH_MAX 1024
#define BRAILLE_START	10240

int wflag = 80;
int width;
char *argv0;

/*
 * Turn the bit at position (row, col) on in the .
 */
static void
plot_dot(long *out, int row, int col)
{
	long flags[4][2] = {
		{ 0x01, 0x08 },
		{ 0x02, 0x10 },
		{ 0x04, 0x20 },
		{ 0x40, 0x80 },
	};

	*out |= flags[row][col];;
}

static void
plot_val(long *out, double val, double max, int row)
{
	int col, c;

	val = MIN(max, val);
	col = (int)(val * (double)(width - 1) / max * 2);
	for (c = 0; c < col; c++)
		plot_dot(out + c / 2, row, c % 2);
}

/*
 * Change the braille characters on a whole row, this for all the
 * values line.
 */
static time_t
plot_row(long *out, char *line, double *max, int nrow, int ncol)
{
	time_t epoch;
	double val;
	int n;
	char *tok;

	if ((tok = strsep(&line, ",")) == NULL)
		fputs("*** missing epoch value\n", stderr), exit(1);
	epoch = eatol(tok);

	for (n = 0; (tok = strsep(&line, ",")) != NULL; n++) {
		if (n >= ncol)
			fputs("too many values\n", stderr), exit(1);
		val = atof(tok);
		plot_val(out + n * width, val, max[n], nrow);
	}
	if (n < ncol)
		fputs("not enough values\n", stderr), exit(1);

	return epoch;
}

/*
 * Read enough input in order to print one line and plot it into 'out'.
 */
static time_t
plot_line(long *out, double *max, int ncol)
{
	time_t epoch;
	int n, nrow;
	long *o, rune;
	char line[LINE_MAX];

	for (rune = BRAILLE_START, o = out, n = ncol * width; n > 0; o++, n--)
		memcpy(o, &rune, sizeof(rune));
	*o = '\0';
	for (rune = 0x2502, o = out, n = 0; n < ncol; o += width, n++)
		memcpy(o, &rune, sizeof(rune));
	out++;

	for (nrow = 0; nrow < 4; nrow++) {
		if ((esfgets(line, LINE_MAX, stdin)) == NULL)
			exit(0);
		epoch = plot_row(out, line, max, nrow, ncol);
	}

	return epoch;
}

static void
put_time(time_t epoch, time_t last, int nline)
{
	char *out, buf[sizeof("XXxXXxXX  ")];

	switch (nline % 3) {
	case 0:
		strftime(buf, sizeof(buf), "%H:%M:%S _", localtime(&epoch));
		out = buf;
		break;
	case 1:
		strftime(buf, sizeof(buf), "%y/%m/%d  ", localtime(&last));
		out = buf;
		break;
	case 2:
		out = "          ";
		break;
	}

	fputs(out, stdout);
}

static void
put_line(long *out)
{
	for (; *out != '\0'; out++)
		put3utf(*out);
	puts("│");
}

static void
plot(char labels[LINE_MAX], double *max, int ncol)
{
	time_t epoch, last_epoch;
	long out[WIDTH_MAX + 1];
	int n;

	last_epoch = epoch = 0;

	for (n = 0;; n = (n == 25 ? 0 : n + 1)) {
		if (n == 0)
			put_time(0, 0, 2), fputs(labels, stdout), puts("│");

		epoch = plot_line(out, max, ncol);
		put_time(epoch, last_epoch, n);
		last_epoch = epoch;
		put_line(out);

		fflush(stdout);
	}
}

/*
 * Label must be able to store all pointers to token buf has to
 * offer: sizeof(*buf / 2).
 */
static int
read_labels(char *labv[LINE_MAX])
{
	int ncol;
	char *l, line[LINE_MAX], *tok;

	if ((l = esfgets(line, LINE_MAX, stdin)) == NULL)
		fputs("missing label line\n", stderr), exit(1);

	if (strcmp(strsep(&l, ","), "epoch") != 0)
		fputs("first label must be \"epoch\"\n", stderr), exit(1);

	for (ncol = 0; (tok = strsep(&l, ",")) != NULL; ncol++, labv++)
		*labv = tok;
	*labv = NULL;

	if (ncol < 1)
		fputs("no label found\n", stderr), exit(1);

	return ncol;
}

static void
fmt_labels(char out[LINE_MAX], int ncol, char *labels[LINE_MAX / 2])
{
	int i, n;

	for (i = 0; i < ncol; labels++, i++) {
		n = LINE_MAX - (width + sizeof("│")) * i;
		out += snprintf(out, n, "│%-*s", width - 1, *labels);
	}
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-w width] maxval... <csv\n", argv0);
	exit(1);
}

static int
parse_args(int argc, char **argv, double *max)
{
	int n;

	ARGBEGIN {
	case 'w':
		wflag = atoi(EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND;

	if (argc == 0)
		usage();

	for (n = argc; n > 0; n--, argv++, max++)
		*max = eatof(*argv);

	return argc;
}

int
main(int argc, char **argv)
{
	double max[LINE_MAX / 2];
	int ncol, nmax;
	char *labv[LINE_MAX / 2], labels[LINE_MAX];

	setvbuf(stdin, NULL, _IOLBF, 0);
	nmax = parse_args(argc, argv, max);
	ncol = read_labels(labv);
	width = (wflag - sizeof("XXxXXxXX _")) / ncol - sizeof("|");
	fmt_labels(labels, ncol, labv);
	if (ncol != nmax)
		fputs("not as many labels and arguments\n", stderr), exit(1);
	plot(labels, max, ncol);

	return 0;
}
