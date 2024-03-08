;------------------------------------------------------------------------------
; Initial Build::
; File:        	Lab04_P1 - main.asm
; Function:		Takes in string, count number of non-alphanumeric characters,
;				add all characters that aren't counted to new string, and output
;				characters counted
;
; Description:		Takes in string, for each character in the string, confirms if
;				said character falls within the range of values - if not, gets
;				removed from string and special character counter increases -, any
;				characters that fall within that range get added to the new string
;				and the number of non-alphanumeric characters gets outputted
;
; Input:		.cstring string of characters
; Output:		int number to P4OUT register of non-alphanumeric characters
; Author(s):   	Polickoski, Nick
; Date:        	September 6, 2023
;----------------------------------------------------------------------------*/
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
			.data
myStr		.cstring "Welcome To MSP430 Assembly:)!"
newStr		.space 30
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
; Call to Main
main		mov.w 	#myStr,		R4	; pointer to first 2 bytes in myStr
			mov.w 	#newStr,	R5 	; pointer to first 2 bytes in newStr
			clr 	R6				; setting specCount = 0
			mov.w 	#1,			R8	; byte swap flag (1 = lower byte, 0 = upper byte)


; Conditional For-Loop
for			mov.b 	@R4+, 		R7	; move each new byte section into R7
			cmp.b 	#0, 		R7  ; while (myStr[i] != NULL)
			jeq		loopEnd

			; Conditional Checks for Special Characters (check ASCII charts)
			cmp.b 	#30h, 		R7	; if myStr[i] < 0x30
			jl 		isSpec

			cmp.b 	#7Bh,		R7  ; if myStr[i] >= 0x7B
			jge		isSpec

			cmp.b	#3Ah,		R7	; if myStr[i] < 0x3A
			jl		validChar

			cmp.b	#41h,		R7	; if myStr[i] < 0x41
			jl 		isSpec

			cmp.b 	#5Bh,		R7	; if myStr[i] < 0x5B
			jl		validChar

			cmp.b	#61h, 		R7	; if myStr[i] < 0x61
			jl		isSpec


;Adding Characters to newStr
validChar	cmp.w 	#0,			R8 	; if R8 = 0 = upper byte
			jeq 	upperByte

lowerByte	dec.b	R8				; decrements byte flag to 0
			mov.b	R7,		0(R5)	; add character to newStr
			swpb 	0(R5)			; swaps lower/upper bytes
			jmp		for

upperByte	inc.b	R8				; increments byte flag to 1
			mov.b	R7,		0(R5)	; add character to newStr
			swpb 	0(R5)			; swaps lower/upper bytes
			incd.w	R5				; increment next word of newStr
			jmp		for

isSpec		inc.b	R6				; specCount++
			jmp 	for				; continue to next iteratoon of for-loop


; End of Loop and Printing
loopEnd		mov.b 	R7,		0(R5)	; add NULL character to newStr
			cmp.b	#0,			R8	; if R8 = 0 = upper byte
			jne		output			; if no swap byte swap needs to happen
			swpb 	0(R5)			; swaps lower/upper bytes


output		mov.b	R6,		&P4OUT	; output to P4 pin

                                            
			nop						; for debugger
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
