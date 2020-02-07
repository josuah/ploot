#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define LEN(x)		(sizeof(x) / sizeof(*x))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define ABS(x)		((x) < 0 ? -(x) : (x))

/*
 * Canvas to draw on with braille characters.
 */
struct drawille {
	int		col, row;	/* number of dots in total */
	uint8_t		buf[];		/* buffer of size (col * row) */
};

/*
 * Bitmapped font saved as a '_' and 'X' pattern in a C source file.
 */
struct font {
	int		height;		/* The width is variable. */
	char		*glyph[128];	/* 0: end, 1: off, 2: on.  */
};

/* drawille.c */

size_t		drawille_fmt_row	(struct drawille *, char *, size_t, int);
void		drawille_dot		(struct drawille *, int, int);
struct drawille *drawille_new		(int, int);
void		drawille_line		(struct drawille *, int, int, int, int);
void		drawille_line_hist	(struct drawille *, int, int, int, int, int);
void		drawille_dot_hist	(struct drawille *, int, int, int);
char *		drawille_text		(struct drawille *, int, int, struct font *, char *);

/* font.c */

size_t		font_width		(struct font *, int);
size_t		font_strlen		(struct font *, char *);

/* font13.c */

struct font font13;

/* font7.c */

struct font font8;

/* font8.c */

struct font font8;

/* util.c */

void		put3utf			(long);
char *		strsep			(char **, const char *);
void		estriplf		(char *);
double		eatof			(char *);
long		eatol			(char *);
char *		esfgets			(char *, size_t, FILE *);
int		humanize		(char *, double);
