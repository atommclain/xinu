/* x_echo.c - x_echo */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_echo  --  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */
COMMAND	x_echo(nargs, args)
int	nargs;
char	*args[];
{
	char	str[80];
	int	i;

	if (nargs == 1)
		return(OK);
	for (strcpy(str,args[1]), i = 2; i < nargs; i++) {
		if (strlen(str)+1+strlen(args[i]) >= sizeof(str)) {
			write(STDOUT, str, strlen(str));
			str[0] = NULLCH;
		}
		strcat(str, " ");
		strcat(str, args[i]);
	}
	write(STDOUT, str, strlen(str));
	putc(STDOUT, '\n');
	return(OK);
}
