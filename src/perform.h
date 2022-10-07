/* perform.h */

/* Definitions for run-time performance measure	*/
/* Times are measured in 60ths of a second	*/

#define	PQUANTUM	(5*60)			/* Granularity of stats	*/
#define	LOADAV1		(1L*60*60)		/* Time for 1st load av.*/
#define	LOADAV2		(5L*60*60)		/* Time for 2nd load av.*/
#define	LOADAV3		(15L*60*60)		/* Time for 3rd load av.*/
#define	NPERIOD		181 /*LOADAV3/PQUANTUM+1/* # intervals measured	*/

extern	int		busytim[NPERIOD];	/* Total busy time	*/
extern	int		cperiod;		/* Current stat interval*/
extern	unsigned long	busystrt, pstart;

extern	unsigned long	runtime;
