;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    SW_dot                  ; software dot product formula

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Subroutine
;-------------------------------------------------------------------------------
SW_dot:
    		push	R4						; unknown data -> must be saved to be popped back later
			push 	R5						; unknown data ->
			push 	R6						; unknown data ->
			push 	R7						; unknown data ->
			push 	R8						; unknown data ->
			push	R9						; unknown data ->

			mov.w	20(SP),	R4				; base address of arr1[]
			mov.w	18(SP),	R5				; base address of arr2[]
			clr		R9						; result = 0

mainLoop:									; loop runs through every element of arrIn
			; for (length > 0)
			cmp.w	#0,		16(SP)
			jeq		endMainLoop
			dec.w	16(SP)					; length--

			; Register Value Initalization
			mov.w	@R4+,	R6				; arr1[i]
			mov.w 	@R5+,	R7				; arr2[i]
			clr		R8						; bitNum = 0

bitLoop:									; shift-add alg
; STEP #3:  // for (8 > bitNum) -> 8 = number of bits
			cmp.w	#8,		R8
			jeq		step4					; if bitNum = 8 -> next element in arrIn[]

; STEP #2	// if (0001 & arr2[i] == 1)
			bit.w	#0x01, 	R7
			jz		shift
			add.w	R6,		R9				; result += arr1[i]

shift:
			rla.w	R6						; arr1[i] << 1
			rra.w	R7						; arr2[i] >> 1

			add.b	#1, 	R8				; bitNum++
			jmp 	bitLoop

step4:
			; if (0001 & arr2[i] == 1)
			bit.w	#0x01, 	R7
			jz		endBitLoop
			sub.w	R6,		R9				; result -= arr1[i]

endBitLoop:
			jmp 	mainLoop

endMainLoop:
			mov.w	14(SP),		R4			; memory address of result
			mov.w	R9,			0(R4)		; store result into memory

			pop		R9						; pop unknown info back
			pop		R8						; pop unknown
			pop		R7						; pop unknown
			pop		R6						; pop unknown
			pop		R5						; pop unknown
			pop		R4						; pop unknown

			ret
			.end
