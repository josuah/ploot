/*
 * Render bitmapped font as a farbfeld image
 */

#include <arpa/inet.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

typedef uint16_t Color[4];

enum { R, G, B, A };

Color *canvas;

char *argv0;
int wflag = 0;
int hflag = 0;

void
usage()
{
	fputs("ploot -w width -h height <data.csv >image.ff\n", stderr);
	exit(1);
}

void
ffdraw(Color *canvas, int w, int h)
{
	Color col = { 0xffff, 0xffff, 0x0000, 0xffff };
	size_t n;

	for (n = w * h; n > 0; n--)
		memcpy(canvas + n - 1, col, sizeof(col));
}

int
main(int argc, char **argv)
{
	uint32_t w, h;

	ARGBEGIN {
	case 'w':
		wflag = atoi(EARGF(usage()));
		break;
	case 'h':
		hflag = atoi(EARGF(usage()));
		break;
	} ARGEND;

	if (wflag == 0 || hflag == 0)
		usage();

	if ((canvas = calloc(wflag * hflag, sizeof(*canvas))) == NULL) {
		perror("allocating memory for the canvas");
		return 1;
	}

	fputs("farbfeld", stdout);
	w = htonl(wflag);
	h = htonl(hflag);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	ffdraw(canvas, wflag, hflag);
	fwrite(canvas, wflag * hflag, sizeof(*canvas), stdout);
	return 0;
}
