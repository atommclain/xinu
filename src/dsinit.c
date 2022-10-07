/* dsinit.c - dsinit */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>

struct	dsblk	dstab[Ndsk];
int	dskdbp, dskrbp;
int	dskpid;

/*------------------------------------------------------------------------
 *  dsinit  --  initialize disk drive device
 *------------------------------------------------------------------------
 */
dsinit(devptr)
	struct	devsw	*devptr;
{
	struct	dsblk	*dsptr;

	devptr->dvioblk = (char *) (dsptr = &dstab[ devptr->dvminor ]);
	dsptr->ddrive	= (long) devptr->dvcsr;
	dsptr->dreqlst	= DRNULL;
	dsptr->dbusy	= FALSE;
	dsptr->dnum	= devptr->dvnum;
	iosetvec(devptr->dvnum, dsptr, dsptr);

	if (devptr->dvminor == 0) {		/* only done once	*/
		dskdbp = mkpool(DBUFSIZ, NDBUFF);
		dskrbp = mkpool(DREQSIZ, NDREQ);
	}
	return(OK);
}
