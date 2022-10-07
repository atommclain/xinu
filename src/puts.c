/* puts.c - puts */

#include <conf.h>

/*------------------------------------------------------------------------
 *  puts  --  write a null-terminated string to STDOUT
 *------------------------------------------------------------------------
 */
puts(s)
register char *s;
{
	register c;

	while (c = *s++)
		putc(STDOUT,c);
	return(putc(STDOUT,'\n'));
}
