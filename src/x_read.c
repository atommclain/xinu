/* x_read.c - x_read */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <name.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  x_read  --  (command read) read from device and print result
 *------------------------------------------------------------------------
 */
COMMAND	x_read(nargs, args)
int	nargs;
char	*args[];
{
	int	dev, count;
	char	newname[NAMLEN];
	char	buf[DBUFSIZ];
	int	i, j;

	if (nargs != 3) {
		fprintf(STDERR, "Usage: %s device count\n", args[0]);
		return(SYSERR);
	}
	if ((dev=nammap(args[1],newname))==SYSERR || newname[0]!=NULLCH) {
		fprintf(STDERR, "Cannot find %s\n", args[1]);
		return(SYSERR);
	}
	count = atoi(args[2]);
	if (read(dev, buf, count) == SYSERR) {
		fprintf(STDERR, "read(%d, buf, %d): SYSERR\n",dev,count);
		return(SYSERR);
	}
	for (i = 0; i < sizeof(buf); i += 16) {
		printf("%4d: ", i);
		for (j = 0; j < 16; j += 2)
			printf("%02x%02x ",
				ctoi(buf[i+j]), ctoi(buf[i+j+1]));
		printf("  ");
		for (j = 0; j < 16; ++j)
			printf("%c", iscntrl(buf[i+j])? '.' : buf[i+j]);
		putc(STDOUT, '\n');
	}
	return(OK);
}
