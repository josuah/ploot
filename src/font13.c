#include "font.h"

#define C(x)	static char glyph_##x[]
#define _	2
#define X	3

C(error) = {
	_,_,_,_,_,
	X,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,X,
	_,_,_,_,_,
0};

C(space) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(bang) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(double) = {
	_,_,_,_,_,
	_,X,_,X,_,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(hash) = {
	_,_,_,_,_,
	_,X,_,X,_,
	_,X,_,X,_,
	X,X,X,X,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,X,_,X,_,
	X,X,X,X,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(dollar) = {
	_,_,X,_,_,
	_,X,X,X,_,
	X,_,X,_,X,
	X,_,X,_,_,
	X,_,X,_,_,
	_,X,X,X,_,
	_,_,X,_,X,
	_,_,X,_,X,
	X,_,X,_,X,
	_,X,X,X,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(percent) = {
	_,_,_,_,_,
	X,X,_,_,X,
	X,X,_,_,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,_,_,X,X,
	X,_,_,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(amp) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,X,_,_,
	_,X,X,_,X,
	X,_,_,X,_,
	X,_,_,X,_,
	X,_,_,X,_,
	_,X,X,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(single) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(l_round) = {
	_,_,_,_,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(r_round) = {
	_,_,_,_,_,
	_,X,_,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(asterisk) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	X,_,X,_,X,
	_,X,X,X,_,
	_,_,X,_,_,
	_,X,X,X,_,
	X,_,X,_,X,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(plus) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	X,X,X,X,X,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(coma) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,_,_,_,_,
0};

C(minus) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(dot) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(slash) = {
	_,_,_,_,_,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(0) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,X,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(1) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(2) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(3) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(4) = {
	_,_,_,_,_,
	_,_,_,_,X,
	_,_,_,X,X,
	_,_,X,_,X,
	_,X,_,_,X,
	X,_,_,_,X,
	X,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(5) = {
	_,_,_,_,_,
	X,X,X,X,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(6) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(7) = {
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(8) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(9) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(column) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(semicolumn) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,_,_,_,_,
0};

C(l_angle) = {
	_,_,_,_,_,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	_,X,_,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(equal) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(r_angle) = {
	_,_,_,_,_,
	X,_,_,_,_,
	_,X,_,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(question) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(at) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,X,X,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,X,X,
	X,_,_,_,_,
	_,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(A) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,_,X,_,
	_,X,_,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(B) = {
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(C) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(D) = {
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(E) = {
	_,_,_,_,_,
	X,X,X,X,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(F) = {
	_,_,_,_,_,
	X,X,X,X,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(G) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(H) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(I) = {
	_,_,_,_,_,
	_,X,X,X,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(J) = {
	_,_,_,_,_,
	_,X,X,X,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	X,_,_,X,_,
	_,X,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(K) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,X,_,
	X,_,X,_,_,
	X,X,_,_,_,
	X,_,X,_,_,
	X,_,_,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(L) = {
	_,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(M) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,X,_,X,X,
	X,X,_,X,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(N) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,X,_,_,X,
	X,X,_,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,_,X,X,
	X,_,_,X,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(O) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(P) = {
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(Q) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,X,_,X,
	_,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(R) = {
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	X,_,X,_,_,
	X,_,_,X,_,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(S) = {
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,_,
	X,_,_,_,_,
	_,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(T) = {
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(U) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(V) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(W) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,X,_,X,X,
	X,X,_,X,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(X) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,X,_,_,
	_,X,_,X,_,
	_,X,_,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(Y) = {
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(Z) = {
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(l_square) = {
	_,_,_,_,_,
	_,X,X,X,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(backsl) = {
	_,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(r_square) = {
	_,_,_,_,_,
	_,X,X,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(hat) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,X,_,X,_,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(underscore) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(backtilt) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(a) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	_,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(b) = {
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(c) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(d) = {
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(e) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,X,
	X,_,_,_,_,
	X,_,_,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(f) = {
	_,_,X,X,X,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,X,X,X,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(g) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	X,X,X,X,_,
0};

C(h) = {
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(i) = {
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,X,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(j) = {
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,_,_,
	_,_,X,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	X,_,_,X,_,
	_,X,X,_,_,
0};

C(k) = {
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,X,
	X,_,_,X,_,
	X,_,X,_,_,
	X,X,_,_,_,
	X,_,X,_,_,
	X,_,_,X,_,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(l) = {
	_,X,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(m) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(n) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(o) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(p) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,X,X,X,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
0};

C(q) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	_,_,_,_,X,
0};

C(r) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,X,X,X,
	X,X,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	X,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(s) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,X,X,X,
	X,_,_,_,_,
	X,_,_,_,_,
	_,X,X,X,_,
	_,_,_,_,X,
	_,_,_,_,X,
	X,X,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(t) = {
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,X,X,X,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(u) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(v) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,X,_,X,_,
	_,X,_,X,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(w) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	X,_,X,_,X,
	_,X,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(x) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,_,X,_,
	_,_,X,_,_,
	_,X,_,X,_,
	X,_,_,_,X,
	X,_,_,_,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(y) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	X,_,_,_,X,
	_,X,X,X,X,
	_,_,_,_,X,
	_,_,_,_,X,
	X,X,X,X,_,
0};

C(z) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,X,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	X,X,X,X,X,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(l_curly) = {
	_,_,_,_,_,
	_,_,X,X,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	X,_,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,X,_,_,_,
	_,_,X,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(pipe) = {
	_,_,_,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(r_curly) = {
	_,_,_,_,_,
	_,X,X,_,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,_,X,
	_,_,_,X,_,
	_,_,_,X,_,
	_,_,_,X,_,
	_,X,X,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

C(tilde) = {
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,X,_,_,X,
	X,_,X,_,X,
	X,_,_,X,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
	_,_,_,_,_,
0};

struct font font13 = { 13, {
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_error,	glyph_error,	glyph_error,	glyph_error,
	glyph_space,	glyph_bang,	glyph_double,	glyph_hash,
	glyph_dollar,	glyph_percent,	glyph_amp,	glyph_single,
	glyph_l_round,	glyph_r_round,	glyph_asterisk,	glyph_plus,
	glyph_coma,	glyph_minus,	glyph_dot,	glyph_slash,
	glyph_0,	glyph_1,	glyph_2,	glyph_3,
	glyph_4,	glyph_5,	glyph_6,	glyph_7,
	glyph_8,	glyph_9,	glyph_column,	glyph_semicolumn,
	glyph_l_angle,	glyph_equal,	glyph_r_angle,	glyph_question,
	glyph_at,	glyph_A,	glyph_B,	glyph_C,
	glyph_D,	glyph_E,	glyph_F,	glyph_G,
	glyph_H,	glyph_I,	glyph_J,	glyph_K,
	glyph_L,	glyph_M,	glyph_N,	glyph_O,
	glyph_P,	glyph_Q,	glyph_R,	glyph_S,
	glyph_T,	glyph_U,	glyph_V,	glyph_W,
	glyph_X,	glyph_Y,	glyph_Z,	glyph_l_square,
	glyph_backsl,	glyph_r_square,	glyph_hat,	glyph_underscore,
	glyph_backtilt,	glyph_a,	glyph_b,	glyph_c,
	glyph_d,	glyph_e,	glyph_f,	glyph_g,
	glyph_h,	glyph_i,	glyph_j,	glyph_k,
	glyph_l,	glyph_m,	glyph_n,	glyph_o,
	glyph_p,	glyph_q,	glyph_r,	glyph_s,
	glyph_t,	glyph_u,	glyph_v,	glyph_w,
	glyph_x,	glyph_y,	glyph_z,	glyph_l_curly,
	glyph_pipe,	glyph_r_curly,	glyph_tilde,	glyph_error
} };
