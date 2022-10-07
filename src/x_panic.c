/* x_panic.c - x_panic */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_panic  --  (shell command panic) panic
 *------------------------------------------------------------------------
 */
x_panic(nargs, args)
int	nargs;
char	*args[];
{
	char	*message;

	if (nargs > 2) {
		fprintf(STDERR, "Usage: panic [message]\n");
		return(SYSERR);
	}
	message = (nargs == 2)? args[1] : "from shell";
	panic(message);
}
