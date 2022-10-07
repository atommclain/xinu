/* shell.c - shell, redirect */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <proc.h>
#include <shell.h>
#include <cmd.h>
#include <tty.h>
#include <name.h>

/* Global error messages */
LOCAL	char	fmt[]   = "Cannot open %s\n";
LOCAL	char	fmt2[]  = "[%d]\n";

/*------------------------------------------------------------------------
 *  shell  --  Xinu shell with file redirection and background processing
 *------------------------------------------------------------------------
 */
shell(nargs, args)
int	nargs;
char	*args[];
{
	struct	shvars	*shptr, *getmem();
	int	ncargs, len;
	int	toktype;		/* Token type from lexan	*/
	char	*tokptr;		/* Pointer to token's text	*/
	char	*innam, *outnam, *errnam;/* Names for I/O redirection	*/
	Bool	backgnd;
	int	com;
	int	child;
	long	receive();

	if ((shptr=getmem((long)sizeof(struct shvars)))==(char *)SYSERR) {
		fprintf(STDERR, "shell: not enough memory\n");
		return(SYSERR);
	}
	shptr->shlastc = '\n';
	shptr->shnextc = shptr->shend = shptr->shbuf;
	shptr->shprompt[0] = shptr->shprcon[0] = NULLCH;
	if (nargs < 2)
		shptr->shcmddev = STDIN;
	else if (nargs > 2 && strcmp(args[1], "-c") == 0) {
		shptr->shnextc = args[2];
		shptr->shend = args[2] + strlen(args[2]);
		shptr->shcmddev = BADDEV;
	}
	else if ((shptr->shcmddev=fopen(args[1], "ro")) == SYSERR) {
		fprintf(STDERR, fmt, args[1]);
		freemem(shptr, (long) sizeof(struct shvars));
		return(SYSERR);
	}
	if (shptr->shcmddev == STDIN &&
	    getdev(getpid(), STDIN) == getdev(getpid(), STDOUT)) {
		strcpy(shptr->shprompt, "% ");
		strcpy(shptr->shprcon, "> ");
	}

	for (toktype = '\n'; toktype != EOF; ) {
		getutim(&Log.llast);
		if (toktype == '\n')
			shgetc(shptr, shptr->shprompt);	/* Read cmd line*/
		innam = outnam = errnam = NULL;
		backgnd = FALSE;
		ncargs = 0;
		tokptr = shptr->shstrbuf;
		len = 0;

		/* Parse input */
		while ((toktype=lexan(tokptr,shptr)) != EOF &&
			toktype!='\n' && toktype!=';' && toktype!='&') {
		    switch (toktype) {
		    case '<':		/* I/O redirection		*/
		    case '>':
		    case '^':
			for ( ; ; toktype=lexan(tokptr,shptr)) {
				switch (toktype) {
				case '<':  innam = tokptr;	continue;
				case '>':  outnam = tokptr;	continue;
				case '^':  errnam = tokptr;	continue;
				default:   break;
				}
				break;
			}
			if (toktype != '"') {
				fprintf(STDERR, "No redirect name\n");
				break;
			}
			tokptr += strlen(tokptr) + 1;
			continue;

		    case ')':		/* Unbalanced right paren	*/
			fprintf(STDERR, "Unbalanced \")\"\n");
			break;

		    case '(':		/* Command to run in subshell	*/
			if (ncargs > 0) {
				fprintf(STDERR, "Misplaced \"(\"\n");
				break;
			}
			shptr->shargs[ncargs++] = SHNAME;
			shptr->shargs[ncargs++] = "-c";
			len += sizeof(SHNAME) + 3;
			/* Fall through */

		    default:		/* String added to arg list	*/
			shptr->shargs[ncargs++] = tokptr;
			len	+= strlen(tokptr) + 1;
			tokptr	+= strlen(tokptr) + 1;
			continue;
		    }
		    ncargs = 0;		/* Error: ignore command line	*/
		    shptr->shnextc = shptr->shend;
		    if (shptr->shcmddev != STDIN)
			toktype = EOF;	/* Ignore rest of file		*/
		    break;
		}

		if (ncargs == 0)
			continue;
		backgnd = (toktype == '&');
		shptr->shargs[ncargs] = NULL;

		/* Look up command in table */

		for (com = 0 ; com < ncmds ; com++)
			if (strcmp(cmds[com].cmdnam,shptr->shargs[0])==0)
				break;
		if (com >= ncmds) {
			fprintf(STDERR, "%s: not found\n",
				shptr->shargs[0]);
			continue;
		}

		/* handle built-in commands with procedure call */

		if (cmds[com].cbuiltin) {
			if (innam!=NULL || outnam!=NULL || errnam!=NULL
			    || backgnd)
				fprintf(STDERR,"Builtin command error\n");
			else if ( (*cmds[com].cproc)(ncargs,shptr->shargs)
			    == SHEXIT)
				break;
			continue;
		}

		/* compute space needed for string args. (in bytes) */

		len += (ncargs + 2) * sizeof(char *);
		if (isodd(len))
			++len;

		/* Create process to execute conventional command */

		if ((child=create(cmds[com].cproc, SHCMDSTK+len, SHCMDPRI,
				shptr->shargs[0], 1+(len/sizeof(int)),
				ncargs)) == SYSERR) {
			fprintf(STDERR, "Cannot create \"%s\"\n",
				shptr->shargs[0]);
			continue;
		}
		addarg(child, ncargs, shptr->shargs, len);

		/* Open files and redirect I/O if specified */

		if (redirect(child, STDOUT, outnam, "w") == SYSERR ||
		    redirect(child, STDERR, errnam, "w") == SYSERR ||
		    redirect(child, STDIN, innam, "ro") == SYSERR)
			continue;

		if (backgnd) {
			printf(fmt2, child);
			resume(child);
		} else {
			setnok(getpid(), child);
			control(shptr->shcmddev, TCINT);
			recvclr();
			resume(child);
			if (receive() == INTRMSG) {
				setnok(BADPID, child);
				printf(fmt2, child);
			}
		}
	}
	close(shptr->shcmddev);
	freemem(shptr, (long) sizeof(struct shvars));
	return(OK);
}


/*------------------------------------------------------------------------
 *  redirect  --  connect the given standard I/O device to the named one
 *------------------------------------------------------------------------
 */
LOCAL	redirect(pid, siodev, name, mode)
int	pid, siodev;
char	*name, *mode;
{
	int	dev;

	if (name == NULL)
		return(OK);
	if ((dev=fopen(name, mode)) != SYSERR) {
		if (setdev(pid, siodev, dev) != SYSERR)
			return(OK);
		close(dev);
	}
	fprintf(STDERR, fmt, name);
	kill(pid);
	return(SYSERR);
}
