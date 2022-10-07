/* fprintf.c - fprintf */

#include <kernel.h>

/*------------------------------------------------------------------------
 *  fprintf  --  print a formatted message on specified device (file)
 *------------------------------------------------------------------------
 */
fprintf(dev, fmt, args)
	int  dev;
	char *fmt;
{
	int     putc();

	_doprnt(fmt, &args, putc, dev);
	return(OK);
}
