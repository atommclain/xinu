/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  setdev  --  set a device entry in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(pid, siodev, dev)
int	pid;				/* process to change		*/
int	siodev, dev;			/* device descriptor to set	*/
{
	int	minor, *pdev;

	if (isbadpid(pid) || isbaddev(siodev)
	    || (minor=devtab[siodev].dvminor) >= Nsio)
		return(SYSERR);
	close( *(pdev = &proctab[pid].pdevs[minor]) );
	*pdev = (dev == SYSERR? BADDEV : dev);
	return(OK);
}
