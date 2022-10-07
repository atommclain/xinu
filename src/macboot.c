/* macboot.c - macboot */

#include <kernel.h>
#include <syserr.h>
#include <pb.h>

/*------------------------------------------------------------------------
 *  macboot  --  eject all disks and reboot the Macintosh
 *------------------------------------------------------------------------
 */
macboot()
{
	int		ps;
	ParamBlkRec	pb;
	int		vn;

	disable(ps);
	pb.ioNamePtr = NULL;
	for (pb.u.vp.ioVolIndex=1; PBGetVInfo(&pb,MFALSE)==noErr; ) {
		Eject(NULL, vn=pb.ioVRefNum);
		if (PBGetVInfo(&pb, MFALSE)!=noErr || vn==pb.ioVRefNum)
			pb.u.vp.ioVolIndex++;
	}
	Restart();			/* Restart should not return	*/
	restore(ps);
	return(SYSERR);
}
