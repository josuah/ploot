#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "arg.h"
#include "config.h"

#define ABS(x)		((x) < 0 ? -(x) : (x))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define LEN(buf)	(sizeof(buf) / sizeof(*(buf)))

char *argv0;

/*
** Add 'val' at the current position 'pos' of the 'ring' buffer and set pos to
** the next postion.
*/
#define RING_ADD(rbuf, len, pos, val)					\
do {									\
	rbuf[pos] = val;						\
	pos = (pos + 1 < len) ? (pos + 1) : (0);			\
} while (0)

/*
** Copy the ring buffer 'rbuf' content with current position 'pos' into the
** buffer 'buf'.  Both buffer of length 'len'.
*/
#define RING_COPY(buf, rbuf, len, pos)					\
do {									\
	memcpy(buf, rbuf + pos, (len - pos) * sizeof(*rbuf));		\
	memcpy(buf + (len - pos), rbuf, pos * sizeof(*rbuf));		\
} while (0)

#define MAX_VAL	80
#define MARGIN	7

int	hflag = 20;
char	*tflag = NULL;
time_t	oflag = 0;

/*
** Set 'str' to a human-readable form of 'num' with always a width of 7 (+ 1
** the '\0' terminator).  Buffer overflow is ensured not to happen due to the
** max size of a double.
*/
void
humanize(char *str, double val)
{
	int	exp, precision;
	char	*label = "\0kMGTE";

	for (exp = 0; ABS(val) > 1000; exp++)
		val /= 1000;

	precision = (ABS(val) < 10) ? (3) : (ABS(val) < 100) ? (2) : (1);
	if (exp == 0)
		precision++;
	snprintf(str, 8, "%+.*f%c", precision, val, label[exp]);
	str[7] = '\0';
	if (val >= 0)
		str[0] = ' ';
}

/*
** Returns the maximal double of values between 'beg' and 'end'.
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
** If not null, print the title 'str' centered on width.
*/
void
title(char *str, int width)
{
	if (str == NULL)
		return;
	printf("%*s\n", (int)(width + strlen(str)) / 2 + MARGIN + 3, str);
}

/*
** Print vertical axis with humanized number from time to time, with occurences
** determined after the position on the vertical axis from the bottom 'pos'.
*/
void
vaxis(double val, int pos)
{
	char	label[10];

	if (pos % 4 == 0) {
		humanize(label, val);
		printf("%*s -", MARGIN, label);
	} else {
		printf("%*c  ", MARGIN, ' ');
	}
}

/*
** Print horizontal axis for up to 'col' values along with dates if reading time
** series.
*/
void
haxis(double *beg, double *end, time_t time)
{
	double		*tp;
	char		buf[9], dbeg[11], dend[11];

	printf("%*d -+", MARGIN, 0);
	for (tp = beg; tp < end; tp++)
		putchar((*tp < 0) ? ('x') : ('-'));
	putchar('\n');
	if (oflag > 0) {
		printf("%*c", MARGIN - 1, ' ');
		strftime(dbeg, sizeof(dbeg), "%Y/%m/%d", localtime(&time));
		for (tp = beg; tp < end; tp += 7) {
			strftime(buf, sizeof(buf), "  %H:%M", localtime(&time));
			fputs(buf, stdout);
			time += oflag * 7;
		}
		strftime(dend, sizeof(dend), "%Y/%m/%d", localtime(&time));
		printf("\n     %-*s %s\n", (int)(beg - end) + 4, dbeg, dend);
	}
}

/*
** Print two rows of a plot into a single line using ' ', '.' and ':'.
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
** Plot values between 'beg' and 'end' in a plot of height 'height'.
** If 'str' is not NULL, it is set as a title above the graph.
*/
void
plot(double *beg, double *end, int height, char *str, time_t start)
{
	double	top, bot, max;
	int	h;

	putchar('\n');

	max = maxdv(beg, end);
	for (h = height + height % 2; h > 0; h -= 2) {
		top = h * max / height;
		bot = (h - 1) * max / height;

		vaxis(top, h);
		line(beg, end, top, bot);
	}

	haxis(beg, end, start);

	if (str != NULL)
		title(str, end - beg);

	putchar('\n');
}

/*
** Read a simple format with one double per line and save the last 'MAX_WIDTH'
** values into 'buf' which must be at least MAX_VAL wide and return a pointer
** to the last element or NULL if the input contains error.
*/
double *
read_simple(double buf[MAX_VAL])
{
	double	rbuf[MAX_VAL], val;
	size_t	p, pos, len;

	len = LEN(rbuf);
	for (p = pos = 0; scanf("%lf\n", &val) > 0; p++)
		RING_ADD(rbuf, len, pos, val);
	len = MIN(len, p);

	RING_COPY(buf, rbuf, len, pos);

	return buf + len;
}

/*
** Read a format with blank separated time_t-double pairs, one per line and save
** the last 'MAX_WIDTH' values into 'tbuf' and 'vbuf' which must both be at
** least MAX_VAL wide and return a pointer to the last element of 'vbuf' or
** NULL if the input contains error.
*/
time_t *
read_time_series(double *vbuf, time_t *tbuf)
{
	size_t	p, pos, nul, len;
	double	vrbuf[MAX_VAL], vval, dval;
	time_t	trbuf[MAX_VAL], tval;

	len = LEN(vrbuf);
	for (p = pos = 0; scanf("%lf %lf\n", &dval, &vval) > 0; p++) {
		tval = (time_t)dval;
		RING_ADD(trbuf, len, pos, tval);
		RING_ADD(vrbuf, len, nul, vval);
	}
	len = MIN(len, p);

	RING_COPY(tbuf, trbuf, len, pos);
	RING_COPY(vbuf, vrbuf, len, pos);

	return tbuf + len;
}

/*
** Walk from 'tbeg' and 'tend' and add offset in 'tbuf' every time there is no
** value in 'step' amount of time, by setting a value to -1.
*/
double *
skip_gaps(time_t *tbeg, time_t *tend, double *vbuf, time_t step)
{
	size_t	p, pos, len;
	time_t	*tp, toff;
	double	*vp, vrbuf[MAX_VAL];

	/* Compute the average alignment of the timestamps values according to
	** the step size. */
	toff = 0;
	for (tp = tbeg; tp < tend; tp++)
		toff += *tp % step;
	toff = *tbeg + toff / (tend - tbeg) + step / 2;

	/* Fill 'vbuf' with gap added at each time gap using vrbuf as
	** intermediate ring buffer. */
	len = LEN(vrbuf);
	for (p = pos = 0, tp = tbeg, vp = vbuf; tp < tend; p++, vp++, tp++) {
		for (; toff < *tp; toff += step)
			RING_ADD(vrbuf, len, pos, -1);
		RING_ADD(vrbuf, len, pos, *vp);
		toff += step;
	}
	len = MAX(MIN(p, len), pos);

	RING_COPY(vbuf, vrbuf, len, pos);

	return vbuf + len;
}

void
usage(void)
{
	printf("usage: ploot [-h <height>] [-o <offset>] [-t <title>]\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	time_t	tbuf[MAX_VAL], *tend, start;
	double	vbuf[MAX_VAL], *vend;

	ARGBEGIN(argc, argv) {
	case 'h':
		if ((hflag = atoi(EARGF(usage()))) <= 0)
			usage();
		break;
	case 't':
		tflag = EARGF(usage());
		break;
	case 'o':
		oflag = atol(EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND

	if (oflag == 0) {
		vend = read_simple(vbuf);
		start = 0;
	} else {
		tend = read_time_series(vbuf, tbuf);
		vend = skip_gaps(tbuf, tend, vbuf, oflag);
		start = *tbuf;
	}

	plot(vbuf, vend, hflag, tflag, start);
	return 0;
}
