;------------------------------------------------------------------------------
; Initial Build::
; File:        	HW_linear.asm
; Subroutine:	Uses the hardware multiplier to calculute the equation "Y = mX + c"
;
; Description:		Uses the built-in multiplier registers on the MSP430 board to
;				to calculate the 16-bit numbers "m" and iterated value "X" together,
;				then adds the value "C" to the be inputted into memory in the place
;				where the output arry "arrHW[]" rides
;
; Input:		16-bit integer values: arrIn, length, constant, and slope
; Output:		16-bit integer array arrHW
; Author(s):   	Polickoski, Nick
; Date:        	September 17, 2023
;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    HW_linear               ; software linear formula

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Subroutine
;-------------------------------------------------------------------------------
HW_linear:
			push 	R4						; unknown data -> must be saved to be popped back later
			push	R5						; unknown data ->

			mov.w	14(SP),	R4				; base address of arrIn[]
			mov.w	6(SP),	R5				; base address of arrHW[]

mainLoop:									; loop runs through every element of arrIn
			; for (length > 0)
			cmp.w	#0,		12(SP)
			jeq		endMainLoop

Multi:
			mov.w	10(SP),	&MPYS			; initialize 'm'
			mov.w	@R4+,	&OP2			; inialize 'x' <- arrIn[i]

			dec.w	12(SP)					; length-- (done here to use up a clock cycle - more efficient
			nop

AddConst:
			add.w	8(SP),	RESLO			; arrSW[i] + C
			mov.w	RESLO,	0(R5)			; write arrHW[] to memory
			incd.w	R5						; iterate to next element in arrHW[]
			jmp 	mainLoop

endMainLoop:
			pop		R5						; pop unknown info back
			pop		R4						; pop unknown

			ret								; return to main
			.end
