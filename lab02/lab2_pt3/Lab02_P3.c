/*----------------------------------------------------------------------------------
 * Initial Build::
 * File:        Lab02_P3.c
 * Description: Multiplication of two matrices together
 *
 *
 * Board:       5529
 * Input:       Two hardcoded, two-dimensional float arrays
 * Output:      Console print of the two inputted arrays, as well as a
 *              two-dimensonal float array result of the matrix multiplication
 *              on the two inputted arrrays
 * Author:      Polickoski, Nick
 * Date:        August 28, 2023
 *----------------------------------------------------------------------------*/

/// Preprocessor Directives
#include <msp430.h>
#include <stdio.h>
#include <float.h> // wasn't sure if needed since the float datatype was used

#define SIZE 8
/* set scope of SIZE to global as its use is in the function prototypes,
 * the call to main, and the function definitions
 */



/// Function Prototypes
void MatrixMul(float[SIZE][SIZE], float[SIZE][SIZE], float[SIZE][SIZE]);
/* takes in two matrices and multiplies them by each other
 */
void PrintMatrix(float[SIZE][SIZE]);
/* prints all elements of a matrix
*/



/// Call to Main
int main()
{
    // Watchdog Timer
    WDTCTL = WDTPW + WDTHOLD;


    // Initialization of Matrices
    float matrix1[SIZE][SIZE] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,
                                 8.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,
                                 7.0,8.0,1.0,2.0,3.0,4.0,5.0,6.0,
                                 6.0,7.0,8.0,1.0,2.0,3.0,4.0,5.0,
                                 5.0,6.0,7.0,8.0,1.0,2.0,3.0,4.0,
                                 4.0,5.0,6.0,7.0,8.0,1.0,2.0,3.0,
                                 3.0,4.0,5.0,6.0,7.0,8.0,1.0,2.0,
                                 2.0,3.0,4.0,5.0,6.0,7.0,8.0,1.0};

    float matrix2[SIZE][SIZE] = {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
                                 0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,
                                 0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,
                                 0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,
                                 0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,
                                 0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,
                                 0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,
                                 0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0};

//    float matrix2[SIZE][SIZE] = {2.0,3.0,4.0,5.0,6.0,7.0,8.0,1.0,
//                                 3.0,4.0,5.0,6.0,7.0,8.0,1.0,2.0,
//                                 4.0,5.0,6.0,7.0,8.0,1.0,2.0,3.0,
//                                 5.0,6.0,7.0,8.0,1.0,2.0,3.0,4.0,
//                                 6.0,7.0,8.0,1.0,2.0,3.0,4.0,5.0,
//                                 7.0,8.0,1.0,2.0,3.0,4.0,5.0,6.0,
//                                 8.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,
//                                 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};

    float matrixResult[SIZE][SIZE];


    // Matrix Multiplication Function Call
    MatrixMul(matrix1, matrix2, matrixResult);


    // Print to Console
    printf("\nMatrix #1: \n");
    PrintMatrix(matrix1);

    printf("\nMatrix #2: \n");
    PrintMatrix(matrix2);

    printf("\n============================================"
            "====================================================\n");

    printf("\nMultiplied Matrix: \n");
    PrintMatrix(matrixResult);


    return 0;
}



/// Function Definitions
void MatrixMul(float mtx1[SIZE][SIZE], float mtx2[SIZE][SIZE], float mtxRslt[SIZE][SIZE])
/* takes in two matrices and multiplies them by each other
 */
{
    // Iterates through each element in the result matrix
    unsigned short int row, col, temp;
    for (row = 0; row < SIZE; row++)
    {
        for (col = 0; col < SIZE; col++)
        {
            mtxRslt[row][col] = 0; // sets current matrix element to 0 before value assigning

            // Iterator that adds every vector combo of the original two matrices
            for (temp = 0; temp < SIZE; temp++)
            {
                mtxRslt[row][col] += mtx1[row][temp] * mtx2[temp][col];
            }
        }
    }

    return;
}


void PrintMatrix(float mtx[SIZE][SIZE])
/* prints all elements of a matrix
*/
{
    printf("------------------------------\n");

    // For-loops iterate through each element for the given matrix
    unsigned short int row, col;
    for (row = 0; row < SIZE; row++)
    {
        for (col = 0; col < SIZE; col++)
        {
            if (col == 0) // conditional to create more eye-pleasing console print output
            {
                printf("[");
            }

            printf("%10f, ", mtx[row][col]); // prints every element in the inputted matrix
        }

        printf("]\n"); // adds a ending bracket and new line character after each
    }                  //   row finishes printing to console

    return;
}

