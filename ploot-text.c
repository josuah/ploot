#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef __OpenBSD__
#define pledge(...) 0
#endif

#include "drawille.h"
#include "font.h"
#include "util.h"

void
usage(void)
{
	fprintf(stderr, "usage: %s [-12] text\n", arg0);
	exit(100);
}

int
main(int argc, char **argv)
{
	struct font *ft;
	struct drawille *drw;
	char *text;
	size_t h, w;
	int c, row;

	if (pledge("stdio", "") < 0)
		err(1, "pledge: %s", strerror(errno));

	ft = &font8;
	arg0 = *argv;
	while ((c = getopt(argc, argv, "12")) > -1) {
		switch (c) {
		case '1':
			ft = &font8;
			break;
		case '2':
			ft = &font13;
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();

	text = *argv;

	h = (ft->height + 3) / 4;
	w = font_strlen(ft, text) / 2;
	if ((drw = drawille_new(h, w)) == NULL)
		err(1, "drawille_new: %s", strerror(errno));
	drawille_text(drw, 0, 0, ft, text);

	for (row = 0; row < drw->row; row++) {
		drawille_put_row(stdout, drw, row);
		fprintf(stdout, "\n");
	}

	free(drw);
	return 0;
}
