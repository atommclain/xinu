/* sioopen.c - sioopen */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  sioopen  --  open a process-specific standard I/O device
 *------------------------------------------------------------------------
 */
sioopen(devptr, name, mode)
struct	devsw	*devptr;
char	*name;
char	*mode;
{
	int	dev;

	/* Reopen the device by closing the current device and	*/
	/* opening the named device.				*/

	if ((dev=open(NAMESPACE, name, mode)) == SYSERR)
		return(SYSERR);
	setdev(getpid(), devptr->dvnum, dev);
	return(reopen(devptr->dvnum));
}
