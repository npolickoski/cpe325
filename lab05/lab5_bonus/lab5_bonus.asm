;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

          	.ref	SW_dot					; software dot product formula
          	.ref	HW_dot					; hardware dot product formula
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
			.data
grandma		.int	1, 2, 3, 4, 5, 6, 7, 8	; input array #1
grandpa		.int	1, 2, 3, 4, 5, 6, 7, 8  ; input array #2
length		.int	8						; length of BOTH arrays (vectors)
resultSW	.space 	2						; allocated space for SW result
resultHW	.space	2						; allocated space for HW result
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			push 	#grandma				; base address of array #1
			push 	#grandpa				; base address of array #2
			push 	length					; length of arrays

funcSWdot:
			push 	#resultSW				; memory for resultSW
			call	#SW_dot					; function call: SW_dot
			mov.w   #0x4400,	SP			; pop base addreses of array #1, #2, as well as the result, and length


			push 	#grandma				; base address of array #1
			push 	#grandpa				; base address of array #2
			push 	length					; length of arrays
                                            
funcHWdot:
			push 	#resultHW				; memory for resultHW
			call 	#HW_dot					; function call: HW_dot
			mov.w	#0x4400,	SP			; pop base addreses of array #1, #2, as well as the result, and length

end:
			nop								; for debugging

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
            
