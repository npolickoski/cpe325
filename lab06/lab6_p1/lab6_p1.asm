;------------------------------------------------------------------------------
; Initial Build::
; File:        	lab6_p1.asm
; Description:
;
;
; Input:
; Output:
; Author(s):   	Polickoski, Nick
; Date:        	September 24, 2023
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .def	SW1_ISR					; SW1 interrupt
            .def	SW2_ISR					; SW2 interrupt
;-------------------------------------------------------------------------------
			.data

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
			;; Switch Interfacing
			; Switch #2
			bic.b	#0x02,	&P1DIR			; pin P1.1 = intput  				(P1DIR &= ~0x02)
			bis.b	#0x02,	&P1REN			; enable resister for switch #2 	(P1REN |= 0x02)
			bis.b	#0x02,	&P1OUT			; proper I/O setup					(P1OUT |= 0x02)

			; Switch #1
			bic.b	#0x02,	&P2DIR			; pin P2.1 = intput  				(P2DIR &= ~0x02)
			bis.b	#0x02,	&P2REN			; enable resister for switch #1 	(P2REN |= 0x02)
			bis.b   #0x02,	&P2OUT			; proper I/O setup					(P2OUT |= 0x02)


			; LED Interfacing and Intitial Conditions
			bis.b	#0x01,	&P1DIR			; pin P1.0 = output LED1
			bic.b	#0x01,	&P1OUT			; LED1 = off

			bis.b	#0x80,	&P4DIR			; pin P4.7 = output LED2
			bic.b	#0x80,	&P4OUT			; LED2 = off


			;; Interrupts Setup
			nop								; required
			bis.w 	#GIE, 	SR 				; Enable Global Interrupts
			nop								; required

			; Switch #2
 			bis.b 	#0x02, 	&P1IE 			; enable port P1.1 interrupt
			bis.b 	#0x02, 	&P1IES 			; Set interrupt P1.1 to call from hi to low
			bic.b 	#0x02, 	&P1IFG 			; Clear interrupt P1.1 flag

			; Switch #1
			bis.b 	#0x02, 	&P2IE 			; enable port P2.1 interrupt
			bis.b 	#0x02, 	&P2IES 			; Set interrupt P2.1 to call from hi to low
			bic.b 	#0x02, 	&P2IFG 			; Clear interrupt P2.1 flag


endingLoop:
			jmp		$
			nop								; for debugger

;-------------------------------------------------------------------------------
; Interrupts
;-------------------------------------------------------------------------------
SW1_ISR:
			bic.b 	#0x02, 	&P2IFG 			; Clear interrupt P2.1 flag within iterrupt


; 1st Check for Rising/Falling Edge Status
;	- needed bc switch #1 acts as a button (pulse signal) vs a lever (continuous signal)
;	- different outputs for falling edge and rising edge of button press

			; if (P2IES == 0)       		; check if switch press on rising or falling edge
			bit.b	#0x02,	&P2IES			; 0 = rising, 1 = falling
			jz		SW1CheckRising1			; if on rising edge

			; if on falling edge
			bit.b	#0x02,	&P2IN			; check for switch #1 press
			jnz		endSW1					; return to main if NOT pressed
			jmp		debounceSW1

SW1CheckRising1:
			; if on rising edge
			bit.b	#0x02,	&P2IN			; check for switch #1 release
			jz		endSW1					; return to main if pressed


; Debouncing Initialization and Delay Loop
debounceSW1:
			mov.w	#5000,	R4				; debounce decrementor (iter)
											; 5000 decrementor * 4cc total delay = 20000 -> ~20ms
SW1dLoop:
			nop								; nop (1cc delay)
			dec.w	R4						; iter-- (1cc delay)
			jnz		SW1dLoop				; continue loop until 20ms have passed (2cc delay)
											; ----------------------------
											; = total delay per loop iteration = 4cc


; 2nd Check for Rising/Falling Edge Status
			; if (P2IES == 0)       		; check if switch press on rising or falling edge
			bit.b	#0x02,	&P2IES
			jz		SW1checkRising2			; if on rising edge

			; if on falling edge
			bit.b	#0x02,	&P2IN			; check for switch #1 press
			jnz		endSW1					; return to main if NOT pressed
			jmp		toggleSW1				; jump to switch output

SW1checkRising2:
			; if on rising edge
			bit.b	#0x02,	&P2IN			; check for switch #1 release
			jz		endSW1					; return to main if pressed


; Actual Switch #1 Output
toggleSW1:
			xor.b	#0x01, 	&P1OUT			; toggle LED1
			xor.b	#0x02,	&P2IES			; toggle edge selection


; Exit SW2 Interrupt - Return to Main
endSW1:
			reti							; return to main

;-------------------------------------------------------------------------------
SW2_ISR:
			bic.b 	#0x02, 	&P1IFG 			; Clear interrupt P1.1 flag within iterrupt


; 1st Check for Button Press
;	- needed bc switch #2 acts as a lever (continuous singal) vs a button (pulse signal)

			; if (P1IN & 0x02 == 0)       	; check if switch is pressed
			bit.b	#0x02,	&P1IN
			jnz		endSW2					; if released, return to main


; Debouncing Initialization and Delay Loop
debounceSW2:
			mov.w	#5000,	R5				; debounce decrementor (iter)
											; 5000 decrementor * 4cc total delay = 20000 -> ~20ms
SW2dLoop:
			nop								; nop (1cc delay)
			dec.w	R5						; iter-- (1cc delay)
			jnz		SW2dLoop				; continue loop until 20ms have passed (2cc delay)
											; ----------------------------
											; = total delay per loop iteration = 4cc


; 2nd Check for Rising/Falling Edge Status
			; if (P1IN & 0x02 == 0)       	; check if switch is pressed
			bit.b	#0x02,	&P1IN
			jnz		endSW2					; if released, return to main


; Actual Switch #2 Output
			xor.b	#0x80, 	&P4OUT			; toggle LED2


; Exit SW2 Interrupt - Return to Main
endSW2:
			reti							; return to main

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
            .sect	".int42"				; P2.X Vector
            .short	SW1_ISR
            .sect	".int47"				; P1.X Vector
            .short	SW2_ISR
            .end


