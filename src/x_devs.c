/* x_devs.c - x_devs */

#include <conf.h>
#include <kernel.h>

LOCAL	char hd1[] =
       "Num  Device   minor refct   CSR    i-vect.  o-vect.  cntrl blk\n";
LOCAL	char hd2[] =
       "--- --------  ----- ----- -------- -------- -------- ---------\n";

/*------------------------------------------------------------------------
 *  x_devs  --  (command devs) print main fields of device switch table
 *------------------------------------------------------------------------
 */
COMMAND	x_devs(nargs, args)
int	nargs;
char	*args[];
{
	struct	devsw	*devptr;
	char	str[80];
	int	i;

	write (STDOUT, hd1, strlen(hd1) );
	write (STDOUT, hd2, strlen(hd2) );
	for (i=0 ; i<NDEVS ; i++) {
		devptr = &devtab[i];
		sprintf(str,"%2d. %-9s %3d   %3d   %08X %08X %08X %08X\n",
			i, devptr->dvname,devptr->dvminor,devptr->dvrefct,
			devptr->dvcsr, devptr->dvivec, devptr->dvovec,
			devptr->dvioblk);
		write(STDOUT, str, strlen(str));
	}
}
