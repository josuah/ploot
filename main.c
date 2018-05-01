#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "arg.h"
#include "ploot.h"

#define LEN(x) (sizeof(x) / sizeof(*x))

char *argv0;
char *tflag = "";
char *uflag = "";

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-t title] [-u unit] label:color...\n", argv0);
	exit(1);
}

void
read_labels(Vlist *v, int n)
{
	(void)v;
	(void)n;
}

void
read_values(Vlist *v, int n)
{
	(void)v;
	(void)n;
}

int
main(int argc, char **argv)
{
	Vlist *v;
	double vmin, vmax, vstep;
	time_t tmin, tmax, tstep;

	ARGBEGIN {
	case 't':
		tflag = EARGF(usage());
		break;
	case 'u':
		uflag = EARGF(usage());
		break;
	} ARGEND;

	if ((v = calloc(argc, sizeof(*v))) == NULL)
		perror("calloc value list"), exit(1);

	vmin = -30; vmax =  700; vstep = 120;
	tmin =   0; tmax = 2000; tstep = 300;

	read_labels(v, argc);
	read_values(v, argc);

	ffdraw(tflag, uflag, v, argc,
		vmin, vmax, vstep,
		tmin, tmax, tstep);

	return 0;
}
