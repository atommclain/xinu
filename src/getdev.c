/* getdev.c - getdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  getdev  --  return the device the standard I/O device is connected to
 *------------------------------------------------------------------------
 */
SYSCALL	getdev(pid, siodev)
int	pid;
int	siodev;
{
	int	minor;

	if (isbadpid(pid) || isbaddev(siodev)
	    || (minor=devtab[siodev].dvminor) >= Nsio)
		return(SYSERR);
	return(proctab[pid].pdevs[minor]);
}
