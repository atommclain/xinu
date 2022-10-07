/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <conf.h>

/* device independent I/O switch */

struct	devsw	devtab[NDEVS] = {

/*  Format of entries is:
device-number, device-name, ref-count,
init, open, close,
read, write, seek,
getc, putc, cntl,
device-csr-address, input-vector, output-vector,
iint-handler, oint-handler, control-block, minor-device,
*/

/*  CONSOLE  is con  */

0, "CONSOLE", 0,
coninit, ioopen, ioerr,
conread, conwrite, ioerr,
congetc, conputc, concntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0,

/*  STDIN  is sio  */

1, "STDIN", 0,
ionull, sioopen, sioclose,
sioread, siowrite, sioseek,
siogetc, sioputc, siocntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0,

/*  STDOUT  is sio  */

2, "STDOUT", 0,
ionull, sioopen, sioclose,
sioread, siowrite, sioseek,
siogetc, sioputc, siocntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 1,

/*  STDERR  is sio  */

3, "STDERR", 0,
ionull, sioopen, sioclose,
sioread, siowrite, sioseek,
siogetc, sioputc, siocntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 2,

/*  KEYBOARD  is key  */

4, "KEYBOARD", 0,
ionull, ioerr, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0xefe1fe, 0x000, 0x000,
keyiin, ioerr, NULLPTR, 0,

/*  DISK1  is dsk  */

5, "DISK1", 0,
dsinit, ioerr, ioerr,
dsread, dswrite, ioerr,
ioerr, ioerr, dscntl,
0x000001, 0x000, 0x000,
dsinter, dsinter, NULLPTR, 0,

/*  DISK2  is dsk  */

6, "DISK2", 0,
dsinit, ioerr, ioerr,
dsread, dswrite, ioerr,
ioerr, ioerr, dscntl,
0x000002, 0x000, 0x000,
dsinter, dsinter, NULLPTR, 1,

/*  LFILSYS  is lfm  */

7, "LFILSYS", 0,
ionull, lfopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, lfcntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0,

/*  LFILE1  is lf  */

8, "LFILE1", 0,
lfinit, ioerr, lfclose,
lfread, lfwrite, lfseek,
lfgetc, lfputc, lfcntl,
0x000000, 0x000, 0x000,
lfinter, lfinter, NULLPTR, 0,

/*  LFILE2  is lf  */

9, "LFILE2", 0,
lfinit, ioerr, lfclose,
lfread, lfwrite, lfseek,
lfgetc, lfputc, lfcntl,
0x000000, 0x000, 0x000,
lfinter, lfinter, NULLPTR, 1,

/*  LFILE3  is lf  */

10, "LFILE3", 0,
lfinit, ioerr, lfclose,
lfread, lfwrite, lfseek,
lfgetc, lfputc, lfcntl,
0x000000, 0x000, 0x000,
lfinter, lfinter, NULLPTR, 2,

/*  LFILE4  is lf  */

11, "LFILE4", 0,
lfinit, ioerr, lfclose,
lfread, lfwrite, lfseek,
lfgetc, lfputc, lfcntl,
0x000000, 0x000, 0x000,
lfinter, lfinter, NULLPTR, 3,

/*  LFILE5  is lf  */

12, "LFILE5", 0,
lfinit, ioerr, lfclose,
lfread, lfwrite, lfseek,
lfgetc, lfputc, lfcntl,
0x000000, 0x000, 0x000,
lfinter, lfinter, NULLPTR, 4,

/*  WINDOW  is winm  */

13, "WINDOW", 0,
ionull, winopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0,

/*  WINDOW1  is win  */

14, "WINDOW1", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0,

/*  WINDOW2  is win  */

15, "WINDOW2", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 1,

/*  WINDOW3  is win  */

16, "WINDOW3", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 2,

/*  WINDOW4  is win  */

17, "WINDOW4", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 3,

/*  WINDOW5  is win  */

18, "WINDOW5", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 4,

/*  WINDOW6  is win  */

19, "WINDOW6", 0,
wininit, ioerr, winclose,
winread, winwrite, ioerr,
wingetc, winputc, wincntl,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 5,

/*  NAMESPACE  is nam  */

20, "NAMESPACE", 0,
naminit, namopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0x000000, 0x000, 0x000,
ioerr, ioerr, NULLPTR, 0
	};
