/*------------------------------------------------------------------------------
 * Initial Build::
 * File:        Lab03_P1.c
 * Function:        To introduce the user to using the MSP430 board, using interactions
 *              with different registers, components (switches, LEDs), debouncing
 *              switch input, and cycle timing delay (Hertz)
 *
 * Description:     Program turns on, off, and toggles LED lights depending on the
 *              state of which switches are activated in the MSP430 board
 *
 * Input:       Switches S1 (P2.1) and S2 (P1.1) on MSP430 micro-controller
 * Output:      LEDs: LED1 (P1.0) and LED2 (P4.7) on MSP430 micro-controller
 * Author(s):   Polickoski, Nick
 * Date:        September 2, 2023
 *----------------------------------------------------------------------------*/

//// Preprocessor Directives
// Libraries
#include <msp430.h>
#include <stdio.h>

// Macros
#define LED1 BIT0   // 0x00
#define LED2 BIT7   // 0x80

#define S1 BIT1     // 0x01
#define S2 BIT1     // 0x01



//// Function Prototypes
void BothSwitches();
/* function for when both switches are activated simultaneously
 */
void FirstSwitch();
/* function for when only the first switch is activated
 */
void SecondSwitch();
/* function for when only the second switch is activated
 */



//// Call to Main
int main(void)
{
    /// Watchdog Timer
    WDTCTL = WDTPW + WDTHOLD;


    /// Interfacing
    // Switch #1
    P2DIR &= ~S1;           // telling board to look for INPUT signal from S1 location (P2.1)
    P2REN |= S1;            // resistor for high V for S1 (P2.1)
    P2OUT |= S1;            // proper I/O for S1 (P2.1)

    // Switch #2
    P1DIR &= ~S2;           // telling board to look for INPUT from S1 (P1.1)
    P1REN |= S2;            // resistor for high V for S2 (P1.1)
    P1OUT |= S2;            // proper I/O for S1 (P2.1)

    // LED #1
    P1DIR |= LED1;          // telling board to look for OUTPUT from LED1 (P1.0)
    P1OUT |= LED1;          // LED1 = ON

    // LED #2
    P4DIR |= LED2;          // telling board to look for OUTPUT signal from LED2 location (P4.7)
    P4OUT &= ~LED2;         // LED2 = OFF


    /// Switch Detection
    while (1)
    {
        // S1 & S2 activation
        if ( ((P2IN & S1) == 0) && ((P1IN & S2) == 0) )         // logical AND, not bitwise
        {
            BothSwitches();                                     // Function Call: BothSwitches()
        }
        // If ONLY S1 is activated
        else if ( ((P2IN & S1) == 0) && ((P1IN & S2) != 0) )
        {
            FirstSwitch();                                      // Function Call: FirstSwitch()
        }
        // If ONLY S2 is activated
        else if ( ((P2IN & S1) != 0) && ((P1IN & S2) == 0) )
        {
            SecondSwitch();                                     // Function Call: SecondSwitch()
        }
        // Initial Condition if NO switch activation
        else
        {
            P1OUT |= LED1;                                      // LED1 = ON
            P4OUT &= ~LED2;                                     // LED2 = OFF
        }
    }

    return 0;
}



//// Function Definitions
void BothSwitches()
/* upon activation of both switches, both LEDs blink at 5Hz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);                             // ~20ms debounce

    if ( ((P2IN & S1) == 0) && ((P1IN & S2) == 0) )         // check again if switch 1 still activated between debouncing
    {
        P1OUT |= LED1;                                      // initally turn both on so both in synch
        P4OUT |= LED2;                                      //

        while ( ((P2IN & S1) == 0) && ((P1IN & S2) == 0) )
        {
            unsigned int j;
            for (j = 0; j < 10000; j++);                    // 5Hz LED blinking (5Hz -> T = 1/(5) * 1/2 = 1/10s per toggle)

            P1OUT ^= LED1;                                  // LED1 blinking toggle
            P4OUT ^= LED2;                                  // LED2 blinking toggle
        }
    }

    return;
}


void FirstSwitch()
/* upon activation of only the first switch, LED1 turns off and LED2 blinks at 7Hz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);                             // ~20ms debounce

    if ( ((P2IN & S1) == 0) && ((P1IN & S2) != 0) )         // check again if switch 1 still activated between debouncing
    {
        P1OUT &= ~LED1;                                     // LED1 = OFF

        while ( ((P2IN & S1) == 0) && ((P1IN & S2) != 0) )
        {
            unsigned int j;
            for (j = 0; j < 7412; j++);                     // 7Hz LED2 blinking (7Hz -> T = 1/(7) * 1/2 = 1/14s per toggle)

            P4OUT ^= LED2;                                  // LED2 blinking toggle
        }
    }

    return;
}


void SecondSwitch()
/* upon activation of only the second switch, LED2 turns on and LED1 blinks at 2Hz
 */
{
    unsigned int i;
    for (i = 2000; i > 0; i--);                             // ~20ms debounce

    if ( ((P2IN & S1) != 0) && ((P1IN & S2) == 0) )         // check again if switch 2 still activated between debouncing
    {
        P4OUT|= LED2;                                       // LED2 = ON

        while ( ((P2IN & S1) != 0) && ((P1IN & S2) == 0) )
        {
            unsigned int j;
            for (j = 0; j < 25000; j++);                    // 2Hz LED1 blinking (2Hz -> T = 1/(2) * 1/2 = 1/4s per toggle)

            P1OUT ^= LED1;                                  // LED1 blinking toggle
        }
    }

    return;
}

