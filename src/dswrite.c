/* dswrite.c - dswrite */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <proc.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dswrite  --  write a block (system buffer) onto a disk device
 *------------------------------------------------------------------------
 */
dswrite(devptr, buff, block)
	struct	devsw	*devptr;
	char	*buff;
	DBADDR	block;
{
	struct	dreq	*drptr;
	int	ps;
	SYSCALL	*getbuf();

	disable(ps);
	drptr = (struct dreq *) getbuf(dskrbp);
	drptr->drbuff = buff;
	drptr->drdba = block;
	drptr->drpid = currpid;
	drptr->drop = DWRITE;
	dskenq(drptr, devptr->dvioblk);
	restore(ps);
	return(OK);
}
