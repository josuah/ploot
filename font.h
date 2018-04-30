#include <stddef.h>
#include <stdint.h>
#include "ffdraw.h"

/*
 * Macros to make the fonts header file more readable.
 */
#define _ 0
#define X 1
#define C(x) char glyph_ ## x[FONT_WIDTH * FONT_HEIGHT]

#define FONT(x) Font x = { FONT_WIDTH, FONT_HEIGHT, { \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, \
	glyph_space, glyph_bang, glyph_double, glyph_hash, \
	glyph_dollar, glyph_percent, glyph_ampersand, glyph_single, \
	glyph_l_round, glyph_r_round, glyph_asterisk, glyph_plus, \
	glyph_coma, glyph_minus, glyph_dot, glyph_slash, \
	glyph_0, glyph_1, glyph_2, glyph_3, \
	glyph_4, glyph_5, glyph_6, glyph_7, \
	glyph_8, glyph_9, glyph_column, glyph_semicolumn, \
	glyph_l_angle, glyph_equal, glyph_r_angle, glyph_question, \
	glyph_column, glyph_semicolumn, glyph_l_angle, glyph_equal, \
	glyph_r_angle, glyph_question \
} }
