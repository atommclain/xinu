/* winfind.c - winfind */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winfind  --  return the minor device number of window with given name
 *------------------------------------------------------------------------
 */
winfind(name)
char	*name;
{
	int		ps;
	register int	i;

	disable(ps);
	for (i = 0; i <= Nwin; ++i)	/* CONSOLE is in wintab[Nwin]	*/
		if (wintab[i].wstate == WUSED &&
		    strcmp(wintab[i].wname, name) == 0) {
			restore(ps);
			return(i);
		}
	restore(ps);
	return(SYSERR);
}
