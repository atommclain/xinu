;/* panic.s - panic, panic0, _panic, _halt */

	include	"sys.s"
	xdef	panic, panic0, _panic, _halt, kernstk
	xref	start, _kprintf, _panhalt, _maxaddr
	xref	__newrom
;*------------------------------------------------------------------------
;*  panic0  --  entry point for "branch to zero" error; never called
;*------------------------------------------------------------------------
; Prevent context switches by disabling VBL tasks as well as disabling
; interrupts, because some Macintosh debuggers enable interrupts.
panic0:
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	move	sr,kernstk-2		; save old ps for message
	move	#DISABLE,sr		; disable interrupts
	move.l	sp,kernstk		; save old sp for message
	lea	kernlim,sp		; sp = address of kernstk limit
	move.l	sp,HeapEnd		; set stack limit for QuickDraw
	lea	kernstk-2,sp		; use kernel stack for printing
	pea	panmsg1			; push address of message
	jsr	_kprintf		; print	message
	ori	#inVBL,VBLQueue+qFlags	; disable VBL tasks
	jsr	_panhalt
	tst	d0			; does user want to halt?
	beq	panboot			;  no => restart system
	bsr	_halt			;  yes=> halt
panboot:
	jmp	start			; restart system if user continues

;*------------------------------------------------------------------------
;*  _panic  --  entry point when called by user programs
;*------------------------------------------------------------------------
_panic:					; Note: "_panic" pushes the
					; PC/PS to simulate a trap so it
					; can flow into "panic" (the
					; trap entry point, below)
	move	sr,-(sp)		; push PS for trap simulation
	move	#DISABLE,sr		; disable interrupts
	jsr	__newrom		; check Macintosh ROM
	cmp	#2,d0			; is this a Macintosh II?
	bne.s	else			;  no => get user's message
	move	(sp),-(sp)		; make room for 68020 format word
	move.l	4(sp),2(sp)		; shift return address
	move	#debugVec,6(sp)		; normal 4-word stack frame fmt
	move.l	8(sp),-(sp)		; push address of user's message
	bra.s	endif
else:
	move.l	6(sp),-(sp)		; push address of user's message
endif:
	move.l	a5,-(sp)		; Save A5
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	move.l	4(sp),pmsguser		; address of user's message
	movea.l	(sp)+,a5		; Restore A5
	addq.l	#4,sp			; Pop address of user's message
	move	#0,ccr			; Put "user trap" code in SR
	bra.s	panic

;*------------------------------------------------------------------------
;*  Trap entry points
;*------------------------------------------------------------------------
	public	panic1, panic2, panic3, panic4, panic5
	public	panic6, panic7, panic8, panic9, panic10
	public	panic11, panic12, panic13, panic14, panic15
	public	panic16, panic17, panic18, panic19, panic20
	public	panic21, panic22, panic23, panic24, panic25
	public	panic26, panic27

panic1:	move	#DISABLE+1,sr		; Bus error
	jsr	__newrom		; Check Macintosh ROM
	cmp	#2,d0			; Is this a Macintosh II?
	beq.s	endif1			;  yes=> set SR again
	addq.l	#8,sp			;  no => pop extra status info
endif1:	move	#DISABLE+1,sr		; Bus error
	bra.s	panic
panic2:	move	#DISABLE+2,sr		; Address error
	jsr	__newrom		; Check Macintosh ROM
	cmp	#2,d0			; Is this a Macintosh II?
	beq.s	endif2			;  yes=> set SR again
	addq.l	#8,sp			;  no => pop extra status info
endif2:	move	#DISABLE+2,sr		; Address error
	bra.s	panic
panic3:	move	#DISABLE+3,sr		; Illegal instruction
	bra.s	panic
panic4:	move	#DISABLE+4,sr		; Division by zero
	bra.s	panic
panic5:	move	#DISABLE+5,sr		; CHK or CHK2 instruction
	bra.s	panic
panic6:	move	#DISABLE+6,sr		; cpTRAPcc, TRAPcc, or TRAPV
	bra.s	panic
panic7:	move	#DISABLE+7,sr		; Privelege violation
	bra.s	panic
panic8:	move	#DISABLE+8,sr		; Trace bit
	bra.s	panic
panic9:	move	#DISABLE+9,sr		; Line 1010 emulator
	bra.s	panic
panic10:move	#DISABLE+10,sr		; Line 1111 emulator
	bra.s	panic
panic11:move	#DISABLE+11,sr		; Coprocessor protocol violation
	bra.s	panic
panic12:move	#DISABLE+12,sr		; Format error
	bra.s	panic
panic13:move	#DISABLE+13,sr		; Uninitialized interrupt vector
	bra.s	panic
panic14:move	#DISABLE+14,sr		; Spurious interrupt
	bra.s	panic
panic15:move	#DISABLE+15,sr		; Interrupt button
	cmpi.b	#$27,(sp)		; Were interrupts disabled?
	beq.s	rte			;  yes=> return
	move	#DISABLE+15,sr		;  no => set trap type
	bra.s	panic			;	 and continue
rte:	rte
panic16:move	#DISABLE+16,sr		; TRAP instruction
	bra.s	panic
panic17:move	#DISABLE+17,sr		; FPCP br./set on unordered cond.
	bra.s	panic
panic18:move	#DISABLE+18,sr		; FPCP inexact result
	bra.s	panic
panic19:move	#DISABLE+19,sr		; FPCP divide by zero
	bra.s	panic
panic20:move	#DISABLE+20,sr		; FPCP underflow
	bra.s	panic
panic21:move	#DISABLE+21,sr		; FPCP operand error
	bra.s	panic
panic22:move	#DISABLE+22,sr		; FPCP overflow
	bra.s	panic
panic23:move	#DISABLE+23,sr		; FPCP signaling NAN
	bra.s	panic
panic24:move	#DISABLE+24,sr		; PMMU configuration
	bra.s	panic
panic25:move	#DISABLE+25,sr		; PMMU illegal operation
	bra.s	panic
panic26:move	#DISABLE+26,sr		; PMMU access level violation
	bra.s	panic
panic27:move	#DISABLE+27,sr		; Unknown device

;*------------------------------------------------------------------------
;*  panic  --  entry point for traps and exceptions only; not called
;*------------------------------------------------------------------------
panic:
	move	sr,-(sp)		; Push SR
	move.l	a5,-(sp)		; Push A5
	movea.l	CurrentA5,a5		; Set A5 to point to global data
	move.l	(sp)+,savea5		; Pop old A5 into save area
	move	(sp)+,pansav		; save SR
	move.l	2(sp),kernstk		; push on kernel stack:	old pc
	move	(sp),kernstk-2		;	old ps
	move	VBLQueue+qFlags,kernstk-4 ;	old VBL queue flags
	move.l	HeapEnd,kernstk-8	;	old stack limit
	move.l	sp,kernstk-12		;	old sp (from before trap)
	addi.l	#6,kernstk-12
	lea	kernstk-12,sp
	movem.l	d0-d7/a0-a6,-(sp)	;	old d0-d7 & a0-a6
	move.l	savea5,52(sp)		; insert original value of A5
	ori	#inVBL,VBLQueue+qFlags	; disable VBL tasks
	lea	kernlim,a0		; a0 = address of kernstk limit
	move.l	a0,HeapEnd		; set stack limit for QuickDraw

; set up call to kprintf to print register and stack dump

	move	kernstk-2,-(sp)		; user's ps
	move.l	kernstk,-(sp)		; user's pc
	move	pansav,d0		; get message index from old SR
	andi	#$001F,d0		; mask off type
	asl	#2,d0			; use as index into pmsglist
	lea	pmsglist,a0
	move.l	0(a0,d0.w),-(sp)	; go through vector to message
	asr	#2,d0			; get back trap type
	move	d0,-(sp)
	pea	panmsg2			; push format address
	jsr	_kprintf		; print	message
	lea	16(sp),sp		; pop off args to printf, leaving
					;  saved regs, even though printed

; print stack dump

	movea.l	kernstk-12,a3		; a3 = stack pointer before trap
	move.l	a3,-(sp)		; push argument pointer
	pea	panmsg3			; format for kprintf
	jsr	_kprintf		; print address of arguments
	addq.l	#8,sp			; pop kprintf args
	bsr.s	prargs			; print stack dump

; print function call backtrace

	move.l	kernstk,d5		; d5 = PC value before trap
	movea.l	kernstk-16,a4		; a4 = frame pointer before trap
	moveq	#5,d7			; (# of stack frames to print)-1
panloop:
	move.l	a4,-(sp)		; push frame pointer
	addq.l	#8,(sp)			; get pointer to arguments
	move.l	d5,-(sp)		; push PC value
	pea	panmsg4			; printf format
	jsr	_kprintf		; print PC and argument pointer
	lea	12(sp),sp		; pop kprintf args
	move.w	a4,d0			; check validity of frame pointer:
	andi.w	#1,d0			;	frame pointer must be even
	bne.s	pandone
	cmpa.l	_maxaddr,a4		;	must be within XINU heap
	bhi.s	pandone
	cmpa.l	kernstk-12,a4		;	stay within process stack
	bcs.s	pandone
	move.l	4(a4),d5		; d5 = next PC value
	lea	8(a4),a3		; a3 = address of args
	movea.l	(a4),a4			; a4 = next frame pointer
	bsr.s	prargs			; print argument list
	dbf	d7,panloop		; decrement counter and loop

; clean	up and restore the state so user can continue after halt

pandone:
	pea	panmsg5			; push right paren format
	jsr	_kprintf		; print right paren
	addq.l	#4,sp			; pop format arg
	lea	(8+7)*4(sp),a3		; a3 = address of saved SP
	movea.l	(a3),a4			; a4 = user's stack pointer
	jsr	_panhalt		; find out if user wants to halt
	move	d0,-(a4)		; push result on user's stack
	move.l	a4,(a3)			; save user's new SP value
	move.l	kernstk-8,HeapEnd	; restore stack limit
	movem.l	(sp)+,d0-d7/a0-a6	; restore registers
	movea.l	(sp),sp			; restore user's stack pointer
	tst	(sp)+			; did user want to halt?
	beq.s	panret			;  no => return
	bsr.s	_halt			;  yes=> halt
	ori	#DISABLE,sr		; Aztec debugger enables intr.
panret:
	movea.l	CurrentA5,a5		; user probably won't continue
	move.l	kernstk,-(sp)		; but we should	restore	PS and PC
	move.w	kernstk-2,-(sp)		; in case he does, especially
	move	kernstk-4,VBLQueue+qFlags ; for call to _panic
	movea.l	savea5,a5
	rte

;*------------------------------------------------------------------------
;*  prargs  --  print function arguments in backtrace
;*------------------------------------------------------------------------
prargs:					; a3 = address of arguments
	moveq	#9,d6			; (# of args to print)-1
	clr.w	-(sp)			; leave room for argument
	pea	argmsg			; format arg for kprintf
argloop:
	cmpa.l	_maxaddr,a3		; avoid references beyond valid
	bhi.s	argdone			;  memory addresses
	move.w	(a3)+,4(sp)		; insert value
	jsr	_kprintf		; print one value
	dbf	d6,argloop		; decrement counter and loop
argdone:
	addq.l	#6,sp			; pop args to kprintf
	rts

;*------------------------------------------------------------------------
;*  halt  --  simulate a level-7 interrupt to invoke the debugger
;*------------------------------------------------------------------------
	xref	_lowcore
_halt:
	move	sr,-(sp)	; Push SR on top of PC
	move	#DISABLE,sr	; Mask interrupts
	cmpi.b	#$27,(sp)	; Are saved interrupts disabled?
	bne.s	hckrom		;  no => check ROM type
	move.b	#$26,(sp)	;  yes=> pretend level-7 int. were enabled
hckrom:
	jsr	__newrom	; Check Macintosh ROM
	cmp	#2,d0		; Is this a Macintosh II?
	bne.s	hintr		;  no => find interrupt handler
	move	(sp),-(sp)	; Make room for 68020 format word
	move.l	4(sp),2(sp)	; Shift return address
	move	#debugVec,6(sp)	; Normal 4-word stack frame format
hintr:
	clr.l	-(sp)		; Make room for addr. of intr. handler
	move.l	a5,-(sp)	; Save A5
	movea.l	CurrentA5,a5	; Load global data pointer
	move.l	_lowcore+debugVec,4(sp) ; Get address of intr. handler
	bne.s	hret		; If addr. in save area is zero,
	move.l	debugVec,4(sp)	;  get actual vector value
hret:
	movea.l	(sp)+,a5	; Restore A5
	move	4(sp),ccr	; Restore original condition codes
	rts			; Jump to interrupt handler

	dseg
pansav	ds.l	1			; long word for saving state info.
savea5	ds.l	1			; long word for saving original A5
panmsg1	dc.b	10
	dc.b	"Panic:  branch to loc. 0, ps=%04x sp=%08X"
	dc.b	10,0
panmsg2	dc.b	10
	dc.b	"Panic:  trap type %d (%s)"
	dc.b	10
	dc.b	"State:  pc=%08X ps=%04x"
	dc.b	10
	dc.b	"d0=%08X d1=%08X d2=%08X d3=%08X"
	dc.b	10
	dc.b	"d4=%08X d5=%08X d6=%08X d7=%08X"
	dc.b	10
	dc.b	"a0=%08X a1=%08X a2=%08X a3=%08X"
	dc.b	10
	dc.b	"a4=%08X a5=%08X a6=%08X"
	dc.b	10
	dc.b	"Stack and function backtrace:  (top at %08X)"
	dc.b	10,0
panmsg3	dc.b	"Stack:  (%08X:"
	dc.b	0
panmsg4	dc.b	")"
	dc.b	10
	dc.b	"%08X(%08X:"
	dc.b	0
panmsg5	dc.b	")"
	dc.b	10,0
argmsg	dc.b	" %04x"
	dc.b	0

	even				; Align pmsglist on word boundary
pmsglist:
pmsguser:
	ds.l	1			; Gets overwritten on panic call
	dc.l	pmsg1
	dc.l	pmsg2
	dc.l	pmsg3
	dc.l	pmsg4
	dc.l	pmsg5
	dc.l	pmsg6
	dc.l	pmsg7
	dc.l	pmsg8
	dc.l	pmsg9
	dc.l	pmsg10
	dc.l	pmsg11
	dc.l	pmsg12
	dc.l	pmsg13
	dc.l	pmsg14
	dc.l	pmsg15
	dc.l	pmsg16
	dc.l	pmsg17
	dc.l	pmsg18
	dc.l	pmsg19
	dc.l	pmsg20
	dc.l	pmsg21
	dc.l	pmsg22
	dc.l	pmsg23
	dc.l	pmsg24
	dc.l	pmsg25
	dc.l	pmsg26
	dc.l	pmsg27
pmsg1	dc.b	"Bus error"
	dc.b	0
pmsg2	dc.b	"Address error"
	dc.b	0
pmsg3	dc.b	"Illegal instruction"
	dc.b	0
pmsg4	dc.b	"Division by zero"
	dc.b	0
pmsg5	dc.b	"CHK/CHK2 instruction"
	dc.b	0
pmsg6	dc.b	"cpTRAPcc/TRAPcc/TRAPV instruction"
	dc.b	0
pmsg7	dc.b	"Privilege violation"
	dc.b	0
pmsg8	dc.b	"Trace bit"
	dc.b	0
pmsg9	dc.b	"Line 1010 emulator"
	dc.b	0
pmsg10	dc.b	"Line 1111 emulator"
	dc.b	0
pmsg11	dc.b	"Coprocessor protocol violation"
	dc.b	0
pmsg12	dc.b	"Format error"
	dc.b	0
pmsg13	dc.b	"Uninitialized interrupt vector"
	dc.b	0
pmsg14	dc.b	"Spurious interrupt"
	dc.b	0
pmsg15	dc.b	"Interrupt button"
	dc.b	0
pmsg16	dc.b	"TRAP instruction"
	dc.b	0
pmsg17	dc.b	"FPCP branch or set on unordered condition"
	dc.b	0
pmsg18	dc.b	"FPCP inexact result"
	dc.b	0
pmsg19	dc.b	"FPCP divide by zero"
	dc.b	0
pmsg20	dc.b	"FPCP underflow"
	dc.b	0
pmsg21	dc.b	"FPCP operand error"
	dc.b	0
pmsg22	dc.b	"FPCP overflow"
	dc.b	0
pmsg23	dc.b	"FPCP signaling NAN"
	dc.b	0
pmsg24	dc.b	"PMMU configuration"
	dc.b	0
pmsg25	dc.b	"PMMU illegal operation"
	dc.b	0
pmsg26	dc.b	"PMMU access level violation"
	dc.b	0
pmsg27	dc.b	"Unknown device"
	dc.b	0

; Stack used by kernel at startup and to handle panic messages.

kernlim	ds.w	2000			; decimal words in kernel stack
kernstk	ds.l	1			; Otherwise kernstk overlaps
					; next symbol
