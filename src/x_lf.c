/* x_lf.c - x_lf */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  x_lf  --  (command lf) format and print local file status
 *------------------------------------------------------------------------
 */
COMMAND	x_lf(nargs, args)
int	nargs;
char	*args[];
{
	struct	lfblk	*lfptr;
	char	str[80];
	int	i;

	sprintf(str, "Local files: first dev=%d, file system mutex=%d\n",
			Lf.device, Lf.lmutex);
	write(STDOUT, str, strlen(str));
	for (i=0 ; i<Nlf; i++) {
		if ((lfptr = &Lf.lftab[i])->lf_state == LFREE)
			continue;
		sprintf(str,
			"%3d. refnum=%5d, pos=%6D, mode=%03o, sem=%d\n",
				lfptr->lf_dnum, lfptr->lf_refnum,
				lfptr->lf_pos,  lfptr->lf_mode,
				lfptr->lf_mutex);
		write(STDOUT, str, strlen(str));
	}
	return(OK);
}
