#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef uint16_t Color[4];

typedef struct {
	int w;		/* width */
	int h;		/* height */
	Color *b;	/* buffer */
} Canvas;

typedef struct {
	int w;		/* width */
	int h;		/* height */
	char *b[128];	/* buffer */
} Font;

/* ffdraw.c */
void		 ffdraw_pixel	(Canvas *, Color, int, int);
void		 ffdraw_rectangle(Canvas *, Color, int, int, int, int);
void		 ffdraw_line	(Canvas *, Color, int, int, int, int);
void		 ffdraw_char	(Canvas *, Color, char, Font *, int, int);
void		 ffdraw_str	(Canvas *, Color, char *, Font *, int, int);
void		 ffdraw_fill	(Canvas *, Color);
