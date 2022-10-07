/* wincntl.c - wincntl, winmove, winsize, update */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <event.h>
#include <desk.h>
#include <tty.h>
#include <xwindow.h>

#undef	disable			/* Use a mutual exclusion mechanism that*/
#undef	restore			/*  does not disable mouse interrupts	*/
#define	disable(ps)	(ps = chprio(getpid(), MAXINT))
#define	restore(ps)	chprio(getpid(), ps)

/*------------------------------------------------------------------------
 *  wincntl  --  control a window device
 *------------------------------------------------------------------------
 */
wincntl(devptr, func, arg1, arg2)
struct	devsw	*devptr;
int		func, arg1, arg2;
{
	int		ps;
	struct	winblk	*winptr;
	struct	tty	*ttyp;
	int		ret;

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	ttyp = &winptr->wtty;
	if (winptr->wstate == WFREE) {
		restore(ps);
		return(SYSERR);
	}
	switch (func) {
	case WCIDLE:
		winblink(winptr);
		break;

	case WCACTIVATE:
		winactiv(winptr, arg1);
		break;

	case WCUPDATE:
		winupdate(winptr);
		break;

	case WCMOVE:
		winmove(winptr, arg1, arg2);
		break;

	case WCSIZE:
		winsize(winptr, arg1, arg2);
		break;

	case WCSCROLL:
		winscroll(winptr, arg1, arg2);
		break;

	case WCASCROLL:
		winascroll(winptr);
		break;

	case WCKILL:
		winptr->wpersist = FALSE;
		while (winptr->wstate != WFREE)
			winclose(devptr);
		break;

	case TCNEXTC:
		wait(ttyp->isem);
		ret = ttyp->ibuff[ttyp->itail];
		restore(ps);
		signal(ttyp->isem);
		return(ret);

	case TCMODER:
		ttyp->imode = IMRAW;
		break;

	case TCMODEC:
		ttyp->imode = IMCOOKED;
		break;

	case TCMODEK:
		ttyp->imode = IMCBREAK;
		break;

	case TCECHO:
		ttyp->iecho = TRUE;
		break;

	case TCNOECHO:
		ttyp->iecho = FALSE;
		break;

	case TCICHARS:
		ret = scount(ttyp->isem);
		restore(ps);
		return(ret);

	case TCINT:
		ttyp->iintr = TRUE;
		ttyp->iintpid = getpid();
		break;

	case TCINTCH:
		ttyp->iintrc = arg1;
		break;

	case TCNOINT:
		ttyp->iintr = FALSE;
		break;

	default:
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	return(OK);
}



/*------------------------------------------------------------------------
 *  winmove  --  move a window on the screen
 *------------------------------------------------------------------------
 */
winmove(winptr, newh, newv)
struct	winblk	*winptr;
int		newh, newv;
{
	GrafPtr	saveport;
	Point	origin;

	newv = max(newv, MBARHT + 10);
	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	origin.h = winptr->wrec.port.portRect.left;
	origin.v = winptr->wrec.port.portRect.top;
	LocalToGlobal(&origin);
	if (newh != origin.h || newv != origin.v) {
		MoveWindow(&winptr->wrec, newh, newv, MFALSE);
		update();
	}
	SetPort(saveport);
}



/*------------------------------------------------------------------------
 *  winsize  --  change the size of a window
 *------------------------------------------------------------------------
 */
winsize(winptr, width, height)
struct	winblk	*winptr;
int		width, height;
{
	GrafPtr	saveport;
	Rect	crect;

	width = max(width, SICONWD + GICONWD);
	height = max(max(height, GICONHT), SICONHT);
	if (winptr->wrec.port.portRect.right
	    - winptr->wrec.port.portRect.left == width
	    && winptr->wrec.port.portRect.bottom
	    - winptr->wrec.port.portRect.top == height)
		return;
	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	DiffRgn(winptr->wrec.port.visRgn, winptr->wrec.port.clipRgn,
		winrgn);
	InvalRgn(winrgn);
	SizeWindow(&winptr->wrec, width, height, MTRUE);
	crect = winptr->wrec.port.portRect;
	winptr->wsbox.bottom	= crect.bottom;
	winptr->wsbox.right	= crect.right - GICONWD;
	winptr->wsbox.top	= winptr->wsbox.bottom - SICONHT;
	winptr->wsbox.left	= winptr->wsbox.right  - SICONWD;
	crect.right	-= GICONWD;
	crect.bottom	-= GICONHT;
	ClipRect(&crect);
	if (FrontWindow() == &winptr->wrec) {
		RectRgn(winrgn, &winptr->wsbox);
		DiffRgn(winptr->wrec.port.clipRgn, winrgn,
			winptr->wrec.port.clipRgn);
	}
	DiffRgn(winptr->wrec.port.visRgn, winptr->wrec.port.clipRgn,
		winrgn);
	InvalRgn(winrgn);
	SetEmptyRgn(winrgn);
	SetPort(saveport);
	update();
}



/*------------------------------------------------------------------------
 *  update  --  update the contents of all windows
 *------------------------------------------------------------------------
 */
LOCAL	update()
{
	EventRecord	event;
	WindowRecord	*win;

	while (CheckUpdate(&event))
		if (SystemEvent(&event))
			continue;	/* ROM updates system windows	*/
		else if ( (win=(WindowRecord *)event.message)->windowKind
		    < userKind
		    || control((int) win->refCon, WCUPDATE) == SYSERR)
			SetEmptyRgn(win->updateRgn);
	return(OK);
}
