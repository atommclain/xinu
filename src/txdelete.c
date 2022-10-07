/* txdelete.c - txdelete */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  txdelete  --  erase a character in a window
 *------------------------------------------------------------------------
 */
txdelete(winptr)
struct	winblk	*winptr;
{
	PenState		savepen;
	register struct	text	*txptr, *txptr2;
	register int		i;
	register char		ch;
	Point			oldloc;
	Rect			srect;

	txptr = winptr->txtail.prev;
	while (txptr->len == 0) {
		txptr->prev->next = txptr->next;
		txptr->next->prev = txptr->prev;
		txptr = (txptr2=txptr)->prev;
		freemem(txptr2, (long) sizeof(struct text));
	}
	if (txptr == &winptr->txhead)
		return(OK);
	GetPenState(&savepen);
	oldloc = winptr->wpenpos;
	caret(winptr, FALSE);
	switch (ch = txptr->text[--txptr->len]) {
	case NEWLINE:
		txptr->dv -= winptr->wlineht;
		winptr->wpenpos.v -= winptr->wlineht;
		/* Fall through */

	case RETURN:
	case '\t':
		for (txptr2=txptr;txptr2->prev!=NULL;txptr2=txptr2->prev)
			for (i=txptr2->len-1; i>=0; --i)
				if (txptr2->text[i] == NEWLINE ||
				    txptr2->text[i] == RETURN)
					goto at_margin;
	at_margin:
		++i;				/* Skip over \r or \n	*/
		if (txptr2 == &winptr->txhead) {
			txptr2 = txptr2->next;
			i = 0;
		}
		MoveTo(winptr->wrect.left, winptr->wpenpos.v);
		for ( ; txptr2 != txptr; txptr2 = txptr2->next, i = 0)
			txdraw(winptr, &txptr2->text[i], txptr2->len-i);
		txdraw(winptr, &txptr->text[i], txptr->len-i);
		GetPen(&winptr->wpenpos);
		break;

	default:
		if (iscntrl(ch)) {
			winptr->wpenpos.h -= CharWidth(UPARROW);
			ch = (ch == 0177? '?' : ch+0100);
		}
		winptr->wpenpos.h -= CharWidth(ch);
		break;
	}
	srect.bottom	= winptr->wpenpos.v + winptr->wdescent;
	srect.top	= srect.bottom - winptr->wlineht;
	srect.left	= winptr->wpenpos.h;
	srect.right	= (*winptr->wrec.port.clipRgn)->rgnBBox.right;
	if (winptr->wpenpos.v == oldloc.v) {
		ScrollRect(&srect, winptr->wpenpos.h-oldloc.h, 0, winrgn);
		InvalRgn(winrgn);
	}
	SetPenState(&savepen);
	return(OK);
}
