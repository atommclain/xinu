;/* machdep.s - _disable, _restore, enable, pause */

;*------------------------------------------------------------------------
;*  Assembler escapes cannot be done in macros in Aztec C
;*------------------------------------------------------------------------

	include	"sys.s"
	xdef	__disable,__restore,_enable,_pause

__disable:
	movea.l	4(sp),a0	; Get address of saved SR location
	move	sr,(a0)		; Save old SR
	ori	#DISABLE,sr	; Disable interrupts
	rts

__restore:
	movea.l	4(sp),a0	; Get address of saved SR location
	move	(a0),sr		; Restore interrupt status
	rts

_enable:
	move	#ENABLE,sr	; Enable interrupts
	rts

_pause:
	stop	#ENABLE		; Wait for interrupt
	rts
