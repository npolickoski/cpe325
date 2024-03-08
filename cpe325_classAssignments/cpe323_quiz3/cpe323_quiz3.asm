;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                             ; make it known to linker..
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
			mov.w	#arr,		R4
			mov.w	#arrend,	R5
			sub.w	R4,			R5
			rra		R5
			push	R4				; base address of array
			push	R5				; difference between low and high memory addresses of array (length)
			call	#findDelta
			mov.w	#0x4400, 	SP
			jmp		$

arr:		.int	1, 17, -8, 2, 6, 19, 21, 0
arrend:
END


findDelta:
			push	R6				; unknown data
			push	R7				; unknown data

			mov.w	8(SP),		R4	; base address of array
			mov.w	@R4,		R6	; max val initialization - start w array[0]
			mov.w	@R4+,		R7	; min val initialization - start w array[0]

loop:		; while (length > 1)  ->  array[0] is initalized -> comparing to array[1-7]
			cmp.w	#1,		6(SP)
			jeq		endLoop
			dec.w	6(SP)			; length--

maxComp:	; if (max < array[i])
			cmp.w	@R4,	R6
			jn		maxChange
			jmp		minComp			; if (max > array[i]) -> max = max

maxChange:
			mov.w	@R4, 	R6		; else (max < array[i]) -> max = array[i]

minComp:	; if (min > array[i])
			cmp.w	@R4,	R7
			jn		loopBack		; if (min < array[i]) -> min = min
			mov.w	@R4, 	R7		; else (min > array[i]) -> min = array[i]

loopBack:
			incd.w	R4				; increment to next element in array
			jmp		loop

endLoop:
			sub.w	R7, 	R6		; max - min = distance
			mov.w	R6,		R12		; distance -> R12

			pop		R7				; return val. to R7
			pop		R6				; return val. to R6
			pop 	R5				; pop length val back into R5
			pop		R4				; pop base address of array back into R4

			ret						; return flow of control to main



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
            
