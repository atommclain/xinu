/* lfopen.c - lfopen */

#include <conf.h>
#include <kernel.h>
#include <pb.h>
#include <file.h>
#include <lfile.h>

/*------------------------------------------------------------------------
 *  lfopen  --  open a local file
 *------------------------------------------------------------------------
 */
lfopen(devptr, name, mode)
struct	devsw	*devptr;
char	*name;
char	*mode;
{
	struct	lfblk	*lfptr;
	int	i;
	char	lfname[LNAMLEN], dirname[LNAMLEN], *dir;
	int	mbits;
	int	fd;
	char	*index(), *ctop();

	if (strlen(name) > LNAMLEN || lfnammap(name, lfname) == SYSERR ||
	    (mbits=ckmode(mode)) == SYSERR || (i=lfalloc()) == SYSERR)
		return(SYSERR);
	lfptr = &Lf.lftab[i];
	wait(lfptr->lf_mutex);
	lfptr->lf_mode	= mbits;
	lfptr->lf_pos	= 0L;
	lfptr->lf_crlf	= TRUE;

	if (!(mbits & FLOLD)) {
		dir = index(lfname, LFPSEP);
		while ((dir=index(dir+1, LFPSEP)) != NULL) {
			strncpy(dirname, lfname, dir-lfname);
			dirname[dir-lfname] = NULLCH;
			if (lfnamop(PBDirCreate,ctop(dirname),FLRW,NULL)
			    == SYSERR) {
				lfptr->lf_state = LFREE;
				return(SYSERR);
			}
		}
	}
	ctop(lfname);
	if ((mbits&FLOLD) || (fd=lfnamop(PBCreate,lfname,mbits,NULL))==OK
	    || !(mbits&FLNEW))
		fd = lfnamop(PBOpen, lfname, mbits, NULL);
	if ( (lfptr->lf_refnum=fd) == SYSERR )
		lfptr->lf_state = LFREE;
	signal(lfptr->lf_mutex);
	return(fd == SYSERR? SYSERR : lfptr->lf_dnum);
}


/*------------------------------------------------------------------------
 *  lfnammap  --  map a name to the format used by the ROM routines
 *------------------------------------------------------------------------
 */
lfnammap(name, newname)
char	*name;
char	*newname;
{
#ifdef	PATHSEP
	int	nsep;			/* # of separators encountered	*/

	if (*name == PATHSEP)		/* ROM absolute names do not	*/
		return(SYSERR);		/*  begin with a path separator	*/
	for (nsep = 0; *name != NULLCH; )
		if (*name == LFPSEP)
			return(SYSERR);
		else if (*name != PATHSEP)
			*newname++ = *name++;
		else {
			nsep++;
			*newname++ = LFPSEP;
			if (*++name == PATHSEP)
				return(SYSERR);
		}
	if (nsep == 0)			/* ROM absolute names contain	*/
		return(SYSERR);		/*  at least one separator	*/
	*newname = NULLCH;
#else
	strcpy(newname, name);
#endif
	return(OK);
}
