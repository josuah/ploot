#include <stdio.h>
#include <string.h>

#include "config.h"

#define ABS(x)	((x) < 0 ? -(x) : (x))
#define LEN(x)	(sizeof(x) / sizeof(*x))

/*
 * Set `str' to a human-readable form of `num' with always a width of 7 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.
 */
void
humanize(char *str, double num)
{
	int	exp;
	char	*label = " kMGTE", fmt[] = "%+.?f%c";

	for (exp = 0; ABS(num) > 1000; exp++)
		num /= 1000;

	fmt[3] = (ABS(num) < 10) ? '3' : (ABS(num) < 100) ? '2' : '1';
	if (exp == 0) {
		fmt[5] = '\0';
		fmt[3]++;
	}
	snprintf(str, 8, fmt, num, label[exp]);
	if (num >= 0)
		str[0] = ' ';
}

/*
 * Print two rows of a plot into a single line using ' ', '.' and ':'.
 */
void
line(double *beg, double *end, double top, double bot)
{
	double	*val;

	for (val = beg; val <= end; val++)
		putchar((*val < bot) ? ' ' : (*val < top) ? '.' : ':');
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
 * Plot values between `beg' and `end' in a plot of height `height'.
 */
void
plot(int height, double *beg, double *end)
{
	double	top, bot, max;
	int	h;
/*
	char	label[8];
*/

	max = maxdv(beg, end);

	for (h = height + height % 2; h > 0; h -= 2) {
		top = h * max / height;
		bot = (h - 1) * max / height;
		line(beg, end, top, bot);
		putchar('\n');
	}
}

int
main()
{
	double	val[] = { 0.0, 0.4, 3.4, 2.1, 3.5, 3.0, 1.1, 2.0 } ;
	plot(10, val, val + LEN(val));
	return 0;
}
