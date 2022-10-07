/* cwd.h */

typedef	char	cwdtab[NPROC][NAMLEN];

/* Control functions for the cwd device */

#define	GETCWD	1
#define	SETCWD	2

#ifdef	Ncwd
extern	cwdtab	cwd[Ncwd];
#endif
