/* via.h */

/* VIA base addresses */

#define vBase	0xefe1fe	/* Main base for VIA chip (in var. VIA)	*/
#define aVBufB	vBase		/* Register B base			*/
#define aVBufA	0xeffffe	/* Register A base			*/
#define aVBufM	aVBufB		/* Register containing mouse signals	*/
#define aVIFR	0xeffbfe	/* Interrupt flag register		*/
#define aVIER	0xeffdfe	/* Interrupt enable register		*/

/* Offsets from vBase */

#define vBufB	512*0		/* Register B (zero offset)		*/
#define vDirB	512*2		/* Register B direction register	*/
#define vDirA	512*3		/* Register A direction register	*/
#define vT1C	512*4		/* Timer 1 counter (low-order byte)	*/
#define vT1CH	512*5		/* Timer 1 counter (high-order byte)	*/
#define vT1L	512*6		/* Timer 1 latch (low-order byte)	*/
#define vT1LH	512*7		/* Timer 1 latch (high-order byte)	*/
#define vT2C	512*8		/* Timer 2 counter (low-order byte)	*/
#define vT2CH	512*9		/* Timer 2 counter (high-order byte)	*/
#define vSR	512*10		/* Shift register (keyboard data)	*/
#define vACR	512*11		/* Auxiliary control register		*/
#define vPCR	512*12		/* Peripheral control register		*/
#define vIFR	512*13		/* Interrupt flag register		*/
#define vIER	512*14		/* Interrupt enable register		*/
#define vBufA	512*15		/* Register A				*/

/* VIA register A constants */

#define vAOut	0x7f		/* Direction reg. A: 1 bits = outputs	*/
#define vAInit	0x7b		/* Initial vBufA value (medium volume)	*/
#define vSound	7		/* Sound volume bits			*/

/* VIA register A bit masks */

#define vSndPg2		0010	/* 0 = alternate sound buffer		*/
#define vOverlay	0020	/* 1 = ROM overlay (system startup only)*/
#define vHeadSel	0040	/* disk SEL control line		*/
#define vPage2		0100	/* 0 = alternate screen buffer		*/
#define vSCCWReq	0200	/* SCC wait/request line		*/

/* VIA register B constants */

#define vBOut	0x87		/* Direction reg. B: 1 bits = outputs	*/
#define vBInit	0x07		/* Initial value for vBufB		*/

/* VIA register B bit masks */

#define rTCData		0001	/* Real-time clock serial data line	*/
#define rTCClk		0002	/* Real-time clock data-clock line	*/
#define rTCEnb		0004	/* Real-time clock serial enable	*/
#define vSW		0010	/* 0 = mouse button is down		*/
#define vX2		0020	/* Mouse X quadrature level		*/
#define vY2		0040	/* Mouse Y quadrature level		*/
#define vH4		0100	/* 1 = horizontal blanking		*/
#define vSndEnb		0200	/* 0 = sound enabled, 1 = disabled	*/

/* Masks for VIA interrupt flag and enable registers */

#define v1SecInt	0001	/* One-second interrupt			*/
#define vVBLInt		0002	/* Vertical blanking interrupt		*/
#define vKeyRdy		0004	/* Keyboard data ready			*/
#define vKeyBit		0010	/* Keyboard data bit			*/
#define vKeyClk		0020	/* Keyboard clock			*/
#define vT2Int		0040	/* Timer 2 interrupt			*/
#define vT1Int		0100	/* Timer 1 interrupt			*/
#define vIRQ		0200	/* Any enabled interrupt pending	*/
#define vIntEnb		0200	/* For write to IFR: 1=enable, 0=disable*/

/* Macintosh global variables */

#define ROMBase		(*(char **)0x2ae)/* Base address of ROM		*/
#define SoundBase	(*(char **)0x266)/* Address of main sound buffer*/
#define VIA		(*(char **)0x1d4)/* VIA base address		*/
