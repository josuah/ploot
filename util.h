#define LEN(x)		(sizeof(x) / sizeof(*x))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#define ABS(x)		((x) < 0 ? -(x) : (x))

void		 put3utf	(long);
char		*strsep		(char **, const char *);
void		 estriplf	(char *);
double		 eatof		(char *);
long		 eatol		(char *);
char		*esfgets	(char *, size_t, FILE *);
int		 humanize	(char *, double);
