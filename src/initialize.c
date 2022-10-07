/* initialize.c - nulluser, sysinit */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <inits.h>
#include <font.h>
#include <memory.h>
#include <pb.h>
#include <proc.h>
#include <sem.h>
#include <sleep.h>
#include <mem.h>
#include <q.h>
#include <io.h>
#include <disk.h>

extern	int	main();		/* address of user's main program	*/
extern	int	eventd();	/* process that handles events		*/
int		evtpid;		/* process id of eventd			*/

/* Declarations of major kernel variables */

struct	pentry	proctab[NPROC]; /* process table			*/
int	nextproc;		/* next process slot to use in create	*/
struct	sentry	semaph[NSEM];	/* semaphore table			*/
int	nextsem;		/* next semaphore slot to use in screate*/
struct	qent	q[NQENT];	/* q table (see queue.c)		*/
int	nextqueue;		/* next slot in q structure to use	*/
int	*maxaddr;		/* max memory address (set in startup.s)*/
#ifdef	NDEVS
struct	intmap	intmap[NDEVS];	/* interrupt dispatch table		*/
#endif
struct	mblock	memlist;	/* list of free memory blocks		*/

/* active system status */

int	numproc;		/* number of live user processes	*/
int	currpid;		/* id of currently running process	*/
int	reboot = 0;		/* non-zero after first boot		*/

int	rdyhead,rdytail;	/* head/tail of ready list (q indexes)	*/
char	vers[] = VERSION;	/* Xinu version printed at startup	*/

/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED, and      ***/
/***   must eventually be enabled explicitly.  This routine turns     ***/
/***   itself into the null process after initialization.  Because    ***/
/***   the null process must always remain ready to run, it cannot    ***/
/***   execute code that might cause it to be suspended, wait for a   ***/
/***   semaphore, or put to sleep, or exit.                           ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  nulluser  --  initialize system and become the null process (id==0)
 *------------------------------------------------------------------------
 */
nulluser()				/* babysit CPU when no one home */
{
	macinit();			/* Initialize Macintosh system	*/
					/*  and CONSOLE window		*/
	kprintf("\nXinu Version %s", vers);
	if (reboot++ < 1)
		kprintf("\n");
	else
		kprintf("   (reboot %d)\n", reboot);
	sysinit();			/* initialize all of Xinu	*/
	kprintf("%U real mem\n", MemTop);
	kprintf("%U avail mem\n", (long)maxaddr+sizeof(int)-(long)&end);
	kprintf("clock %sabled\n\n", clkruns? "en" : "dis");

	/* create a process to handle Macintosh events */

	resume(evtpid = create(eventd,EVTSTK,EVTPRIO,EVTNAME,EVTARGS));
	InitCursor();	/* Signal to user that we are responding now	*/
	enable();	/* enable interrupts (after creating eventd)	*/

	/* start a process executing the user's main program */

	resume( create(main,INITSTK,INITPRIO,INITNAME,INITARGS) );

	while (TRUE) {			/* run forever without actually */
		pause();		/*  executing instructions	*/
	}
}

/*------------------------------------------------------------------------
 *  sysinit  --  initialize all Xinu data structures and devices
 *------------------------------------------------------------------------
 */
LOCAL	sysinit()
{
	int	i;
	struct	pentry	*pptr;
	struct	sentry	*sptr;
	struct	mblock	*mptr;

	numproc  = 0;			/* initialize system variables	*/
	nextproc = NPROC-1;
	nextsem  = NSEM-1;
	nextqueue= NPROC;		/* q[0..NPROC-1] are processes	*/

	HeapEnd =			/* protect XINU heap from Mac	*/
		(char *)maxaddr + sizeof(int) - NULLSTK;
	memlist.mnext = mptr =		/* initialize free memory list	*/
		(struct mblock *) roundew(&end);
	mptr->mnext = (struct mblock *)NULL;
	mptr->mlen = (long) truncew((long)HeapEnd - (long)mptr);

	for (i=0 ; i<NPROC ; i++)	/* initialize process table	*/
		proctab[i].pstate = PRFREE;

	pptr = &proctab[NULLPROC];	/* initialize null process entry*/
	pptr->pstate = PRCURR;
	pptr->pprio = 0;
	strcpy(pptr->pname, "prnull");
	pptr->plimit = HeapEnd;
	pptr->pstklen = NULLSTK;
	pptr->pregs[SP] = (long)(pptr->pbase = maxaddr);
	*(pptr->pbase) = MAGIC;
	pptr->paddr = nulluser;
	pptr->pargs = 0;
	for (i = 0; i < Nsio; ++i)
		pptr->pdevs[i] = CONSOLE;
	currpid = NULLPROC;

	for (i=0 ; i<NSEM ; i++) {	/* initialize semaphores	*/
		(sptr = &semaph[i])->sstate = SFREE;
		sptr->sqtail = 1 + (sptr->sqhead = newqueue());
	}

	rdytail = 1 + (rdyhead=newqueue());/* initialize ready list	*/

#ifdef	MEMMARK
	_mkinit();			/* initialize memory marking	*/
#endif
	clkinit();			/* initialize r.t. clock	*/
#ifdef	NDEVS
	for ( i=0 ; i<NDEVS ; i++ )	/* initialize devices		*/
		init(i);
#endif
	return(OK);
}
