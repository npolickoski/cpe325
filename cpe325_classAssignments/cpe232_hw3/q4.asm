;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			MOV 	#arr, 	R4 				;
			MOV 	#aend, 	R5 				;
			SUB 	#arr, 	R5 				; (#aend - #arr1)/2 is the number of elements
			RRA 	R5
			CALL 	#COMPUTEPARITY
			JMP 	$

arr 		.word 	1, 11, 17, 23 ; unsigned array
aend
END

COMPUTEPARITY:
			; while (length > 0)
loop:		cmp.w	#0,			R5
			jeq		endLoop
			dec		R5						; length--

			clr		R6						; bitIter
			clr		R8						; 1counter
			mov.w	@R4,		R7			; array[i]

			; for (i < 14)					; loop through first 15 bits
bitLoop		cmp		#14,		R6
			jeq		next
			inc.w	R6						; bitIter++

			bit.w	#0x0001,	R7			; checks for LSB = 1
			jz		noCount
			inc.w	R8						; 1counter++

noCount:	rra		R7						; move next LSB right
			jmp		bitLoop


next:		bit.w	#0x0001,	R8			; check for even or odd number
			jnz		contLoop				; if odd number -> do nothing
			bis.w	#0x8000,	0(R4)		; if even number -> add even parity bit

contLoop
			incd.w	R4						; i++
			jmp		loop

endLoop:
			ret
			.end
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
