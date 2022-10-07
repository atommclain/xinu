/* x_help.c - x_help */

#include <conf.h>
#include <kernel.h>
#include <cmd.h>

#define	COLUMNS	4			/* number of columns to print	*/

/*------------------------------------------------------------------------
 *  x_help  --  (command help) print possible command names for user
 *------------------------------------------------------------------------
 */
COMMAND	x_help(nargs, args)
int	nargs;
char	*args[];
{
	int	inc;			/* command names per column	*/
	int	i;			/* move through printed rows	*/
	int	j;			/* move across printed columns	*/


	if ( (inc=(ncmds+COLUMNS-1)/COLUMNS) <= 0)
		inc = 1;
	printf("Commands are:\n");
	for(i=0 ; i<inc && i<ncmds ; i++) {
		printf("  ");
		for (j=i ; j<ncmds ; j+=inc)
			printf("%-16s", cmds[j].cmdnam);
		printf("\n");
	}
	return(OK);
}
