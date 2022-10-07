/* printf.c - printf */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  printf  --  write formatted output to STDOUT
 *------------------------------------------------------------------------
 */
printf(fmt, args)
	char	*fmt;
{
	int	putc();

	_doprnt(fmt, &args, putc, STDOUT);
	return(OK);
}
