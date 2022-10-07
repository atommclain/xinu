/* wininit.c - wininit */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

struct	winblk	wintab[Nwin+1];
Rect		winrect;
int		hsrate, vsrate;
RgnHandle	winrgn;			/* Initialized in initcon	*/

/*------------------------------------------------------------------------
 *  wininit  --  initialize a window pseudo-device
 *------------------------------------------------------------------------
 */
wininit(devptr)
struct	devsw	*devptr;
{
	struct	winblk	*winptr;

	devptr->dvioblk = (char *) (winptr=&wintab[devptr->dvminor]);
	winptr->wdnum	= devptr->dvnum;
	winptr->wstate	= WFREE;

	if (devptr->dvminor == 0) {	/* done just once */
		winrect.top	= 42;
		winrect.left	= 10;
		winrect.bottom	= 200;
		winrect.right	= 300;
		hsrate = vsrate = 60;
	}
}
