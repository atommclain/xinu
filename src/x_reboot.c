/* x_reboot.c - x_reboot */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_reboot  --  (builtin command reboot) restart the system from scratch
 *------------------------------------------------------------------------
 */
BUILTIN	x_reboot(nargs, args)
int	nargs;
char	*args[];
{
	start();	/* warning! This terminates everything. */
}
