#include <math.h> 
#include <gmp.h>
#include <stdio.h> 
#include <mpi.h> 
#include <stdlib.h>
#include <sys/resource.h>
#define MAX_PROCESSORS 100 

typedef struct {
    double execution_time;
    double cpu_usage;
    long memory_usage;
} ProcessorStats;

ProcessorStats stats[MAX_PROCESSORS];

void log_benchmark_stats(int rank, double execution_time) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    double cpu_usage = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0;
    long memory_usage = usage.ru_maxrss;

    stats[rank].execution_time = execution_time;
    stats[rank].cpu_usage = cpu_usage;
    stats[rank].memory_usage = memory_usage;
}

void organize_benchmark_stats(int rank, int p) {
    MPI_Gather(&stats[rank], sizeof(ProcessorStats), MPI_BYTE, 
               stats, sizeof(ProcessorStats), MPI_BYTE, 
               0, MPI_COMM_WORLD);
}

void finalize_benchmark_stats(int rank, int p) {
    if (rank == 0) {
        double avg_execution_time = 0;
        double avg_cpu_usage = 0;
        double avg_memory_usage = 0;

        for (int i = 0; i < p; i++) {
            avg_execution_time += stats[i].execution_time;
            avg_cpu_usage += stats[i].cpu_usage;
            avg_memory_usage += stats[i].memory_usage;
        }
        avg_execution_time /= p;
        avg_cpu_usage /= p;
        avg_memory_usage /= p;

        FILE *file = fopen("./benchmark.txt", "a");
        if (file == NULL) {
            perror("Unable to open benchmark.txt");
            return;
        }

        for (int i = 0; i < p; i++) {
            fprintf(file, "Processor %d:\n", i + 1);
            fprintf(file, "    Execution Time: %f\n", stats[i].execution_time);
            fprintf(file, "    CPU Usage: %f\n", stats[i].cpu_usage);
            fprintf(file, "    Memory Usage: %ld KB\n", stats[i].memory_usage);
        }

        fprintf(file, "Overall Average:\n");
        fprintf(file, "    Execution Time: %f\n", avg_execution_time);
        fprintf(file, "    CPU Usage: %f\n", avg_cpu_usage);
        fprintf(file, "    Memory Usage: %f KB\n", avg_memory_usage);

        fclose(file);
    }
}
