#include <stddef.h>
#include <stdint.h>

#include "ploot.h"

/*
 * Macros to make the fonts header file more readable.
 */
#define _ 0
#define X 1
#define C(x) static char glyph_ ## x[FONT_WIDTH * FONT_HEIGHT]

#define FONT(x) Font x = { FONT_WIDTH, FONT_HEIGHT, { \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_error, glyph_error, glyph_error, glyph_error, \
	glyph_space, glyph_bang, glyph_double, glyph_hash, \
	glyph_dollar, glyph_percent, glyph_ampersand, glyph_single, \
	glyph_l_round, glyph_r_round, glyph_asterisk, glyph_plus, \
	glyph_coma, glyph_minus, glyph_dot, glyph_slash, \
	glyph_0, glyph_1, glyph_2, glyph_3, \
	glyph_4, glyph_5, glyph_6, glyph_7, \
	glyph_8, glyph_9, glyph_column, glyph_semicolumn, \
	glyph_l_angle, glyph_equal, glyph_r_angle, glyph_question, \
	glyph_at, glyph_A, glyph_B, glyph_C, \
	glyph_D, glyph_E, glyph_F, glyph_G, \
	glyph_H, glyph_I, glyph_J, glyph_K, \
	glyph_L, glyph_M, glyph_N, glyph_O, \
	glyph_P, glyph_Q, glyph_R, glyph_S, \
	glyph_T, glyph_U, glyph_V, glyph_W, \
	glyph_X, glyph_Y, glyph_Z, glyph_l_square, \
	glyph_backslash, glyph_r_square, glyph_hat, glyph_underscore, \
	glyph_backtilt, glyph_a, glyph_b, glyph_c, \
	glyph_d, glyph_e, glyph_f, glyph_g, \
	glyph_h, glyph_i, glyph_j, glyph_k, \
	glyph_l, glyph_m, glyph_n, glyph_o, \
	glyph_p, glyph_q, glyph_r, glyph_s, \
	glyph_t, glyph_u, glyph_v, glyph_w, \
	glyph_x, glyph_y, glyph_z, glyph_l_curly, \
	glyph_pipe, glyph_r_curly, glyph_tilde, glyph_error \
} }
