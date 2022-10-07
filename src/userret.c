/* userret.c - userret */

#include <kernel.h>

/*------------------------------------------------------------------------
 *  userret  --  entered when a process exits by return
 *------------------------------------------------------------------------
 */
userret()
{
	kill( getpid() );
}
