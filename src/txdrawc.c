/* txdrawc.c - txdrawc */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  txdrawc  --  draw a character in a window
 *------------------------------------------------------------------------
 */
txdrawc(winptr, ch)
struct	winblk	*winptr;
char		ch;
{
	Point	loc;
	int	dh;

	if (isprint(ch)) {
		DrawChar(ch);
		return;
	}
	switch (ch) {
	case RETURN:
		GetPen(&loc);
		MoveTo(winptr->wrect.left, loc.v);
		break;

	case NEWLINE:
		Move(0, winptr->wlineht);
		break;

	case '\t':
		DrawChar(ch);
		GetPen(&loc);
		if ((dh=(loc.h-winptr->wrect.left)%winptr->wtabwid) > 0)
			Move(winptr->wtabwid - dh, 0);
		break;

	default:
		DrawChar(UPARROW);
		DrawChar(ch == 0177? '?' : ch+0100);
		break;
	}
}
