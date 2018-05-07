#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define LEN(x) (sizeof(x) / sizeof(*x))

/* util.c */
char		*strsep		(char **, const char *);
void		 estriplf	(char *);
double		 eatof		(char *);
long		 eatol		(char *);
char		*esfgets	(char *, size_t, FILE *);
