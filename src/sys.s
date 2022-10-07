;/* sys.s - constants often used in assembly language */

MAXINT		equ	$7FFF	; maximum integer

DISABLE		equ	$2700	; SR to	disable	interrupts
INT3MASK	equ	$2300	; SR to mask level-3 interrupts
ENABLE		equ	$2000	; SR to enable interrupts

debugVec	equ	$7C	; Loc. of level-7 interrupt auto-vector

HeapEnd		equ	$114	; End of Macintosh heap/Mac stack limit
VBLQueue	equ	$160	; VBL queue header
qFlags		equ	0	; Offset of qFlags field in QHdr record
inVBL		equ	$4000	; QFlags bit set to disable VBL tasks
vblCount	equ	10	; Offset of count field in VBLTask record
CurrentA5	equ	$904	; Value of A5 for accessing global data
CurStackBase	equ	$908	; Base of Macintosh stack region
