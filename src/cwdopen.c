/* cwdopen.c - cwdopen */

#include <conf.h>
#include <kernel.h>
#include <name.h>
#include <cwd.h>

/*------------------------------------------------------------------------
 *  cwdopen -- attach the current prefix to name and reopen it
 *------------------------------------------------------------------------
 */
cwdopen(devptr, name, mode)
struct	devsw	*devptr;
char		*name;
char		*mode;
{
	char	*prefix;
	char	fullname[NAMLEN], newname[NAMLEN];
	int	dev;

	prefix = cwd[devptr->dvminor][getpid()];
	if (strlen(prefix) + strlen(name) >= NAMLEN)
		return(SYSERR);
	strcpy(fullname, prefix);
	strcat(fullname, name);
	if ( (dev=nammap(fullname, newname)) == SYSERR
	    || dev == devptr->dvnum)
		return(SYSERR);
	return( reopen(open(dev, newname, mode)) );
}
