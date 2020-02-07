#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>

#include "def.h"

void
put3utf(long rune)
{
	putchar((char)(0xe0 | (0x0f & (rune >> 12))));	/* 1110xxxx */
	putchar((char)(0x80 | (0x3f & (rune >> 6))));	/* 10xxxxxx */
	putchar((char)(0x80 | (0x3f & (rune))));	/* 10xxxxxx */
}

char *
strsep(char **strp, const char *sep)
{
	char	*s, *prev;

	if (*strp == NULL)
		return NULL;
	for (s = prev = *strp; strchr(sep, *s) == NULL; s++);
	if (*s == '\0') {
		*strp = NULL;
		return prev;
	}
	*s = '\0';
	*strp = s + 1;

	return prev;
}

void
estriplf(char *line)
{
	char *lf;

	if ((lf = strchr(line, '\n')) == NULL || lf[1] != '\0')
		fputs("invalid input\n", stderr), exit(1);
	*lf = '\0';
}

double
eatof(char *str)
{
	char *s;

	for (s = str; *s != '\0'; s++)
		if (!isdigit(*s) && *s != '-' && *s != '.')
			fputs("invalid float format\n", stderr), exit(1);
	return atof(str);
}

long
eatol(char *str)
{
	char *s;

	for (s = str; *s != '\0'; s++)
		if (!isdigit(*s) && *s != '-')
			fputs("invalid number format\n", stderr), exit(1);
	return atol(str);
}

char *
esfgets(char *buf, size_t n, FILE *file)
{
	if (fgets(buf, n, file) == NULL) {
		if (ferror(stdin))
			perror("fread from stdin"), exit(1);
		else
			return NULL;
	}
	estriplf(buf);
	return buf;
}

/*
 * Set 'str' to a human-readable form of 'num' with always a width of 8 (+ 1
 * the '\0' terminator).  Buffer overflow is ensured not to happen due to the
 * max size of a double.  Return the exponent.
 */
int
humanize(char *str, double val)
{
	int exp, precision;
	char label[] = { '\0', 'M', 'G', 'T', 'E' };

	for (exp = 0; ABS(val) > 1000; exp++)
		val /= 1000;

	precision = (ABS(val) < 10) ? 2 : (ABS(val) < 100) ? 1 : 0;
	precision += (exp == 0);

	snprintf(str, 9, "%+.*f %c", precision, val, label[exp]);
	str[8] = '\0';
	if (val >= 0)
		str[0] = ' ';

	return exp * 3;
}
