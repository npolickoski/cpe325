;------------------------------------------------------------------------------
; Initial Build::
; File:        	lab5_main.asm
; Function:		To demonstrate use of the stack to use subroutines in a given program
;
; Description:		Initializes an input array and two output arrays, as well as
;				array length, offset, and slope constants to calculate the
;				equation "Y = mX + c" using two different methods, which get
;				passed to using two subroutines
;
; Input:		Hardcoded 16-bit integer values: arrIn, length, constant, and slope
; Output:		Two 16-bit integer arrays: arrSW and arrHW
; Author(s):   	Polickoski, Nick
; Date:        	September 17, 2023
;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

    		.ref	SW_linear				; software linear formula
    		.ref	HW_linear				; hardware linear formula
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
;-------------------------------------------------------------------------------
			.data
arrIn		.int	1, 2, -3, 4, 0, 6, -7, 8; input array of 8 int elements
length		.int	8						; length
slope		.int	3						; m
constant	.int	4						; C
arrSW		.space	16						; output array for software calculation
arrHW		.space	16						; output array for hardware calculation
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			push	#arrIn					; base address of arrIn
			push	length					; length
			push	slope					; m
			push	constant				; C

funcSWLinear:
			push	#arrSW					; base address of arrSW
			call	#SW_linear				; function call: SW_linear
			mov.w   #0x4400,	SP			; pop base address arrSW[], C, m, length, and base address arrIn[]


			push	#arrIn					; base address of arrIn
			push	length					; length
			push	slope					; m
			push	constant				; C

funcHWlinear:
			push	#arrHW					; base address of arrHW
			call	#HW_linear				; function call: HW_linear
			mov.w   #0x4400,	SP			; pop base address arrHW[], C, m, length, and base address arrIn[]

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
            
