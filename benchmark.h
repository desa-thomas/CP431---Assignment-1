#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <mpi.h>

// Max processors to store stats for
#define MAX_PROCESSORS 100 

// Structure to store execution stats
typedef struct {
    double execution_time;
    double cpu_usage;
    long memory_usage;
} ProcessorStats;

// Function declarations
void log_benchmark_stats(int rank, double execution_time);
void organize_benchmark_stats(int rank, int p);
void finalize_benchmark_stats(int rank, int p);

#endif