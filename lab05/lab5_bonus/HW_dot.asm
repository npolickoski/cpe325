;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    HW_dot                  ; hardware dot product formula

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Subroutine
;-------------------------------------------------------------------------------
HW_dot:
			push 	R4						; unknown data -> must be saved to be popped back later
			push	R5						; unknown data ->

			mov.w	12(SP),	R4				; base address of arr1[]
			mov.w	10(SP),	R5				; base address of arr2[]
			clr		RESLO					; for proper instantiation of accumulation multiplication

mainLoop:									; loop runs through every element of arrIn
			; for (length > 0)
			cmp.w	#0,		8(SP)
			jeq		endMainLoop

Multi:
			mov.w	@R4+,	&MACS			; opeerand #1: arr1[i]
			mov.w	@R5+,	&OP2			; opeerand #2: arr2[i]

			; Must wait 3cc -> dec = 2cc, jmp = 2cc = 4cc
			dec.w	8(SP)					; length-- (done here to use up a clock cycle - more efficient
			jmp 	mainLoop

endMainLoop:
			mov.w	6(SP),	R4				; address of result
			mov.w	RESLO,	0(R4)			; write result to memory

			pop		R5						; pop unknown info back
			pop		R4						; pop unknown

			ret
			.end
