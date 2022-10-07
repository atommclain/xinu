/* conascroll.c - conascroll */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  conascroll  --  wherever the caret is, scroll it into view
 *------------------------------------------------------------------------
 */
conascroll(winptr)
struct	winblk	*winptr;
{
	Rect	*prect;
	int	dv;

	prect = &winptr->wrec.port.portRect;
	dv = prect->bottom-BMARGIN - (winptr->wpenpos.v+winptr->wdescent);
	if (dv < 0) {
		SetOrigin(prect->left, prect->top - dv);
		ScrollRect(prect, 0, dv, winrgn);
	}
}
