/* ex2.c - main, prA, prB */

#include <conf.h>

/*------------------------------------------------------------------------
 *  main  --  example of creating proceses in Xinu
 *------------------------------------------------------------------------
 */
main()
{
	int	prA(), prB();

	resume( create(prA, 8000L, 20, "proc 1", 0) );
	resume( create(prB, 8000L, 20, "proc 2", 0) );
}

/*------------------------------------------------------------------------
 *  prA  --  repeatedly print 'A' without ever terminating
 *------------------------------------------------------------------------
 */
prA()
{
	while( 1 )
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 *  prB  --  repeatedly print 'B' without ever terminating
 *------------------------------------------------------------------------
 */
prB()
{
	while( 1 )
		putc(CONSOLE, 'B');
}
