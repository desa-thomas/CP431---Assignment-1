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

    unsigned long long lgap = 0;            //largest gap
    unsigned long long p_1 = 0;             //primes giving largest gap
    unsigned long long p_2 = 0; 
    unsigned long long prev_prime = 0;      //previous gap
    int first_prime = 0;                    //p_i's first prime (for gaps between procs)
    int last_prime = 0;                     //p_i's last prime  (for gaps between procs)

    /*main loop for each proc*/
    for(unsigned long long x = i_start; x < i_start + n_p; x++){
        unsigned long long gap; 

        //Set it to a gmp type
        mpz_t mpz_val;
        mpz_init(mpz_val);
        mpz_set_ui(mpz_val, x); 

        //check if prime
        int prime = mpz_probab_prime_p(mpz_val, 15); 

        if(prime){
            last_prime = x;

            if(prev_prime){
                gap = x - prev_prime;

                //update procs largest gap
                if(gap > lgap){
                    lgap = gap;
                    p_1 = prev_prime;
                    p_2 = x; 
                }
            }

            //if first prime
            else{
                prev_prime = x;
                //if not first proc
                if(rank > 1){
                    first_prime = x; 
                }
            }
        }

    }

    MPI_Barrier(MPI_COMM_WORLD); 

    /*Prime gaps between procs intervals*/
    /*All procs send first prime to previous proc*/
    if(rank > 1){
        MPI_send()
    }

    if(rank < p){}


    MPI_Finalize(); 
}
