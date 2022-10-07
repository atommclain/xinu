;/* startup.s - start */

	include	"sys.s"

;*------------------------------------------------------------------------
;*  Xinu system entry point  --  first location beyond interrupt vectors
;*------------------------------------------------------------------------
	xdef	start,_start,_Croot
	xref	_maxaddr,_nulluser
start:
_start:					; Name usable from C
_Croot:					; Aztec C entry point name
	move	#DISABLE,sr		; Disable interrupts
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	movea.l	#0,a6			; No stack frame yet
	move.l	CurStackBase,_maxaddr	; _maxaddr set to max address
	subq.l	#2,_maxaddr		; Leave room for magic word
	movea.l	_maxaddr,sp		; Switch stack to high memory
	bra.s	_nulluser		; Jump to C startup routine
