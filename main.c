#include <stdio.h> 
#include <mpi.h>
//math libraries
#include <math.h> 
#include <gmp.h>

#include <stdlib.h>

int main(int argc, char **argv){

    MPI_Init(NULL, NULL);

    //Number of processors
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //processcor rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    //If a command line argument isn't given for the celiling prime value exit the program
    if (argc < 2){
        fprintf(stderr, "\nMissing argument: Celiling prime number\n\n");
        exit(EXIT_FAILURE); 
    }

    //Convert ascii to integer
    int n = atoi(argv[1]);

    //If an integer isn't given or its <= 0 
    if (n <= 0){
        fprintf(stderr, "\nEnter a valid integer > 0\n\n"); 
        exit(EXIT_FAILURE);
    }

    printf("%d\n", n);

    MPI_Finalize(); 
}
