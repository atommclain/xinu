/* reopen.c - reopen */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  reopen  --  decrement refcount so dev can be returned by another dev
 *------------------------------------------------------------------------
 */
reopen(dev)
int	dev;
{
	if (!isbaddev(dev))
		--devtab[dev].dvrefct;
	return(dev);
}
