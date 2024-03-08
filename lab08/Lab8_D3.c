/*--------------------------------------------------------------------------------
 * File:          Lab8_D3.c
 * Function:      Displays real-time clock in serial communication client.
 * Description:   This program maintains real-time clock and sends time
 *                (10 times a second) to the workstation through
 *                a serial asynchronous link (UART).
 *                The time is displayed as follows: "sssss:tsec".
 *
 *                Baud rate divider with 1048576hz = 1048576/19200 = ~54
 * Clocks:        ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 1048576Hz
 * Instructions:  Set the following parameters in putty/hyperterminal
 * Port: COM1
 * Baud rate: 19200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1
 * Flow Control: None
 *
 *        MSP430xG461x
 *     -----------------
 * /|\ |            XIN|-
 *  |  |               | 32kHz
 *  |--|RST        XOUT|-
 *     |               |
 *     |   P2.4/UCA0TXD|------------>
 *     |               | 19200 - 8N1
 *     |   P2.5/UCA0RXD|<------------
 *     |           P5.1|----> LED4
 *
 * Author:      A. Milenkovic, milenkovic@computer.org
 * Date:        October 2018
--------------------------------------------------------------------------------*/
#include <msp430xG46x.h>
#include <stdio.h>

// Current time variables
unsigned int sec = 0;              // Seconds
unsigned int tsec = 0;             // 1/10 second
char Time[8];                      // String to keep current time

void UART_setup(void) {
    UCA0CTL1 |= UCSWRST;           // Set software reset during initialization
    P2SEL |= BIT4 | BIT5;          // Set UC0TXD and UC0RXD to transmit and receive
    UCA0CTL1 |= UCSSEL_2;          // Clock source SMCLK
    UCA0BR0 = 54;                  // 1048576 Hz / 19200 = 54 | 5
    UCA0BR1 = 0;
    UCA0MCTL = 0x0A;               // Modulation
    UCA0CTL1 &= ~UCSWRST;          // Clear software reset
}

void TimerA_setup(void) {
    TACTL = TASSEL_2 + MC_1 + ID_3; // Select SMCLK/8 and up mode
    TACCR0 = 13107;                 // 100ms interval
    TACCTL0 = CCIE;                 // Capture/compare interrupt enable
}

void UART_putCharacter(char c) {
    while (!(IFG2 & UCA0TXIFG));    // Wait for previous character to transmit
    UCA0TXBUF = c;                  // Put character into tx buffer
}

void SetTime(void) {
    tsec++;
    if (tsec == 10){
        tsec = 0;
        sec++;
        P5OUT ^= BIT1;              // Toggle LED4
    }
}

void SendTime(void) {
    int i;
    sprintf(Time, "%05d:%01d", sec, tsec);// Prints time to a string

    for (i = 0; i < sizeof(Time); i++) {  // Send character by character
        UART_putCharacter(Time[i]);
    }
    UART_putCharacter('\r');        // Carriage Return
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer
    UART_setup();                   // Initialize UART
    TimerA_setup();                 // Initialize Timer_B
    P5DIR |= BIT1;                  // P5.1 is output;

    while (1) {
        _BIS_SR(LPM0_bits + GIE);   // Enter LPM0 w/ interrupts
        SendTime();                 // Send Time to HyperTerminal/putty
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void TIMERA_ISA(void) {
    SetTime();                       // Update time
    _BIC_SR_IRQ(LPM0_bits);          // Clear LPM0 bits from 0(SR)
}
