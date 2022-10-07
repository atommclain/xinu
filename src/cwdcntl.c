/* cwdcntl.c - cwdcntl */

#include <conf.h>
#include <kernel.h>
#include <name.h>
#include <cwd.h>

/*------------------------------------------------------------------------
 *  cwdcntl -- control the cwd device
 *------------------------------------------------------------------------
 */
cwdcntl(devptr, func, arg)
struct	devsw	*devptr;
int		func;
char		*arg;
{
	int	ps;
	char	*prefix;
	char	newname[NAMLEN];

	prefix = cwd[devptr->dvminor][getpid()];
	switch(func) {
	case GETCWD:
		disable(ps);
		if (*prefix == NULLCH)
			namrepl(NULLSTR, arg);
		else
			strcpy(arg, prefix);
		break;

	case SETCWD:
		if (*arg == NULLCH || strlen(arg) >= NAMLEN)
			return(SYSERR);
		disable(ps);
		if (namrepl(NULLSTR, newname) == devptr->dvnum &&
		    strcmp(arg, newname) == 0)
			*prefix = NULLCH;
		else if (namrepl(arg, newname) != devptr->dvnum)
			strcpy(prefix, arg);
		else if (strlen(prefix) + strlen(newname) < NAMLEN)
			strcat(prefix, newname);
		else
			return(SYSERR);
		break;

	default:
		return(SYSERR);
	}
	restore(ps);
	return(OK);
}
