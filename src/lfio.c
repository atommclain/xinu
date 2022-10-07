/* lfio.c - lfio */

#include <conf.h>
#include <kernel.h>
#include <syserr.h>
#include <pb.h>
#include <file.h>
#include <lfile.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  lfio  --  perform input or output on a local file
 *------------------------------------------------------------------------
 */
lfio(lfptr, romfunc, buff, len)
struct	lfblk	*lfptr;
int	(*romfunc)();
char	*buff;
int	len;
{
	ParamBlkRec	*pbptr;
	int		stat;
	int		ps;

	wait(lfptr->lf_mutex);
	if (len < 0 || lfptr->lf_state == LFREE) {
		signal(lfptr->lf_mutex);
		return(SYSERR);
	}
	wait(Lf.lmutex);
	pbptr = &Lf.lpb;
	pbptr->ioCompletion = inint;		/* interrupt routine	*/
	pbptr->u.iop.ioRefNum	= lfptr->lf_refnum;
	pbptr->u.iop.ioBuffer	= buff;		/* I/O buffer		*/
	pbptr->u.iop.ioReqCount	= len;		/* # of bytes		*/
	pbptr->u.iop.ioPosMode	= fsFromStart;	/* absolute positioning	*/
	pbptr->u.iop.ioPosOffset= lfptr->lf_pos;

	disable(ps);			/* Save & restore PS, because	*/
	(*romfunc)(pbptr, MTRUE);
	restore(ps);			/*  ROM call enables interrupts	*/

	wait(Lf.ldone);			/* Wait for op. to complete	*/
	if (romfunc == PBRead || romfunc == PBWrite) {
		lfptr->lf_pos = pbptr->u.iop.ioPosOffset;
		stat = pbptr->u.iop.ioActCount;
		stat = (pbptr->ioResult==eofErr && stat==0? EOF :
			pbptr->ioResult==eofErr || pbptr->ioResult==noErr?
			stat : SYSERR);
	}
	else {
		stat = (pbptr->ioResult==noErr? OK : SYSERR);
		if (romfunc == PBClose && stat == OK)
			lfptr->lf_state = LFREE;
	}
	signal(Lf.lmutex);
	signal(lfptr->lf_mutex);
	return(stat);
}
