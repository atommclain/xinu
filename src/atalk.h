/* atalk.h */

/* AppleTalk definitions and constants */

#define	ATMINPAK	ATHLEN		/* ?? minimum packet length	*/
#define	ATMAXPAK	(ATHLEN+ATDLEN)	/* maximum size of xinu packet	*/
#define	ATHLEN	sizeof(struct aheader)	/* size of AppleTalk pkt header	*/
#define	ATDLEN		586		/* maximum DDP datagram data	*/
typedef	AddrBlock	Aaddr;		/* AppleTalk physical address	*/
#define	APADLEN		sizeof(Aaddr)	/* # octets in physical address	*/
#define	EXRETRY		3		/* # times to retry xmit errors	*/
#define	ATBCAST		'\377'		/* AppleTalk broadcast address	*/

typedef	char		ATskt;		/* DDP socket number		*/
typedef short		ATnet;		/* Atalk net number		*/

struct	aheader	{		/* format of AppleTalk packet header	*/
	unsigned char	abOpcode;	/* operation (read, write, etc.)*/
	short		abResult;	/* result code			*/
	struct atblk	*abUserReference;/* pointer to control block	*/
	short		ddpType;	/* DDP protocol type		*/
	short		ddpSocket;	/* local socket number		*/
	Aaddr		ddpAddress;	/* foreign socket address	*/
	short		ddpReqCount;	/* requested data length	*/
	short		ddpActCount;	/* actual data length		*/
	char		*ddpDataPtr;	/* pointer to data buffer	*/
	short		ddpNodeID;	/* original dest. node ID (read)*/
};

#define DDP_IP	22		/* packet type: DARPA Internet protocol	*/

/* Ethernet packet types, not used by AppleTalk */

#define	EP_IP	0x0800		/* packet type: DARPA Internet protocol	*/
#define	EP_ARP	0x0806		/* packet type: Address resolution  "	*/
#define	EP_RARP	0x8035		/* packet type: reverse  "  "       "	*/


struct	apacket	{		/* complete structure of AppleTalk pkt	*/
	struct	aheader ap_hdr;		/* packet header		*/
	char	ap_data[ATDLEN];	/* data in the packet		*/
};

struct	atblk	{			/* AppleTalk control block	*/
	int	atdnum;			/* network's device # in devtab	*/
	Aaddr	atpaddr;		/* my AppleTalk physical addr	*/
	int	atrpid;			/* reader's process id		*/
	int	atwpid;			/* writer's process id		*/
	int	atrsem;			/* mutex for reading		*/
	int	atwsem;			/* mutex for writing		*/
	int	atwtry;			/* # times to retry xmit errors	*/
};

extern struct atblk atalk[];

#define IPSOCKET	72		/* socket # FastPath talks to	*/
#define SYNCH		MFALSE
#define ASYNCH		MTRUE
