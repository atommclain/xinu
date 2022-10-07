/* shutdown.c - shutdown */

#include <conf.h>
#include <kernel.h>
#include <quickdraw.h>
#include <toolutil.h>
#include <pb.h>
#include <disk.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  shutdown  --  shut down XINU and execute the quit routine
 *------------------------------------------------------------------------
 */
SYSCALL	shutdown(quit)
int	(*quit)();
{
	int	ps;
	int	i;

	disable(ps);
	InitCursor();			/* Make sure cursor is visible	*/
	SetCursor(*GetCursor(watchCursor));

#ifdef	Nlf
	for (i = 0; i < Nlf; ++i) {
		devtab[Lf.lftab[i].lf_dnum].dvrefct = 1;
		close(Lf.lftab[i].lf_dnum);
	}
	control(LFILSYS, DSKSYNC);
#endif
#ifdef	Ndsk
	for (i = 0; i < Ndsk; ++i)
		control(dstab[i].dnum, DSKSYNC);
#endif

	/* Shut down */
	(*quit)(0);
	panic("shutdown: quit function returned");
	SetCursor(&arrow);
	restore(ps);
	return(SYSERR);
}
