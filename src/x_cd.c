/* x_cd.c - x_cd */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  x_cd  --  (builtin command cd) change the default prefix
 *------------------------------------------------------------------------
 */
BUILTIN	x_cd(nargs, args)
int	nargs;
char	*args[];
{
	int	ps;
	char	arg[NAMRLEN], prefix[NAMLEN], prefix2[NAMLEN];
	int	len;
	char	*strcpy();

	if (nargs != 2 || *args[1] == NULLCH) {
		fprintf(STDERR, "Usage: cd new-prefix\n");
		return(SYSERR);
	}
	if ( (len=strlen(args[1])) >= NAMRLEN ||
	    args[1][len-1] != PATHSEP && len >= NAMRLEN-1) {
		fprintf(STDERR, "Prefix too long\n");
		return(SYSERR);
	}
	if (strcpy(arg,args[1])[len-1] != PATHSEP) {
		arg[len++] = PATHSEP;
		arg[len]   = NULLCH;
	}
	disable(ps);
	if ((namrepl(arg, prefix) == SYSERR ||
	     (unmount(NULLSTR), namrepl(arg, prefix2) != SYSERR)?
	     mount(NULLSTR, NAMESPACE, arg) :
	     mount(NULLSTR, NAMESPACE, prefix)) == SYSERR) {
		fprintf(STDERR, "Cannot cd to \"%s\"\n", args[1]);
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	return(OK);
}
