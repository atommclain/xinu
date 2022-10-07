/* dscntl.c - dscntl */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dscntl  --  control disk driver/device
 *------------------------------------------------------------------------
 */
dscntl(devptr, func)
	struct	devsw	*devptr;
{
	int	stat;
	int	ps;

	disable(ps);
	switch (func) {
	case DSKSYNC:
		stat = dsksync(devptr);
		break;

	case DSKEJECT:
		stat = dskeject(devptr);
		break;

	default:
		stat = SYSERR;
		break;
	}
	restore(ps);
	return(stat);
}
