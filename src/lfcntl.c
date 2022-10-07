/* lfcntl.c - lfcntl */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <disk.h>
#include <file.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfcntl  --  control the local file access system
 *------------------------------------------------------------------------
 */
lfcntl(devptr, func, addr, addr2)
struct	devsw	*devptr;
int	func;
char	*addr;
char	*addr2;
{
	char	fname[LNAMLEN], fname2[LNAMLEN];
	char	*ctop();

	switch (func) {

	case DSKSYNC:		/* wait for outstanding requests	*/
		wait(Lf.lmutex);
		signal(Lf.lmutex);
		return(OK);

	/* Universal file manipulation functions */

	case FLACCESS:
		return(OK);

	case FLREMOVE:
		if (lfnammap(addr, fname) == SYSERR)
			return(SYSERR);
		return( lfnamop(PBDelete, ctop(fname), 0, NULL) );

	case FLRENAME:
		if (lfnammap(addr, fname) == SYSERR ||
		    lfnammap(addr2, fname2) == SYSERR)
			return(SYSERR);
		return( lfnamop(PBRename, ctop(fname), 0, ctop(fname2)) );

	default:
		return(SYSERR);
	}
}
