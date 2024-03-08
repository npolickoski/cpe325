/*----------------------------------------------------------------------------------
 * Initial Build::
 * File:        Lab02_P1.c
 * Description: Prints a comprehensive table of datatype name, size, and ranges use in C/C++
 *
 *
 * Board:       5529
 * Input:       Hardcoded inputs for datatype names, sizeof datatype, minimum and maximum value of datatype
 * Output:      Console print of the above data
 * Author:      Polickoski, Nick
 * Date:        August 28, 2023
 *----------------------------------------------------------------------------*/

//// Preprocessor Directives
#include <msp430.h>
#include <stdio.h>
#include <limits.h> // adds macros for datatype range max/mins
#include <float.h>  // adds macros for “float” and “double” datatype range max/mins


//// Call to Main
int main()
{
    /// Watchdog Timer
    WDTCTL = WDTPW + WDTHOLD;

    /// Header
    printf("-------------------------------------------------------------------------------------------\n");
    printf("| %-22s | %-15s | %-20s | %-20s |\n", "Data Type", "Size (in bytes)", "Minimum", "Maximum");
    printf("-------------------------------------------------------------------------------------------\n");



    /// Datatype Data: Name, Sizeof, Minimum, Maximum
    // char
    printf("| %-22s | %-15d | %-20d | %-20d |\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);

    // short int
    printf("| %-22s | %-15d | %-20d | %-20d |\n", "short int", sizeof(short int), SHRT_MIN, SHRT_MAX);

    // int
    printf("| %-22s | %-15d | %-20d | %-20d |\n", "int", sizeof(int), INT_MIN, INT_MAX);

    // long int
    printf("| %-22s | %-15d | %-20ld | %-20ld |\n", "long int", sizeof(long int), LONG_MIN, LONG_MAX);

    // long long int
    printf("| %-22s | %-15d | %-20lld | %-20lld |\n", "long long int", sizeof(long long int), LLONG_MIN, LLONG_MAX);

    // unsigned char
    printf("| %-22s | %-15d | %-20d | %-20u |\n", "unsigned char", sizeof(unsigned char), 0, UCHAR_MAX);

    // unsigned short int
    printf("| %-22s | %-15d | %-20d | %-20u |\n", "unsigned short int", sizeof(unsigned short int), 0, USHRT_MAX);

    // unsigned int
    printf("| %-22s | %-15d | %-20d | %-20u |\n", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);

    // unsigned long int
    printf("| %-22s | %-15d | %-20d | %-20lu |\n", "unsigned long int", sizeof(unsigned long int), 0, ULONG_MAX);

    // unsigned long long int
    printf("| %-22s | %-15d | %-20d | %-20llu |\n", "unsigned long long int", sizeof(unsigned long long int), 0, ULLONG_MAX);

    // float
    printf("| %-22s | %-15d | %-20e | %-20e |\n", "float", sizeof(float), FLT_MIN, FLT_MAX);
ft
    // double
    printf("| %-22s | %-15d | %-20e | %-20e |\n", "double", sizeof(double), DBL_MIN, DBL_MAX);



    /// End
    printf("-------------------------------------------------------------------------------------------\n");

    return 0;
}

