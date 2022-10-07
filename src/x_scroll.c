/* x_scroll.c - x_scroll */

#include <conf.h>
#include <kernel.h>

extern	int	hsrate, vsrate;

/*------------------------------------------------------------------------
 *  x_scroll  --  (command scroll) set window scrolling sensitivity
 *------------------------------------------------------------------------
 */
COMMAND	x_scroll(nargs, args)
int	nargs;
char	*args[];
{
	int	val;

	if (nargs == 1) {
		printf("Horizontal=%d vertical=%d\n", hsrate, vsrate);
		return(OK);
	}
	if (nargs != 3) {
		fprintf(STDERR, "Usage: %s [horizontal vertical]\n",
				args[0]);
		return(SYSERR);
	}
	if ( (val=atoi(args[1])) >= 0 )
		hsrate = val;
	if ( (val=atoi(args[2])) >= 0 )
		vsrate = val;
	return(OK);
}
