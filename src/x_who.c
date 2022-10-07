/* x_who.c - x_who */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  x_who  --  (command who) print name user is logged in under
 *------------------------------------------------------------------------
 */
COMMAND	x_who()
{
	char	str[80];
	long	idle;

	if (unmarked(Log.lmark) || !Log.lused)
		return(OK);
	sprintf(str, "%-8s %-10s ", Log.luser, devtab[Log.ldev].dvname);
	write(STDOUT, str, strlen(str));
	ascdate(ut2ltim(Log.llogin), str);
	write(STDOUT, str, strlen(str));
	getutim(&idle);
	idle -= Log.llast;
	if (idle >= SECPERDY)
		sprintf(str, " %Dd\n", idle/SECPERDY);
	else if (idle >= SECPERHR)
		sprintf(str, " %2D:%02D\n",
			idle/SECPERHR, (idle%SECPERHR)/SECPERMN);
	else if (idle >= SECPERMN)
		sprintf(str, "   :%02D\n", idle/SECPERMN);
	else if (idle < 0)
		sprintf(str, " %D?\n", idle);
	else
		strcpy(str, "\n");
	write(STDOUT, str, strlen(str));
	return(OK);
}
