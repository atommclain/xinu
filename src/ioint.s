;/* ioint.s - inint, outint */

; I/O interrupts trap here.  Original PC is on top of the stack upon
; entry.  Register A0 contains a pointer to the parameter block used in
; the asynchronous ROM call.  Interrupts are enabled.

	include	"../sys.s"

ioResult	equ	16		; Offset of result in param blk
abortErr	equ	-27		; I/O request abort result code

	xdef	_inint, _outint
	xref	_intmap
_outint:				; Output interrupt entry point
	move	sr,-(sp)		; Save SR
	move	#DISABLE,sr		; Disable interrupts
	movem.l	d0/a1/a5,-(sp)		; Save D0, A1 & A5
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	lea	_intmap+8,a1		; Point A1 to output in intmap
	bra.s	ioint			; Go do common part of code
_inint:					; Input interrupt entry point
	move	sr,-(sp)		; Save SR
	move	#DISABLE,sr		; Disable interrupts
	movem.l	d0/a1/a5,-(sp)		; Save D0, A1 & A5
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	lea	_intmap,a1		; Point A1 to input in intmap
ioint:					; Code common to input & output
	cmp.w	#abortErr,ioResult(a0)	; Was operation aborted?
	beq.s	ioret			;  yes=> return
	move.w	-2(a0),d0		; Get desc. from control block
	asl.w	#4,d0			; Pick correct entry in intmap
	adda.w	d0,a1			; Form pointer to intmap entry
	move.l	4(a1),-(sp)		; Push "code" from intmap as arg
	movea.l	(a1),a1			; Get addr. of interrupt routine
	jsr	(a1)			; Call interrupt routine
	addq.l	#4,sp			; Pop argument
ioret:
	movem.l	(sp)+,d0/a1/a5		; Restore D0, A1 & A5
	move	(sp)+,sr		; Restore SR
	rts				; Return from interrupt
