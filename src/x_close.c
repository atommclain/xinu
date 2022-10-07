/* x_close.c - x_close */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_close  --  (command close) close a device given its id
 *------------------------------------------------------------------------
 */
COMMAND	x_close(nargs, args)
int	nargs;
char	*args[];
{
	int	dev;

	if (nargs != 2) {
		fprintf(STDERR, "Usage: close device-number\n");
		return(SYSERR);
	}
	if (close(dev=atoi(args[1])) == SYSERR) {
		fprintf(STDERR, "Cannot close %d\n", dev);
		return(SYSERR);
	}
	return(OK);
}
