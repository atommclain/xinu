/* dskeject.c - dskeject */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <proc.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dskeject  --  eject the disk from the drive
 *------------------------------------------------------------------------
 */
dskeject(devptr)
struct	devsw	*devptr;
{
	struct	dsblk	*dsptr;
	struct	dreq	*drptr, *p, *q;
	SYSCALL	*getbuf();

	dsptr = (struct dsblk *) devptr->dvioblk;
	drptr = (struct dreq *) getbuf(dskrbp);
	drptr->drdba = 0;
	drptr->drpid = currpid;
	drptr->drbuff = NULL;
	drptr->drop = DEJECT;
	drptr->drnext = DRNULL;

	/* place at end of request list */

	if ( (q=dsptr->dreqlst) == DRNULL )
		dskenq(drptr, dsptr);
	else {
		for (p=q->drnext ; p!=DRNULL ; q=p,p=p->drnext)
			;
		q->drnext = drptr;
	}

	return(OK);
}
