/*----------------------------------------------------------------------------------------
* File:             Lab_9_Source_Code.c
* Description:      Program uses UART and SPI to communicate with the PC and the 2013
*                   board respectively. It converts user input into a workable format 
*                   for the 2013, and it sends legible information back to the PC. This
*                   program performs error handling, and shields the 2013 board
*
* Author:           Jonathan Shreve
* Lab Section:      08
* Date:             October 16, 2023
*----------------------------------------------------------------------------------------*/

#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TXBUF_Free (IFG2 & UCA0TXIFG)
#define Char_Arrived (IFG2 & UCA0RXIFG)




//// Function Prototypes

void UART_Initialize();

void UART_Send_Char(char c);

char UART_Get_Char();

void UART_Send_String(char* string);

void UART_Get_Line(char* buffer, int limit);

void SPI_Setup();

unsigned char SPI_Get_State();

void SPI_Set_State(unsigned char State);



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	            // Stop watchdog timer

	_EINT();                                // Enable interrupts

	UART_Initialize();

	SPI_Setup();

	// Variable Setup
    char Prompt[] = "Enter duty value (0-100), - to reset blinks, ? for current blinks: ";

    char Self_Entry[3];

    char Blink_Reset[] = "Current blinks reset";

    char Blinks[] = "Current blinks: ";

    int Blink_Total;

    char Blink_Total_String[4];

    char Invalid[] = "Invalid duty cycle entered";

    char Line_Reset[] = "\n\r";

    int Duty_Value;


    for (;;)
    {
        // Get User Input
        UART_Send_String(Prompt);
        UART_Get_Line(Self_Entry, 4);
        UART_Send_String(Line_Reset);

        Duty_Value = atoi(Self_Entry);

        if (!strcmp(Self_Entry, "-"))                   // Special Case 1
        {
            // Reset blinks via SPI
            // Send 200 to 2013
            SPI_Set_State(200);
            UART_Send_String(Blink_Reset);
            UART_Send_String(Line_Reset);
        }

        else if (!strcmp(Self_Entry, "?"))              // Special Case 2
        {
            // Display current blinks via SPI
            // Send 255 to 2013
            Blink_Total = (int) SPI_Get_State();                        // Convert received char to int
            snprintf(Blink_Total_String, 4, "%d", Blink_Total);         // "Convert" int to string
            UART_Send_String(Blinks);
            UART_Send_String(Blink_Total_String);
            UART_Send_String(Line_Reset);
        }

        else if (!Duty_Value)                           // The value is 0
        {
            if (!strcmp(Self_Entry, "0"))               // Check if 0 was actually input
            {
                SPI_Set_State(Duty_Value);
            }

            else                                        // Invalid input caused the 0
            {
                UART_Send_String(Invalid);
                UART_Send_String(Line_Reset);
            }

        }

        else if (Duty_Value)                            // Valid number
        {
            // Send number to 2013
            if (Duty_Value > 0 && Duty_Value <= 100)
            {
                SPI_Set_State(Duty_Value);
            }

            else
            {
                UART_Send_String(Invalid);
                UART_Send_String(Line_Reset);
            }
        }

    }

	return 0;
}


//// Function Definitions

void UART_Initialize()
{
    UCA0CTL1 |= UCSWRST;            // Set software reset during initialization

    // Pin Setup
    P2SEL |= BIT4;                  // Set UC0RXD to receive
    P2SEL |= BIT5;                  // Set UC0TXD to transmit

    // Baud Rate Setup
    UCA0CTL0 = 0;                   // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;           // Clock source SMCLK
    UCA0BR0 = 18;                   // 1048576 Hz / 57600 = 18 | 1
    UCA0BR1 = 0;
    UCA0MCTL = 0x02;                // Modulation

    UCA0CTL1 &= ~UCSWRST;           // Clear software reset
    return;
}



void UART_Send_Char(char c)
{
    while (!(IFG2&UCA0TXIFG));      // Wait for buffer to be ready
    UCA0TXBUF = c;                  // Load char into TX buffer

    return;
}



char UART_Get_Char()
{
    while (!(IFG2&UCA0RXIFG));      // Wait for char to arrive

    while (!(IFG2&UCA0TXIFG));      // Wait for buffer to be ready

    char Char = UCA0RXBUF;

    UCA0TXBUF = Char;               // Echo

    return Char;
}



void UART_Send_String(char* string)
{
    int i;
    for (i = 0; string[i] != 0; i++)                  // Loop until the end of the string
    {
        UART_Send_Char(string[i]);
    }

    return;
}



void UART_Get_Line(char* buffer, int limit)
{
    char Char;
    int i;

    for (i = 0; i < limit - 1; i++)             // Loop until one less than limit because null char needs to be added
    {
        Char = UART_Get_Char();

        if (Char == '\r')                       // Detect enter key press
        {
            break;
        }

        buffer[i] = Char;
    }

    buffer[i] = 0;                              // Add null char to end of string

    return;
}



void SPI_Setup()
{
    UCB0CTL0 = UCMSB + UCMST + UCSYNC;          // Sync. mode, 3-pin SPI, Master mode, 8-bit data
    UCB0CTL1 = UCSSEL_2 + UCSWRST;              // SMCLK and Software reset

    UCB0BR0 = 0x02;                             // Data rate = SMCLK/2 ~= 500kHz
    UCB0BR1 = 0x00;

    P3SEL |= BIT1 + BIT2 + BIT3;                // P3.1,P3.2,P3.3 option select

    UCB0CTL1 &= ~UCSWRST;                       // **Initialize USCI state machine**

    return;
}



unsigned char SPI_Get_State()
{
    while (P3IN & 0x01);                        // Verifies busy flag

    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = 255;                            // Dummy write to start SPI

    while (!(IFG2 & UCB0RXIFG));                // USCI_B0 TX buffer ready?

    return UCB0RXBUF;
}



void SPI_Set_State(unsigned char State)
{
    while (P3IN & 0x01);                        // Verifies busy flag

    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = State;                          // Write new state

    while (!(IFG2 & UCB0RXIFG));                // USCI_B0 TX buffer ready?

    return;
}
