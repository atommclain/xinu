/* x_ps.c - x_ps */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

LOCAL	char	hd1[] =
       "pid   name   state prio  stack range  stack length sem message\n";
LOCAL	char	hd2[] =
       "--- -------- ----- ---- ------------- ------------ --- -------\n";
LOCAL	char	*pstnams[] = {"curr ","free ","ready","recv ",
			    "sleep","susp ","wait ","rtim "};

/*------------------------------------------------------------------------
 *  x_ps  --  (command ps) format and print process table information
 *------------------------------------------------------------------------
 */
COMMAND	x_ps(nargs, args)
int	nargs;
char	*args[];
{
	int	i;
	char	str[80];
	struct	pentry	*pptr;
	char	*currstk;

	snapregs(proctab[currpid].pregs);	/* capture current SP	*/
	write(STDOUT, hd1, strlen(hd1));
	write(STDOUT, hd2, strlen(hd2));
	for (i=0 ; i<NPROC ; i++) {
		if ((pptr = &proctab[i])->pstate == PRFREE)
			continue;
		sprintf(str, "%3d %8s %s ", i, pptr->pname,
			pstnams[pptr->pstate-1]);
		write(STDOUT, str, strlen(str));
		sprintf(str, "%4d %6X-%6X ", pptr->pprio,  pptr->plimit,
			pptr->pbase);
		write(STDOUT, str, strlen(str));
		currstk = (char *) pptr->pregs[SP];
		if (currstk < pptr->plimit || pptr->pbase < currstk)
			sprintf(str, " OVERFLOWED)");
		else
			sprintf(str, " %4D/%5D   ",
			    (long)pptr->pbase+sizeof(int)-(long)currstk,
			    (long)pptr->pbase+sizeof(int)
			    -(long)pptr->plimit);
		write(STDOUT, str, strlen(str));
		if (pptr->pstate == PRWAIT)
			sprintf(str, "%2d", pptr->psem);
		else
			sprintf(str, "- ");
		write(STDOUT, str, strlen(str));
		if (pptr->phasmsg)
			sprintf(str, "%8X\n", pptr->pmsg);
		else
			sprintf(str, "   -\n");
		write(STDOUT, str, strlen(str));
	}
}
