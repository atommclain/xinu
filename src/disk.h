/* disk.h */

#ifndef	Ndsk
#define	Ndsk	1
#endif

typedef	unsigned	DBADDR;		/* disk data block addresses	*/
#define	DBNULL		(DBADDR)0177777	/* null disk block address	*/

struct	dsblk	{			/* disk driver control block	*/
	int	ddrive;			/* Macintosh drive # (1 or 2)	*/
	struct	dreq	*dreqlst;	/* list of pending requests	*/
	Bool	dbusy;			/* TRUE iff in dskstrt		*/
	int	dnum;			/* device number of this disk	*/
	ParamBlkRec	dpb;		/* holds command that interface	*/
					/*  sends to ROM routines	*/
};
extern	struct	dsblk	dstab[];

struct	dreq	{			/* node in list of requests	*/
	DBADDR	drdba;			/* disk block address to use	*/
	int	drpid;			/* process id making request	*/
	char	*drbuff;		/* buffer address for read/write*/
	char	drop;			/* operation: READ/WRITE/SEEK	*/
	int	drstat;			/* returned status OK/SYSERR	*/
	struct	dreq	*drnext;	/* ptr to next node on req. list*/
};

#define	DRNULL	(struct dreq *) 0	/* null pointer in request list	*/
#define	DONQ	2			/* status if request enqueued	*/
#define	DREFNUM	(-5)			/* Macintosh floppy ref. number	*/
#define	DBUFSIZ	512			/* size of disk data block	*/
#define	DREQSIZ	sizeof(struct dreq)	/* size of disk request node	*/
#ifndef	NDBUFF
#define	NDBUFF	10			/* number of disk data buffers	*/
#endif
#define	NDREQ	max(10, NDBUFF)		/* number of disk request buf.	*/
#define	DREAD	0			/* read command in dreq.drop	*/
#define	DWRITE	1			/* write   "			*/
#define	DEJECT	2			/* eject   "			*/
#define	DSYNC	3			/* sync    "			*/

extern	int	dskrbp;			/* disk request node buffer pool*/
extern	int	dskdbp;			/* disk data block buffer pool	*/

/* disk control function codes */

#define	DSKSYNC		0		/* synchronize (flush all I/O)	*/
#define	DSKEJECT	1		/* eject disk from drive	*/
