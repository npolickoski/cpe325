;------------------------------------------------------------------------------
; Initial Build::
; Sub File:   	SW_linear.asm
; Function:		Uses the Shift-And-Add Algorithm to calculate the equation "Y = mX + c"
;
; Description:		Uses a series of conditonal additions and bit shifting to
;				calculate the the multiplication of "m" and iterated array element
;				value "X", adds "c", and writes the value to the memory location of
;				the output array "arrSW"

; Input:		16-bit integer values: arrIn, length, constant, and slope
; Output:		16-bit integer array arrHW
; Author(s):   	Polickoski, Nick
; Date:        	September 17, 2023
;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    SW_linear               ; software linear formula

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Subroutine
;-------------------------------------------------------------------------------
SW_linear:
			push	R4						; unknown data -> must be saved to be popped back later
			push 	R5						; unknown data ->
			push 	R6						; unknown data ->
			push 	R7						; unknown data ->
			push 	R8						; unknown data ->
			push	R9						; unknown data ->

			mov.w	22(SP),	R4				; base address of arrIn[]
			mov.w	14(SP),	R6				; base address of arrSW[]

mainLoop:									; loop runs through every element of arrIn
			; for (length > 0)
			cmp.w	#0,		20(SP)
			jeq		endMainLoop
			dec.w	20(SP)					; length--

			; Register Value Initalization
			mov.w	@R4+,	R5				; arrIn[i];
			clr 	R7						; arrSW[i] = 0
			clr		R8						; bitNum = 0
			mov.w	18(SP),	R9				; initialize 'm'

bitLoop:									; shift-add alg
; STEP #3:  // for (8 > bitNum) -> 8 = number of bits
			cmp.w	#8,		R8
			jeq		step4					; if bitNum = 8 -> next element in arrIn[]

; STEP #2	// if (0001 & arrIn[i] == 1)
			bit.w	#0x01, 	R5
			jz		shift
			add.w	R9,	R7					; arrSW[i] += m

shift:
			rla.w	R9						; m << 1
			rra.w	R5						; arrIn[i] >> 1

			add.b	#1, 	R8				; bitNum++
			jmp 	bitLoop

step4:
			; if (0001 & arrIn[i] == 1)
			bit.w	#0x01, 	R5
			jz		endBitLoop
			sub.w	R9,	R7					; arrSW[i] -= m

endBitLoop:
			add.w	16(SP),	R7				; arrSW[i] + C
			mov.w	R7,		0(R6)			; write arrSW[] to memory
			incd.w	R6						; iterate to next element in arrSW[]
			jmp 	mainLoop

endMainLoop:
			pop		R9						; pop unknown info back
			pop		R8						; pop unknown
			pop		R7						; pop unknown
			pop		R6						; pop unknown
			pop		R5						; pop unknown
			pop		R4						; pop unknown

			ret								; return to main
			.end


