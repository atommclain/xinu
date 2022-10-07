/* panhalt.c - panhalt */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <quickdraw.h>
#include <event.h>
#include <proc.h>

#define	PANMENU	"\
1) Continue; 2) Debug; 3) Reboot XINU; 4) Reboot Macintosh;\n\
5) Quit"

extern	int	*lowcore[];

/*------------------------------------------------------------------------
 *  panhalt  --  get keyboard response; return TRUE iff user wants to halt
 *------------------------------------------------------------------------
 */
panhalt()
{
	int			ps;
	int			*savevec;
	register struct pentry	*pptr;
	EventRecord		event;
	int			start(), macboot(), xexit();

	disable(ps);
	savevec = intvec[DEBUG];
	setdebug(lowcore[DEBUG]);
	pptr = &proctab[currpid];
	kprintf("\nProcess %d (%s), stack limit/base: %X/%X\n%s\n",
		currpid, pptr->pname, pptr->plimit, pptr->pbase,
		PANMENU);
	FlushEvents(keyDownMask, 0);

	/* Wait for the user to press a key */
	while (TRUE) {
#asm
		stop	#$2000		; Enable & wait for interrupt
#endasm
		if (GetOSEvent(keyDownMask, &event))
			switch (event.message & charCodeMask) {
			case '1':	setdebug(savevec);
					restore(ps);
					return(FALSE);

			case '2':	setdebug(savevec);
					restore(ps);
					return(TRUE);

			case '3':	setdebug(savevec);
					shutdown(start);

			case '4':	setdebug(savevec);
					shutdown(macboot);

			case '5':	setdebug(savevec);
					shutdown(xexit);
			}
	}
}
