/* lfnamop.c - lfnamop */

#include <conf.h>
#include <kernel.h>
#include <syserr.h>
#include <pb.h>
#include <file.h>
#include <lfile.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  lfnamop  --  execute ROM call for by-name operations
 *------------------------------------------------------------------------
 */
lfnamop(romfunc, name, mode, buff)
int	(*romfunc)();
char	*name;
int	mode;
char	*buff;
{
	ParamBlkRec	*pbptr;
	FInfo		finfo;
	int		stat;
	int		ps;

	wait(Lf.lmutex);
	pbptr = &Lf.lpb;
	pbptr->ioCompletion	= inint;	/* interrupt routine	*/
	pbptr->ioNamePtr	= name;		/* file name		*/
	pbptr->ioVRefNum	= 0;		/* default volume	*/
	pbptr->u.iop.ioVersNum	= 0;		/* version # (unused)	*/
	pbptr->u.iop.ioPermssn	= (mode&FLREAD? fsRdPerm : 0)
				| (mode&FLWRITE? fsWrPerm : 0);
	pbptr->u.iop.ioMisc	= buff;		/* buffer or file name	*/

	disable(ps);			/* Save & restore PS, because	*/
	(*romfunc)(pbptr, MTRUE);
	restore(ps);			/*  ROM call enables interrupts	*/

	wait(Lf.ldone);			/* Wait for op. to complete	*/
	if (romfunc == PBDirCreate)
		stat = (pbptr->ioResult==noErr||pbptr->ioResult==dupFNErr?
			OK : SYSERR);
	else if (romfunc == PBCreate) {
		finfo.fdType	= LFTYPE;
		finfo.fdCreator	= LFOWNER;
		finfo.fdFlags	= 0;
		stat = (pbptr->ioResult == noErr &&
			SetFInfo(name, 0, &finfo) == noErr?
			OK : SYSERR);
	}
	else if (romfunc == PBOpen)
		stat = (pbptr->ioResult==noErr||pbptr->ioResult==opWrErr?
			pbptr->u.iop.ioRefNum : SYSERR);
	else
		stat = (pbptr->ioResult == noErr? OK : SYSERR);
	signal(Lf.lmutex);
	return(stat);
}
