#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "arg.h"
#include "util.h"

#define LEN(x) (sizeof(x) / sizeof(*x))

#define WIDTH_MAX 1024

int screenwidth = 80;

char *argv0;

static void
usage(void)
{
	fprintf(stderr, "usage: %s <csv\n", argv0);
	exit(1);
}

void
fmt_labels(char out[LINE_MAX], int ncol, char *labels[LINE_MAX / 2])
{
	int i, w;

	w = screenwidth / ncol;
	for (i = 0; i < ncol; labels++, i++)
		out += snprintf(out, w - 1, " %.*s", w - 1, *labels);
}

/*
 * Label must be able to store all pointers to token buf has to
 * offer: sizeof(*buf / 2).
 */
static int
read_labels(char out[LINE_MAX])
{
	int ncol;
	char *l, line[LINE_MAX], *labels[LINE_MAX / 2], *tok;

	l = line;
	if (esfgets(line, LINE_MAX, stdin) == NULL)
		fputs("missing label line\n", stderr), exit(1);

	if (strcmp(strsep(&l, ","), "epoch") != 0)
		fputs("first label must be \"epoch\"\n", stderr), exit(1);

	for (ncol = 1; (tok = strsep(&l, ",")) != NULL; ncol++)
		*labels = tok;
	*labels = NULL;

	if (ncol < 2)
		fputs("no label found\n", stderr), exit(1);

	fmt_labels(out, ncol, labels);

	return ncol;
}

void
plot_val(char *out, double val, int nrow, int width)
{
	(void)val;
	(void)out;
	(void)nrow;
	(void)width;
}

/*
 * Change the braille characters on a whole row, this for all the
 * values line.
 */
time_t
plot_row(char *out, char *line, int nrow, int ncol, int width)
{
	time_t epoch;
	double val;
	int n;
	char *tok;

	if ((tok = strsep(&line, ",")) == NULL)
		fputs("*** missing epoch value\n", stderr), exit(1);
	epoch = eatol(tok);

	for (n = 1; (tok = strsep(&line, ",")) != NULL; n++) {
		if (n >= ncol)
			fputs("too many values\n", stderr), exit(1);
		val = eatof(tok);
		plot_val(out + n * width, nrow, val, width);
	}
	if (n < ncol)
		fputs("not enough values\n", stderr), exit(1);

	return epoch;
}

/*
 * Read enough input in order to print one line and plot it into 'out'.
 */
time_t
plot_line(char *out, int ncol, int width)
{
	time_t epoch;
	int nrow;
	char line[LINE_MAX];

	for (nrow = 0; nrow < 4; nrow++) {
		if ((esfgets(line, LINE_MAX, stdin)) == NULL)
			exit(0);
		epoch = plot_row(out, line, nrow, ncol, width);
	}

	return epoch;
}

void
put_time(time_t epoch, time_t last, int nline)
{
	char *out, buf[sizeof("XXxXXxXX  |")];

	switch (nline % 3) {
	case 0:
		strftime(buf, sizeof(buf), "%H:%M:%S _|", localtime(&epoch));
		out = buf;
		break;
	case 1:
		strftime(buf, sizeof(buf), "%y/%m/%d  |", localtime(&last));
		out = buf;
		break;
	case 2:
		out = "          |";
		break;
	}

	fputs(out, stdout);
}

void
plot(char labels[LINE_MAX], int ncol)
{
	time_t epoch, last_epoch;
	int n, width;
	char out[WIDTH_MAX * 3 + 1];

	width = screenwidth / ncol;
	last_epoch = epoch = 0;

	for (n = 0;; n++) {
		if (n >= 20) {
			puts(labels);
			n = 0;
		}

		epoch = plot_line(out, ncol, width);
		put_time(epoch, last_epoch, n);
		last_epoch = epoch;
		puts(out);

		fflush(stdout);
	}
}

void
parse_args(int argc, char **argv)
{
	argv0 = *argv;
	if (argc != 1)
		usage();
}

int
main(int argc, char **argv)
{
	char labels[LINE_MAX];
	int ncol;

	parse_args(argc, argv);
	ncol = read_labels(labels);
	plot(labels, ncol);

	return 0;
}
