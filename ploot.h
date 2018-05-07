#include <time.h>
#include <stdint.h>

typedef uint16_t Color[4];

typedef struct {
	int w;		/* width */
	int h;		/* height */
	Color *b;	/* buffer */
	int x;		/* x offset */
	int y;		/* x offset */
} Canvas;

typedef struct {
	int w;		/* width */
	int h;		/* height */
	char *b[128];	/* buffer */
} Font;

typedef struct {
	Color col;	/* for drawing the curve and the legend */
	time_t *t;	/* array of timestamps */
	double *v;	/* array of values */
	int n;		/* number of values */
	char *label;	/* for the legend */
} Vlist;

typedef struct {
	char *name;
	Color col;
} ColorList;

/* ffdraw.c */
void		 ffdraw_pixel	(Canvas *, Color *, int, int);
void		 ffdraw_rectangle(Canvas *, Color *, int, int, int, int);
void		 ffdraw_line	(Canvas *, Color *, int, int, int, int);
void		 ffdraw_char	(Canvas *, Color *, char, Font *, int, int);
void		 ffdraw_str_left(Canvas *, Color *, char *, Font *, int, int);
void		 ffdraw_str_center(Canvas *, Color *, char *, Font *, int, int);
void		 ffdraw_str_right(Canvas *, Color *, char *, Font *, int, int);
void		 ffdraw_fill	(Canvas *, Color *);
void		 ffdraw_print	(Canvas *);

/* ffplot.c */
void		 ffplot		(Vlist *, int, char *, char *);

/* util.c */
char		*strsep		(char **, const char *);

