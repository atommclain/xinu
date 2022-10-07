/* conf.h (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
struct	devsw	{			/* device table entry */
	int	dvnum;
	char	*dvname;
	int	dvrefct;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	*dvcsr;
	int	*dvivec;
	int	*dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};

extern	struct	devsw devtab[];		/* one entry per device */


/* Device name definitions */

#define	CONSOLE     0			/* type con      */
#define	STDIN       1			/* type sio      */
#define	STDOUT      2			/* type sio      */
#define	STDERR      3			/* type sio      */
#define	KEYBOARD    4			/* type key      */
#define	DISK1       5			/* type dsk      */
#define	DISK2       6			/* type dsk      */
#define	LFILSYS     7			/* type lfm      */
#define	LFILE1      8			/* type lf       */
#define	LFILE2      9			/* type lf       */
#define	LFILE3      10			/* type lf       */
#define	LFILE4      11			/* type lf       */
#define	LFILE5      12			/* type lf       */
#define	WINDOW      13			/* type winm     */
#define	WINDOW1     14			/* type win      */
#define	WINDOW2     15			/* type win      */
#define	WINDOW3     16			/* type win      */
#define	WINDOW4     17			/* type win      */
#define	WINDOW5     18			/* type win      */
#define	WINDOW6     19			/* type win      */
#define	NAMESPACE   20			/* type nam      */

/* Control block sizes */

#define	Nkey	1
#define	Ncon	1
#define	Nwinm	1
#define	Nwin	6
#define	Ndsk	2
#define	Nlfm	1
#define	Nlf	5
#define	Nsio	3
#define	Nnam	1

#define	NDEVS	21

/* Declarations of I/O routines referenced */

extern	int	coninit();
extern	int	ioopen();
extern	int	ioerr();
extern	int	conread();
extern	int	conwrite();
extern	int	concntl();
extern	int	congetc();
extern	int	conputc();
extern	int	ionull();
extern	int	sioopen();
extern	int	sioclose();
extern	int	sioread();
extern	int	siowrite();
extern	int	sioseek();
extern	int	siocntl();
extern	int	siogetc();
extern	int	sioputc();
extern	int	keyiin();
extern	int	dsinit();
extern	int	dsread();
extern	int	dswrite();
extern	int	dscntl();
extern	int	dsinter();
extern	int	lfopen();
extern	int	lfcntl();
extern	int	lfinit();
extern	int	lfclose();
extern	int	lfread();
extern	int	lfwrite();
extern	int	lfseek();
extern	int	lfgetc();
extern	int	lfputc();
extern	int	lfinter();
extern	int	winopen();
extern	int	wininit();
extern	int	winclose();
extern	int	winread();
extern	int	winwrite();
extern	int	wincntl();
extern	int	wingetc();
extern	int	winputc();
extern	int	naminit();
extern	int	namopen();


/* Configuration and Size Constants */

#define	NPROC	    30			/* number of user processes	*/
#define	NSEM	    50			/* number of semaphores		*/
#define	MEMMARK				/* define if memory marking used*/
#define	STKCHK				/* resched checks stack overflow*/
#define MACHEAP	    (70*1024L)		/* space reserved for Mac OS	*/
#define	PATHSEP     '/'			/* file pathname separator	*/
#define	VERSION	    "6.9b (1/1/89)"	/* label printed at startup	*/
