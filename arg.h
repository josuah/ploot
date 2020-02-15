#ifndef ARG_H
#define ARG_H

#define ARG_SWITCH(argc, argv)						\
	arg0 = *argv;							\
	while (++argv && --argc && **argv == '-' && (*argv)[1])		\
		if ((*argv)[1] == '-' && (*argv)[2] == '\0') {		\
			++argv; break;					\
		} else for (int stop = 0; !stop && *++*argv != '\0' ;)	\
			switch (**argv)

#define ARG ((*++*argv != '\0' || *++argv != NULL)			\
		? ((stop = 1), argc--, *argv)				\
		: (usage(), NULL))

extern char const *arg0;

#endif
