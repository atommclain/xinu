/* dsread.c - dsread */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <proc.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dsread  --  read a block from a disk device
 *------------------------------------------------------------------------
 */
dsread(devptr, buff, block)
	struct	devsw	*devptr;
	char	*buff;
	DBADDR	block;
{
	struct	dreq	*drptr;
	int	stat;
	int	ps;
	SYSCALL	*getbuf();

	disable(ps);
	drptr = (struct dreq *) getbuf(dskrbp);
	drptr->drdba = block;
	drptr->drpid = currpid;
	drptr->drbuff = buff;
	drptr->drop = DREAD;
	if ( (stat=dskenq(drptr, devptr->dvioblk)) == DONQ) {
		drptr->drstat = SYSERR;
		suspend(currpid);
		stat = drptr->drstat;
	}
	freebuf(drptr);
	restore(ps);
	return(stat);
}
