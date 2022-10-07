/* x_format.c - x_format */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_format  --  (command format) format a disk
 *------------------------------------------------------------------------
 */
COMMAND	x_format(nargs, args)
int	nargs;
char	*args[];
{
	int	oldprio;
	char	newname[NAMLEN];
	int	dev, drive, stat;
	int	dsread();

	if (nargs != 2) {
		fprintf(STDERR, "Usage: %s device\n", args[0]);
		return(SYSERR);
	}

	oldprio = chprio(getpid(), MAXINT);/* Exclude other processes	*/
	if ((dev=nammap(args[1],newname))==SYSERR || newname[0]!=NULLCH ||
	    devtab[dev].dvread!=dsread || control(dev,DSKSYNC)==SYSERR ) {
		fprintf(STDERR, "Cannot format %s\n", args[1]);
		return(SYSERR);
	}
	drive = (long) devtab[dev].dvcsr;
	printf("Formatting %s (drive %d)...\n", args[1], drive);
	stat = DIFormat(drive);
	chprio(getpid(), oldprio);	/* End critical section		*/

	if (stat < 0) {
		fprintf(STDERR, "format: error %d\n", stat);
		return(SYSERR);
	}
	return(OK);
}
