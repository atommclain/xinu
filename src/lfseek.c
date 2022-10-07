/* lfseek.c - lfseek */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfseek  --  seek to a specified position of a local file
 *------------------------------------------------------------------------
 */
lfseek(devptr, offset)
struct	devsw	*devptr;
long	offset;
{
	struct	lfblk	*lfptr;

	lfptr = (struct lfblk *)devptr->dvioblk;
	wait(lfptr->lf_mutex);
	lfptr->lf_pos = offset;
	signal(lfptr->lf_mutex);
	return(OK);
}
