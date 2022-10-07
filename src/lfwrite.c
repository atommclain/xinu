/* lfwrite.c - lfwrite */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <tty.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfwrite  --  write one or more bytes to a local file
 *------------------------------------------------------------------------
 */
lfwrite(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
	struct	lfblk	*lfptr;
	int		i, wlen;
	char		*cp;
	char		*lfbuff, *getbuf();

	lfptr = (struct lfblk *) devptr->dvioblk;
	lfbuff = getbuf(lfdbp);
	for (i = len; i > 0; i -= wlen, buff += wlen) {
		blkcopy(lfbuff, buff, wlen=min(i,LBUFSIZ));
		if (lfptr->lf_crlf)
			for (cp = lfbuff; cp < lfbuff+wlen; cp++)
				if (*cp == RETURN)
					*cp = NEWLINE;
				else if (*cp == NEWLINE)
					*cp = RETURN;
		if ( (wlen=lfio(lfptr, PBWrite, lfbuff, wlen)) <= 0 ) {
			freebuf(lfbuff);
			return(SYSERR);
		}
	}
	freebuf(lfbuff);
	return(len);
}
