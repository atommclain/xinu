/* arp.h */

/* DARPA Internet Address Resolution Protocol  (see RFCs 826, 920) */

#define	AR_HRD	1		/* Ethernet hardware type code		*/
#define	AR_PROT	0x0800		/* IP protocol address code		*/
#define	AR_HLEN	6		/* Ethernet physical address length	*/
#define	AR_PLEN	4		/* IP Protocol address length		*/
#define	AR_TAB	6		/* size of IP-to-Ether addr. cache (>1)	*/
#define	AR_TIME	10		/* time to wait for reply in 1/10 secs	*/
#define	AR_RTRY	2		/* num. of times to try an arp request	*/

/* Definitions of codes used in operation field of ARP packet */

#define	AR_REQ	1		/* arp request to resolve address	*/
#define	AR_RPLY	2		/* reply to a resolve request		*/
#define	AR_RREQ	3		/* reverse ARP request (RARP packets)	*/
#define	AR_RRLY	4		/* reply to a reverse request (RARP ")	*/

struct	arppak	{		/* format of ARP packet (actually NBP)	*/
	unsigned char	abOpcode;
	short		abResult;
	struct	atblk	*abUserReference;
	EntityPtr	nbpEntityPtr;
	char		*nbpBufPtr;
	short		nbpBufSize;
	short		nbpDataField;
	Aaddr		nbpAddress;
	RetransType	nbpXmit;
	EntityName	nbpName;
	char		nbpBuf[120];
};

/* Format of the IP-to-Ethernet address resolution cache */

struct	arpent	{		/* format of entry in ARP cache		*/
	char	arp_state;	/* state of this entry (see below)	*/
	Aaddr	arp_Aad;	/* AppleTalk address of this host	*/
	IPaddr	arp_Iad;	/* IP address of this host		*/
	int	arp_dev;	/* Xinu device for this host route	*/
};

/* Definitions of table entry states */

#define	AR_FREE		'\0'	/* Entry is unused (initial value)	*/
#define	AR_ALLOC	'\1'	/* Entry is used but route still unknown*/
#define	AR_RGATE	'\2'	/* Entry is reachable only by gateway	*/
#define	AR_RSLVD	'\3'	/* Entry has been resolved to Eth. addr.*/

struct	arpblk	{		/* all information about ARP cache	*/
	struct arpent arptab[AR_TAB]; /* IP-to-AppleTalk address cache	*/
	int	atabsiz;	/* current entries in arptab		*/
	int	atabnxt;	/* next position in arptab to use	*/
	int	arpsem;		/* semaphore for access to ARP service	*/
	int	arppid;		/* id of process waiting for ARP reply	*/
	IPaddr	arpwant;	/* IP addr. process waiting to resolve	*/
	int	rarppid;	/* id of process waiting for RARP reply	*/
	int	rarpsem;	/* semaphore for access to RARP service	*/
};

/* Definitions needed to talk to Kinetics gateway (pseudo-RARP) */

struct	gwpak	{			/* Kinetics gateway protocol pkt*/
	long	gw_op;			/* opcode			*/
	IPaddr	gw_addr;		/* my IP address		*/
	IPaddr	gw_name;		/* my name server		*/
	IPaddr	gw_bcast;		/* my broadcast address		*/
	IPaddr	gw_file;		/* my file server		*/
	long	gw_other[4];		/* other addresses/flags	*/
	char	gw_string[128];		/* null terminated string	*/
};

#define GWASSIGN	1		/* assign me an address		*/
#define GWNAME		2		/* name lookup			*/

/* NBP definitions */
#define ZONE		"*"
#define ANYOBJ		"="
#define HOSTTYPE	"IPADDRESS"
#define GWTYPE		"IPGATEWAY"

extern	struct	arpblk	Arp;
