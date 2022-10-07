/* x_color.c - x_color */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <tty.h>
#include <xwindow.h>

#define	trcolor(x)	( (x) < whiteColor? 68*(x) + 1 : (x) )

/*------------------------------------------------------------------------
 *  x_color  --  (command color) change colors in STDOUT window
 *------------------------------------------------------------------------
 */
COMMAND	x_color(nargs, args)
int	nargs;
char	*args[];
{
	int	ps;
	int	dev;
	struct	winblk	*winptr;
	GrafPtr	saveport;
	long	color;

	if (nargs > 4) {
		fprintf(STDERR, "Usage: color [fg [bg [plane]]]\n");
		return(SYSERR);
	}
	dev = getdev(getpid(), STDOUT);
	winptr = (struct winblk *)devtab[dev].dvioblk;
	if (winptr < &wintab[0] || &wintab[Nwin] < winptr) {
		fprintf(STDERR,
			"color: output device must be a window\n");
		return(SYSERR);
	}
	if (nargs == 1) {
		fprintf(STDERR,"foreground=%D, background=%D, plane=%d\n",
			winptr->wrec.port.fgColor,
			winptr->wrec.port.bkColor,
			winptr->wrec.port.colorBit);
		return(OK);
	}

	disable(ps);
	GetPort(&saveport);
	SetPort(&winptr->wrec);
	if ( (color=atoi(args[1])) > 0)
		ForeColor( trcolor(color) );
	if (nargs > 2) {
		if ( (color=atoi(args[2])) > 0)
			BackColor( trcolor(color) );
		if (nargs > 3)
			ColorBit( atoi(args[3]) );
	}
	InvalRect(&thePort->portRect);
	control(dev, WCUPDATE);
	SetPort(saveport);
	restore(ps);
	return(OK);
}
