/* x_unmou.c -  x_unmou */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_unmou  --  (command unmount) remove prefix from the namespace table
 *------------------------------------------------------------------------
 */
COMMAND	x_unmou(nargs, args)
int	nargs;
char	*args[];
{
	int	i;

	if (nargs < 2) {
		fprintf(STDERR, "Usage: unmount prefix\n");
		return(SYSERR);
	}
	for (i = 1; i < nargs; i++)
		if (unmount(args[i]) == SYSERR) {
			fprintf(STDERR,"Cannot unmount \"%s\"\n",args[i]);
			return(SYSERR);
		}
	return(OK);
}
