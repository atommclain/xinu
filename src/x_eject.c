/* x_eject.c - x_eject */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_eject  --  (command eject) eject a disk
 *------------------------------------------------------------------------
 */
COMMAND	x_eject(nargs, args)
int	nargs;
char	*args[];
{
	int	dev;
	char	newname[NAMLEN];

	if (nargs != 2) {
		fprintf(STDERR, "Usage: %s device\n", args[0]);
		return(SYSERR);
	}
	if ((dev=nammap(args[1],newname))==SYSERR || newname[0]!=NULLCH ||
	    control(dev, DSKEJECT) == SYSERR )
		fprintf(STDERR, "Cannot eject %s\n", args[1]);
	return(OK);
}
