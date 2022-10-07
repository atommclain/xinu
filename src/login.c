/* login.c - login */

#include <conf.h>
#include <kernel.h>
#include <shell.h>

#define BUFLEN	128
#define	MOTD	"\n	Welcome to Xinu (type ? for help)\n\n"
LOCAL	char	*shargs[] = {SHNAME, NULL};
struct	loginfo	Log;			/* Login information		*/

/*------------------------------------------------------------------------
 *  login  --  log user onto system
 *------------------------------------------------------------------------
 */
login()
{
	int	len;
	char	*buf, *getmem();

	Log.lused = FALSE;
	Log.luser[0] = NULLCH;
	buf = getmem((long) BUFLEN);
	while (TRUE) {
		printf("\n\nMacintosh - The magic of Xinu\n\nlogin: ");
		while ( (len=read(STDIN,buf,BUFLEN))==0 || len==1)
			printf("login: ");
		if (len == EOF) {
			read(STDIN, buf, 0);
			Log.lused = FALSE;
			continue;
		}
		buf[len-1] = NULLCH;
		strncpy(Log.luser, buf, SHNAMLEN-1);
		Log.luser[SHNAMLEN-1] = NULLCH;
		Log.ldev = getdev(getpid(), STDIN);
		getutim(&Log.llogin);
		Log.llast = Log.llogin;
		Log.lused = TRUE;
		mark(Log.lmark);
		printf("%s", MOTD);
		freemem(buf, (long) BUFLEN);
		return(shell(1, shargs));
	}
}
