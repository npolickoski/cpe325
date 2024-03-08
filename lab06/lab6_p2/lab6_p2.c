/*------------------------------------------------------------------------------
 * Initial Build::
 * File:        lab6_p2.c
 * Description:
 *
 *
 * Input:
 * Output:
 * Author(s):   Polickoski, Nick
 * Date:        September 25, 2023
 *----------------------------------------------------------------------------*/

//// Preprocessor Directives
// Libraries
#include <msp430.h>
#include <stdio.h>

// Macros
#define SW1 (P2IN & BIT1)
#define SW2 (P1IN & BIT1)



//// Function Prototypes
void BothSwitches();
/* activation of both switches -> frequency of clock set to 8MHz
 */
void FirstSwitch();
/* activation of first switch -> frequency of clock set to 3MHz
 */
void SecondSwitch();
/* activation of second switch -> frequency of clock reset to 1MHz
 */



//// Call to Main
void main(void)
{
    // Stop Watchdog Timer
    WDTCTL = WDTPW + WDTHOLD;


    // Initialize Switches
    P2DIR &= ~0x02;                     // set P2.1 as input (switch #1)
    P2REN |=  0x02;                     // enable P2.1 resistor
    P2OUT |=  0x02;                     // setup proper I/O

    P1DIR &= ~0x02;                     // set P1.1 as input (switch #2)
    P1REN |=  0x02;                     // enable P1.1 resistor
    P1OUT |=  0x02;                     // setup proper I/O


    // Initialize LEDs
    P1DIR |= 0x01;                      // set P1.0 as output
    P1OUT &= ~0x01;                     // LED1 = off

    P4DIR |= 0x80;                      // set P4.7 as output
    P4OUT |= 0x80;                      // LED2 = on


    // Initialize Clocks
    UCSCTL3 = SELREF_2;                 // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                  // Set ACLK = REFO
    UCSCTL0 = 0x0000;                   // Set lowest possible DCOx, MODx


    // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;              // Clear fault flags

    } while (SFRIFG1 & OFIFG);          // Test oscillator fault flag


    for (;;)
    {
        if (!SW1 && !SW2)               // Set Master Clock to 8MHz
        {
            BothSwitches();
        }
        else if (!SW1 && SW2)           // Set Master Clock to 3MHz
        {
            FirstSwitch();
        }
        else if (SW1 && !SW2)           // Reset Master Clock to 1MHz
        {
            SecondSwitch();
        }

        long int i;
        for (i = 0; i < 50000; i++);    // delay ~0.5s

        P1OUT ^= 0x01;                  // toggle LED1
        P4OUT ^= 0x80;                  // toggle LED2
    }
}



//// Function Definitions
void BothSwitches()
/* activation of both switches -> frequency of clock set to 8MHz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);         // ~20ms debounce

    if (!SW1 && !SW2)
    {
        __bis_SR_register(SCG0);        // disable FLL control loop

        UCSCTL1 = DCORSEL_6;            // select frequency range
        UCSCTL2 = 255;                  // [(8Mhz) / 32768] - 1 = 255

        __bic_SR_register(SCG0);        // enable FLL control loop

        __delay_cycles(8 * 32768);      // DCO settling time

    }
}


void FirstSwitch()
/* activation of first switch -> frequency of clock set to 3MHz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);         // ~20ms debounce

    if (!SW1 && SW2)
    {
        __bis_SR_register(SCG0);        // disable FLL control loop

        UCSCTL1 = DCORSEL_5;            // select the frequency range
        UCSCTL2 = 95;                   // [(3Mhz) / 32768] - 1 = 95

        __bic_SR_register(SCG0);        // enable FLL control loop

        __delay_cycles(3 * 32768);      // DCO settling time
    }
}


void SecondSwitch()
/* activation of second switch -> frequency of clock reset to 1MHz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);         // ~20ms debounce

    if (SW1 && !SW2)
    {
        __bis_SR_register(SCG0);        // disable FLL control loop

        UCSCTL1 = DCORSEL_3;            // select frequency range
        UCSCTL2 = 31;                   // [(1Mhz) / 32768] - 1 = 31

        __bic_SR_register(SCG0);        // enable FLL control loop

        __delay_cycles(1 * 32768);      // DCO settling time
    }
}

