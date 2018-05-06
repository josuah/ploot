#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>

#include "ploot.h"

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
			fputs("invalid float format\n", stderr), exit(0);
	return atof(str);
}

long
eatol(char *str)
{
	char *s;

	for (s = str; *s != '\0'; s++)
		if (!isdigit(*s) && *s != '-')
			fputs("invalid number format\n", stderr), exit(0);
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
