/* dsinter.c - dsinter */

#include <conf.h>
#include <kernel.h>
#include <syserr.h>
#include <pb.h>
#include <proc.h>
#include <q.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dsinter  --  process disk interrupt (from Macintosh ROM)
 *------------------------------------------------------------------------
 */
INTPROC	dsinter(dsptr)
	struct	dsblk	*dsptr;
{
	ParamBlkRec	*pbptr;
	struct	dreq	*drptr;
	int		oldprio;

	drptr = dsptr->dreqlst;
	if (drptr == DRNULL) {
		panic("Disk interrupt when disk not busy");
		return;
	}
	pbptr = &dsptr->dpb;
	drptr->drstat = (pbptr->ioResult==noErr? OK : SYSERR);
	oldprio = proctab[currpid].pprio;
	proctab[currpid].pprio = MAXINT;	/* Postpone rescheduling*/
	if ( (dsptr->dreqlst=drptr->drnext) != DRNULL )
		dskstrt(dsptr);
	proctab[currpid].pprio = oldprio;	/* Resume rescheduling	*/
	switch (drptr->drop) {
	case DREAD:
	case DSYNC:
		ready(drptr->drpid, RESCHYES);
		break;

	case DWRITE:
		freebuf(drptr->drbuff);
		/* fall through */
	case DEJECT:
		if (drptr->drstat == SYSERR)
			panic("Disk error");
		freebuf(drptr);
		if (lastkey(rdytail) > oldprio)
			resched();
	}
}
