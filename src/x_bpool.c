/* x_bpool.c - x_bpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  x_bpool  --  (command bpool) format and print buffer pool information
 *------------------------------------------------------------------------
 */
COMMAND	x_bpool(nargs, args)
int	nargs;
char	*args[];
{
	struct	bpool	*bpptr;
	char	str[80];
	int	i;

#ifdef	MEMMARK
	if (unmarked(bpmark))
		poolinit();
#endif
	for (i=0 ; i<nbpools ; i++) {
		bpptr = &bptab[i];
		sprintf(str,
		    "pool=%2d. bsize=%4d, sem=%2d, count=%d\n",
			i, bpptr->bpsize, bpptr->bpsem,
			scount(bpptr->bpsem));
		write(STDOUT, str, strlen(str));
	}
	return(OK);
}
