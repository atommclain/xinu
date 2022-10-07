/* xexit.c - xexit */

#include <kernel.h>
#include <syserr.h>
#include <segment.h>
#include <retrace.h>
#include <pb.h>

extern	VBLTask	clkTask;

/*------------------------------------------------------------------------
 *  xexit  --  restore interrupt vectors and exit to the Finder
 *------------------------------------------------------------------------
 */
xexit()
{
	int		ps;
	int		i;
	ParamBlkRec	pb;

	disable(ps);
	VRemove(&clkTask);	/* Remove clock interrupt routine	*/
	vecinit();		/* Restore interrupt vectors		*/
	enable();		/* We are now a normal Mac application	*/
	pb.ioNamePtr = NULL;
	for (i=1; pb.u.vp.ioVolIndex=i,PBGetVInfo(&pb,MFALSE)==noErr; i++)
		PBFlushVol(&pb, MFALSE);
	ExitToShell();		/* Exit to Finder (should not return)	*/
	restore(ps);
	return(SYSERR);
}
