/* winputc.c - winputc */

#include <conf.h>
#include <kernel.h>
#include <osutil.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

#undef	disable			/* Use a mutual exclusion mechanism that*/
#undef	restore			/*  does not disable mouse interrupts	*/
#define	disable(ps)	(ps = chprio(getpid(), MAXINT))
#define	restore(ps)	chprio(getpid(), ps)

/*------------------------------------------------------------------------
 *  winputc  --  insert a character into a window
 *------------------------------------------------------------------------
 */
winputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	int		ps;
	GrafPtr		saveport;
	PenState	savepen;
	register struct	winblk	*winptr;
	register struct	text	*txptr;
	Rect		*prect;
	Point		oldloc;
	SYSCALL		*getmem();

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	if (winptr->wstate == WFREE || ! (winptr->wmode&FLWRITE)) {
		restore(ps);
		return(SYSERR);
	}
	if (ch == NEWLINE && winptr->wtty.ocrlf)
		winputc(devptr, RETURN);
	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	switch(ch) {
	case BELL:
		InvertRect(prect = &winptr->wrec.port.portRect);
		SysBeep(4);
		InvertRect(prect);
		SetPort(saveport);
		restore(ps);
		return(OK);

	case BACKSP:
		txdelete(winptr);
		SetPort(saveport);
		restore(ps);
		return(OK);
	}

	if ((txptr=winptr->txtail.prev)->len >= TXBLKLEN &&
	    (txptr=(struct text *)getmem((long) sizeof(struct text)))
	    != (struct text *) SYSERR) {
		txptr->len	= 0;
		txptr->dv	= 0;
		txptr->prev	= winptr->txtail.prev;
		txptr->next	= &winptr->txtail;
		txptr->prev->next	= txptr;
		txptr->next->prev	= txptr;
	}
	if (txptr == (struct text *) SYSERR)
		txptr = winptr->txtail.prev;
	else
		txptr->text[txptr->len++] = ch;
	caret(winptr, FALSE);
	GetPenState(&savepen);
	MoveTo(oldloc.h=winptr->wpenpos.h, oldloc.v=winptr->wpenpos.v);
	txdrawc(winptr, ch);
	GetPen(&winptr->wpenpos);
	if (winptr->wpenpos.v > oldloc.v) {
		txptr->dv += winptr->wpenpos.v - oldloc.v;
		winascroll(winptr);
	}
	SetPenState(&savepen);
	SetPort(saveport);
	restore(ps);
	return(OK);
}
