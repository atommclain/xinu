/* lfgetc.c - lfgetc */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfgetc  --  get a character from a local file
 *------------------------------------------------------------------------
 */
lfgetc(devptr)
struct	devsw	*devptr;
{
	char	ch;
	int	retcode;

	if ( (retcode=read(devptr->dvnum, &ch, 1)) == 1)
		return(ch);
	else
		return(retcode==EOF? EOF : SYSERR);
}
