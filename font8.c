#include "font.h"

#define C(x)	static char glyph_##x[]
#define _	2
#define X	3

C(error) = {
	X,X,X,X,
	X,X,X,X,
	X,X,X,X,
	X,X,X,X,
	X,X,X,X,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(A) = {
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(B) = {
	X,X,X,_,
	X,_,_,X,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(C) = {
	_,X,X,X,
	X,_,_,_,
	X,_,_,_,
	X,_,_,_,
	X,_,_,_,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(D) = {
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(E) = {
	X,X,X,X,
	X,_,_,_,
	X,X,X,_,
	X,_,_,_,
	X,_,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(F) = {
	X,X,X,X,
	X,_,_,_,
	X,_,_,_,
	X,X,X,_,
	X,_,_,_,
	X,_,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(G) = {
	_,X,X,X,
	X,_,_,_,
	X,_,_,_,
	X,_,X,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(H) = {
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(I) = {
	X,X,X,
	_,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	X,X,X,
	_,_,_,
	_,_,_,
0};

C(J) = {
	_,X,X,X,
	_,_,X,_,
	_,_,X,_,
	_,_,X,_,
	_,_,X,_,
	X,X,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(K) = {
	X,_,_,X,
	X,_,X,_,
	X,X,_,_,
	X,X,_,_,
	X,_,X,_,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(L) = {
	X,_,_,_,
	X,_,_,_,
	X,_,_,_,
	X,_,_,_,
	X,_,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(M) = {
	X,_,_,_,X,
	X,X,_,X,X,
	X,_,X,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(N) = {
	X,_,_,X,
	X,X,_,X,
	X,X,_,X,
	X,_,X,X,
	X,_,X,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(O) = {
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(P) = {
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	X,_,_,_,
	X,_,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(Q) = {
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,X,X,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(R) = {
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	X,_,X,_,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(S) = {
	_,X,X,X,
	X,_,_,_,
	_,X,_,_,
	_,_,X,_,
	_,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(T) = {
	X,X,X,X,X,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(U) = {
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(V) = {
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	X,_,X,_,
	X,_,X,_,
	X,X,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(W) = {
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(X) = {
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(Y) = {
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,X,_,_,
	X,_,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(Z) = {
	X,X,X,X,
	_,_,_,X,
	_,_,X,_,
	_,X,_,_,
	X,_,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(a) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(b) = {
	X,_,_,_,
	X,_,_,_,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(c) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,_,_,_,
	X,_,_,_,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(d) = {
	_,_,_,X,
	_,_,_,X,
	_,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(e) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,X,X,X,
	X,_,_,_,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(f) = {
	_,X,X,
	X,_,_,
	X,_,_,
	X,X,_,
	X,_,_,
	X,_,_,
	_,_,_,
	_,_,_,
0};

C(g) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,X,
	_,X,X,_,
0};

C(h) = {
	X,_,_,_,
	X,_,_,_,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(i) = {
	_,X,_,
	_,_,_,
	X,X,_,
	_,X,_,
	_,X,_,
	_,X,X,
	_,_,_,
	_,_,_,
0};

C(j) = {
	_,X,_,
	_,_,_,
	X,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	X,_,_,
0};

C(k) = {
	X,_,_,_,
	X,_,_,_,
	X,_,_,X,
	X,_,X,_,
	X,X,X,_,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(l) = {
	X,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	X,X,X,
	_,_,_,
	_,_,_,
0};

C(m) = {
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(n) = {
	_,_,_,_,
	_,_,_,_,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(o) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(p) = {
	_,_,_,_,
	_,_,_,_,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	X,X,X,_,
	X,_,_,_,
	X,_,_,_,
0};

C(q) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,X,
	_,_,_,X,
0};

C(r) = {
	_,_,_,_,
	_,_,_,_,
	X,_,X,X,
	X,X,_,_,
	X,_,_,_,
	X,_,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(s) = {
	_,_,_,_,
	_,_,_,_,
	_,X,X,X,
	X,X,_,_,
	_,_,X,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(t) = {
	X,_,_,
	X,_,_,
	X,X,X,
	X,_,_,
	X,_,_,
	_,X,X,
	_,_,_,
	_,_,_,
0};

C(u) = {
	_,_,_,_,
	_,_,_,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(v) = {
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(w) = {
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(x) = {
	_,_,_,_,
	_,_,_,_,
	X,_,_,X,
	_,X,X,_,
	_,X,X,_,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(y) = {
	_,_,_,_,
	_,_,_,_,
	X,_,_,X,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,X,
	_,X,X,_,
0};

C(z) = {
	_,_,_,_,
	_,_,_,_,
	X,X,X,X,
	_,_,X,_,
	_,X,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(0) = {
	_,X,X,_,
	X,_,_,X,
	X,_,X,X,
	X,X,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(1) = {
	_,X,_,
	X,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	X,X,X,
	_,_,_,
	_,_,_,
0};

C(2) = {
	_,X,X,_,
	X,_,_,X,
	_,_,_,X,
	_,_,X,_,
	_,X,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(3) = {
	X,X,X,_,
	_,_,_,X,
	_,X,X,X,
	_,_,_,X,
	_,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(4) = {
	_,_,X,X,
	_,X,_,X,
	X,_,_,X,
	X,X,X,X,
	_,_,_,X,
	_,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(5) = {
	X,X,X,X,
	X,_,_,_,
	X,X,X,_,
	_,_,_,X,
	_,_,_,X,
	X,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(6) = {
	_,X,X,_,
	X,_,_,_,
	X,X,X,_,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(7) = {
	X,X,X,X,
	_,_,_,X,
	_,_,X,_,
	_,_,X,_,
	_,X,_,_,
	_,X,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(8) = {
	_,X,X,_,
	X,_,_,X,
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(9) = {
	_,X,X,_,
	X,_,_,X,
	X,_,_,X,
	_,X,X,X,
	_,_,_,X,
	_,X,X,_,
	_,_,_,_,
	_,_,_,_,
0};

C(space) = {
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
0};

C(exclam) = {
	_,X,_,
	_,X,_,
	_,X,_,
	_,X,_,
	_,_,_,
	_,X,_,
	_,_,_,
	_,_,_,
0};

C(dquote) = {
	X,_,X,
	X,_,X,
	X,_,X,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
0};

C(hash) = {
	_,_,_,_,_,
	_,X,_,X,_,
	X,X,X,X,X,
	_,X,_,X,_,
	X,X,X,X,X,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(dollar) = {
	_,X,_,_,
	X,X,X,X,
	X,X,_,_,
	_,X,X,_,
	_,X,_,X,
	X,X,X,X,
	_,X,_,_,
	_,_,_,_,
0};

C(percent) = {
	_,_,_,_,
	_,_,_,_,
	X,_,_,X,
	_,_,X,_,
	_,X,_,_,
	X,_,_,X,
	_,_,_,_,
	_,_,_,_,
0};

C(amp) = {
	_,X,_,_,
	X,_,X,_,
	_,X,_,_,
	X,_,X,X,
	X,_,X,_,
	_,X,X,X,
	_,_,_,_,
	_,_,_,_,
0};

C(squote) = {
	_,X,_,
	_,X,_,
	_,X,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
0};

C(lparen) = {
	_,X,
	X,_,
	X,_,
	X,_,
	X,_,
	X,_,
	_,X,
	_,_,
0};

C(rparen) = {
	X,_,
	_,X,
	_,X,
	_,X,
	_,X,
	_,X,
	X,_,
	_,_,
0};

C(star) = {
	_,_,_,_,_,
	_,_,X,_,_,
	X,_,X,_,X,
	_,X,X,X,_,
	X,_,X,_,X,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(plus) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	X,X,X,X,X,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(coma) = {
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,X,_,
	_,X,_,
	X,_,_,
	_,_,_,
0};

C(minus) = {
	_,_,_,_,
	_,_,_,_,
	_,_,_,_,
	X,X,X,X,
	_,_,_,_,
	_,_,_,_,
	_,_,_,_,
	_,_,_,_,
0};

C(dot) = {
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,X,_,
	_,_,_,
	_,_,_,
0};

C(slash) = {
	_,_,X,
	_,_,X,
	_,X,_,
	_,X,_,
	X,_,_,
	X,_,_,
	_,_,_,
	_,_,_,
0};

C(lbracket) = {
	X,X,
	_,_,
	X,_,
	X,_,
	X,_,
	X,_,
	X,X,
	_,_,
0};

C(rbracket) = {
	X,X,
	_,X,
	_,X,
	_,X,
	_,X,
	_,X,
	X,X,
	_,_,
0};

C(bkslash) = {
	X,_,_,
	X,_,_,
	_,X,_,
	_,X,_,
	_,_,X,
	_,_,X,
	_,_,_,
	_,_,_,
0};

C(hat) = {
	_,X,_,
	X,_,X,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
0};

C(underscore) = {
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	_,_,_,
	X,X,X,
	_,_,_,
	_,_,_,
0};

struct font font8 = { 8, {
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_space,	glyph_exclam,	glyph_dquote,	glyph_hash,
	glyph_dollar,	glyph_percent,	glyph_amp,	glyph_squote,
	glyph_lparen,	glyph_rparen,	glyph_star,	glyph_plus,
	glyph_coma,	glyph_minus,	glyph_dot,	glyph_slash,
	glyph_0,	glyph_1,	glyph_2,	glyph_3,
	glyph_4,	glyph_5,	glyph_6,	glyph_7,
	glyph_8,	glyph_9,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_A,	glyph_B,	glyph_C,
	glyph_D,	glyph_E,	glyph_F,	glyph_G,
	glyph_H,	glyph_I,	glyph_J,	glyph_K,
	glyph_L,	glyph_M,	glyph_N,	glyph_O,
	glyph_P,	glyph_Q,	glyph_R,	glyph_S,
	glyph_T,	glyph_U,	glyph_V,	glyph_W,
	glyph_X,	glyph_Y,	glyph_Z,	glyph_lbracket,
	glyph_bkslash,	glyph_rbracket,	glyph_hat,	glyph_underscore,
	glyph_error,	glyph_a,	glyph_b,	glyph_c,
	glyph_d,	glyph_e,	glyph_f,	glyph_g,
	glyph_h,	glyph_i,	glyph_j,	glyph_k,
	glyph_l,	glyph_m,	glyph_n,	glyph_o,
	glyph_p,	glyph_q,	glyph_r,	glyph_s,
	glyph_t,	glyph_u,	glyph_v,	glyph_w,
	glyph_x,	glyph_y,	glyph_z,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error
} };
