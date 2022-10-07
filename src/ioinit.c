/* ioinit.c - ioinit, iosetvec */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ioinit  --  standard interrupt dispatch initialization
 *------------------------------------------------------------------------
 */
ioinit(descrp)
int	descrp;
{
	long	minor;

	if (isbaddev(descrp) )
		return(SYSERR);
	minor = devtab[descrp].dvminor;
	iosetvec(descrp, (long) minor, (long) minor);
	return(OK);
}

/*------------------------------------------------------------------------
 *  iosetvec  --  fill in interrupt dispatch table entries
 *------------------------------------------------------------------------
 */
iosetvec(descrp, incode, outcode)
int	descrp;
long	incode;
long	outcode;
{
#ifdef	NDEVS
	struct	devsw	*devptr;
	struct	intmap	*map;

	if (isbaddev(descrp))
		return(SYSERR);
	devptr = &devtab[descrp];
	map = &intmap[devptr->dvnum];	/* fill in interrupt dispatch	*/
	map->iin   = devptr->dviint;	/*   map with addresses of high-*/
	map->icode = incode;		/*   level input and output	*/
	map->iout  = devptr->dvoint;	/*   interrupt handlers and	*/
	map->ocode = outcode;		/*   minor device numbers	*/
#endif
	return(OK);
}
