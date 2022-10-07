/* winopen.c - winopen */

#include <conf.h>
#include <kernel.h>
#include <font.h>
#include <event.h>
#include <proc.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

extern	Rect	winrect;

/*------------------------------------------------------------------------
 *  winopen  --  create a window and return a device descriptor for it
 *------------------------------------------------------------------------
 */
winopen(devptr, name, mode)
struct	devsw	*devptr;
char	*name;
char	*mode;
{
	struct	winblk	*winptr;
	int		i;
	int		mbits;
	Bool		persist;
	char		pname[WNAMLEN+1];
	GrafPtr		saveport;
	Rect		*prect;
	FontInfo	info;
	struct	text	*txptr;
	struct	tty	*ttyptr;
	EventRecord	event;
	char		*ctop(), *strcpy();

	if ( persist=(*name == '!') )
		name++;
	if (strlen(name) > WNAMLEN || (mbits=ckmode(mode)) == SYSERR ||
	    ((i=winfind(name)) == SYSERR? (mbits&FLOLD) : (mbits&FLNEW)))
		return(SYSERR);
	if (i != SYSERR) {
		winptr = &wintab[i];
		winptr->wmode |= mbits;
		return(winptr->wdnum);
	}
	if ((i=winalloc()) == SYSERR)
		return(SYSERR);
	winptr = &wintab[i];

	strcpy(winptr->wname, name);
	winptr->wpersist	= persist;
	winptr->wmode		= mbits;
	if (NewWindow(&winptr->wrec, &winrect, ctop(strcpy(pname, name)),
	    MTRUE, documentProc, (WindowPtr) -1, MTRUE,
	    (long)winptr->wdnum) != &winptr->wrec) {
		winptr->wstate = WFREE;
		return(SYSERR);
	}
	winrect.top	+= 20;
	winrect.bottom	+= 20;
	winrect.left	+= 30;
	winrect.right	+= 30;
	if (winrect.bottom >= screenBits.bounds.bottom
	    || winrect.right >= screenBits.bounds.right) {
		winrect.top	= 42;
		winrect.left	= 10;
		winrect.bottom	= 200;
		winrect.right	= 300;
	}

	GetPort(&saveport);
	SetPort(&winptr->wrec.port);
	ValidRect(prect = &winptr->wrec.port.portRect);
	TextFont(TXFONT);
	TextSize(TXSIZE);
	GetFontInfo(&info);
	SetPort(saveport);

	winptr->wrect.top	= prect->top	+ TMARGIN;
	winptr->wrect.left	= prect->left	+ LMARGIN;
	winptr->wrect.bottom	= prect->bottom	- BMARGIN;
	winptr->wrect.right	= prect->right	- RMARGIN;
	winptr->wsbox.bottom	= prect->bottom;
	winptr->wsbox.right	= prect->right - GICONWD;
	winptr->wsbox.top	= winptr->wsbox.bottom - SICONHT;
	winptr->wsbox.left	= winptr->wsbox.right  - SICONWD;
	winptr->wlineht		= info.leading+info.ascent+info.descent;
	winptr->wdescent	= info.descent;
	winptr->wtabwid		= TABWIDTH;
	winptr->wpenpos.h	= winptr->wrect.left;
	winptr->wpenpos.v	= winptr->wrect.top
					+ info.leading + info.ascent;
	winptr->wcarvis		= FALSE;

	txptr = &winptr->txhead;
	txptr->len	= TXBLKLEN;
	txptr->dv	= 0;
	txptr->prev	= NULL;
	txptr->next	= &winptr->txtail;

	txptr = &winptr->txtail;
	txptr->len	= 0;
	txptr->dv	= 0;
	txptr->prev	= &winptr->txhead;
	txptr->next	= NULL;

	ttyptr = &winptr->wtty;
	ttyptr->ihead = ttyptr->itail = 0;	/* empty input queue	*/
	ttyptr->isem = screate(0);
	ttyptr->imode = IMCOOKED;
	ttyptr->iecho = ttyptr->evis = TRUE;	/* echo input		*/
	ttyptr->ierase = TRUE;			/* honor erase using ^H	*/
	ttyptr->ierasec = BACKSP;
	ttyptr->ecrlf = ttyptr->icrlf = TRUE;	/* map RETURN on input	*/
	ttyptr->ocrlf = TRUE;
	ttyptr->ieof = ttyptr->ikill = TRUE;	/* set line kill = ^U	*/
	ttyptr->iintr = ttyptr->ieback = FALSE;
	ttyptr->iintrc = INTRCH;
	ttyptr->iintpid = BADPID;
	ttyptr->ikillc = KILLCH;
	ttyptr->ieofc = EOFC;
	ttyptr->icursor = 0;
	ttyptr->ifullc = TFULLC;

	/* Handle activate events immediately */

	while(GetNextEvent(activMask, &event))
			doevent(&event);

	return(winptr->wdnum);
}
