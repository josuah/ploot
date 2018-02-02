#include <stdio.h>

#define ABS(x)	((x) < 0 ? -(x) : (x))

/*
 * Set `str' to a human-readable form of `num' with always a width of 8
 * (including '\0' terminator).
 */
void
humanize(double num, char *str, size_t len)
{
	int	exp;
	char	*label = " kMGTE", fmt[] = "%+.?f%c";

	for (exp = 0; ABS(num) > 1000; exp += 3)
		num /= 1000;

	fmt[3] = (ABS(num) < 10) ? '3' : (ABS(num) < 100) ? '2' : '1';
	if (exp == 0) {
		fmt[5] = '\0';
		fmt[3]++;
	}
	snprintf(str, len, fmt, num, label[exp / 3]);
	if (num > 0)
		str[0] = ' ';
}

int
main()
{
	char	str[8];

	humanize(-1 << 18, str, sizeof(str));
	printf("%s\n", str);

	return 0;
}
