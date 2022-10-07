/* lfile.h */

/* Local file device control block and defined constants */

/* Constants for lf pseudo-device control block */

#define	LNAMLEN		80		/* Maximum bytes in file name	*/
#define	LFREE		-1		/* This pseudo-device is unused	*/
#define	LUSED		1		/* This pseudo-device is used	*/

/* Declaration of lf pseudo-device I/O control block */

struct	lfblk	{			/* Local file control block	*/
	int	lf_dnum;		/* File's device num in devtab	*/
	int	lf_state;		/* State of this pseudo device	*/
	int	lf_refnum;		/* ROM file reference number	*/
	int	lf_mode;		/* FLREAD, FLWRITE or both	*/
	int	lf_mutex;		/* Exclusion for this file	*/
	long	lf_pos;			/* Current byte offset in file	*/
	Bool	lf_crlf;		/* Swap CR and LF on read/write?*/
};

#ifndef	Nlf
#define	Nlf	0
#endif
struct	lfinfo	{			/* Local file system info.	*/
	int		device;		/* Descriptor for master device	*/
	ParamBlkRec	lpb;		/* Parameters for curr operation*/
	int		lmutex;		/* Mutual exclusion for local fs*/
	int		ldone;		/* File op. completion semaphore*/
	struct	lfblk	lftab[max(1,Nlf)];/* Local file control blocks	*/
};

extern	struct	lfinfo	Lf;

#define	LBUFSIZ	512			/* Size of file data buffer	*/
#define	LNBUFF	Nlf			/* Number of file data buffers	*/
extern	int	lfdbp;			/* File data buffer pool	*/

/* Macintosh ROM constants */

#define	LFTYPE		'TEXT'		/* Type of file Xinu creates	*/
#define	LFOWNER		'EDIT'		/* Application to read files	*/
#define	LFPSEP		':'		/* ROM file pathname separator	*/
