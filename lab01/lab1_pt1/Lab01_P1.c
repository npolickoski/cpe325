/*----------------------------------------------------------------------------------
 * Initial Build::
 * File:        Lab01_P1.c
 * Description: This program calculates the value of a number raised to a power and
 *                  outputs the value in a print to console statement
 *
 * Board:       5529
 * Input:       Two hardcoded short int numbers
 * Output:      Console print of the two hardcoded numbers as well as the function value
 * Author:      Polickoski, Nick
 * Date:        August 17, 2023
 *----------------------------------------------------------------------------*/

#include <msp430.h>
#include <stdio.h>

// Function Prototypes
long int CalcPower(int, int);           // calculates a number to a power


// Call to Main
int main()
{
    // Variable Declaration
    const int a = 2, p = 3;             // a = base value, p = power

    long int b;                         // b = result
    b = CalcPower(a, p);                // function call

    printf("%d raised to the power %d is %ld\n", a, p, b);

    return 0;
}


// Function Definitions
long int CalcPower(int A, int P)
/* function to calculate number A to the power P
 */
{
    long int total = A;                  // 'total' variable --> to be changed

    for (; P > 1; P--)                   // multiplies number 'A' to changing variable 'total' 'P' times
    {
        total *= A;
    }

    return total;                        // returns total multiplied number
}

