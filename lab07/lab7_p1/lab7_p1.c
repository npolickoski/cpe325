/*------------------------------------------------------------------------------
 * Initial Build::
 * File:        lab7_p1.c
 * Description:
 *
 *
 * Input:
 * Output:
 * Author(s):   Polickoski, Nick
 * Date:        September 30, 2023
 *----------------------------------------------------------------------------*/

//// Preprocessor Directives
// Libraries
#include <msp430.h>
#include <stdio.h>

// Macros
#define SW1 (P2IN & BIT1)
#define SW2 (P1IN & BIT1)



//// Call to Main
void main(void)
{
    // Stop Watchdog Timer
    WDTCTL = WDT_ADLY_1000;         // set watchdog timer to go off every 1s
    //WDTCTL = WDTPW + WDTHOLD;

    // Enable Interrupts
    _EINT();                        // enable global interrupts
    SFRIE1 |= WDTIE;                // enable watchdog timer interrupts


    //// Switch Interfacing
    // Switch #1
    P2DIR &= ~BIT1;                 // set P2.1 as input (switch #1)
    P2REN |=  BIT1;                 // enable P2.1 resistor
    P2OUT |=  BIT1;                 // setup proper I/O

    P2IE  |=  BIT1;                 // enable port P2.1 interrupt
    P2IES |=  BIT1;                 // set interrupt P2.1 to call from high to low
    P2IFG &= ~BIT1;                 // clear interrupt P2.1 flag

    // Switch #2
    P1DIR &= ~BIT1;                 // set P1.1 as input (switch #2)
    P1REN |=  BIT1;                 // enable P1.1 resistor
    P1OUT |=  BIT1;                 // setup proper I/O

    P1IE  |=  BIT1;                 // enable port P1.1 interrupt
    P1IES |=  BIT1;                 // set interrupt P1.1 to call from high to low
    P1IFG &= ~BIT1;                 // clear interrupt P1.1 flag


    // LED1 Interfacing
    P1DIR |= BIT0;                  // set P1.0 as output
    P1OUT &= ~BIT0;                 // initialize LED1 = off (different power levels)


    // Timer A shit
    TA0CTL |= TASSEL_1 | MC_1;      // ACLK is clock source; UP mode
    TA0CCTL0 |= CCIE;               // CCR0 triggers interrupt
    TA0CCTL1 |= CCIE;               // CCR1 triggers interrupt

    TA0CCR0 = 110;                  // Set TA0 (and maximum) count value
    TA0CCR1 = 50;                   // Set TA0 middle count value

    TA0CCTL0 &= ~CCIFG;             // clear CCR0 interrupt flag
    TA0CCTL1 &= ~CCIFG;             // clear CCR1 interrupt flag


    // Microcontroller Enters Sleep Mode
    _BIS_SR(LPM3 + GIE);            // Enter Low Power Mode 3

    return;
}



//// Interrupts Definitions
// Switch #1 (P2.1)
#pragma vector = PORT2_VECTOR
__interrupt void switch1ISR(void)
{
    P2IFG &= ~BIT1;                     // clear interrupt P2.1 flag

    if (!SW1 && SW2)
    {
        unsigned int i;
        for (i = 2000; i > 0; i--);     // ~20ms debounce

        if (!SW1 && SW2)                // 2nd check for switch #1 press
        {
            if (TA0CCR1 < 100)          // if upper bound is met
            {
                TA0CCR1 += 10;          // increase brightness322q
            }
        }
    }

    return;
}


// Switch #2 (P1.1)
#pragma vector = PORT1_VECTOR
__interrupt void switch2ISR(void)
{
    P1IFG &= ~BIT1;                         // clear interrupt P2.1 flag

    if (SW1 && !SW2)
    {
        unsigned int i;
        for (i = 2000; i > 0; i--);     // ~20ms debounce

        if (SW1 && !SW2)                // 2nd check for switch #1 press
        {
            if (TA0CCR1 > 10)           // if lower bound is met
            {
                TA0CCR1 -= 10;          // decrease brightness
            }
        }
    }

    return;
}


// Watch Dog Timer
#pragma vector = WDT_VECTOR
__interrupt void BtimerISR(void)
{
    SFRIFG1 &= ~WDTIFG;                     // Clear the flag

    static int i = 0;                       // Initialize counter for determining wait time

    if (i == 2)                             // i is two -> 2 (s) wait reached | 0 (wait) 1 (wait) 2
    {
        i = 0;                              // Reset counter

        if (!SW1 && !SW2)                   // Check if both switches pressed
        {
            unsigned int i;
            for (i = 2000; i > 0; i--);     // ~20ms debounce

            if (!SW1 && !SW2)               // Check if both still pressed
            {
                TA0CCTL1 ^= CCIE;           // Toggle interrupt enable for LED1 on interrupt
            }
        }
    }

    i++;

    return;
}


// Timer A0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0()
{
    TA0CCTL0 &= ~CCIFG;                 // Clear the flag
    P1OUT |= BIT0;                      // turn on LED1
    return;
}


// Timer A1
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A1()
{
    TA0CCTL1 &= ~CCIFG;                 // Clear the flag
    P1OUT &= ~BIT0;                     // turn off LED1
    return;
}





