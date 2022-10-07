/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <quickdraw.h>
#include <proc.h>
#include <q.h>
#include <io.h>

/*------------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRCURR.
 *------------------------------------------------------------------------
 */
int	resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio))
		return(OK);

	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
	preempt = QUANTUM;		/* reset preemption counter	*/
#ifdef	STKCHK
	if ( *(nptr->pbase) != MAGIC ) {
		kprintf("Bad magic pid=%d (%s), value=%x at %X\n",
			currpid,nptr->pname,*(nptr->pbase),nptr->pbase);
		panic("stack corrupted");
	}
	if ((unsigned long)nptr->pregs[SP]<(unsigned long)nptr->plimit) {
		kprintf("Bad SP pid=%d (%s), limit=%X, will be %X\n",
			currpid, nptr->pname, nptr->plimit,
			nptr->pregs[SP]);
		panic("stack overflow");
	}
#endif
	optr->pgport = thePort;		/* save current GrafPort for	*/
	thePort = nptr->pgport;		/*  old process and set new one	*/
	HeapEnd = nptr->plimit;		/* HeapEnd = new stack limit	*/
	ctxsw(optr->pregs,nptr->pregs);	/* save/load registers		*/

	/* The OLD process returns here when resumed. */
	return(OK);
}
