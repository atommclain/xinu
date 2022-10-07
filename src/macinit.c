/* macinit.c - macinit, reserror, growzone, macbomb */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <syserr.h>
#include <inits.h>
#include <resource.h>
#include <quickdraw.h>
#include <event.h>
#include <memory.h>
#include <osutil.h>
#include <retrace.h>
#include <pb.h>
#include <mem.h>
#include <tty.h>
#include <xwindow.h>

pascal	void	RDrvrInstall() = 0xa04f;
extern	int	*maxaddr;

/*------------------------------------------------------------------------
 *  macinit  --  initialize the Macintosh system
 *------------------------------------------------------------------------
 */
macinit()
{
	int		ps;
	VBLTask		*vptr;
	struct	DrvQEl	*dptr;		/* Drive queue element pointer	*/
	int		i;
	ParamBlkRec	pb;		/* Parameter block for vol. info*/
	Zone		*zptr;		/* Pointer to Mac heap zone	*/
	int		reserror(), macbomb();
	long		growzone();

	disable(ps);			/* Save processor status	*/
	devtab[CONSOLE].dvioblk = NULL;	/* kprintf can't use CONSOLE yet*/
	ResumeProc = (int (*)()) NULL;	/* Cannot resume without CONSOLE*/
	ResErrProc = reserror;		/* Set Resource Mgr error proc	*/
	HeapEnd = &end;			/* HeapEnd value the Mac expects*/

	/* Remove VBL tasks that will be clobbered by Macintosh heap	*/
	/* initialization, including clock VBL task			*/

	VBLQueue.qFlags = 0;		/* Clear inVBL bit		*/
	for (vptr = (VBLTask *) VBLQueue.qHead; vptr != NULL;
	    vptr = (VBLTask *) vptr->qLink)
		if (isodd(vptr))
			SysError(101);	/* VBL queue corrupted		*/
		else if (ApplZone < vptr && vptr < maxaddr ||
		    ApplZone < vptr->vblAddr && vptr->vblAddr < maxaddr)
			VRemove(vptr);

	/* Abort pending I/O requests */

	for (dptr = (struct DrvQEl *) DrvQHdr.qHead; dptr != NULL;
	    dptr = (struct DrvQEl *) dptr->qLink)
		if (isodd(dptr))
			SysError(102);	/* Drive queue corrupted	*/
		else
			KillIO(dptr->dQRefNum);
	enable();			/* We are now in the old	*/
					/*  Macintosh one-process world	*/

	/* Flush all volumes */

	pb.ioNamePtr = NULL;
	for (i=1; pb.u.vp.ioVolIndex=i,PBGetVInfo(&pb,MFALSE)==noErr; i++)
		PBFlushVol(&pb, MFALSE);

	/* Allocate space for a new heap on first boot */

	if ((zptr=TheZone) == ApplZone) {	/* First XINU boot	*/
		vecinit();		/* Init exception&intr. vectors	*/
		if ( (zptr=(Zone *)NewPtr((long) MACHEAP)) == NULL)
			SysError(104);	/* Not enough memory		*/
		SetApplLimit(HeapEnd);	/* Protect XINU heap from the	*/
	}				/*  Macintosh memory manager	*/

	/* Initialize resources and heap zone (making new zone current) */

	RsrcZoneInit();		/* WARNING: This frees the code segment.*/
				/*	    Must not allocate memory	*/
				/*	    from the Application Zone	*/
				/*	    from now on.		*/
	CurApRefNum = 0;		/* RsrcZoneInit closes res files*/
	InitZone(growzone, 128, ((char *)zptr)+GetPtrSize(zptr), zptr);
	MenuList = NULL;		/* InitZone invalidates handles	*/
	GrayRgn = NULL;
	RDrvrInstall();			/* Init unit table? (kill DAs)	*/
	InitAllPacks();			/* Initialize Package Manager	*/
	CurApRefNum = OpenResFile(CurApName);

	/* Standard Macintosh initializations, console & menu creation */

	InitGraf(&thePort);		/* Initialize QuickDraw		*/
	InitFonts();			/* Initialize Font Manager	*/
	FlushEvents(everyEvent, 0);	/* Flush all pending events	*/
	InitWindows();			/* Initialize Window Manager	*/
	initcon();			/* Initialize CONSOLE window	*/
	InitMenus();			/* Initialize Menu Manager	*/
	TEInit();			/* Initialize TextEdit		*/
	InitDialogs(macbomb);		/* Initialize Dialog Manager	*/
	initmenu();			/* Create XINU menus		*/

	HNoPurge(GetIcon(SICON));	/* Keep scroll icon in memory	*/
	HNoPurge(GetCursor(watchCursor));/* Keep watch cursor in memory	*/
	DILoad();			/* Load Disk Initialization Pkg	*/
	restore(ps);
}


/*------------------------------------------------------------------------
 *  reserror  --  report a Resource Manager error
 *------------------------------------------------------------------------
 */
LOCAL	reserror()
{
	kprintf("\n\nResource Manager error %d\n", ResErr);
	panic("Resource Manager error");
}


/*------------------------------------------------------------------------
 *  growzone  --  Mac memory manager calls this when it can't get memory
 *------------------------------------------------------------------------
 */
LOCAL	long	growzone(size)
	long	size;
{
	kprintf("\n\nCannot allocate %D bytes in Mac heap, %D free\n",
		size, TheZone->zcbFree);
	panic("out of memory");
	return(0);
}


/*------------------------------------------------------------------------
 *  macbomb  --  Mac system error handler calls this for resume button
 *------------------------------------------------------------------------
 */
LOCAL	macbomb()
{
	panic("Macintosh system error");
}
