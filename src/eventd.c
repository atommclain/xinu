/* eventd.c - eventd, doevent, domouse */

#include <conf.h>
#include <kernel.h>
#include <syserr.h>
#include <quickdraw.h>
#include <event.h>
#include <window.h>
#include <desk.h>
#include <menu.h>
#include <retrace.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  eventd  --  do periodic tasks, including handling events
 *------------------------------------------------------------------------
 */
PROCESS	eventd()
{
	WindowRecord	*win;
	EventRecord	event;
	long		recvtim(), GetCaretTime();

	while (TRUE) {
		SystemTask();
		if ( (win=(WindowRecord *)FrontWindow()) != NULL &&
		    win->windowKind >= userKind )
			control((int) win->refCon, WCIDLE);
		if (recvtim((int) GetCaretTime()/6) == TIMEOUT)
			continue;	/* Wake up now&then to run DAs	*/
		while(GetNextEvent(everyEvent, &event))
			doevent(&event);
	} /* end while (TRUE) */
}


/*------------------------------------------------------------------------
 *  doevent  --  respond to an event
 *------------------------------------------------------------------------
 */
doevent(evtptr)
EventRecord	*evtptr;
{
	WindowRecord	*win;
	char		ch;

	switch (evtptr->what) {
	case mouseDown:
		domouse(evtptr);
		break;

	case keyDown:
	case autoKey:
		if ( (win=(WindowRecord *)FrontWindow()) == NULL )
			break;
		ObscureCursor();
		ch = (char) evtptr->message;
		if (evtptr->modifiers & cmdKey)
			ch =	(ch == BACKSP?		'\177' :
				 ch == '2'?		'\0' :
				 ch == '6'?		'^' - 0100 :
				 ch == '-'?		'_' - 0100 :
				 0100<=ch && ch<0140?	ch - 0100 :
				 'a'<=ch && ch<='z'?	ch - 'a' + 1 :
							ch);
		keyiin(devtab[win->refCon].dvioblk, ch);
		break;

	case activateEvt:
		win = (WindowRecord *) evtptr->message;
		if (win->windowKind >= userKind)
			control((int) win->refCon, WCACTIVATE,
				evtptr->modifiers & activeFlag);
		break;

	case updateEvt:
		win = (WindowRecord *) evtptr->message;
		if (win->windowKind < userKind ||
		    control((int) win->refCon, WCUPDATE) == SYSERR)
			SetEmptyRgn(win->updateRgn);
		break;

	case diskEvt:
		if (evtptr->message>>16 != noErr)
			printf("\nError %d in drive %d\n",
				evtptr->message);
		break;
	} /* end switch (evtptr->what) */
}


extern	int	hsrate, vsrate;

/*------------------------------------------------------------------------
 *  domouse  --  respond to mouse clicks, based on where they occur
 *------------------------------------------------------------------------
 */
LOCAL	domouse(evtptr)
EventRecord	*evtptr;
{
	WindowRecord	*evtwin;
	struct	winblk	*winptr;
	int		wdev;
	GrafPtr		port;
	Point		loc, old;
	register int	winh, winv;
	long		now, hlast, vlast;
	EventRecord	event;

	switch (FindWindow(pass(evtptr->where), &evtwin)) {
	case inSysWindow:
		SystemClick(evtptr, evtwin);
		break;

	case inMenuBar:
		domenu(MenuSelect(pass(evtptr->where)));
		break;

	case inGoAway:
		if (TrackGoAway(evtwin, pass(evtptr->where)))
			control((int) evtwin->refCon, WCKILL);
		break;

	case inDrag:
		if (evtwin->windowKind < userKind)
			break;
		wdev = evtwin->refCon;
		SetPort(evtwin);
		loc = topLeft(evtwin->port.portRect);
		LocalToGlobal(&loc);
		winh = evtptr->where.h - loc.h;
		winv = evtptr->where.v - loc.v;
		while (!GetNextEvent(mUpMask, &event)) {
			GetMouse(&loc);
			LocalToGlobal(&loc);
			control(wdev, WCMOVE, loc.h-winh, loc.v-winv);
		}
		control(wdev, WCMOVE, event.where.h-winh,
					event.where.v-winv);
		break;

	case inGrow:
		if (evtwin->windowKind < userKind)
			break;
		wdev = evtwin->refCon;
		winh = evtptr->where.h -
		(evtwin->port.portRect.right-evtwin->port.portRect.left);
		winv = evtptr->where.v -
		(evtwin->port.portRect.bottom-evtwin->port.portRect.top);
		while (!GetNextEvent(mUpMask, &event)) {
			GetMouse(&loc);
			LocalToGlobal(&loc);
			control(wdev, WCSIZE, loc.h-winh, loc.v-winv);
		}
		control(wdev, WCSIZE, event.where.h-winh,
					event.where.v-winv);
		break;

	case inContent:
		if (evtwin != FrontWindow()) {
			SelectWindow(evtwin);
			break;
		}
		if (evtwin->windowKind < userKind)
			break;
		wdev = evtwin->refCon;
		SetPort(evtwin);
		old = evtptr->where;
		GlobalToLocal(&old);
		if (PtInRgn(pass(old), evtwin->port.clipRgn)) {
			while (!GetNextEvent(mUpMask, &event)) {
				GetMouse(&loc);
				control(wdev, WCSCROLL, loc.h-old.h,
						loc.v-old.v);
			}
			GlobalToLocal(&event.where);
			control(wdev, WCSCROLL, event.where.h-old.h,
					event.where.v-old.v);
			break;
		}
		winptr = (struct winblk *) devtab[wdev].dvioblk;
		if (!PtInRect(pass(old), &winptr->wsbox))
			break;
		hlast = vlast = evtptr->when;
		while (StillDown()) {
			old.h = winptr->wsbox.left + SICONWD/2;
			old.v = winptr->wsbox.top  + SICONHT/2;
			GetMouse(&loc);
			now = TickCount();
			winh = (old.h-loc.h)*(now-hlast)*hsrate/600;
			winv = (old.v-loc.v)*(now-vlast)*vsrate/600;
			hlast = (loc.h == old.h || hsrate == 0? now :
				hlast + winh*600/((old.h-loc.h)*hsrate));
			vlast = (loc.v == old.v || vsrate == 0? now :
				vlast + winv*600/((old.v-loc.v)*vsrate));
			control(wdev, WCSCROLL, winh, winv);
		}
		break;
	} /* end switch (FindWindow...) */
}
