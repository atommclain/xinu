/* dskqopt.c - dskqopt */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dskqopt  --  optimize requests to read/write/seek to the same block
 *------------------------------------------------------------------------
 */
dskqopt(p, q, drptr)
struct	dreq	*p, *q, *drptr;
{
	DBADDR	block;

	/* By definition, eject and sync requests cannot be optimized.	*/
	/* Also, cannot optimize read requests if already reading.	*/

	if (drptr->drop==DEJECT || drptr->drop==DSYNC ||
	    (drptr->drop==DREAD && p->drop==DREAD))
		return(SYSERR);

	if (p->drop==DWRITE && drptr->drop==DWRITE) {	/* dup write	*/
		drptr->drnext = p->drnext;
		q->drnext = drptr;
		freebuf(p->drbuff);
		freebuf(p);
		return(OK);
	}

	if (drptr->drop==DREAD && p->drop==DWRITE) {	/* satisfy read	*/
		blkcopy(drptr->drbuff, p->drbuff, DBUFSIZ);
		return(OK);
	}

	if (drptr->drop==DWRITE && p->drop==DREAD) {	/* sat. old read*/
		block = drptr->drdba;
		for (; p!=DRNULL && p->drdba==block ; p=p->drnext) {
			q->drnext = p->drnext;
			p->drstat =
			    blkcopy(p->drbuff, drptr->drbuff, DBUFSIZ);
			ready(p->drpid, RESCHNO);
		}
		q->drnext = drptr;
		drptr->drnext = p;
		resched();
		return(DONQ);
	}

	return(SYSERR);
}
