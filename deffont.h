#include <stddef.h>

#define _ 0
#define X 1
#define B(x) char glyph_ ## x[WIDTH * HEIGHT]
#define b(x) glyph_ ## x
#define NOPR NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
#define NOPRINT NOPR, NOPR, NOPR, NOPR

#define FONT(x) char *font_ ## x[] = { \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	b(space), b(bang), b(double), b(hash), \
	b(dollar), b(percent), b(ampersand), b(single), \
	b(l_round), b(r_round), b(asterisk), b(plus), \
	b(coma), b(minus), b(dot), b(slash), \
	b(0), b(1), b(2), b(3), \
	b(4), b(5), b(6), b(7), \
	b(8), b(9), b(column), b(semicolumn), \
	b(l_angle), b(equal), b(r_angle), b(question), \
	b(column), b(semicolumn), b(l_angle), b(equal), \
	b(r_angle), b(question) \
}
