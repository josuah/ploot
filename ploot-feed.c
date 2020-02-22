#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "tool.h"
#include "log.h"

#define WIDTH_MAX 1024
#define BRAILLE_START	10240

char const *arg0 = NULL;
static int wflag = 80;
static int width = 0;

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

	tok = strsep(&line, ",");
	if (!tok)
		fatal(100, "*** missing epoch value");
	epoch = strtol(tok, NULL, 10);
	if (errno)
		error("*** parsing epoch '%s'", tok);

	for (n = 0; (tok = strsep(&line, ",")) != NULL; n++) {
		if (n >= ncol)
			fatal(100, "too many values");
		val = atof(tok);
		plot_val(out + n * width, val, max[n], nrow);
	}
	if (n < ncol)
		fatal(100, "not enough values");

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
	char *line;
	size_t sz;

	for (rune = BRAILLE_START, o = out, n = ncol * width; n > 0; o++, n--)
		memcpy(o, &rune, sizeof(rune));
	*o = '\0';
	for (rune = 0x2502, o = out, n = 0; n < ncol; o += width, n++)
		memcpy(o, &rune, sizeof(rune));
	out++;

	line = NULL, sz = 0;
	for (nrow = 0; nrow < 4; nrow++) {
		if (getline(&line, &sz, stdin) == -1) {
			if (ferror(stdin))
				fatal(111, "reading row from stdin");
			exit(0);
		}
		epoch = plot_row(out, line, max, nrow, ncol);
	}

	free(line);
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
plot(char labels[4069], double *max, int ncol)
{
	time_t epoch, last_epoch;
	long out[WIDTH_MAX + 1];
	int n;

	last_epoch = epoch = 0;

	for (n = 0;; n = (n == 25 ? 0 : n + 1)) {
		if (n == 0) {
			put_time(0, 0, 2);
			fputs(labels, stdout);
			puts("│");
		}

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
read_labels(char **labv)
{
	int ncol;
	char *cp, *line, *tok;
	size_t sz;

	line = NULL, sz = 0;
	if (getline(&line, &sz, stdin) == -1) {
		if (ferror(stdin))
			fatal(111, "reading labels from stdin");
		fatal(100, "missing label line", stderr);
	}
	strchomp(line);
	cp = line;

	if (strcmp(strsep(&cp, ","), "epoch") != 0)
		fatal(100, "first label must be 'epoch'");

	for (ncol = 0; (tok = strsep(&cp, ",")) != NULL; ncol++, labv++)
		*labv = tok;
	*labv = NULL;

	if (ncol < 1)
		fatal(100, "no label found");
	return ncol;
}

static void
fmt_labels(char out[4069], int ncol, char *labels[4069 / 2])
{
	int i, n;

	for (i = 0; i < ncol; labels++, i++) {
		n = 4069 - (width + sizeof("│")) * i;
		out += snprintf(out, n, "│%-*s", width - 1, *labels);
	}
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-w width] maxval... <csv\n", arg0);
	exit(1);
}

int
main(int argc, char **argv)
{
	double max[4069 / 2], *m;
	int ncol, nmax;
	char *labv[4069 / 2], labels[4069];
	int c;

	optind = 0;
	while ((c = getopt(argc, argv, "w:")) > -1) {
		switch (c) {
		case 'w':
			wflag = atoi(optarg);
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	nmax = argc;
	for (m = max; argc > 0; argc--, argv++, m++) {
		*m = strtod(*argv, NULL);
		if (errno)
			error("*** parsing float '%s'", *argv);
	}

	ncol = read_labels(labv);
	width = (wflag - sizeof("XXxXXxXX _")) / ncol - sizeof("|");
	fmt_labels(labels, ncol, labv);
	if (ncol != nmax)
		fatal(100, "not as many labels and arguments");
	plot(labels, max, ncol);

	return 0;
}
