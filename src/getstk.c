/* getstk.c - getstk */

#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  getstk  --  allocate stack memory, returning address of topmost int
 *------------------------------------------------------------------------
 */
SYSCALL	*getstk(nbytes)
	unsigned long nbytes;
{
	int	ps;
	struct	mblock	*p, *q;	/* q follows p along memlist		*/
	struct	mblock	*fits, *fitsq;
	unsigned long	len;

	if (nbytes == 0)
		return( (int *)SYSERR );
	nbytes = (long)roundew(nbytes);
	fits = NULL;
	q = &memlist;
	disable(ps);
	for (p = q->mnext ; p != NULL ; q = p,p = p->mnext)
		if ( p->mlen >= nbytes) {
			fitsq = q;
			fits = p;
		}
	if (fits == NULL) {
		restore(ps);
		return( (int *)SYSERR );
	}
	if (nbytes == (len = fits->mlen) )
		fitsq->mnext = fits->mnext;
	else
		fits->mlen -= nbytes;
	fits = (struct mblock *)((long)fits + len - sizeof(int));
	restore(ps);
	return( (int *)fits );
}
