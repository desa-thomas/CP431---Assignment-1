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
    //n = how many numbers to check up to
    unsigned long long n = atoi(argv[1]);

    //If an integer isn't given or its <= 0 
    if (n <= 0){
        fprintf(stderr, "\nEnter a valid integer > 0\n\n"); 
        exit(EXIT_FAILURE);
    }

    //n_p = range of values processor p will check
    //i_start = integer that processor p will start checking at
    unsigned long long n_p = floor(n/p);
    if(rank < n %p) n_p += 1; 
    unsigned long long i_start = rank*floor(n/p)+ fmin(rank, n%p) + 1; 

    printf("rank: %d, n_p: %llu, i_start: %llu\n", rank, n_p, i_start); 

   
    unsigned long long lgap = 0;               //largest gap
    unsigned long long prev_prime = NULL;     
    int first_prime = NULL;
    int last_prime = NULL;
    /*main loop*/
    for(unsigned long long x = i_start; x < i_start + n_p; x++){
        
    }

    MPI_Finalize(); 
}
