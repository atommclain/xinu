/* open.c - open */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  open  --  open a connection to a device/file (parms 2&3 are optional)
 *------------------------------------------------------------------------
 */
SYSCALL	open(descrp, name, mode)
int	descrp;
char	*name;
char	*mode;
{
	int		ps;
	struct	devsw	*devptr;
	int		dev;

	if ( isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	disable(ps);
	if (devptr->dvrefct > 0)
		dev = (*name == NULLCH)? descrp : SYSERR;
	else
		dev = (*devptr->dvopen)(devptr, name, mode);
	if (!isbaddev(dev))
		++devtab[dev].dvrefct;
	restore(ps);
	return(dev);
}
