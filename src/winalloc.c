/* winalloc.c - winalloc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winalloc  --  allocate pseudo-device for a window; return id
 *------------------------------------------------------------------------
 */
winalloc()
{
	int	i;
	int	ps;

	disable(ps);
	for (i = 0; i < Nwin; ++i)
		if (wintab[i].wstate == WFREE) {
			wintab[i].wstate = WUSED;
			restore(ps);
			return(i);
		}
	restore(ps);
	return(SYSERR);
}
