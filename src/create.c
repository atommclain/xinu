/* create.c - create, newpid */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <proc.h>
#include <mem.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  create  --  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL create(procaddr,ssize,priority,name,nargs,args)
	int	(*procaddr)();		/* procedure address		*/
	long	ssize;			/* stack size in bytes		*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	int	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	int	pid;			/* stores new process id	*/
	struct	pentry	*pptr;		/* pointer to proc. table entry */
	int	*currdevs;
	int	i;
	int	*a;			/* points to list of args	*/
	int	*saddr;			/* stack address		*/
	int	ps;			/* saved processor status	*/
	int	INITRET(), *getstk();

	disable(ps);
	ssize = (long) roundew(ssize);
	if (ssize < MINSTK + nargs*sizeof(int) || isodd(procaddr) ||
	    priority < 1 || (pid=newpid()) == SYSERR ||
	    (saddr=getstk(ssize)) == SYSERR) {
		restore(ps);
		return(SYSERR);
	}
	numproc++;
	pptr = &proctab[pid];
	pptr->pstate = PRSUSP;
	for (i=0 ; i<PNMLEN && (pptr->pname[i]=name[i])!=NULLCH ; i++)
		;
	pptr->pname[PNMLEN-1] = NULLCH;
	pptr->pprio = priority;
	pptr->pbase = saddr;
	pptr->pstklen = ssize;
	pptr->psem = 0;
	pptr->phasmsg = FALSE;
	pptr->plimit = (short *)((long)saddr + sizeof(int) - ssize);
	*(short *)saddr = MAGIC;
	pptr->pargs = nargs;
	for (i=0 ; i<PNREGS ; i++)
		pptr->pregs[i]=INITREG;
	pptr->pregs[A5] = INITA5;
	pptr->pregs[PC] = (long)(pptr->paddr = procaddr);
	pptr->pregs[PS] = INITPS;
	GetPort(&pptr->pgport);		/* new proc inherits GrafPort	*/
	pptr->pnxtkin = BADPID;
	currdevs = proctab[currpid].pdevs;
	for (i = 0; i < Nsio; ++i)
		if ((pptr->pdevs[i]=open(currdevs[i], NULLSTR, "rw"))
		    == SYSERR)
			pptr->pdevs[i] = BADDEV;
	a = (&args) + nargs;		/* point past last argument	*/
	for ( ; nargs > 0 ; --nargs)	/* machine dependent; copy args	*/
		*--saddr = *--a;	/* onto created process' stack	*/
	*--(int (**)())saddr = INITRET;	/* push on return address	*/
	pptr->pregs[SP] = (long)saddr;
	restore(ps);
	return(pid);
}

/*------------------------------------------------------------------------
 *  newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
