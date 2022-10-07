/* x_debug.c - x_debug */

#include <kernel.h>

/* Prevent context switches by setting maximum priority instead of by	*/
/* disabling interrupts, since some debuggers enable interrupts.	*/

#undef	disable
#undef	restore
#define	disable(ps)	(ps = chprio(getpid(), MAXINT))
#define	restore(ps)	chprio(getpid(), ps)

/*------------------------------------------------------------------------
 *  x_debug  --  (builtin command debug) trap to the debugger
 *------------------------------------------------------------------------
 */
BUILTIN	x_debug(nargs, args)
int	nargs;
char	*args[];
{
	int	ps;

	disable(ps);
	halt();
	restore(ps);
	return(OK);
}
