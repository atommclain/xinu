/* lfalloc.c - lfalloc */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfalloc  --  allocate pseudo device for a local file; return id
 *------------------------------------------------------------------------
 */
lfalloc()
{
	int	i;
	int	ps;

	disable(ps);
	for (i=0 ; i<Nlf ; i++)
		if (Lf.lftab[i].lf_state == LFREE) {
			Lf.lftab[i].lf_state = LUSED;
			restore(ps);
			return(i);
		}
	restore(ps);
	return(SYSERR);
}
