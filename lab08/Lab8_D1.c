/*----------------------------------------------------------------------------------
 * File:          Lab8_D1.c
 * Function:      Echo a received character, using polling.
 * Description:   This program echos the character received from UART back to UART.
 *                Toggle LED4 with every received character.
 *                Baud rate: low-frequency (UCOS16=0);
 *                1048576/115200 = ~9.1 (0x0009|0x01)
 * Clocks:        ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO
 *
 * Instructions: Set the following parameters in putty
 * Port: COM1
 * Baud rate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1
 * Flow Control: None
 *
 *    MSP430xG461x
 *   -----------------
 * /|\ |            XIN|-
 *  |  |               | 32kHz
 *  |--|RST        XOUT|-
 *     |               |
 *     |   P2.4/UCA0TXD|------------>
 *     |               | 115200 - 8N1
 *     |   P2.5/UCA0RXD|<------------
 *     |           P5.1|----> LED4
 *
 * Input:     None (Type characters in putty/MobaXterm/hyperterminal)
 * Output:    Character echoed at UART
 * Author:    A. Milenkovic, milenkovic@computer.org
 * Date:      October 2018
 *--------------------------------------------------------------------------------*/
#include <msp430xG46x.h>

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;   // Set USCI_A0 RXD/TXD to receive/transmit data
    UCA0CTL1 |= UCSWRST;    // Set software reset during initialization
    UCA0CTL0 = 0;           // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;   // Clock source SMCLK
    UCA0BR0 = 0x09;         // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;         // upper byte
    UCA0MCTL = 0x02;        // Modulation (UCBRS0=0x01, UCOS16=0)
    UCA0CTL1 &= ~UCSWRST;   // Clear software reset to initialize USCI state machine
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait for previous character to transmit
    UCA0TXBUF = c;                // Put character into tx buffer
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;     // Stop WDT
    P5DIR |= BIT1;                // Set P5.1 to be output
    UART_setup();            // Initialize UART

    while (1) {
       while(!(IFG2&UCA0RXIFG)); // Wait for a new character
       // New character is here in UCA0RXBUF
       while(!(IFG2&UCA0TXIFG)); // Wait until TXBUF is free
       UCA0TXBUF = UCA0RXBUF;    // TXBUF <= RXBUF (echo)
       P5OUT ^= BIT1;            // Toggle LED4
    }
}
