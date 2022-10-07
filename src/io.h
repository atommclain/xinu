/* io.h -  fgetc, fputc, getchar, isbaddev, putchar */

#define	INTVECI	inint		/* input interrupt dispatch routine	*/
#define	INTVECO	outint		/* output interrupt dispatch routine	*/
extern	int	INTVECI();
extern	int	INTVECO();

struct	intmap	{		/* device-to-interrupt routine mapping	*/
	int	(*iin)();	/* address of input interrupt routine	*/
	long	icode;		/* argument passed to input routine	*/
	int	(*iout)();	/* address of output interrupt routine	*/
	long	ocode;		/* argument passed to output routine	*/
};

#ifdef	NDEVS
extern	struct	intmap intmap[NDEVS];
#define	isbaddev(f)	( (f)<0 || (f)>=NDEVS )
#else
#define	isbaddev(f)	TRUE
#endif
#define	BADDEV		-1

/* In-line I/O procedures */

#define	getchar()	getc(STDIN)
#define	putchar(ch)	putc(STDOUT,(ch))
#define	fgetc(unit)	getc((unit))
#define	fputc(unit,ch)	putc((unit),(ch))
