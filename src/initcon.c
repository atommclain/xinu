/* initcon.c - initcon */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <font.h>
#include <tty.h>
#include <xwindow.h>

struct	text	ctext[NCTEXT];		/* Text blocks for kprintf	*/
int	nextct;
Rect	conrect = {42, 20, 330, 490};
#define	CNAME	"\PCONSOLE"

/*------------------------------------------------------------------------
 *  initcon  --  initialize CONSOLE window so that kprintf can use it
 *------------------------------------------------------------------------
 */
initcon()
{
	struct	winblk	*winptr;
	Rect		*prect;
	FontInfo	info;
	int		i;

	winptr = &wintab[Nwin];
	if (NewWindow(&winptr->wrec, &conrect, CNAME, MTRUE, documentProc,
	    (WindowPtr) -1, MFALSE, (long)CONSOLE) != &winptr->wrec)
		panic("cannot open CONSOLE window");
	SetPort(&winptr->wrec.port);
	ValidRect(prect = &winptr->wrec.port.portRect);
	TextFont(TXFONT);
	TextSize(TXSIZE);
	DrawChar(BLANK);	/* Use the font for SetFontLock		*/
	SetFontLock(MTRUE);	/* Keep font in memory			*/
	GetFontInfo(&info);
	ForeColor((long) blueColor);
	winrgn = NewRgn();

	devtab[CONSOLE].dvioblk = (char *) winptr;
	winptr->wdnum		= CONSOLE;
	winptr->wstate		= WUSED;
	winptr->wpersist	= FALSE;
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
				+ winptr->wlineht - winptr->wdescent;
	winptr->wcarvis		= FALSE;
	winptr->txhead.len	= TXBLKLEN;
	winptr->txhead.dv	= 0;
	winptr->txhead.prev	= NULL;
	winptr->txhead.next	= &winptr->txtail;
	winptr->txtail.len	= 0;
	winptr->txtail.dv	= 0;
	winptr->txtail.prev	= &winptr->txhead;
	winptr->txtail.next	= NULL;

	nextct = NCTEXT - 1;
	for (i = 0; i < NCTEXT; ++i)
		ctext[i].prev = ctext[i].next = NULL;
}
