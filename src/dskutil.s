;/* dskutil.s - PBOffLine */

OffLine	equ	$a035			; OffLine trap word
async	equ	$0400			; Bit to set for async calls

;*------------------------------------------------------------------------
;*  PBOffLine  --  flush a volume and place it off-line
;*------------------------------------------------------------------------
	xdef	_PBOffLine
_PBOffLine:
	movea.l	4(sp),a0		; A0 = pointer to param block
	dc.w	OffLine			; OffLine trap word
	rts				; Return
