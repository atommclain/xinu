/* vecinit.c - vecinit */

#include <kernel.h>
#include <macvars.h>

extern	int	*lowcore[], *endvec;	/* New interrupt vectors	*/

/*------------------------------------------------------------------------
 *  vecinit  --  initialize exception, interrupt and trap vectors
 *------------------------------------------------------------------------
 */
vecinit()
{
	register int	v, *swap;

	for (v = 0; v < &endvec-lowcore; ++v)
		if (lowcore[v] != 0) {
			swap = intvec[v];
			intvec[v] = lowcore[v];
			lowcore[v] = swap;
		}
	setdebug(intvec[DEBUG]);
}

/*------------------------------------------------------------------------
 *  setdebug  --  set interrupt switch interrupt vector to specified value
 *------------------------------------------------------------------------
 */
setdebug(newvec)
int	*newvec;
{
	register int	v;

	if (_newrom() == 2)		/* Mac II: set level 7 vector	*/
		intvec[DEBUG] = newvec;
	else				/* Otherwise: set levels 4-7	*/
		for (v = INT4; v <= DEBUG; ++v)
			intvec[v] = newvec;
}
