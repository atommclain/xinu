/* types.h */

/* Aztec C (Pascal) type definitions */

#ifndef _TYPES

typedef	unsigned char	Byte;
typedef char		SignedByte;
typedef	char *		Ptr;
typedef	Ptr *		Handle;
typedef	short		(*ProcPtr)();
typedef	char		Boolean;
typedef	long		Fract;
typedef	long		Fixed;

#define MTRUE		(-1)		/* MTRUE and MFALSE are the	*/
#define MFALSE		0		/*  Boolean values used by the	*/
					/*  Macintosh ROM		*/
#define toBool(b)	((b)? MTRUE : MFALSE)

#define	String(x)	struct {						\
				unsigned char length;	\
				unsigned char text[x];	\
			}
typedef	String(255)	Str255;
typedef Str255 *	StringPtr;
typedef Str255 **	StringHandle;

typedef short		OSErr;
typedef long		OSType;

#define _TYPES
#endif
