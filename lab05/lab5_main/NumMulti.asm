;------------------------------------------------------------------------------
; Initial Build::
; Sub File:   	SW_linear.asm
; Function:		Uses the Shift-And-Add Algorithm to calculate the equation "Y = mX + c"
;
; Description:		Uses a series of conditonal additions and bit shifting to
;				calculate the the multiplication of "m" and iterated array element
;				value "X", adds "c", and writes the value to the memory location of
;				the output array "arrSW"

; Input:		16-bit integer values: arrIn, length, constant, and slope
; Output:		16-bit integer array arrHW
; Author(s):   	Polickoski, Nick
; Date:        	September 17, 2023
;----------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def     NumMulti               ; software linear formula

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Subroutine
;-------------------------------------------------------------------------------
NumMulti:
            push    R4                      ; save previous memory
            push    R5                      ;
            mov.w   8(SP), R4               ; move address of string into R4
            mov.w   6(SP), R5               ; move address of spot in memory for result

            mov.w   #1, &R5                 ; move a 1 into memory address in R5 (for easy multiplication later)

for:        
            cmp




			ret								; return to main
			.end


