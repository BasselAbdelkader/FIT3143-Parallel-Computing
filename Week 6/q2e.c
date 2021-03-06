#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<mpi.h>
#include<math.h>
#include<stdbool.h>

bool isPrime(int n);
void writeToFile(char *pFilename, int *pArray, int counter);

int main(int argc, char **argv)
{

    int rank, n, size, start, end, elementsPerProcess, primeCounter, i;
    char rankFile[50];
    int *primeNumbers;
    int *masterArray;
    int *masterPrimeCount;
    primeCounter = 0;
    primeNumbers = malloc(n *sizeof(int)); 
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    //Clock variables
    double start_time, end_time, timetaken, start_comp_time, serial_start_time;
    
    if (rank == 0) {
    	// STEP - 1 Only the root process reads the input
        printf("Enter a number: ");
        fflush(stdout);
        scanf("%d", &n);
        masterArray = malloc(n * sizeof(int));
        masterPrimeCount = malloc(size * sizeof(int));
    }
    
    //Get the current time
    start_time = MPI_Wtime();
    
    // STEP - 2 Broadcast the integer number to all other MPI processess in the group
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //assign n amount of memory for primeNumbers array to be created
    primeNumbers = malloc(n *sizeof(int)); 
    
    //The number of elements each process should run
    elementsPerProcess = n / size;
    
    start = rank * elementsPerProcess; // starting element of each process
    end = start + elementsPerProcess;  //ending element of each process
    
    if (rank == size - 1) {
    	//last process takes the remainder
        end += (n % size);
    }
    
    //For the first process start checking prime numbers from 2
    if (start == 0) {
        start += 2;
    }
    
    //Get the current time
    start_comp_time = MPI_Wtime();
    
    // STEP 3 - Parallel computing takes place here
    for (i = start; i < end; i++) {
        if (isPrime(i)) {
            primeNumbers[primeCounter] = i;
            primeCounter++;
        }
    }
    
    // Using MPI_Gather to retrieve all data from other processors into the root
    // Gather all the arrays from other processors
    MPI_Gather(primeNumbers, primeCounter, MPI_INT, masterArray, primeCounter, MPI_INT, 0, MPI_COMM_WORLD);
    // Gather all the prime counter from other processors
    MPI_Gather(&primeCounter, 1, MPI_INT, masterPrimeCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    timetaken = end_time - start_comp_time; 
    printf("Rank: %d. Overall time (product only)(s): %lf\n", rank, timetaken);
    
    int totalPrimeCount = 0;
    int k;
    if (rank == 0){
        // Get the clock current time again
        // Subtract end from start to get the CPU time used.
        serial_start_time = MPI_Wtime();

        // Root only
        // Sum up the prime counter to know the total length of the array
        for (k=0; k<size; k++){
            totalPrimeCount += masterPrimeCount[k];
        }

        //writes the name of each process file in string format
        sprintf(rankFile, "root_%d.txt", rank);
        
        //write the output elements of each process to a file
        writeToFile(rankFile, masterArray, totalPrimeCount);

        // Get the clock current time again
        // Subtract end from start to get the CPU time used.
        end_time = MPI_Wtime();
        timetaken = end_time - start_time; 

        // printf("Rank: %d. Overall time (s): %lf\n", rank, timetaken);
        printf("Rank: %d. Overall time (Including read, product and write)(s): %lf\n", rank, timetaken);	// ts

        timetaken = end_time - serial_start_time;
        // printf("Rank: %d. Overall time (s): %lf\n", rank, timetaken);
        printf("Rank: %d. Serial Time: %lf\n", rank, timetaken);	// ts
    }
    
    MPI_Finalize();
    return 0;
}

bool isPrime(int n){

	// Operation to check if the given number is prime 
	for(int j=2; j<=sqrt(n); j++){
		if(n%j==0){
			return false;
		}
	}
	
	return true;
}

void writeToFile(char *pFilename, int *pArray, int counter) {
    int i;
    FILE *fp = fopen(pFilename, "w");
    
    for(i=0; i<counter;i++) {
        fprintf(fp, "%d\n", pArray[i]);
    }
    fclose(fp);
}
