/* dskstrt.c - dskstrt, PBEject */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <io.h>

#define	ejectCode	7		/* ROM control code: disk eject	*/
#define	drvStsCode	8		/* ROM status code: drive status*/

LOCAL	int	(*romfunc[])() = {PBRead, PBWrite, PBEject, PBStatus};

/*------------------------------------------------------------------------
 *  dskstrt  --  start an I/O operation on a disk device
 *------------------------------------------------------------------------
 */
dskstrt(dsptr)
	struct	dsblk	*dsptr;
{
	int		ps;
	struct	dreq	*drptr;
	ParamBlkRec	*pbptr;

	if (dsptr->dbusy)
		return;
	dsptr->dbusy = TRUE;

	/* build parameter block for ROM call */

	pbptr = &dsptr->dpb;
	pbptr->ioResult = 0;		/* <= 0 means operation complete*/
	while (pbptr->ioResult <= 0 && (drptr=dsptr->dreqlst) != DRNULL) {
		pbptr->ioCompletion	= inint;	/* intr. routine*/
		pbptr->ioNamePtr	= NULL;		/* volume name	*/
		pbptr->ioVRefNum	= dsptr->ddrive;/* drive number	*/
		pbptr->u.iop.ioRefNum	= DREFNUM;	/* driver ref. #*/
		pbptr->u.iop.ioBuffer	= drptr->drbuff;/* I/O buffer	*/
		pbptr->u.iop.ioReqCount	= DBUFSIZ;	/* # of bytes	*/
		pbptr->u.iop.ioPosMode	= fsFromStart;	/* absolute pos.*/
		pbptr->u.iop.ioPosOffset= (long) drptr->drdba * DBUFSIZ;
		if (drptr->drop == DSYNC)
			pbptr->u.cp.csCode = drvStsCode;

		disable(ps);		/* Save & restore PS, because	*/
		(*romfunc[drptr->drop])(pbptr, MTRUE);
		restore(ps);		/*  ROM call enables interrupts	*/
	}
	dsptr->dbusy = FALSE;
}


/*------------------------------------------------------------------------
 *  PBEject  --  eject the disk, honoring async flag
 *------------------------------------------------------------------------
 */
PBEject(pbptr, async)
ParamBlkRec	*pbptr;
int		async;
{
	int	(*iocomp)();

	iocomp = pbptr->ioCompletion;	/* Save completion routine	*/
	PBOffLine(pbptr);		/* Place disk volume off-line	*/
	pbptr->ioCompletion = iocomp;	/* Restore completion routine	*/
	pbptr->u.cp.csCode = ejectCode;
	PBControl(pbptr, async);	/* Eject disk			*/
}
