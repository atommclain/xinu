/* xwindow.h */

#ifndef	WindowRecord
#include <quickdraw.h>
#include <toolutil.h>
#include <window.h>
#endif

#ifndef	Nwin
#define Nwin
#endif

#define MBARHT		 20	/* Height of Macintosh menu bar		*/
#define GICONHT		 15	/* Height of Mac window grow icon	*/
#define GICONWD		 15	/* Width of Mac window grow icon	*/
#define SICONHT		 32	/* Height of scroll icon		*/
#define SICONWD		 32	/* Width of scroll icon			*/
#define SICON		128	/* Macintosh resource ID of scroll icon	*/

#define	LMARGIN		 4	/* Left margin for text in pixels	*/
#define	RMARGIN		 4	/* Right margin for text in pixels	*/
#define	TMARGIN		 0	/* Top margin for text in pixels	*/
#define	BMARGIN		 1	/* Bottom margin for text in pixels	*/
#define	TABWIDTH	48	/* Distance between tab stops in pixels	*/
#define	TXFONT		monaco	/* Font used in all windows		*/
#define	TXSIZE		 9	/* Font size in points			*/

#define WFREE		1		/* This window is free		*/
#define WUSED		2		/* This window is in use	*/

#define WNAMLEN		255		/* Maximum bytes in window name	*/
#define TXBLKLEN	116		/* Max bytes in a chunk of text	*/
#define NCTEXT	(40*80/TXBLKLEN)	/* Blocks reserved for CONSOLE	*/

/* Structure of text in a window */

struct	text	{
	unsigned int	len;		/* No. of chars in this block	*/
	unsigned int	dv;		/* Vert. dist. traveled in text	*/
	struct	text	*prev, *next;	/* Backward and forward links	*/
	char	text[TXBLKLEN];		/* Contents of this block	*/
};

/* Declaration of window pseudo-device I/O control block */

struct	winblk	{			/* Window control block		*/
	int		wdnum;		/* Window's device # in devtab	*/
	int		wstate;		/* free or used			*/
	unsigned char	wname[WNAMLEN+1];/* Window name			*/
	Bool		wpersist;	/* Window remains after close?	*/
	int		wmode;		/* FLREAD, FLWRITE or both	*/
	WindowRecord	wrec;		/* Macintosh window description	*/
	Rect		wrect;		/* Rectangle containing text	*/
	Rect		wsbox;		/* Scroll area			*/
	int		wlineht;	/* Height of a line of text	*/
	int		wdescent;	/* Descent of text font		*/
	int		wtabwid;	/* Tab width in this window	*/
	Point		wpenpos;	/* Position of pen in window	*/
	Bool		wcarvis;	/* TRUE iff caret is visible	*/
	struct	text	txhead, txtail;	/* Contents of window		*/
	struct	tty	wtty;		/* Tty control block		*/
};

extern	struct	winblk	wintab[Nwin+1];
extern	RgnHandle	winrgn;		/* Used to maintain update rgn	*/

/* Window control function codes */

#define WCIDLE		101		/* Perform periodic action	*/
#define WCACTIVATE	102		/* Activate/deactivate window	*/
#define WCUPDATE	103		/* Redraw contents of window	*/
#define WCMOVE		104		/* Move a window on the screen	*/
#define WCSIZE		105		/* Change the size of a window	*/
#define	WCSCROLL	106		/* Scroll contents of a window	*/
#define	WCASCROLL	107		/* Auto-scroll caret into view	*/
#define	WCKILL		108		/* Delete window		*/
