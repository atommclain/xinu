/* winblink.c - winblink */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>
#include <event.h>

LOCAL	long	nextblink = 0;

/*------------------------------------------------------------------------
 *  winblink  --  make the caret in the active window blink
 *------------------------------------------------------------------------
 */
winblink(winptr)
struct	winblk	*winptr;
{
	GrafPtr	saveport;
	long	now = TickCount();
	long	GetCaretTime();

	if (now < nextblink)
		return(OK);
	nextblink = now + GetCaretTime();
	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	caret(winptr, !winptr->wcarvis);
	SetPort(saveport);
	return(OK);
}
