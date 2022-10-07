/* winactiv.c - winactiv */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winactiv  --  activate or deactivate a window
 *------------------------------------------------------------------------
 */
winactiv(winptr, activate)
struct	winblk	*winptr;
int		activate;
{
	GrafPtr	saveport;
	Rect	crect;

	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	crect.top	= winptr->wrec.port.portRect.top;
	crect.left	= winptr->wrec.port.portRect.left;
	crect.bottom	= winptr->wrec.port.portRect.bottom - GICONHT;
	crect.right	= winptr->wrec.port.portRect.right - GICONWD;
	ClipRect(&crect);
	if (activate) {
		RectRgn(winrgn, &winptr->wsbox);
		DiffRgn(winptr->wrec.port.clipRgn, winrgn,
			winptr->wrec.port.clipRgn);
	}
	DiffRgn(winptr->wrec.port.visRgn, winptr->wrec.port.clipRgn,
		winrgn);
	InvalRgn(winrgn);
	InvalRect(&winptr->wsbox);
	caret(winptr, activate);
	winupdate(winptr);
	SetPort(saveport);
	return(OK);
}
