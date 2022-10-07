/* getpid.c - getpid */

#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  getpid  --  get the process id of the currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	return(currpid);
}
