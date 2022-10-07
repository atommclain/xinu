/* lfclose.c - lfclose */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfclose  --  close a local file by deallocating pseudo device
 *------------------------------------------------------------------------
 */
lfclose(devptr)
struct	devsw	*devptr;
{
	struct	lfblk	*lfptr;

	lfptr = (struct lfblk *) devptr->dvioblk;
	return(lfio(lfptr, PBClose, NULL, 0));
}
