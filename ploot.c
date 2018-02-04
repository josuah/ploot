#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

#define MAX_VAL	80
#define MARGIN	7

#define ABS(x)		((x) < 0 ? -(x) : (x))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define LEN(x)		(sizeof(x) / sizeof(*x))

/*
 * Set `str' to a human-readable form of `num' with always a width of 7 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.
 */
void
humanize(char *str, double val)
{
	int	exp, precision;
	char	*label = "\0kMGTE";

	for (exp = 0; ABS(val) > 1000; exp++)
		val /= 1000;

	precision = (ABS(val) < 10) ? 3 : (ABS(val) < 100) ? 2 : 1;
	if (exp == 0)
		precision++;
	snprintf(str, 8, "%+.*f%c", precision, val, label[exp]);
	if (val >= 0)
		str[0] = ' ';
}

/*
 * Returns the maximal double of values between `beg' and `end'.
 */
double
maxdv(double *beg, double *end)
{
	double	*val, max;

	max = *beg;
	for (val = beg; val < end; val++) {
		if (*val > max)
			max = *val;
	}
	return max;
}

/*
 * If not null, print the title `str' centered on width.
 */
void
title(char *str, int width)
{
	if (str == NULL)
		return;
	printf("%*s\n", (int)(width + strlen(str) + MARGIN) / 2, str);
}

/*
 * Print vertical axis with humanized number from time to time, with occurences
 * determined after the position on the vertical axis from the bottom `pos'.
 */
void
vaxis(double val, int pos)
{
	char	label[8];

	if (pos % 4 == 0) {
		humanize(label, val);
		printf("%*s -", MARGIN, label);
	} else {
		printf("%*c  ", MARGIN, ' ');
	}
}

/*
 * Print horizontal axis for up to `col' values.
 */
void
haxis(int col)
{
	printf("%*d -+", MARGIN, 0);
	while (col-- > 0)
		putchar('-');
	putchar('\n');
}

/*
 * Print two rows of a plot into a single line using ' ', '.' and ':'.
 */
void
line(double *beg, double *end, double top, double bot)
{
	double	*val;

	putchar('|');
	for (val = beg; val != end; val++)
		putchar((*val < bot) ? ' ' : (*val < top) ? '.' : ':');
	putchar('\n');
}

/*
 * Plot values between `beg' and `end' in a plot of height `height'.
 * If `str' is not NULL, it is set as a title above the graph.
 */
void
plot(int height, double *beg, double *end, char *str)
{
	double	top, bot, max;
	int	h;

	if (str != NULL)
		title(str, end - beg);

	max = maxdv(beg, end);
	for (h = height + height % 2; h > 0; h -= 2) {
		top = h * max / height;
		bot = (h - 1) * max / height;

		vaxis(top, h);
		line(beg, end, top, bot);
	}
	haxis(end - beg);
}

/*
 * Add `val' at the current position `pos' of the `ring' buffer and return the
 * next postion.
 */
size_t
ring_add(double *rbuf, size_t len, size_t pos, double val)
{
	*rbuf = val;

	return (pos < len) ? pos + 1 : 0;
}

/*
 * Copy the ring buffer `rbuf' content with current position `pos' into the
 * buffer `buf'.  Both buffer of length `len'.
 */
void
ring_copy(double *buf, double *rbuf, size_t len, size_t pos)
{
	size_t	i = 0;

	memcpy(buf, rbuf + pos, (len - pos) * sizeof(*rbuf));
	memcpy(buf + (len - pos), rbuf, pos * sizeof(*rbuf));
	printf("len: %zd, pos: %zd\n", len, pos);
	for (i = 0; i < len; i++)
		printf("%03zd: %lf\n", i, buf[i]);
}

/*
 * Read a simple format with one double per line and save the last `MAX_WIDTH'
 * values into `buf' which must be at least MAX_VAL wide and return a pointer
 * to the last element or NULL if the input contains error.
 */
double *
read_simple(double buf[MAX_VAL])
{
	double	rbuf[MAX_VAL], val;
	size_t	p, pos, len;

	len = LEN(rbuf);
	for (p = pos = 0; scanf("%lf\n", &val) > 0; p++)
		pos = ring_add(rbuf, len, pos, val);
	len = MIN(len, p);
	pos = MIN(pos, p);

	ring_copy(buf, rbuf, len, pos);

	return buf + len;
}

/*
 * Read a format with blank-separated time_t-double pairs, one per line and save
 * the last `MAX_WIDTH' values into `timev' and `valv' which must both be at
 * least MAX_VAL wide and return a pointer to the last element or NULL if the
 * input contains error.
 */
double *
read_time_series(double *valv, time_t *timev)
{
	time_t	time_rbuf[MAX_VAL];
	double	val_rbuf[MAX_VAL];

	(void)time_rbuf;
	(void)val_rbuf;
	(void)timev;

	return valv;
}

void
usage(void)
{
	printf("usage: ploot [-h height]\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	double	val[MAX_VAL], *end;
	char	c;

	int	flag_h = 20;
	char	*flag_t = NULL;

	while ((c = getopt(argc, argv, "h:t:")) != -1) {
		switch (c) {
		case -1:
			break;
		case 'h':
			if ((flag_h = atoi(optarg)) <= 0)
				usage();
			break;
		case 't':
			flag_t = optarg;
			break;
		default:
			usage();
		}
	}

	end = read_simple(val);
	plot(flag_h, val, end, flag_t);
	return 0;
}
