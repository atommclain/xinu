/* network.h */

/* All includes needed for the network */

#include <appletalk.h>
#include <atalk.h>
#include <ip.h>
#include <icmp.h>
#include <udp.h>
#include <net.h>
#include <dgram.h>
#include <arp.h>
#include <fserver.h>
#include <rfile.h>
#include <domain.h>

/* Declarations data conversion and checksum routines */

#define	hs2net(x)	(x)		/* host to network short	*/
#define	net2hs(x)	(x)		/* network to host short	*/
#define	hl2net(x)	(x)		/* host to network long		*/
#define	net2hl(x)	(x)		/* network to host long		*/
extern	long		hs2vax();	/* 68000-to-vax short		*/
extern	long		vax2hs();	/* vax-to-68000 short		*/
extern	long		hl2vax();	/* 68000-to-vax long		*/
extern	long		vax2hl();	/* vax-to-68000 long		*/
extern	short		cksum();	/* 1s comp of 16-bit 2s comp sum*/
