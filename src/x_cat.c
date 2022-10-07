/* x_cat.c - x_cat */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_cat  --  (command cat) concatenate files and write on stdout
 *------------------------------------------------------------------------
 */
COMMAND	x_cat(nargs, args)
int	nargs;
char	*args[];
{
	int	device;
	char	buf[512];
	int	ret;
	int	len;
	int	i;

	ret = OK;
	if (nargs == 1) {
		while ( (len=read(STDIN, buf, sizeof(buf))) > 0)
			write(STDOUT, buf, len);
	}
	for (i=1 ; i<nargs ; i++) {
		if ( (device = fopen(args[i], "ro")) == SYSERR) {
			fprintf(STDERR, "Cannot open %s\n", args[i]);
			ret = SYSERR;
			break;
		}
		while ( (len=read(device, buf, sizeof(buf))) > 0)
			write (STDOUT, buf, len);
		close(device);
	}
	return(ret);
}
