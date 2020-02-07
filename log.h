#ifndef LOG_H
#define LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const *arg0;  /* Should be set by the library caller. */

static inline void
vlog(char const *base, char const *fmt, va_list va)
{
	fprintf(stderr, "%s: ", base);
	vfprintf(stderr, fmt, va);
	if (errno)
		fprintf(stderr, ": %s", strerror(errno));
	fputc('\n', stderr);
	fflush(stderr);
	errno = 0;  /* avoid repeating the error in loop */
}

static inline void
warn(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlog(arg0, fmt, va);
	va_end(va);
}

static inline void
err(int e, char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlog(arg0, fmt, va);
	va_end(va);
	exit(e);
}

#endif
