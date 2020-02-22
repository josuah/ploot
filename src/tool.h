#ifndef TOOL_H
#define TOOL_H

#include <stddef.h>

#define LEN(x)		(sizeof(x) / sizeof(*x))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define ABS(x)		((x) < 0 ? -(x) : (x))

/**/
size_t		strlcpy			(char *, const char *, size_t);
void		put3utf			(long);
char *		strsep			(char **, const char *);
void		estriplf		(char *);
double		eatof			(char *);
long		eatol			(char *);
int		humanize		(char *, double);

#endif
