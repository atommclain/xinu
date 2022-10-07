/* lfinter.c - lfinter */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfinter  --  handle local file operation completion
 *------------------------------------------------------------------------
 */
INTPROC	lfinter(minor)
	long	minor;
{
	signal(Lf.ldone);
}
