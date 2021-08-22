// Juvent Benarivo - 28322649
// Bassel Abdelkader- 29796776
// Last Modified: 18 August 2021

// OMP CHEAT SHEET
// https://www.openmp.org/wp-content/uploads/OpenMP-4.0-C.pdf

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void main() {
    int i, j, k, l, num;
    int integerArray[10];
    int sharedPointer = 0;

    int checkedNum[10];
    int checkPointer = 0;
    int skip = 0;
    int occurence = 1;

    // Initialize random generator
    srand(time(NULL));

    // Create threads for parallel computing
    #pragma omp parallel private(i) shared(integerArray, sharedPointer) num_threads(4)
	{
        // Threads will compute the for loop in parallel
        #pragma omp for schedule(dynamic, 2)
        for (i=0; i<10; i++)
        {
            // Get a random number from 1 to 25
            num = rand() % 25 + 1;
            
            // Assign the random number to the array
            integerArray[sharedPointer] = num;
            sharedPointer += 1;
        }

        // Wait for all threads to finish
        #pragma omp barrier
    }

    // Series
    // Check for winning numbers
    // Print Array
    for (i=0; i<10; i++)
    {
        printf("%d ", integerArray[i]);
    }
    printf("\n");

    // Loop through array
    for (j=0; j<10; j++)
    {
        // If there is, loop through checked numbers
        for (l=0; l<checkPointer; l++)
        {
            // If the number has already been checked
            if (integerArray[j] == checkedNum[l])
            {
                // Skip
                skip = 1;
                break;
            }
        }

        if (skip == 1)
        {
            skip = 0;
            continue;
        }

        // Loop through the rest of the numbers
        for (k=j+1; k<10; k++)
        {
            // If the number matches
            if (integerArray[j] == integerArray[k])
            {
                // Loop through checked numbers
                for (l=0; l<checkPointer; l++)
                {
                    // If the number has already been checked
                    if (integerArray[j] == checkedNum[l])
                    {
                        // Skip adding to checked number array
                        skip = 1;
                        // And increment tally
                        occurence += 1;
                        break;
                    }
                }

                if (skip == 1)
                {
                    skip = 0;
                    continue;
                }

                // If it is a new matching number, add it into the checked number array
                checkedNum[checkPointer] = integerArray[j];
                // Increment variables
                checkPointer += 1;
                occurence += 1;
            }
        }

        // If the number occurs more than once
        if (occurence > 1)
        {
            // Output
            printf("The Number '%d' appeared %d times \n", integerArray[j], occurence);
        }
        // Reset occurence
        occurence = 1;
    }

    printf("NUMBER OF WINS: %d\n", checkPointer);
}