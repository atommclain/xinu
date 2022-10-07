/* concntl.c - concntl */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <event.h>
#include <desk.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  concntl  --  control the console device
 *------------------------------------------------------------------------
 */
concntl(devptr, func, arg1, arg2)
struct	devsw	*devptr;
int		func, arg1, arg2;
{
	int		ps;
	struct	winblk	*winptr;
	struct	tty	*ttyp;
	int		ret;

	disable(ps);
	winptr = (struct winblk *) devptr->dvioblk;
	ttyp = &winptr->wtty;
	switch (func) {
	case WCASCROLL:
		conascroll(winptr);
		break;

	case TCNEXTC:
		wait(ttyp->isem);
		ret = ttyp->ibuff[ttyp->itail];
		restore(ps);
		signal(ttyp->isem);
		return(ret);

	case TCMODER:
		ttyp->imode = IMRAW;
		break;

	case TCMODEC:
		ttyp->imode = IMCOOKED;
		break;

	case TCMODEK:
		ttyp->imode = IMCBREAK;
		break;

	case TCECHO:
		ttyp->iecho = TRUE;
		break;

	case TCNOECHO:
		ttyp->iecho = FALSE;
		break;

	case TCICHARS:
		ret = scount(ttyp->isem);
		restore(ps);
		return(ret);

	case TCINT:
		ttyp->iintr = TRUE;
		ttyp->iintpid = getpid();
		break;

	case TCINTCH:
		ttyp->iintrc = arg1;
		break;

	case TCNOINT:
		ttyp->iintr = FALSE;
		break;

	default:
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	return(OK);
}
