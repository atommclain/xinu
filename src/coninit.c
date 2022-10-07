/* coninit.c - coninit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

#define	CNAME	"CONSOLE"

/*------------------------------------------------------------------------
 *  coninit  --  initialize console window device
 *------------------------------------------------------------------------
 */
coninit(devptr)
struct	devsw	*devptr;
{
	struct	winblk	*winptr;

	winptr = (struct winblk *) devptr->dvioblk;

	strcpy(winptr->wname, CNAME);
	winptr->wmode = FLRW;
	winptr->wtty.ihead = winptr->wtty.itail = 0;
	winptr->wtty.isem = screate(0);
	winptr->wtty.imode = IMCOOKED;
	winptr->wtty.iecho = winptr->wtty.evis = TRUE;
	winptr->wtty.ierase = TRUE;
	winptr->wtty.ierasec = BACKSP;
	winptr->wtty.ecrlf = winptr->wtty.icrlf = TRUE;
	winptr->wtty.ocrlf = TRUE;
	winptr->wtty.ieof = winptr->wtty.ikill = TRUE;
	winptr->wtty.iintr = winptr->wtty.ieback = FALSE;
	winptr->wtty.iintrc = INTRCH;
	winptr->wtty.iintpid = BADPID;
	winptr->wtty.ikillc = KILLCH;
	winptr->wtty.ieofc = EOFC;
	winptr->wtty.icursor = 0;
	winptr->wtty.ifullc = TFULLC;
}
