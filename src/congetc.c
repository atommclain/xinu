/* congetc.c - congetc */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  congetc  -  read one character from the keyboard
 *------------------------------------------------------------------------
 */
congetc(devptr)
struct	devsw	*devptr;
{
	int	ps;
	int	ch;
	struct	tty	*iptr;
	struct	winblk	*winptr;

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	iptr = &winptr->wtty;
	wait(iptr->isem);		/* wait	for a character	in buff	*/
	ch = ctoi(iptr->ibuff[iptr->itail++]);
	if (iptr->itail	>= IBUFLEN)
		iptr->itail = 0;
	if (iptr->ieof && (iptr->ieofc == ch) )
		ch = EOF;
	restore(ps);
	return(ch);
}
