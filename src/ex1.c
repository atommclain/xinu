/* ex1.c - main */

#include <conf.h>

/*------------------------------------------------------------------------
 *  main  --  write "hi" on the console
 *------------------------------------------------------------------------
 */
main()
{
	putc(CONSOLE, 'h'); putc(CONSOLE, 'i');
	putc(CONSOLE, '\r'); putc(CONSOLE, '\n');
}
