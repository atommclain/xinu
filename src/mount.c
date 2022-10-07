/* mount.c - mount */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  mount  --  give meaning to a prefix in the abstract name space
 *------------------------------------------------------------------------
 */
SYSCALL	mount(prefix, dev, replace)
char	*prefix;
char	*replace;
int	dev;
{
	struct	nament	*nptr, *nptr2;
	struct	nament	*nlast;
	int	i, j;
	int	ps;

	if (prefix == NULL)
		prefix == NULLSTR;
	if (replace == NULL)
		replace == NULLSTR;
	if (strlen(prefix) >= NAMPLEN || strlen(replace) >= NAMRLEN)
		return(SYSERR);
	disable(ps);
	nlast = &Nam.nametab[Nam.nnames];
	for (nptr=Nam.nametab ; nptr<nlast ; nptr++) {
		if (strcmp(prefix,nptr->npre) == 0) {
			strcpy(nptr->nrepl, replace);
			nptr->ndev = dev;
			restore(ps);
			return(OK);
		}
		if (strncmp(prefix,nptr->npre,strlen(nptr->npre)) == 0)
			break;
	}
	if (Nam.nnames >= NNAMES) {
		restore(ps);
		return(SYSERR);
	}
	Nam.nnames++;
	for (nptr2=nlast-1 ; nptr2 >= nptr ; )
		*nlast-- = *nptr2--;
	strcpy(nptr->npre, prefix);
	strcpy(nptr->nrepl, replace);
	nptr->ndev = dev;
	restore(ps);
	return(OK);
}
