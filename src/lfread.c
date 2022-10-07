/* lfread.c - lfread */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <tty.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfread  --  read one or more bytes from a local file
 *------------------------------------------------------------------------
 */
lfread(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
	struct	lfblk	*lfptr;
	int		i;

	lfptr = (struct lfblk *) devptr->dvioblk;
	len = lfio(lfptr, PBRead, buff, len);
	if (!lfptr->lf_crlf)
		return(len);
	for (i = 0; i < len; i++, buff++)
		if (*buff == RETURN)
			*buff = NEWLINE;
		else if (*buff == NEWLINE)
			*buff = RETURN;
	return(len);
}
