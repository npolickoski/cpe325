/*------------------------------------------------------------------------------
 * Initial Build::
 * File:        lab8_main.c
 * Description: a chat bot that responds to the user and prompts their age
 * Input:       user ASCII character input on terminal
 * Output:      "bot" ASCII charcter output in terminal
 * Author(s):   Polickoski, Nick
 * Date:        October 11, 2023
 *----------------------------------------------------------------------------*/

// Preprocessor Directives
#include <msp430.h>
#include <stdio.h>


// Global Variables
const char userTitle[] = "\e[31mMe: \e[39m";              // User title: red
const char botTitle[] = "\e[96mGlados: \e[39m";              // Bot title: cyan
const char lineReset[] = "\r\n";                          // line reset
int secCount = 0;                                   // number of seconds that have passed


// Function Prototypes
void UART_initialize(void);
void UART_sendCharacter(char c);
char UART_getCharacter();
void UART_sendString(char* string);
void UART_getLine(char* buffer, int limit);



// Call To Main
void main(void)
{
    // Watchdog Timer/UART Initialization
    WDTCTL = WDT_ADLY_1000;     // set watchdog timer to go off every 1s (WDTCTL = WDTPW + WDTHOLD;)
    UART_initialize();


    // Enable Interrupts
    _EINT();                                            // enable global interrupts
    SFRIE1 |= WDTIE;                                    // enable watchdog timer interrupts


    for (;;)
    {
        // Initialize String
        char greeting[] = "Hey, Bot!";
        char buffer[sizeof(greeting)];                  // buffer string received from terminal


        // Correct Greeting Confirmation
        UART_sendString(userTitle);
        UART_getLine(buffer, sizeof(greeting));         // receiving buffer text
        secCount = 0;                                   // reset sec counter to 0

        if (!strcmp(greeting, buffer))                  // if strings aren't the same -> end interrupt
        {

            // Age Prompting
            UART_sendString(lineReset);
            UART_sendString(botTitle);
            UART_sendString("Hi! How old are you?");    // greeting and age prompt

            char age[5];                                // age string
            UART_sendString(lineReset);
            UART_sendString(userTitle);
            UART_getLine(age, 5);                       // age string retrieval
            secCount = 0;                               // reset sec counter to 0

            if (!strcmp(age, "1000"))                   // if (age == '1000')
            {
                UART_sendString(lineReset);
                UART_sendString(botTitle);
                UART_sendString("This cannot be true!");
                //UART_sendString(lineReset);
            }
            else
            {
                UART_sendString(lineReset);
                UART_sendString(botTitle);
                UART_sendString("You are so young! I am 1");
                UART_sendString(age);
                UART_sendString(" years old!");
                //UART_sendString(lineReset);
            }
        }

        UART_sendString(lineReset);
    }

    return;
}



//// Interrupt Definitions
// Chat Bot - Timer 15s
#pragma vector = WDT_VECTOR
__interrupt void TimerISR(void)
{
    SFRIFG1 &= ~WDTIFG;                                 // Clear the flag

    secCount++;                                         // increase counter by 1 sec

    if (secCount > 15)                                  // if no communication for over 15 sec
    {
        UART_sendString(lineReset);                     // impatient bastard output
        UART_sendString(botTitle);
        UART_sendString("Is anybody here?");

        UART_sendString(lineReset);                     // prompting the user again
        UART_sendString(userTitle);

        secCount = 0;                                   // reset seconds counter to count back up to 15
    }

    return;                                             // leave interrupt
}



// Function Definitions
void UART_initialize(void)
{
    UCA0CTL1 |= UCSWRST;                                // Set software reset during initialization
    P3SEL |= BIT3 | BIT4;                               // Set UC0TXD and UC0RXD to transmit and receive
    UCA0CTL1 |= UCSSEL_2;                               // Clock source SMCLK

    UCA0BR0 = 54;                                       // 1048576 Hz / 19200 = 54 | 5
    UCA0BR1 = 0;
    UCA0MCTL = 0x0A;                                    // Modulation

    UCA0CTL1 &= ~UCSWRST;                               // Clear software reset

    return;
}


void UART_sendCharacter(char c)
{
    while (!(UCA0IFG & UCTXIFG));                         // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;                                      // Put character into tx buffer

    return;
}


char UART_getCharacter()
{
    while (!(UCA0IFG&UCRXIFG));                         // Wait for a new character
    // New character is here in UCA0RXBUF
    while (!(UCA0IFG&UCTXIFG));                         // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                              // TXBUF <= RXBUF (echo)

    return UCA0RXBUF;
}


void UART_sendString(char* string)
{
    int i;
    for (i = 0; string[i] != 0; i++)                    // iterates through all characters in string and sends em
    {
        UART_sendCharacter(string[i]);
    }

    return;
}


void UART_getLine(char* buffer, int limit)
{
    char c;                                             // temp

    int i;
    for (i = 0; i < (limit - 1); i++)                   // break #1: limit reached (1 less than limit to allow space for NULL)
    {
        c = UART_getCharacter();                        // retrieve character
        if (c == '\r')                                  // break #2: carriage return reached
        {
            break;
        }

        buffer[i] = c;                                  // adds characters to buffer
    }

    buffer[i] = 0;                                      // terminate with NULL character

    return;
}

