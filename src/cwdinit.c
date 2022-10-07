/* cwdinit.c - cwdinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <name.h>
#include <cwd.h>

cwdtab	cwd[Ncwd];

/*------------------------------------------------------------------------
 *  cwdinit -- initialize the cwd device (called by null process)
 *------------------------------------------------------------------------
 */
cwdinit(devptr)
struct	devsw	*devptr;
{
	devptr->dvioblk = (char *) cwd[devptr->dvminor];
	*cwd[devptr->dvminor][getpid()] = NULLCH;
}
