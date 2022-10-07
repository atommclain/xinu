/* dskbcpy.c - dskbcpy */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>

/*------------------------------------------------------------------------
 *  dskbcpy  --  copy data into a new disk buffer and return its address
 *------------------------------------------------------------------------
 */
char *dskbcpy(oldbuf)
char	*oldbuf;
{
	char	*newbuf;
	SYSCALL	*getbuf();

	newbuf = (char *) getbuf(dskdbp);
	blkcopy(newbuf, oldbuf, DBUFSIZ);
	return(newbuf);
}
