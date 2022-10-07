/* conread.c - conread */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  conread  --  read one or more characters from the keyboard
 *------------------------------------------------------------------------
 */
conread(devptr, buff, count)
struct	devsw	*devptr;
int	count;
char	*buff;
{
	int	ps;
	struct	winblk	*winptr;
	register struct	tty	*iptr;
	int	avail, nread, ncopy;
	char	ch;
	Bool	eof;

	winptr = (struct winblk *) devptr->dvioblk;
	if (count < 0)
		return(SYSERR);
	iptr = &winptr->wtty;
	disable(ps);

	if ( (avail=scount(iptr->isem)) < 0)
		avail = 0;
	if (count == 0 && (count=avail) == 0) {
		restore(ps);
		return(0);
	}
	eof = FALSE;
	ch = NULLCH;
	ncopy = min(count, avail);
	for (nread=0; nread<ncopy && ch!=NEWLINE && ch!=RETURN; nread++) {
		ch = iptr->ibuff[iptr->itail];
		if ( eof=(ch == iptr->ieofc && iptr->ieof) )
			break;
		*buff++ = ch;
		if (++iptr->itail >= IBUFLEN)
			iptr->itail = 0;
	}
	if (nread > 0)
		sreset(iptr->isem, avail-nread);
	if (!eof)
		for ( ; nread<count && ch!=NEWLINE&&ch!=RETURN; nread++) {
			wait(iptr->isem);
			ch = iptr->ibuff[iptr->itail];
			if ( eof=(ch == iptr->ieofc && iptr->ieof) ) {
				signal(iptr->isem);
				break;
			}
			*buff++ = ch;
			if (++iptr->itail >= IBUFLEN)
				iptr->itail = 0;
		}
	if (eof && nread == 0) {
		wait(iptr->isem);
		if (++iptr->itail >= IBUFLEN)
			iptr->itail = 0;
		nread = EOF;
	}
	restore(ps);
	return(nread);
}
