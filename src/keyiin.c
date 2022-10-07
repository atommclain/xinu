/* keyiin.c - keyiin, erase1, echoch, eputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  keyiin  --  lower-half keyboard device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	keyiin(winptr, ch)
struct	winblk	*winptr;
char		ch;
{
	register struct	tty	*iptr;
	register int		ct;

	iptr = &winptr->wtty;
	if (iptr->imode == IMRAW) {
		if (scount(iptr->isem) >= IBUFLEN)
			return;			/* discard if no space	*/
		iptr->ibuff[iptr->ihead++] = ch;
		if (iptr->ihead	>= IBUFLEN)	/* wrap buffer pointer	*/
			iptr->ihead = 0;
		signal(iptr->isem);
	} else {				/* cbreak | cooked mode	*/
		if ( ch	== RETURN && iptr->icrlf )
			ch = NEWLINE;
		if (iptr->iintr && ch == iptr->iintrc) {
			send(iptr->iintpid, (long)INTRMSG);
			echoch(winptr, ch);
			return;
		}
		if (iptr->imode	== IMCBREAK) {		/* cbreak mode	*/
			if (scount(iptr->isem) >= IBUFLEN) {
				eputc(winptr, iptr->ifullc);
				return;
			}
			iptr->ibuff[iptr->ihead++] = ch;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			if (iptr->iecho)
				echoch(winptr, ch);
			if (scount(iptr->isem) < IBUFLEN)
				signal(iptr->isem);
		} else {				/* cooked mode	*/
			if (ch == iptr->ikillc && iptr->ikill) {
				for ( ; iptr->icursor>0; --iptr->icursor)
					erase1(winptr);
				return;
			}
			if (ch == iptr->ierasec	&& iptr->ierase) {
				if (iptr->icursor > 0) {
					iptr->icursor--;
					erase1(winptr);
				}
				return;
			}
			if (ch == NEWLINE || ch == RETURN ||
			    (ch == iptr->ieofc && iptr->ieof)) {
				if (iptr->iecho) {
					echoch(winptr, ch);
					if (ch == iptr->ieofc)
						echoch(winptr, NEWLINE);
				}
				iptr->ibuff[iptr->ihead++] = ch;
				if (iptr->ihead	>= IBUFLEN)
					iptr->ihead = 0;
				ct = iptr->icursor+1; /* +1 for \n or \r*/
				iptr->icursor =	0;
				signaln(iptr->isem,ct);
				return;
			}
			if ( (ct=scount(iptr->isem)) < 0)
				ct = 0;
			if ((ct	+ iptr->icursor) >= IBUFLEN-1) {
				eputc(winptr, iptr->ifullc);
				return;
			}
			if (iptr->iecho)
				echoch(winptr, ch);
			iptr->icursor++;
			iptr->ibuff[iptr->ihead++] = ch;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
		}
	}
}



/*------------------------------------------------------------------------
 *  erase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL erase1(winptr)
struct	winblk	*winptr;
{
	struct	tty	*iptr = &winptr->wtty;
	char		ch;

	if (--(iptr->ihead) < 0)
		iptr->ihead += IBUFLEN;
	ch = iptr->ibuff[iptr->ihead];
	if (iptr->iecho) {
		eputc(winptr, BACKSP);
		if (iptr->ieback) {
			eputc(winptr, BLANK);
			eputc(winptr, BACKSP);
		}
		if (iscntrl(ch) && iptr->evis) {
			eputc(winptr, BACKSP);
			if (iptr->ieback) {
				eputc(winptr, BLANK);
				eputc(winptr, BACKSP);
			}
		}
	}
}



/*------------------------------------------------------------------------
 *  echoch  --  echo a character with visual and ocrlf options
 *------------------------------------------------------------------------
 */
LOCAL	echoch(winptr, ch)
struct	winblk	*winptr;
char		ch;
{
	struct	tty	*iptr = &winptr->wtty;

	if ((ch == NEWLINE || ch == RETURN) && iptr->ecrlf) {
		eputc(winptr, RETURN);
		eputc(winptr, NEWLINE);
	} else if (iscntrl(ch) && iptr->evis) {
		eputc(winptr, UPARROW);
		eputc(winptr, (ch+0100) & 0177); /* make it printable	*/
	} else
		eputc(winptr, ch);
}



/*------------------------------------------------------------------------
 *  eputc  --  echo a character in a text window
 *------------------------------------------------------------------------
 */
eputc(winptr, ch)
struct	winblk	*winptr;
char		ch;
{
	GrafPtr		saveport;

	putc(winptr->wdnum, ch);

	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	control(winptr->wdnum, WCASCROLL);	/* Always auto-scroll	*/
	caret(winptr, TRUE);
	SetPort(saveport);
}
