/* x_cp.c - x_cp */

#include <conf.h>
#include <kernel.h>
#include <name.h>

LOCAL	char	errfmt[] = "Cannot open %s\n";

/*------------------------------------------------------------------------
 *  x_cp  --  (copy command) copy one file to another
 *------------------------------------------------------------------------
 */
COMMAND	x_cp(nargs, args)
int	nargs;
char	*args[];
{
	char	buf[512];
	int	from, to;
	int	len;

	if (nargs != 3) {
		fprintf(STDERR, "Usage: cp file1 file2\n");
		return(SYSERR);
	}
	if ( (from=fopen(args[1], "ro")) == SYSERR) {
		fprintf(STDERR, errfmt, args[1]);
		return(SYSERR);
	}
	if ( (to=fopen(args[2], "w")) == SYSERR) {
		close(from);
		fprintf(STDERR, errfmt, args[2]);
		return(SYSERR);
	}
	while ( (len = read(from, buf, sizeof(buf))) > 0 )
		write(to, buf, len);
	close(from);
	close(to);
	return(OK);
}
