;/* lowcore.s - (map of interrupt vectors in low part of memory) */

JMP	equ	$4EF9			; jmp absolute opcode

;*------------------------------------------------------------------------
;*  absolute location 0  --  fixed interrupt and exception vectors
;*------------------------------------------------------------------------

; Vectors with values of zero are left with their original values.

	xdef	_lowcore, _endvec
	xref	panic0
	xref	panic1, panic2, panic3, panic4, panic5
	xref	panic6, panic7, panic8, panic9, panic10
	xref	panic11, panic12, panic13, panic14, panic15
	xref	panic16, panic17, panic18, panic19, panic20
	xref	panic21, panic22, panic23, panic24, panic25
	xref	panic26, panic27

	dseg
_lowcore:				; This is loaded into location 0
	dc.w	JMP
	dc.l	panic0
	dc.w	$FFFF			; Filler to get to location 8
	dc.l	panic1			; Bus error
	dc.l	panic2			; Address error
	dc.l	panic3			; Illegal instruction
	dc.l	panic4			; Division by zero
	dc.l	panic5			; CHK/CHK2 instruction
	dc.l	panic6			; cpTRAPcc/TRAPcc/TRAPV instr.
	dc.l	panic7			; Privelege violation
	dc.l	0			; Trace bit
	dc.l	0			; Line 1010 emulator
	dc.l	panic10			; Line 1111 emulator
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic11			; Coprocessor protocol violation
	dc.l	panic12			; Format error
	dc.l	panic13			; Uninitialized interrupt vector
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic14			; Spurious interrupt
	dc.l	0			; VIA interrupt
	dc.l	0			; SCC interrupt
	dc.l	0			; Simultaneous VIA&SCC interrupts
	dc.l	0			; Pre-Mac II interrupt button
	dc.l	0			; Pre-Mac II interrupt button
	dc.l	0			; Pre-Mac II interrupt button
	dc.l	panic15			; Mac II interrupt button
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic16			; TRAP instruction
	dc.l	panic17			; FPCP br./set on unordered cond.
	dc.l	panic18			; FPCP inexact result
	dc.l	panic19			; FPCP divide by zero
	dc.l	panic20			; FPCP underflow
	dc.l	panic21			; FPCP operand error
	dc.l	panic22			; FPCP overflow
	dc.l	panic23			; FPCP signaling NAN
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic24			; PMMU configuration
	dc.l	panic25			; PMMU illegal operation
	dc.l	panic26			; PMMU access level violation
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
	dc.l	panic27			; Reserved (unknown device)
_endvec:
	end
