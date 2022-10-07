/* naminit.c - naminit */

#include <conf.h>
#include <kernel.h>
#include <name.h>

struct	nam	Nam;

/*------------------------------------------------------------------------
 *  naminit  --  initialize the syntactic namespace pseudo-device
 *------------------------------------------------------------------------
 */
naminit()
{
	Nam.nnames = 0;

	/* Xinu namespace definition */

	mount("/dev/console",	CONSOLE,	NULLSTR);
	mount("/dev/stdin=",	STDIN,		NULLSTR);
	mount("/dev/stdout=",	STDOUT,		NULLSTR);
	mount("/dev/stderr=",	STDERR,		NULLSTR);
	mount("/dev/window/",	WINDOW,		NULLSTR);
	mount("/dev/pwindow/",	WINDOW,		"!");
	mount("/dev/disk1",	DISK1,		NULLSTR);
	mount("/dev/disk2",	DISK2,		NULLSTR);
	mount("/dev/",		SYSERR,		NULLSTR);
	mount("/floppy/",	LFILSYS,	"Xinu Disk/");
	mount("/",		LFILSYS,	"HD/");
	mount("+",		NAMESPACE,	"/dev/window/");
	mount("!",		NAMESPACE,	"/dev/pwindow/");
	mount(NULLSTR,		NAMESPACE,	"/");
}
