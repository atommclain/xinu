/* freemem.c - freemem */

#include <kernel.h>
#include <macvars.h>
#include <memory.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
SYSCALL	freemem(block, size)
	struct	mblock	*block;
	unsigned long	size;
{
	int		ps;
	struct	mblock	*p, *q;
	unsigned long	top;

	if (size==0 || block < &end || maxaddr < block)
		return(SYSERR);
	size = (long)roundew(size);
	disable(ps);
	for( p=memlist.mnext,q= &memlist ; (char *)p!=NULL && p<block ;
		    q=p,p=p->mnext )
		;
	if ((top=q->mlen+(long)q)>(long)block && q!= &memlist ||
		(char *)p!=NULL && (size+(long)block) > (long)p) {
		restore(ps);
		return(SYSERR);
	}
	if ( q!= &memlist && top == (long)block )
		q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ( (long)( q->mlen + (long)q ) == (long)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	restore(ps);
	return(OK);
}
