#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

/**/
int log_level;
void		vlogf			(int, char const *, char const *, va_list);
void		die			(int, char const *, ...);
void		error			(char const *, ...);
void		warn			(char const *, ...);
void		info			(char const *, ...);
void		debug			(char const *, ...);

#endif
