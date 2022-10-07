/* gets.c - gets */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  gets  -- get string from the STDIN device reading to user buffer
 *------------------------------------------------------------------------
 */
char *gets(s)
	char *s;
{
	register c;
	register char *cs;

	cs = s;
	while ((c = getc(STDIN)) != '\n' && c != '\r' && c != EOF)
		*cs++ = c;
	if (c==EOF && cs==s)
		return(NULL);
	*cs = '\0';
	return(s);
}
