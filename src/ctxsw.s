;/* ctxsw.s - ctxsw */

	include	"sys.s"

;*------------------------------------------------------------------------
;*  ctxsw  --  actually perform context switch, saving/loading registers
;*------------------------------------------------------------------------
  ; The stack contains three items upon entry to this routine:
  ;
  ;	SP+8 =>	address of 18 long-word save area with new registers + SR
  ;	SP+4 =>	address of 18 long-word save area for old registers + SR
  ;	SP   =>	return address
  ;
  ; The saved state consists of: the values of D0-D7 and A0-A6 upon entry,
  ; SP+4, PC equal to the return address, and the SR (i.e., the PC and SP
  ; are saved as if the calling process had returned to its caller).

	xdef	_ctxsw		; declare the routine name global
_ctxsw:				; entry point to context switch
	move.l	a6,-(sp)	; Temporarily push A6 onto stack
	movea.l	8(sp),a6	; Get address of old register area in A6
	movem.l	d0-d7/a0-a5,(a6)	; Save most of the registers
	adda.l	#(8+6)*4,a6	; Move past already-saved values
	move.l	(sp)+,(a6)+	; Save A6
	addq.l	#4,sp		; Move sp beyond the return address,
				;   as if a return had occurred.
	move.l	sp,(a6)+	; Save stack pointer
	move.l	-(sp),(a6)+	; Save caller's return address as PC
	move	VBLQueue,(a6)+	; Save VBL queue flags as part of PS
	move	sr,(a6)		; Save processor status beyond registers
	movea.l	8(sp),a6	; Pick up address of new registers in A6
				; Ready to load registers for the new
				;   process and abandon the old stack.
	movem.l	(a6)+,d0-d7/a0-a5	; Load registers for new process
	move.l	4(a6),sp	; Load SP (using new stack now)
	move.l	8(a6),-(sp)	; Push new process PC on new process stack
	move	12(a6),VBLQueue	; Load VBL queue flags
	move	14(a6),-(sp)	; Push new process PS on new process stack
	movea.l	(a6),a6		; Finally, load A6 from new area
	move	(sp)+,sr	; Load new PS
	rts			; Load PC and reset SP all at once
