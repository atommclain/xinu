/* winascroll.c - winascroll */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winascroll  --  wherever the caret is, scroll it into view
 *------------------------------------------------------------------------
 */
winascroll(winptr)
struct	winblk	*winptr;
{
	Rect	*crect;
	int	vpos, hpos, dh, dv;

	crect = &(*winptr->wrec.port.clipRgn)->rgnBBox;

	vpos = winptr->wpenpos.v + winptr->wdescent;
	if (vpos > crect->bottom - BMARGIN)
		dv = crect->bottom - BMARGIN - vpos;
	else if (vpos - winptr->wlineht < crect->top + TMARGIN)
		dv = crect->top + TMARGIN - (vpos - winptr->wlineht);
	else
		dv = 0;

	vpos += dv;
	hpos = winptr->wpenpos.h;
	if (vpos > winptr->wsbox.top - BMARGIN
	    && hpos > winptr->wsbox.left - RMARGIN)
		dh = winptr->wsbox.left - RMARGIN - hpos;
	else if (hpos > crect->right - RMARGIN)
		dh = crect->right - RMARGIN - hpos;
	else if (hpos < crect->left + LMARGIN)
		dh = crect->left + LMARGIN - hpos;
	else
		dh = 0;

	return(winscroll(winptr, dh, dv));
}
