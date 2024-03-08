;------------------------------------------------------------------------------
; Initial Build::
; File:        	Lab04_P3 - main.asm
; Function:		Take in string and add 1 to any numeric value within it
;
; Description:		Take in string, iterate through each character until numeric
;				character is found: increment by 1 or roll back to 0 if original
;				value is 9
;
; Input:		.cstring of any form
; Output:		N/A (original string memory is changed)
; Author(s):   	Polickoski, Nick
; Date:        	September 8, 2023
;----------------------------------------------------------------------------
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
			.data
myStr		.cstring 	"I enjoy learning msp430"

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
; Call to Main
main		mov.w	#myStr, 	R4			; take in string
			mov.w 	#1,			R6			; byte swap flag (1 = lower byte, 0 = upper byte)


; Check for Numerical Characters
for			mov.b 	@R4, 		R5			; move each new byte section into R5
			cmp.b 	#0, 		R5  		; while (myStr[i] != NULL)
			jeq 	endLoop

			; if (R5 < 0x30)				; num. chars 0-9
			cmp.b 	#30h, 		R5			; ASCII: 0x30 - 0x39
			jl 		byteTest

			; if (R5 >= 0x3A)
			cmp.b 	#0x3A,		R5
			jge		byteTest


; Incrementing Numerical Values
			cmp.b	#0x39,		R5			; if increment goes past '9' char.
			jne		increment				; go to next char in string

			mov.b	#0x30,		R5			; roll back to 0: 9 -> 0
			mov.b	R5,			0(R4)
			jmp 	byteTest				; skip increment step

increment	inc.b	R5						; increase numerical value by 1
			mov.b	R5,			0(R4)


; Determing Byte (Upper v. Lower)
byteTest	cmp.w	#0,			R6			; if R6 = 0 = upper byte
			jeq		upperByte

lowerByte	dec.b	R6						; decrements byte flag to 0
			swpb 	0(R4)					; swaps lower/upper bytes
			jmp		for

upperByte	inc.b	R6						; increments byte flag to 1
			swpb 	0(R4)					; swaps lower/upper bytes
			incd	R4						; increment to next word
			jmp 	for


; Checking if NULL in Correct Position
endLoop		cmp		#0,			R6			; if R6 = 0 = upper byte
			jne		done
			swpb	0(R4)					; swap back upper byte


; End of Loop
done		nop								; for debugging
                                            
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
            
