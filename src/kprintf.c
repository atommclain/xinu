/* kprintf.c - kprintf, kputc */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <osutil.h>
#include <tty.h>
#include <xwindow.h>

extern	struct	text	ctext[NCTEXT];
extern	int	nextct;
extern	Rect	conrect;

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */
kprintf(fmt, args)
	char *fmt;
{
	int		ps;
	GrafPtr		saveport;
	PenState	savepen;
	struct	winblk	*winptr;
	WindowPtr	kwin;
	int		kputc();

	disable(ps);
	if ((winptr=(struct winblk *)devtab[CONSOLE].dvioblk) == NULL) {
		halt();
		start();
	}
	GetPort(&saveport);
	SetPort(kwin = &winptr->wrec.port);
	control(CONSOLE, WCMOVE, conrect.left, conrect.top);
	control(CONSOLE, WCSIZE,
		conrect.right-conrect.left, conrect.bottom-conrect.top);
	ShowWindow(kwin);
	if (FrontWindow() != kwin) {
		SelectWindow(kwin);	/* Bring CONSOLE window to front*/
		control(CONSOLE, WCACTIVATE, TRUE);
	}
	GetPenState(&savepen);
	_doprnt(fmt, &args, kputc, CONSOLE);
	SetPenState(&savepen);
	SetPort(saveport);
	restore(ps);
	return(OK);
}


/*------------------------------------------------------------------------
 *  kputc  --  put a character into the CONSOLE window
 *------------------------------------------------------------------------
 */
kputc(device, ch)
int	device;
char	ch;
{
	register struct	winblk	*winptr;
	register struct	text	*txptr;
	Rect		*prect;
	Point		oldloc;

	winptr = (struct winblk *) devtab[device].dvioblk;
	if (ch == NEWLINE)
		kputc(device, RETURN);
	else if (ch == BELL) {
		InvertRect(prect = &winptr->wrec.port.portRect);
		SysBeep(4);
		InvertRect(prect);
		return;
	}

	MoveTo(oldloc.h=winptr->wpenpos.h, oldloc.v=winptr->wpenpos.v);
	if ((txptr=winptr->txtail.prev)->len >= TXBLKLEN) {
		txptr = &ctext[nextct--];
		if (nextct < 0)
			nextct = NCTEXT - 1;
		if (txptr->next != NULL) {	/* unlink recycled block*/
			txptr->prev->next = txptr->next;
			txptr->next->prev = txptr->prev;
			winptr->wrect.top += txptr->dv;
		}
		txptr->len	= 0;
		txptr->dv	= 0;
		txptr->prev	= winptr->txtail.prev;
		txptr->next	= &winptr->txtail;
		txptr->prev->next	= txptr;
		txptr->next->prev	= txptr;
	}
	caret(winptr, FALSE);
	txptr->text[txptr->len++] = ch;
	txdrawc(winptr, ch);
	GetPen(&winptr->wpenpos);
	if (winptr->wpenpos.v > oldloc.v) {
		txptr->dv += winptr->wpenpos.v - oldloc.v;
		control(device, WCASCROLL);
	}
}
