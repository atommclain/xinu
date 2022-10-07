/* initmenu.c - initmenu, domenu, doabout */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <resource.h>
#include <quickdraw.h>
#include <font.h>
#include <event.h>
#include <window.h>
#include <menu.h>
#include <desk.h>
#include <mem.h>
#include <sleep.h>

#define	APPLEMENU	128		/* Menu id of apple menu	*/
#define	SHUTDOWNMENU	129		/* Menu id of Shutdown menu	*/

MenuHandle	appleMenu;

/*------------------------------------------------------------------------
 *  initmenu  --  create XINU menus and draw menu bar
 *------------------------------------------------------------------------
 */
initmenu()
{
	MenuHandle	menu;

	appleMenu = NewMenu(APPLEMENU, "\P\024");
	AppendMenu(appleMenu, "\PAbout XINU\311;(-");
	AddResMenu(appleMenu, 'DRVR');	/* Add DA names to menu		*/
	InsertMenu(appleMenu, 0);
	menu = NewMenu(SHUTDOWNMENU, "\PShutdown");
	AppendMenu(menu, "\PReboot XINU;Reboot Macintosh;(-;Quit");
	InsertMenu(menu, 0);
	DrawMenuBar();
}

extern	int	start(), macboot(), xexit();
int	(*sdtype[])() = {ioerr, start, macboot, ioerr, xexit};

/*------------------------------------------------------------------------
 *  domenu  --  perform the action indicated by the menu result
 *------------------------------------------------------------------------
 */
domenu(mResult)
long	mResult;
{
	GrafPtr	saveport;
	int	theMenu, theItem;
	char	name[256];

	theMenu = mResult >> 16;
	theItem = mResult & LOW16;
	switch (theMenu) {
	case APPLEMENU:
		GetPort(&saveport);
		if (theItem == 1)
			doabout();
		else {
			GetItem(appleMenu, theItem, name);
			OpenDeskAcc(name);
		}
		SetPort(saveport);
		break;

	case SHUTDOWNMENU:
		if (theItem < sizeof(sdtype)/sizeof(sdtype[0]))
			shutdown(sdtype[theItem]);
		break;
	}
	HiliteMenu(0);
}

#define	aboutWd	280		/* Width of "About XINU..." dialog box	*/
LOCAL	Rect	aboutRect = {100, 0, 170, 0};
LOCAL	Rect	iconRect  = {32, 220, 64, 252};
extern	int	reboot;		/* Non-zero after first boot		*/
extern	char	vers[];		/* Xinu version printed at startup	*/

/*------------------------------------------------------------------------
 *  doabout  --  display "About XINU..." dialog box
 *------------------------------------------------------------------------
 */
LOCAL	doabout()
{
	WindowRecord	dialog;
	FontInfo	info;
	int		lineht;
	char		str[80];
	struct	mblock	*mptr;
	unsigned long	free;
	EventRecord	event;

	aboutRect.left	= (screenBits.bounds.left +
				screenBits.bounds.right - aboutWd)/2;
	aboutRect.right	= aboutRect.left + aboutWd;
	if (NewWindow(&dialog, &aboutRect, NULLSTR, MTRUE, dBoxProc,
	    (WindowPtr) -1, MFALSE, 0L) != &dialog)
		return;
	SetPort(&dialog.port);
	PlotIcon(&iconRect, GetResource('ICN#', 128));
	TextFont(systemFont);
	GetFontInfo(&info);
	lineht = info.leading + info.ascent + info.descent;
	for(free=0, mptr=memlist.mnext; mptr!=NULL; mptr=mptr->mnext)
		free += mptr->mlen;

	MoveTo(4, lineht);
	sprintf(str, "Xinu Version %s", vers);
	DrawText(str, 0, strlen(str));
	if (reboot > 1) {
		sprintf(str, "   (reboot %d)", reboot);
		DrawText(str, 0, strlen(str));
	}
	MoveTo(4, 2*lineht);
	sprintf(str, "%U real mem", MemTop);
	DrawText(str, 0, strlen(str));
	MoveTo(4, 3*lineht);
	sprintf(str, "%U avail mem", free);
	DrawText(str, 0, strlen(str));
	MoveTo(4, 4*lineht);
	sprintf(str, "clock %sabled", clkruns? "en" : "dis");
	DrawText(str, 0, strlen(str));
	while (!GetNextEvent(mDownMask|keyDownMask, &event))
		;
	CloseWindow(&dialog);
	while (GetNextEvent(activMask, &event))
		;			/* Flush activate events	*/
}
