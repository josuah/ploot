#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>

#include "ffdraw.h"
#include "font-14x6.h"

#define WIDTH 100
#define HEIGHT 100

Color buffer[WIDTH * HEIGHT];

static void
ffdraw(Canvas *can)
{
	Color col1 = { 0x2222, 0x2222, 0x2222, 0xffff };
	Color col2 = { 0x3333, 0xffff, 0x8888, 0xffff };

	ffdraw_fill(can, col1);
	ffdraw_line(can, col2, 49,1,9,79);
	ffdraw_char(can, col2, '0' - 1, &font_14x6, 44, 50);
	ffdraw_char(can, col2, '0' + 0, &font_14x6, 50, 50);
	ffdraw_char(can, col2, '0' + 1, &font_14x6, 56, 50);
}

int
main(void)
{
	uint32_t w, h;
	Canvas can;

	can.b = buffer;
	can.w = WIDTH;
	can.h = HEIGHT;
	w = htonl(WIDTH);
	h = htonl(HEIGHT);
	fputs("farbfeld", stdout);
	fwrite(&w, sizeof(w), 1, stdout);
	fwrite(&h, sizeof(h), 1, stdout);
	ffdraw(&can);
	fwrite(can.b, WIDTH * HEIGHT, sizeof(*can.b), stdout);
	return 0;
}
