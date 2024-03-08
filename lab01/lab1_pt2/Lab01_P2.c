/*------------------------------------------------------------------------------------------------
 * Initial Build::
 * File:        Lab01_S2.c
 * Description: This program finds the prime factorization of a hardcoded value
 *              
 * Board:       5529
 * Input:       Hardcoded short int number
 * Output:      Factors of input value printed to console (ordered low to high)
 * Author:      Douglas Marr
 * Date:        August 16, 2022
 *
 * Bug Fixes::
 * Author:      Polickoski, Nick
 * Date:        August 16, 2023
 * Fixes made:  Syntax Errors
 *              - uncommented a commented variable (line 42)
 *              - added a semicolon (line 71)
 *              - added missing end bracket (line 115)
 *
 *               Logical Errors
 *              - While loop condition "!true" is invalid for running a while loop, so
 *                  change was made to "true" (line 50)
 *              - While loop condition "factor <= value/2" skipped the last value of
 *                  the factorization, so change was made to "factor <= value" (line 56)
 *              - Conditional statement "value % factor == 1" finds the number one
 *                  above the factor of a value, changed to "value % factor == 0" (line 58)
 *              - Missing code for watchdog timer added (line 84)
 *
 *               Readability
 *              - moved loop and condition brackets around
 *              - moved trailing comments to be on the same line, section title comments
 *                  remain in the same position
 *              - added trailing comments for logical and syntactical errors
 *-----------------------------------------------------------------------------------------------*/

#include <msp430.h>
#include <stdio.h>

#define     true    1
#define     false   0


// This function finds all of the factors in `value`
// .. Factors of `value` are output as elements of `factors`
// .. Function return value is number of factors found (0xFFFF for error)
int get_prime_factors(int *factors, long int value)
{
    int output_factor_num = 0;                                                      // SYNTAX ERROR: variable commented out
    int factor;
    char factor_found;
    
    if (value < 0)
        return 0xFFFF;
    
    // Loop while remaining value is not prime
    while (true)                                                                    // LOGICAL ERROR: "!true = false" loop would never run
    {
        factor_found = false;
        factor = 2;
        
        // Get lowest remaining factor of `value`
        while (factor <= value)                                                     // LOGICAL ERROR: "factor <= value/2" skips the last value of the factorization
        {
            if (value % factor == 0) // Is `factor` a factor of `value`?            // LOGICAL ERROR: "value % factor == 1" doesn't find a factor of value
            {  
                factor_found = true; // Factor found
                break;
            }
            
            factor++;                // Factor not found
        }
        
        if (!factor_found)
            break;
        
        // Add factor to array, and divide out of working value
        factors[output_factor_num] = factor;                                        // SYNTAX ERROR: missing semicolon
        output_factor_num++;
        value /= factor;
    }
    
    return output_factor_num;
}


int main(void)
{
	// stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;                                                       // LOGICAL ERROR: no code for watchdog timer implementation
    
    // Input value for factorization
    const long int INPUT_VALUE = 84;
    
    // Output array
    // Array must have at least as many elements as factors of INPUT_VALUE
    int prime_factors[10] = {};
    
    int num_factors;
    int i;
    
    // Calculate prime factors, and check for function error
    num_factors = get_prime_factors(prime_factors, INPUT_VALUE);
    if (num_factors == 0xFFFF)
    {
        return 1;
    }
    
    // Print input value & output prime factors separated by spaces
    printf("Input value:\n %d \n", INPUT_VALUE);
    printf("Output factors: \n");
    for (i = 0; i < num_factors; i++)
    {
        printf("%d ", prime_factors[i]);
    }
    
    printf("\n");
    fflush(stdout);
    
	return 0;
}                                                                                   // SYNTAX ERROR: missing end bracket

