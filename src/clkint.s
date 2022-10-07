;/* clkint.s - clkint */

	include	"sys.s"

;*------------------------------------------------------------------------
;*  clkint  --  real-time clock interrupt service routine
;*------------------------------------------------------------------------
	xdef	_clkint
	xref	_clkTask,_getevent,_count6,_count10
	xref	_clktime,_defclk,_clkdiff,_slnempty,_sltop,_wakeup
	xref	_preempt,_resched
_clkint:
	; Macintosh stuff
	move	sr,-(sp)		; Save SR
	move	#DISABLE,sr		; Disable interrupts
	movem.l	a0/a5,-(sp)		; Save A0 & A5
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	move	#1,_clkTask+vblCount	; Reset VBL tick count

	; XINU stuff
	subq	#1,_count6		; Is this the 6th interrupt?
	bgt.s	clret			;  no => return
	move	#6,_count6		;  yes=> reset counter&continue
	subq	#1,_count10		; Is this 10th tick?
	bgt.s	clckdef			;  no => process tick
	move	#10,_count10		;  yes=> reset counter&continue
	addq.l	#1,_clktime		; Increment time-of-day clock
clckdef:
	tst	_defclk			; Are clock ticks deferred?
	beq.s	notdef			;  no => go process this tick
	addq	#1,_clkdiff		;  yes=> count in clkdiff and
	bra.s	clret			;	 return quickly
notdef:
	jsr	_getevent		; Handle events if any available
	tst	_slnempty		; Is sleep queue nonempty?
	beq.s	clpreem			;  no => go process preemption
	movea.l	_sltop,a0		;  yes=> decrement delta key
	subq	#1,(a0)			;	 on first process,
	bgt.s	clpreem			;	 calling wakeup if
	jsr	_wakeup			;	 it reaches zero
clpreem:
	subq	#1,_preempt		; Decrement preemption counter
	bgt.s	clret			;   and call resched if it
	jsr	_resched		;   reaches zero
clret:
	movem.l	(sp)+,a0/a5		; Restore A0 & A5
	move	(sp)+,sr		; Restore interrupts
	rts				; Return
