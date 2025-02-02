#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "benchmark.h"

ProcessorStats stats[MAX_PROCESSORS];

void log_benchmark_stats(int rank, double execution_time) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    double cpu_usage = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0;
    long memory_usage = usage.ru_maxrss;

    #ifdef __APPLE__
        memory_usage /= 1024;  // Convert bytes to KB on macOS
    #endif

    stats[rank].execution_time = execution_time;
    stats[rank].cpu_usage = cpu_usage;
    stats[rank].memory_usage = memory_usage;
}

void organize_benchmark_stats(int rank, int p) {
    ProcessorStats local_stats = stats[rank];
    ProcessorStats *all_stats = (rank == 0) ? malloc(p * sizeof(ProcessorStats)) : NULL;

    MPI_Gather(&local_stats, sizeof(ProcessorStats), MPI_BYTE,
               all_stats, sizeof(ProcessorStats), MPI_BYTE,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < p; i++) {
            stats[i] = all_stats[i];
        }
        free(all_stats);
    }
}

void finalize_benchmark_stats(int rank, int p) {
    if (rank == 0) {
        double avg_execution_time = 0, avg_cpu_usage = 0, avg_memory_usage = 0;

        for (int i = 0; i < p; i++) {
            avg_execution_time += stats[i].execution_time;
            avg_cpu_usage += stats[i].cpu_usage;
            avg_memory_usage += stats[i].memory_usage;
        }
        avg_execution_time /= p;
        avg_cpu_usage /= p;
        avg_memory_usage /= p;

        // Ensure output directory exists
        system("mkdir -p output");

        FILE *file = fopen("./output/benchmark.txt", "a");
        if (file == NULL) {
            perror("Unable to open benchmark.txt");
            return;
        }

        fprintf(file, "===== Benchmark Results =====\n");
        for (int i = 0; i < p; i++) {
            fprintf(file, "Processor %d:\n", i);
            fprintf(file, "    Execution Time: %f sec\n", stats[i].execution_time);
            fprintf(file, "    CPU Usage: %f sec\n", stats[i].cpu_usage);
            fprintf(file, "    Memory Usage: %ld KB\n", stats[i].memory_usage);
        }
        fprintf(file, "\nOverall Averages:\n");
        fprintf(file, "    Execution Time: %f sec\n", avg_execution_time);
        fprintf(file, "    CPU Usage: %f sec\n", avg_cpu_usage);
        fprintf(file, "    Memory Usage: %f KB\n", avg_memory_usage);

        fclose(file);
    }
}
