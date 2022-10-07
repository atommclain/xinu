/* x_pwd.c - x_pwd */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_pwd  --  (command pwd) print the default prefix
 *------------------------------------------------------------------------
 */
COMMAND	x_pwd(nargs, args)
int	nargs;
char	*args[];
{
	char	prefix[NAMRLEN];

	if (namrepl(NULLSTR, prefix) != NAMESPACE) {
		fprintf(STDERR, "Cannot get default prefix\n");
		return(SYSERR);
	}
	write(STDOUT, prefix, strlen(prefix));
	putc(STDOUT, '\n');
	return(OK);
}
