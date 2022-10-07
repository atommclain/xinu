/* getevent.c - getevent */

#include <kernel.h>
#include <macvars.h>
#include <window.h>
#include <osutil.h>

extern	int	evtpid;		/* process id of eventd			*/

/*------------------------------------------------------------------------
 *  getevent  --  kick eventd if there are events to process
 *------------------------------------------------------------------------
 */
getevent()
{
	if (getpid() == evtpid)
		return;
	if (EventQueue.qHead != NULL ||
	    CurActivate != NULL || CurDeactivate != NULL)
		send(evtpid, (long) OK);
}
