/* x_kill.c - x_kill */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_kill  --  (builtin command kill) terminate a process
 *------------------------------------------------------------------------
 */
BUILTIN	x_kill(nargs, args)
int	nargs;
char	*args[];
{
	int	pid;

	if (nargs != 2) {
		fprintf(STDERR, "Usage: kill process-id\n");
		return(SYSERR);
	}
	if ( (pid=atoi(args[1])) == getpid() )
		fprintf(STDERR, "Shell killed\n");
	if (kill(pid) == SYSERR) {
		fprintf(STDERR, "Cannot kill %d\n", pid);
		return(SYSERR);
	}
	return(OK);
}
