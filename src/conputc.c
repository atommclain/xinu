/* conputc.c - conputc */

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
 *  conputc  --  draw a character in the console window
 *------------------------------------------------------------------------
 */
conputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	int		ps;
	GrafPtr		saveport;
	PenState	savepen;
	register struct	winblk	*winptr;
	Rect		*prect, erect;

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	if (ch == NEWLINE && winptr->wtty.ocrlf)
		conputc(devptr, RETURN);
	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	caret(winptr, FALSE);
	switch(ch) {
	case BELL:
		InvertRect(prect = &winptr->wrec.port.portRect);
		SysBeep(4);
		InvertRect(prect);
		break;

	case BACKSP:
		erect.right	= winptr->wpenpos.h;
		erect.bottom	= winptr->wpenpos.v + winptr->wdescent;
		erect.top	= erect.bottom - winptr->wlineht;
		if (winptr->wpenpos.h > winptr->wrect.left)
			winptr->wpenpos.h -= CharWidth(BLANK);
		erect.left	= winptr->wpenpos.h;
		EraseRect(&erect);
		break;

	default:
		GetPenState(&savepen);
		MoveTo(winptr->wpenpos.h, winptr->wpenpos.v);
		txdrawc(winptr, ch);
		GetPen(&winptr->wpenpos);
		conascroll(winptr);
		SetPenState(&savepen);
		break;
	}
	caret(winptr, TRUE);
	SetPort(saveport);
	restore(ps);
	return(OK);
}
