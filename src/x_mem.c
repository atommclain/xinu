/* x_mem.c - x_mem */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <memory.h>
#include <proc.h>
#include <mem.h>

#define	BUFSIZ	1024L			/* Size of output buffer	*/

/*------------------------------------------------------------------------
 *  x_mem  --  (command mem) print memory use and free list information
 *------------------------------------------------------------------------
 */
COMMAND	x_mem(nargs, args)
int	nargs;
char	*args[];
{
	int		ps;
	int		i;
	struct	mblock	*mptr;
	unsigned long	free;
	unsigned long	avail;
	unsigned long	stkmem;
	char		*str, *sptr, *getmem();

	if ((str=getmem(BUFSIZ)) == (char *) SYSERR) {
		fprintf(STDERR, "%s: out of memory\n", args[0]);
		return(SYSERR);
	}

	/* calculate current size of free memory and stack memory */

	disable(ps);
	sptr = str;
	for( free=0,mptr=memlist.mnext ; mptr!=(struct mblock *)NULL ;
	     mptr=mptr->mnext)
		if (!isodd(mptr) && memlist.mnext<=mptr && mptr<maxaddr)
			free += mptr->mlen;
		else {
			sprintf(sptr, "Free memory list is corrupted\n");
			sptr += strlen(sptr);
			break;
		}
	for (stkmem=0,i=0 ; i<NPROC ; i++) {
		if (proctab[i].pstate != PRFREE)
			stkmem += (unsigned long)proctab[i].pstklen;
	}

	sprintf(sptr, "Mac System  Heap: at %6X, %7D bytes, %7D free\n",
		SysZone, (unsigned long)SysZone->bkLim
			- (unsigned long)&SysZone->heapData,
		SysZone->zcbFree);
	sptr += strlen(sptr);
	sprintf(sptr, "Mac Applic. Heap: at %6X, %7D bytes, %7D free\n",
		ApplZone, (unsigned long)ApplZone->bkLim
			- (unsigned long)&ApplZone->heapData,
		ApplZone->zcbFree);
	sptr += strlen(sptr);
	sprintf(sptr, "Mac Current Heap: at %6X, %7D bytes, %7D free\n",
		TheZone, (unsigned long)TheZone->bkLim
			- (unsigned long)&TheZone->heapData,
		TheZone->zcbFree);
	sptr += strlen(sptr);

	sprintf(sptr,
	       "Memory: %U bytes real memory, %U text, %U data, %U bss\n",
		MemTop, (unsigned long) &_Cend - (unsigned long) &_Corg,
		(unsigned long) &_Dend - (unsigned long) &_Dorg,
		(unsigned long) &_Uend - (unsigned long) &_Uorg);
	sptr += strlen(sptr);
	avail = (unsigned long)maxaddr+sizeof(int)-(unsigned long)&end;
	sprintf(sptr," initially: %5U avail\n", avail);
	sptr += strlen(sptr);
	sprintf(sptr," presently: %5U avail, %5U stack, %5U heap\n",
		free, stkmem, avail - stkmem - free);
	sptr += strlen(sptr);
	sprintf(sptr," free list:\n");
	sptr += strlen(sptr);
	for( mptr=memlist.mnext ; mptr!=(struct mblock *)NULL ;
	     mptr=mptr->mnext) {
		if (isodd(mptr) || mptr<memlist.mnext || mptr>=maxaddr) {
			write(STDOUT, str, strlen(str));
			fprintf(STDERR,"Free memory list corrupted:  ");
			fprintf(STDERR,"next block address = %6X\n",mptr);
			freemem(str, BUFSIZ);
			restore(ps);
			return(SYSERR);
		}
		else if (sptr + 50 > str + BUFSIZ) {
			write(STDOUT, str, strlen(str));
			printf("   etc.\n");
			freemem(str, BUFSIZ);
			restore(ps);
			return(OK);
		}
		sprintf(sptr,"   block at %6X, length %7U (0x%X)\n",
			mptr, mptr->mlen, mptr->mlen);
		sptr += strlen(sptr);
	}
	write(STDOUT, str, strlen(str));
	freemem(str, BUFSIZ);
	restore(ps);
	return(OK);
}
