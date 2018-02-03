#define USED(x) ((void)(x))

extern char *argv0;

#define ARGBEGIN(argc, argv)						\
	for (argv0 = *argv, argv++, argc--;				\
	    argv[0] != NULL && argv[0][0] == '-' && argv[0][1] != '\0';	\
	    argc--, argv++) {						\
		char **arg_v, *arg_s;					\
		if (argv[0][1] == '-' && argv[0][2] == '\0') {		\
			argv++, argc--;					\
			break;						\
		}							\
		arg_v = argv;						\
		for (arg_s = *argv; *arg_s != '\0'; arg_s++) {		\
			if (arg_v != argv)				\
				break;					\
			switch (*arg_s)

#define ARGEND								\
		}							\
	}

#define	EARGF(x)							\
	((argv[1] == NULL) ? ((x), NULL) : (argc--, argv++, argv[0]))
