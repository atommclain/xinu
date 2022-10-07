/* caret.c - caret */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  caret  --  draw or erase caret in a text window
 *------------------------------------------------------------------------
 */
caret(winptr, wantvis)
struct	winblk	*winptr;
Bool	wantvis;
{
	PenState	savepen;

	if (winptr->wcarvis? wantvis : !wantvis)
		return;
	GetPenState(&savepen);
	PenNormal();
	if (! (winptr->wcarvis=wantvis) )
		PenMode(notPatCopy);
	MoveTo(winptr->wpenpos.h-1,
		winptr->wpenpos.v+winptr->wdescent-winptr->wlineht+1);
	Line(0, winptr->wlineht - 2);
	SetPenState(&savepen);
}
