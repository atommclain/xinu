/* x_shutdown.c - x_shutdown */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_shutdown  --  (command shutdown) shut down the system
 *------------------------------------------------------------------------
 */
COMMAND	x_shutdown(nargs, args)
int	nargs;
char	*args[];
{
	int	start(), macboot(), xexit();

	/* Warning! This terminates everything. */
	if (nargs != 2 || *args[1] != '-') {
		fprintf(STDERR, "Usage: %s -[rmq]\n", args[0]);
		return(SYSERR);
	}
	switch (*(args[1]+1)) {
	case 'r':
		shutdown(start);
		break;
	case 'm':
		shutdown(macboot);
		break;
	case 'q':
		shutdown(xexit);
		break;
	default:
		fprintf(STDERR, "Usage: %s -[rmq]\n", args[0]);
		break;
	}
	return(SYSERR);		/* Shutdown should never return */
}
