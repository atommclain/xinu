/* winupdate.c - winupdate */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winupdate  --  redraw the text in the given window
 *------------------------------------------------------------------------
 */
winupdate(winptr)
struct	winblk	*winptr;
{
	GrafPtr			saveport;
	PenState		savepen;
	long			savefg;
	Point			saveloc;
	int			savevis;
	register int		top, bottom, vpos;
	register struct	text	*txptr;

	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	BeginUpdate(&winptr->wrec);
	CopyRgn(winptr->wrec.port.clipRgn, winrgn);
	ClipRect(&winptr->wrec.port.portRect);
	EraseRect(&winptr->wrec.port.portRect);
	DrawGrowIcon(&winptr->wrec);
	if (FrontWindow() == &winptr->wrec) {
		savefg = winptr->wrec.port.fgColor;
		ForeColor((long) blackColor);
		PlotIcon(&winptr->wsbox, GetIcon(SICON));
		ForeColor(savefg);
	}
	CopyRgn(winrgn, winptr->wrec.port.clipRgn);
	SectRgn(winrgn,winptr->wrec.port.visRgn,winptr->wrec.port.visRgn);
	SetEmptyRgn(winrgn);
	top	= (*winptr->wrec.port.visRgn)->rgnBBox.top;
	bottom	= (*winptr->wrec.port.visRgn)->rgnBBox.bottom;
	if (top >= bottom) {
		EndUpdate(&winptr->wrec);
		SetPort(saveport);
		return(OK);
	}

	GetPen(&saveloc);
	savevis = winptr->wcarvis;
	winptr->wcarvis = FALSE;	/* Caret has been erased	*/
	caret(winptr, savevis);		/* Redraw caret			*/
	vpos = winptr->wrect.top + winptr->wlineht;
	for (txptr = winptr->txhead.next; vpos<=top && txptr->next!=NULL;
	    txptr = txptr->next)
		vpos += txptr->dv;
	if (txptr->prev != &winptr->txhead) {
		txptr = txptr->prev;
		vpos -= txptr->dv;
	}
	MoveTo(winptr->wrect.left, vpos - winptr->wdescent);
	for (vpos -= winptr->wlineht; vpos<bottom && txptr->next!=NULL;
	    vpos += txptr->dv, txptr = txptr->next)
		txdraw(winptr, txptr->text, txptr->len);
	MoveTo(saveloc.h, saveloc.v);
	EndUpdate(&winptr->wrec);
	SetPort(saveport);
	return(OK);
}
