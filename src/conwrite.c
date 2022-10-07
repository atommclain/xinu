/* conwrite.c - conwrite */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  conwrite  --  write 'count' characters into the console window
 *------------------------------------------------------------------------
 */
conwrite(devptr, buff, count)
register struct	devsw	*devptr;
register char		*buff;
int			count;
{
	register int	i;

	if (count < 0)
		return(SYSERR);
	for (i = count; i > 0; --i)
		if (conputc(devptr, *buff++) == SYSERR)
			return(SYSERR);
	return(count);
}
