;/* snapregs.s - snapregs */

;*------------------------------------------------------------------------
;*  snapregs  --  get a snapshot of the registers
;*------------------------------------------------------------------------

	xdef	_snapregs

_snapregs:
	move.l	a6,-(sp)	; Save A6
	movea.l	8(sp),a6	; Get address of register area in A6
	movem.l	d0-d7/a0-a5,(a6); Snapshot most of the registers
	adda.l	#(8+6)*4,a6	; Move past already-saved values
	move.l	(sp),(a6)+	; Snapshot A6
	move.l	sp,(a6)		; Snapshot stack pointer
	addi.l	#12,(a6)+	; Get value of SP before call to snapregs
	move.l	4(sp),(a6)+	; Save caller's return address as PC
	move	sr,2(a6)	; Save processor status beyond registers
	movea.l	(sp)+,a6	; Restore A6
	rts
