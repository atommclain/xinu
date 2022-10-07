/* x_sync.c - x_sync */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  x_sync  --  (command sync) wait for pending disk I/O to complete
 *------------------------------------------------------------------------
 */
COMMAND	x_sync(nargs, args)
int	nargs;
char	*args[];
{
	int	i;

#ifdef	LFILSYS
	if (control(LFILSYS, DSKSYNC) == SYSERR)
		fprintf(STDERR, "Cannot sync LFILSYS\n");
#endif
#ifdef	Ndsk
	for (i = 0; i < Ndsk; ++i)
		if (control(dstab[i].dnum, DSKSYNC) == SYSERR)
			fprintf(STDERR, "Cannot sync %s\n",
				devtab[dstab[i].dnum].dvname);
#endif
	return(OK);
}
