/*------------------------------------------------------------------------------
 * Initial Build::
 * File:        lab7_p2.c
 * Description:     The on-board buzzer goes off every other second using
 *              the watch dog timer
 *
 * Input:       Watch Dog Timer going off every 1 second
 * Output:      1KHz Buzz from the buzzer
 * Author(s):   Polickoski, Nick
 * Date:        October 01, 2023
 *----------------------------------------------------------------------------*/

//// Preprocessor Directives
// Libraries
#include <msp430.h>
#include <stdio.h>



// Call To Main
void main(void)
{
    // Watchdog Timer Setup
    WDTCTL = WDT_ADLY_1000;     // set watchdog timer to go off every 1s

    // Enable Interrupts
    _EINT();                    // enable global interrupts
    IE1 |= WDTIE;               // enable watchdog timer interrupts

    // Buzzer Setup
    P3DIR |= BIT5;              // set buzzer to output (P3.5)
    P3SEL |= BIT5;              // set buzzer to have special functionality

    // Timer B Setup
    TB0CTL |= TBSSEL_1;         // set ACLK to source (2^15Hz cc)
    TBCCTL4 |= OUTMOD_4;        // set output mode to toggle
    TB0CCR0 |= 16;              // (1KHz) ~= 2^10 = ACLK/(2*CCR0) = (2^15)/(2*2^4) -> CCR0 = 2^4 = 16
    TB0CTL |= MC_1;             // set Timer B to UP mode

    // Microcontroller Sleep Mode
    _BIS_SR(LPM0);              // sets just CPU to sleep

    return;
}



// Interrupt Definitions
#pragma vector = WDT_VECTOR
__interrupt void WatchdogTimerISR()
{
    P3SEL ^= BIT5;

    return;
}
