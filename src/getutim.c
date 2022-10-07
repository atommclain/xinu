/* getutim.c - getutim */

#include <conf.h>
#include <kernel.h>
#include <macvars.h>
#include <sleep.h>
#include <date.h>

/*------------------------------------------------------------------------
 *  getutim  --  obtain time in seconds past Jan 1, 1970, ut (gmt)
 *------------------------------------------------------------------------
 */
SYSCALL	getutim(timvar)
long	*timvar;
{
	wait(clmutex);
	if (clktime < SECPERHR)		/* assume small numbers invalid	*/
		clktime = mac2xt(Time);
	*timvar = clktime;
	signal(clmutex);
	return(OK);
}
