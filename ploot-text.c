#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "drawille.h"
#include "font.h"
#include "util.h"

char *arg0 = NULL;

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
	int c, row;

	ft = &font8;
	optind = 0;
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
	arg0 = *argv;
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();

	text = *argv;

	assert(drw = drawille_new((ft->height + 3) / 4, font_strlen(ft, text) / 2));
	drawille_text(drw, 0, 0, ft, text);

        for (row = 0; row < drw->row; row++) {
		drawille_put_row(stdout, drw, row);
                fprintf(stdout, "\n");
        }

	free(drw);
}
