/* tty.h - iscntrl, isprint */

/* size constants */

#ifndef	IBUFLEN
#define	IBUFLEN		128		/* num.	chars in input queue	*/
#endif

/* mode constants */

#define	IMRAW		'R'		/* raw mode => nothing done	*/
#define	IMCOOKED	'C'		/* cooked mode => line editing	*/
#define	IMCBREAK	'K'		/* honor echo, etc, no line edit*/
#define	OMRAW		'R'		/* raw mode => normal processing*/

struct	tty	{			/* tty line control block	*/
	int	ihead;			/* head of input queue		*/
	int	itail;			/* tail of input queue		*/
	char	ibuff[IBUFLEN];		/* input buffer for this line	*/
	int	isem;			/* input semaphore		*/
	char	imode;			/* IMRAW, IMCBREAK, IMCOOKED	*/
	Bool	iecho;			/* is input echoed?		*/
	Bool	ieback;			/* do erasing backspace on echo?*/
	Bool	evis;			/* echo control chars as ^X ?	*/
	Bool	ecrlf;			/* echo CR-LF for newline?	*/
	Bool	icrlf;			/* map '\r' to '\n' on input?	*/
	Bool	ierase;			/* honor erase character?	*/
	char	ierasec;		/* erase character (backspace)	*/
	Bool	ikill;			/* honor line kill character?	*/
	char	ikillc;			/* line kill character		*/
	Bool	iintr;			/* is interrupt char honored?	*/
	char	iintrc;			/* interrupt character		*/
	int	iintpid;		/* interrupt process id		*/
	Bool	ieof;			/* honor end-of-file char?	*/
	char	ieofc;			/* end-of-file character	*/
	int	icursor;		/* current cursor position	*/
	Bool	ocrlf;			/* echo CR/LF for LF ?		*/
	char	ifullc;			/* char to send when input full	*/
};

#define	BACKSP	'\b'			/* backspace one character pos.	*/
#define	BELL	'\007'			/* usually an audible tone	*/
#define	BLANK	' '			/* used to print a "space"	*/
#define	EOFC	'\004'			/* end-of-file character (^D)	*/
#define	KILLCH	'\025'			/* line kill character (^U)	*/
#define	NEWLINE	'\n'			/* line feed			*/
#define	RETURN	'\r'			/* carriage return		*/
#define	INTRCH	'\002'			/* control-B is interrupt	*/
#define	UPARROW	'^'			/* usually for visuals like ^X	*/

#define	iscntrl(ch)	((unsigned char)(ch) < BLANK || ch == 0177)
#define	isprint(ch)	((unsigned char)(ch) >= BLANK && ch != 0177)

/* tty control function codes */

#define	TCSETBRK	1		/* turn on BREAK in transmitter	*/
#define	TCRSTBRK	2		/* turn off BREAK "       "	*/
#define	TCNEXTC		3		/* look ahead 1 character	*/
#define	TCMODER		4		/* set input mode to raw	*/
#define	TCMODEC		5		/* set input mode to cooked	*/
#define	TCMODEK		6		/* set input mode to cbreak	*/
#define	TCICHARS	8		/* return number of input chars	*/
#define	TCECHO		9		/* turn on echo			*/
#define	TCNOECHO	10		/* turn off echo		*/
#define	TCINT		11		/* set input interrupt pid	*/
#define	TCINTCH		12		/* set input interrupt char	*/
#define	TCNOINT		13		/* turn off input interrupt	*/
#define	TFULLC		BELL		/* char to echo when buffer full*/
