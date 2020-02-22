#include "log.h"

#include <string.h>

/*
 * log.c - log to standard error according to the log level
 *
 * Instead of logging to syslog, delegate logging to a separate
 * tool, such as FreeBSD's daemon(8), POSIX's logger(1).
 *
 * log_init() sets the log level to the "LOG" environment variable
 * if set, or to 4 (log down to info included) otherwise.
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_DEFAULT 3

int log_level = -1;

void
vlogf(int exitcode, int level, char const *flag, char const *fmt, va_list va)
{
	char *env;

	if (log_level == -1) {
		env = getenv("LOG");
		log_level = env ? atoi(env) : 0;
		log_level = log_level > 0 ? log_level : LOG_DEFAULT;
	}

	if (log_level < level)
		goto end;

	fprintf(stderr, "%s: ", flag);
	vfprintf(stderr, fmt, va);

	if (errno)
		fprintf(stderr, ": %s", strerror(errno));
	errno = 0;

	fprintf(stderr, "\n");
	fflush(stderr);
end:
	if (exitcode)
		exit(exitcode);
}

void
fatal(int exitcode, char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlogf(exitcode, 0, "fatal", fmt, va);
	va_end(va);
}

void
error(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlogf(0, 1, "error", fmt, va);
	va_end(va);
}

void
warn(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlogf(0, 2, "warn", fmt, va);
	va_end(va);
}

void
info(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlogf(0, 3, "info", fmt, va);
	va_end(va);
}

void
debug(char const *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vlogf(0, 4, "debug", fmt, va);
	va_end(va);
}
