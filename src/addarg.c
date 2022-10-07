/* addarg.c - addarg */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  addarg  --  copy arguments to area reserved on process stack
 *------------------------------------------------------------------------
 */
addarg(pid, nargs, args, len)
int	pid;				/* process to receive arguments	*/
int	nargs;				/* number of arguments to copy	*/
char	*args[];			/* the arguments		*/
int	len;				/* size of arg. area (in bytes)	*/
{
	struct	pentry	*pptr;
	char	**fromarg;
	char	**toarg;
	char	*to;

	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate != PRSUSP)
		return(SYSERR);
	toarg = (char **) ( ((unsigned long)pptr->pbase) - (unsigned)len);
	to = (char *) (toarg + (nargs + 2));
	*toarg = (char *) (toarg + 1);
	toarg++;
	for (fromarg = args ; nargs > 0 ; nargs--) {
		*toarg++ = to;
		strcpy(to, *fromarg++);
		to += strlen(to) + 1;
	}
	*toarg = NULL;
	return(OK);
}
