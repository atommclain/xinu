/* init.c - init */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  init  --  initialize a device
 *------------------------------------------------------------------------
 */
init(descrp)
int descrp;
{
	struct	devsw	*devptr;

	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	devptr->dvrefct = 0;
	return( (*devptr->dvinit)(devptr) );
}
