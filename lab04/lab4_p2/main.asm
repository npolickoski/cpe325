;------------------------------------------------------------------------------
; Initial Build::
; File:        Lab04_P2 - main.asm
; Function: 	To multiply two integers together
;
; Description:	Takes in string, checks for correct form, checks that values are
;				within range of ASCII characters, change to binary values, compute
;				multiplication using for-loop, output product value
;
; Input:		.cstring in the form of "a*b" where 'a' and 'b' are single digit ints
; Output:		int product of 'a' and 'b' outputted to P2OUT
; Author(s):   	Polickoski, Nick
; Date:        	September 7, 2023
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
product		.cstring 	"4*6"				; takes form of "a*b"

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
; Call to Main
main		mov.w	#product,	R4			; take in "product" string
			mov.b	#0xFF,		R9			; initialize total = 0xFF


; Taking/Validating Values from String
			mov.b	@R4,		R5			; takes contents of 'a'

			; if (a < 0x30)					; num. chars 0-9
			cmp.b 	#30h, 		R5			; ASCII: 0x30 - 0x39
			jl 		output

			; if (a >= 0x3A)
			cmp.b 	#0x3A,		R5
			jge		output

			inc.w	R4						; increment to 2nd byte
			mov.b	@R4,		R6			; takes in '*'

			; if (R6 != '*')
			cmp		#0x2A,		R6
			jne		output

			inc.w	R4						; increment to 3rd byte
			mov.b	@R4,		R6			; takes contents of 'b'


			; if (b < 0x30)
			cmp.b 	#30h, 		R6
			jl 		output

			; if (b >= 0x3A)
			cmp.b 	#0x3A,		R6
			jge		output

			inc.w	R4						; increment to 4th byte
			mov.b	@R4,		R7			; takes contents of 'NULL'

			; if (R7 != NULL)
			cmp.b	#0x00,		R7
			jne		output


; ASCII to int Conversion
			sub.b	#30h,		R5			; subtracting 0x30 from ASCII .
			sub.b	#30h,		R6			;		num value = binary rep.
			clr		R9						; total = 0

; Find Smaller Number
			cmp.b	R5,			R6			; compares a and b
			jn		negNum					;

			; if (b > a)
			mov.b	R5, 		R7			; iter = a
			mov.b	R6,			R8			; base = b
			jmp 	Multi

			; else if (a >= b)
negNum		mov.b 	R6,			R7			; iter = b
			mov.b	R5,			R8			; base = a


; Multiplication Loop
Multi		cmp.b	#0h,		R7			; if (iter != 0)
			jeq 	output
			add.b	R8,			R9			; total += base
			dec.b	R7						; iter--
			jmp 	Multi


; Output
output		mov.b	R9,			P2OUT		; move to P2 reg.


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
            
