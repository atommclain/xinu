/* x_write.c - x_write */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_write  --  (command write) write to device and print result
 *------------------------------------------------------------------------
 */
COMMAND	x_write(nargs, args)
int	nargs;
char	*args[];
{
	int	dev, count;
	char	newname[NAMLEN];
	int	i;
	char	*buf, *getbuf();

	if (nargs != 3) {
		fprintf(STDERR, "Usage: %s device count\n", args[0]);
		return(SYSERR);
	}
	if ((dev=nammap(args[1],newname))==SYSERR || newname[0]!=NULLCH) {
		fprintf(STDERR, "Cannot find %s\n", args[1]);
		return(SYSERR);
	}
	count = atoi(args[2]);
	if ((buf=getbuf(dskdbp)) == SYSERR) {
		fprintf(STDERR, "write: cannot allocate disk buffer\n");
		return(SYSERR);
	}
	for (i = 0; i < DBUFSIZ; ++i)
		if ( (buf[i]=getc(STDIN)) == EOF )
			break;
		else if (buf[i] == SYSERR) {
			fprintf(STDERR, "write: cannot read data\n");
			freebuf(buf);
			return(SYSERR);
		}
	while (i < DBUFSIZ)
		buf[i++] = NULLCH;
	write(dev, buf, count);
	return(OK);
}
