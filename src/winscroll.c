/* winscroll.c - winscroll */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winscroll  --  scroll window contents dv pixels (up or down)
 *------------------------------------------------------------------------
 */
winscroll(winptr, dh, dv)
struct	winblk	*winptr;
int		dh, dv;
{
	if (winptr->wstate == WFREE)
		return(SYSERR);
	if (dh > winptr->wrec.port.portRect.left)
		dh = winptr->wrec.port.portRect.left;
	if (dv > winptr->wrec.port.portRect.top)
		dv = winptr->wrec.port.portRect.top;
	if (dh == 0 && dv == 0)
		return(OK);

	/* Scroll the window contents */
	SetOrigin(winptr->wrec.port.portRect.left - dh,
		winptr->wrec.port.portRect.top - dv);
	OffsetRgn(winptr->wrec.port.clipRgn, -dh, -dv);
	OffsetRect(&winptr->wsbox, -dh, -dv);
	ScrollRect(&winptr->wrec.port.portRect, dh, dv, winrgn);
	InvalRgn(winrgn);
	control(winptr->wdnum, WCUPDATE);
	return(OK);
}
