;------------------------------------------------------------------------------
; File:        	
; Function:		
;
; Description:		
;
; Input:		
; Output:		
; Author(s):   	
; Date:        	
;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

    		.ref	NumMulti				; software linear formula
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
;-------------------------------------------------------------------------------
			.data
num         .cstring    "12345"             ; number to multiply
result      .space      2                   ; save two bytes of memory for result
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			push	#num					; base address of number string
            push    #result                 ; area in memory of result number
			
funcNumMulti:
			call	#NumMulti				; function call: NumMulti
			mov.w   #0x4400,	SP			; pop SP back to beginnning

end:
            nop								; for debugger

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
            
