/* txmark.c - txmark */

#include <kernel.h>
#include <tty.h>
#include <xwindow.h>

/*------------------------------------------------------------------------
 *  txmark  --  remember a location in the text of a window
 *------------------------------------------------------------------------
 */
struct	text	*txmark(txptr, index, vline)
struct	text	*txptr;
int		index;
int		vline;
{
	register struct	text	*newptr;
	register char		*to, *from;
	register int		len;
	SYSCALL			*getmem();

	while (txptr->next != NULL && index >= txptr->len) {
		index -= txptr->len;
		txptr = txptr->next;
	}
	if (0 < index && index < txptr->len) {
		if ((newptr=
		    (struct text *)getmem((long) sizeof(struct text)))
		    == NULL)
			return(SYSERR);
		newptr->len	= txptr->len - index;
		newptr->dv	= 0;
		newptr->prev	= txptr;
		newptr->next	= txptr->next;
		to	= newptr->text;
		from	= &txptr->text[index];
		len	= newptr->len;
		while (--len >= 0)
			if ((*to++ = *from++) == NEWLINE)
				newptr->dv += vline;
		txptr->len	-= newptr->len;
		txptr->dv	-= newptr->dv;
		newptr->prev->next = newptr;
		newptr->next->prev = newptr;
		txptr = newptr;
	}
	return(txptr);
}
