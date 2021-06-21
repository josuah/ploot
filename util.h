#ifndef TOOL_H
#define TOOL_H

#include <stddef.h>

#define LEN(x) (sizeof(x) / sizeof(*x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) < 0 ? -(x) : (x))

extern char const *arg0;

void	 err(int, char const *fmt, ...);
void	 warn(char const *fmt, ...);
void	 debug(char const *fmt, ...);
size_t	 strlcpy(char *, const char *, size_t);
void	 put3utf(long);
char	*strsep(char **, const char *);
void	 strchomp(char *);
int	 humanize(char *, double);

#endif
