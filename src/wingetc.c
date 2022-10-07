/* wingetc.c - wingetc */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  wingetc  --  read one character from a window pseudo-device
 *------------------------------------------------------------------------
 */
wingetc(devptr)
struct	devsw	*devptr;
{
	int	ps;
	int	ch;
	struct	tty	*iptr;
	struct	winblk	*winptr;

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	iptr = &winptr->wtty;
	if (winptr->wstate == WFREE || ! (winptr->wmode & FLREAD)) {
		restore(ps);
		return(SYSERR);
	}
	wait(iptr->isem);		/* wait	for a character	in buff	*/
	ch = ctoi(iptr->ibuff[iptr->itail++]);
	if (iptr->itail	>= IBUFLEN)
		iptr->itail = 0;
	if (iptr->ieof && (iptr->ieofc == ch) )
		ch = EOF;
	restore(ps);
	return(ch);
}
