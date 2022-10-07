/* winwrite.c - winwrite */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  winwrite  --  write 'count' characters into a window
 *------------------------------------------------------------------------
 */
winwrite(devptr, buff, count)
register struct	devsw	*devptr;
register char		*buff;
int			count;
{
	register int	i;

	if (count < 0)
		return(SYSERR);
	for (i = count; i > 0; --i)
		if (winputc(devptr, *buff++) == SYSERR)
			return(SYSERR);
	return(count);
}
