/* lfputc.c - lfputc */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfputc  --  put a single character into a local file
 *------------------------------------------------------------------------
 */
lfputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	char	outch;

	outch = ch;
	if ( write(devptr->dvnum, &outch, 1) == 1)
		return(OK);
	else
		return(SYSERR);
}
