/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;			/* process to kill		*/
{
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	i;
	int	ps;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 1)
		xdone();
	for (i = 0; i < Nsio; ++i)
		close(pptr->pdevs[i]);
	send(pptr->pnxtkin, (long) pid);
	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	  case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	  case PRWAIT:	semaph[pptr->psem].semcnt++;
						/* fall through */
	  case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	  case PRSLEEP:
	  case PRTRECV:	unsleep(pid);
						/* fall through	*/
	  default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
