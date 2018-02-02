#include <stdio.h>
#include <string.h>

#include "config.h"

#define MARGIN	9

#define ABS(x)	((x) < 0 ? -(x) : (x))
#define LEN(x)	(sizeof(x) / sizeof(*x))

/*
 * Set `str' to a human-readable form of `num' with always a width of 7 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.
 */
void
humanize(char *str, double val)
{
	int	exp;
	char	*label = " kMGTE", fmt[] = "%+.?f%c";

	for (exp = 0; ABS(val) > 1000; exp++)
		val /= 1000;

	fmt[3] = (ABS(val) < 10) ? '3' : (ABS(val) < 100) ? '2' : '1';
	if (exp == 0) {
		fmt[5] = '\0';
		fmt[3]++;
	}
	snprintf(str, 8, fmt, val, label[exp]);
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
	printf("%*s\n\n", (int)(width - strlen(str)) / 2 + MARGIN, str);
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
		printf("%s -", label);
	} else {
		printf("         ");
	}
}

/*
 * Print horizontal axis for up to `col' values.
 */
void
haxis(int col)
{
	printf("%*d -+", MARGIN - 2, 0);
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

void
read_simple()
{
	;
}

int
main()
{
	double	val[] = { 1000, 3030, 3000, 2456, 3005, 3000, 1031, 2000, 3345,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    5000, 3444, 1034, 1833, 2452, 2555, 432, 2456, 435, 1646, 435, 346,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    1833, 2452, 1456, 435, 435, 554, 5456, 1034, 2452, 432, 1435, 1646,
	    1000, 1833, 2452, 432, 1456, 435, 1646, 435, 1345, 554, 5245, 3456,
	    1456, 3498, 834, 834, 804, 234, 544, 3456, 2984, 983, 2583, 2583 };

	plot(30, val, val + LEN(val), "title");
	return 0;
}
