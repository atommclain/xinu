/* winclose.c - winclose */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  winclose  --  close a window pseudo-device
 *------------------------------------------------------------------------
 */
winclose(devptr)
struct	devsw	*devptr;
{
	int		ps;
	struct	winblk	*winptr;
	struct	text	*txptr, *txptr2;

	winptr = (struct winblk *) devptr->dvioblk;
	disable(ps);
	if (winptr->wstate == WFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (winptr->wpersist) {
		restore(ps);
		return(OK);
	}
	winptr->wstate = WFREE;
	CloseWindow(&winptr->wrec);
	for (txptr = winptr->txhead.next; txptr->next != NULL; ) {
		txptr = (txptr2 = txptr)->next;
		freemem(txptr2, (long) sizeof(struct text));
	}
	sdelete(winptr->wtty.isem);
	restore(ps);
	return(OK);
}
