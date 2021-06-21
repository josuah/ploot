#include "util.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const *arg0;

static void
_log(char const *fmt, va_list va)
{
	if (arg0 != NULL)
		fprintf(stderr, "%s: ", arg0);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	fflush(stderr);
}

void
err(int e, char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	_log( fmt, va);
	exit(e);
}

void
warn(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	_log(fmt, va);
}

void
debug(char const *fmt, ...)
{
	static int verbose = -1;
	va_list va;

	if (verbose < 0)
		verbose = (getenv("DEBUG") != NULL);
	if (!verbose)
		return;
	va_start(va, fmt);
	_log(fmt, va);
}

size_t
strlcpy(char *buf, const char *str, size_t sz)
{
	size_t len, cpy;

	cpy = ((len = strlen(str)) > sz) ? (sz) : (len);
	memcpy(buf, str, cpy);
	buf[sz - 1] = '\0';
	return len;
}

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
	char *s, *prev;

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
strchomp(char *s)
{
	char *x = s + strlen(s);

	while (--x >= s && (*x == '\r' || *x == '\n'))
		*x = '\0';
}

/*
 * Set 'str' to a human-readable form of 'num' with always a width of 8 (+1 for
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
