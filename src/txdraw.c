/* txdraw.c - txdraw */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  txdraw  --  draw text in a window
 *------------------------------------------------------------------------
 */
txdraw(winptr, buf, len)
struct	winblk	*winptr;
register char	*buf;
int		len;
{
	register char	*cp, *end;

	for (cp = buf, end = buf+len; cp < end; ++cp)
		if (iscntrl(*cp)) {
			if (buf < cp)
				DrawText(buf, 0, cp-buf);
			txdrawc(winptr, *cp);
			buf = cp + 1;
		}
	if (buf < cp)
		DrawText(buf, 0, cp-buf);
}
