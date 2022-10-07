/* macvars.h */

/* Interrupt vectors */

#define	intvec		(*(void *(*)[]) 0) /* Intr. vectors start at 0	*/
#define	INT4		28		   /* Level-4 autovector number	*/
#define	DEBUG		31		   /* Intr. switch vector number*/

/* Macintosh low-memory global variables */

#define	MemTop		(*(void **) 0x108) /* End of RAM		*/
#define	BufPtr		(*(void **) 0x10c) /* End of jump table		*/
#define	HeapEnd		(*(void **) 0x114) /* End of Macintosh heap	*/
#define TheZone		(*(Zone **) 0x118) /* Current heap zone		*/
#define	EventQueue	(*(QHdr *)  0x14a) /* ROM event queue header	*/
#define VBLQueue	(*(QHdr *)  0x160) /* VBL task queue header	*/
#define	Time		(*(long *)  0x20c) /* Time in secs since 1/1/04	*/
#define SysZone		(*(Zone **) 0x2a6) /* System heap zone		*/
#define ApplZone	(*(Zone **) 0x2aa) /* Application heap zone	*/
#define	DrvQHdr		(*(QHdr *)  0x308) /* Disk drive queue header	*/
#define	DefltStack	(*(long *)  0x322) /* Default stack length	*/
#define VCBQHdr		(*(QHdr *)  0x356) /* Volume cntl block q header*/
#define CurApRefNum	(*(int *)   0x900)
#define CurrentA5	(*(void **) 0x904)
#define CurStackBase	(*(void **) 0x908)
#define CurApName	((char *)   0x910)
#define WindowList	(*(WindowRecord **)0x9d6)
#define	GrayRgn		(*(RgnHandle *)0x9ee)
#define	MenuList	(*(Handle *)0xa1c)
#define	TopMapHndl	(*(Handle *)0xa50)
#define	SysMapHndl	(*(Handle *)0xa54)
#define	SysMap		(*(int *)   0xa58)
#define	ResErr		(*(int *)   0xa60) /* Resource Mgr error code	*/
#define	CurActivate	(*(WindowRecord **)0xa64)
#define	CurDeactivate	(*(WindowRecord **)0xa68)
#define	ResumeProc	(*(int (**)()) 0xa8c)
#define	SysResName	(*(char **) 0xad8)
#define	ResErrProc	(*(int (**)()) 0xaf2)
