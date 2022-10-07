/* mkpool.c - mkpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  mkpool  --  allocate memory for a buffer pool and link together
 *------------------------------------------------------------------------
 */
SYSCALL	mkpool(bufsiz, numbufs)
int	bufsiz, numbufs;
{
	int	ps;
	int	poolid;
	char	*where;
	SYSCALL	*getmem();

#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		poolinit();
#endif
	disable(ps);
	if (bufsiz<BPMINB || bufsiz>BPMAXB ||
	    numbufs<1 || numbufs>BPMAXN ||
	    nbpools >= NBPOOLS ||
	    (where=(char *)getmem((long)(bufsiz+sizeof(int))*numbufs)) ==
	    SYSERR) {
		restore(ps);
		return(SYSERR);
	}
	poolid = nbpools++;
	bptab[poolid].bpnext = where;
	bptab[poolid].bpsize = bufsiz;
	bptab[poolid].bpsem = screate(numbufs);
	bufsiz+=sizeof(int);
	for (numbufs-- ; numbufs>0 ; numbufs--,where+=bufsiz)
		*(char **) where = where + bufsiz;
	*(char **) where = NULL;
	restore(ps);
	return(poolid);
}
