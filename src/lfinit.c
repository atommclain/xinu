/* lfinit.c - lfinit */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

int	lfdbp;

/*------------------------------------------------------------------------
 *  lfinit  --  initialize local file pseudo devices
 *------------------------------------------------------------------------
 */
lfinit(devptr)
struct	devsw	*devptr;
{
	struct	lfblk	*lfptr;

	devptr->dvioblk = (char *) (lfptr = &Lf.lftab[devptr->dvminor]);
	lfptr->lf_dnum = devptr->dvnum;
	lfptr->lf_state = LFREE;
	lfptr->lf_mutex = screate(1);
	lfptr->lf_pos = 0L;
	if (devptr->dvminor == 0) {		/* only done once	*/
		lfdbp = mkpool(LBUFSIZ, LNBUFF);
		Lf.device = devptr->dvnum;
		Lf.lmutex = screate(1);
		Lf.ldone  = screate(0);
		ioinit(devptr->dvnum);
	}
}

struct	lfinfo	Lf;
