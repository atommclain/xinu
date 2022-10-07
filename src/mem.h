/* mem.h - freestk, roundew, truncew */

/*----------------------------------------------------------------------
 * roundew, truncew - round or truncate address to next even long word
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(int *)( (7 + (unsigned long)(x)) & (~7) )
#define	truncew(x)	(int *)( ((unsigned long)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len)	freemem((long)(p)			\
				- (long)(roundew(len))		\
				+ (long)sizeof(int),		\
				roundew(len) )

struct	mblock	{
	struct	mblock	*mnext;
	unsigned long	mlen;
	};
extern	struct	mblock	memlist;	/* head of free memory list	*/
extern	int	*maxaddr;		/* max memory address		*/
extern	int	_Corg, _Cend;		/* bounds of code segment	*/
extern	int	_Dorg, _Dend;		/* bounds of data segment	*/
extern	int	_Uorg, _Uend;		/* bounds of bss segment	*/

/* First location usable as XINU heap					*/

#define end	( *((char *)ApplZone->bkLim + 12) )
